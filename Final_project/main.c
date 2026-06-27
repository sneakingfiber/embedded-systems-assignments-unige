
#include <xc.h>
#include <p33EP512MU810.h>
#include <stdio.h>
#include <stdint.h>
#include "UART/uart.h"
#include "ADC/adc.h"
#include "PWM/pwm.h"
#include "SPI/acc.h"
#include "TIMER/timer.h"
#include "Lights/lights.h"
#include "LED/led.h"
#include "TIMER/timer.h"
#include "SPI/acc.h"
#include "PWM/pwm.h"
#include "UART/uart.h"
#include "TIMER/timer.h"
#include <libpic30.h>       //Literally holds the delay function
#pragma GCC optimize("O0")
//constants
#define OBSTACLE_DISTANCE_THRESHOLD_CM  30.0f
unsigned int speed =100,yaw=0;

//shared Timer Flags (defined in timer ISR)
extern volatile uint8_t time_100ms;
extern volatile uint8_t time_1s;
extern volatile uint8_t timer1_isfinish;
//global variables for raw sensor readings
unsigned int ir_sensor_raw   = 0;
unsigned int battery_adc_raw = 0;

//State Handlers
typedef enum {
    ROBOT_STATE_HALTED             = 0,
    ROBOT_STATE_MOVING             = 1,
    ROBOT_STATE_OBSTACLE_AVOIDANCE = 2
} RobotState;

RobotState run_Avoiding_Algorithm(void);

RobotState enter_halted_state(void)
{
    motor_stop();
    low_intensity_lights(OFF);
    return ROBOT_STATE_HALTED;
}

RobotState run_moving_state(void)
{
    UART1_SendString("-MOVING-");
            //TODO: delete this debug message TOO MUCH UART OUTPUT

    motor_move(100, 100);
    
    left_side_lights(OFF);
    right_side_lights(OFF);
    low_intensity_lights(ON);

    float distance_cm = adc_ir_to_cm(ir_sensor_raw);

    if (distance_cm < OBSTACLE_DISTANCE_THRESHOLD_CM) {
        //TODO: delete this debug message TOO MUCH UART OUTPUT
        UART1_SendString("-AVOIDING-");
        return ROBOT_STATE_OBSTACLE_AVOIDANCE;
    }

    return ROBOT_STATE_MOVING;

}
// to a state “Obstacle Avoidance”.  
// ▪ In this state, the robot should rotate clockwise of about 90 degrees, move forward for two seconds, and 
// then rotate anti-clockwise back to the previous heading. If now it senses no obstacles, it goes back to the 
// “Moving state”. Otherwise, the procedure is repeated for a maximum of three times. If an obstacle is still 
// sensed, the state is changed to “Halted”. 
// ▪ If during the two seconds movement an obstacle is sensed, the robot state is changed to “Halted” 
// immediately. 

RobotState run_obstacle_avoidance_state(void)
{
    RobotState result = ROBOT_STATE_OBSTACLE_AVOIDANCE;
   volatile uint8_t attempts = 0;
    motor_stop();
    UART1_SendString("OBSTACLE");
    left_side_lights(OFF);
    do{
        result = run_Avoiding_Algorithm();
        if (result == ROBOT_STATE_MOVING)
            return ROBOT_STATE_MOVING;
        if (result == ROBOT_STATE_HALTED)
            return ROBOT_STATE_HALTED;

        attempts++;
    } while (attempts < 3 );

    // If an obstacle is still sensed, the state is changed to “Halted”.
    return ROBOT_STATE_HALTED;
}

//Initialization
void system_init(int baudrate)
{
    //heartbeat LED RA0 digital output and turn off LED
    LED_Init();
    //button RE8 digital input
    ANSELEbits.ANSE8 = 0;
    TRISEbits.TRISE8 = 1;
    //IR sensor enable (MIKRObus socket 1, RST = RB9)
    ANSELBbits.ANSB9 = 0;
    TRISBbits.TRISB9 = 0;
    LATBbits.LATB9   = 1;

    //RB3 enable 
    ANSELBbits.ANSB3 = 0;
    TRISBbits.TRISB3 = 0;
    LATBbits.LATB3   = 1;
    //Rd6 enable
    ANSELDbits.ANSD6 = 0;
    TRISDbits.TRISD6 = 0;
    LATDbits.LATD6   = 1;

    //TIMER4=1Hz and TIMER2=100 ms 
    tmr_setup_period(TIMER4, 1000);
    tmr_setup_period(TIMER2, 100);

    //Peripherals
    UART1_Init(baudrate);
    ADC_Init_ScanMode(0x4800); //scan AN14 and AN11
    pwm_init();
    SPI_Init();
    Mag_Init();
    //Lights
    left_side_lights_init();     // RB8 
    right_side_lights_init();    // RF1 
    low_intensity_lights_init();
}



int main(void)
{
    int   accel_x, accel_y, accel_z;
    float   roll_deg, pitch_deg;
    int   mag_x, mag_y, mag_z;
    unsigned char chip_id;
    float heading;
    char  uart_tx_buf[48];
    float distance_cm, battery_voltage_v;

    //Initialization
    system_init(9600);
    RobotState current_state = ROBOT_STATE_HALTED;
    enter_halted_state();

  
    while (1)
    {
        __delay_ms(5); // small delay to avoid busy waiting
        
        ADC_Start_ScanMode(&ir_sensor_raw, &battery_adc_raw);
        
        /* --- 1-second tasks: battery report + status lights --- */
        if (time_1s)
        {
            time_1s = 0;

            battery_voltage_v = adc_battery_voltage(battery_adc_raw);
            sprintf(uart_tx_buf, "$MBATT,%.2f*", (double)battery_voltage_v);
            UART1_SendString(uart_tx_buf);

            if (current_state == ROBOT_STATE_HALTED) {
                left_side_lights_toggle();
                right_side_lights_toggle();
            }
            if (current_state == ROBOT_STATE_OBSTACLE_AVOIDANCE) {
                right_side_lights_toggle();
            }
        }

        /* --- 100ms tasks: distance report + accelerometer angles --- */
        if (time_100ms)
        {
            time_100ms = 0;//unflagging the timer flag

            // distance_cm = adc_ir_to_cm(ir_sensor_raw);
            // sprintf(uart_tx_buf, "$MDIST,%.2f*", (double)distance_cm);
            // UART1_SendString(uart_tx_buf);

            // //heartbeat LED toggle
            // LED_Toggle();

            // ACC_ReadAxes(&accel_x, &accel_y, &accel_z);
            // ACC_ComputeAngles(accel_x, accel_y, accel_z, &roll_deg, &pitch_deg);
            // sprintf(uart_tx_buf, "$MANGLE,ROLL:%d,PITCH:%d*", roll_deg, pitch_deg);
            // UART1_SendString(uart_tx_buf);

            Mag_ReadChipID(&chip_id);
            sprintf(uart_tx_buf, "$MAGID,%d*", chip_id);
            UART1_SendString(uart_tx_buf);
            //printing the raw magnetometer readings and the computed heading
            Mag_ReadAxes(&mag_x, &mag_y, &mag_z);
            sprintf(uart_tx_buf, "$MRAW,%d,%d,%d*", mag_x, mag_y, mag_z);
            UART1_SendString(uart_tx_buf);

            Mag_ComputeHeading(&mag_x, &mag_y, &heading);
            sprintf(uart_tx_buf, "$MHEAD,%.1f*", (double)heading);
            UART1_SendString(uart_tx_buf);
        }

        // toggle HALTED <-> MOVING by pressing the button RE8
        if (PORTEbits.RE8 == 0)
        {
            if (current_state == ROBOT_STATE_HALTED)
                current_state = run_moving_state();
            else
                current_state = enter_halted_state();

            while (PORTEbits.RE8 == 0); //wait for button release to avoid bouncing
        }

        /* --- State machine dispatch --- */
        switch (current_state)
        {
            case ROBOT_STATE_OBSTACLE_AVOIDANCE:
                current_state = run_obstacle_avoidance_state();
                break;
            case ROBOT_STATE_MOVING:
                current_state = run_moving_state();
                break;
            case ROBOT_STATE_HALTED:
            default:
                current_state = enter_halted_state();
                break;
        }
    }

    return 0;
}

//to a state “Obstacle Avoidance”.  
// In this state, the robot should rotate clockwise of about 90 degrees, move forward for two seconds, and 
// then rotate anti-clockwise back to the previous heading. If now it senses no obstacles, it goes back to the 
// “Moving state”. Otherwise, the procedure is repeated for a maximum of three times. If an obstacle is still 
// sensed, the state is changed to “Halted”. 
// If during the two seconds movement an obstacle is sensed, the robot state is changed to “Halted” 
// immediately. 
RobotState run_Avoiding_Algorithm(void)
{
    UART1_SendString("Avoiding");
   volatile int Currentangle = 0; // read from the IMU later
   volatile int lastangle=0; // read from the IMU later
    do
    {
         motor_move(100, -100); // rotate clockwise
        __delay_ms(30);
        lastangle = lastangle+1;// read from the IMU later
      
    } while (lastangle <= Currentangle + 90);
    UART1_SendString("CW");
    //move forward for 2 seconds
    tmr_setup_period(TIMER1, 200);

motor_move(100, 100);
UART1_SendString("MOV");

volatile int ticks = 0;
float distance_cm = 0.0f;
while (ticks < 10) {
    ADC_Start_ScanMode(&ir_sensor_raw, &battery_adc_raw);
    distance_cm = adc_ir_to_cm(ir_sensor_raw);
    if (distance_cm < OBSTACLE_DISTANCE_THRESHOLD_CM) {
        UART1_SendString("AVOID");
        motor_stop();
        return ROBOT_STATE_HALTED;
    }

    if (timer1_isfinish) {
        UART1_SendString("TICK");
        timer1_isfinish = 0;
        ticks++;
    }
}

motor_stop();
    //rotate anti-clockwise back to the previous heading
    Currentangle =90; // read from the IMU later
    lastangle = 0; // read from the IMU later
    do
    {
         motor_move(-100, 100); // rotate anti-clockwise
         __delay_ms(30);
        Currentangle = Currentangle-1;// read from the IMU later
      
    } while (Currentangle >= lastangle);
    UART1_SendString("CCW");
    ADC_Start_ScanMode(&ir_sensor_raw, &battery_adc_raw);
    distance_cm = adc_ir_to_cm(ir_sensor_raw);
    if (distance_cm < OBSTACLE_DISTANCE_THRESHOLD_CM) {
        UART1_SendString("-AVOIDING-");
        return ROBOT_STATE_OBSTACLE_AVOIDANCE;
    }
    UART1_SendString("-FINISHED-");
    return ROBOT_STATE_MOVING;
}
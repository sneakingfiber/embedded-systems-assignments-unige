
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

//constants
#define ADC_MAX_VALUE        4095.0f
#define ADC_VREF             3.3f
#define BATTERY_DIVIDER_RATIO 3.0f
#define OBSTACLE_DISTANCE_THRESHOLD_CM  30.0f
uint8_t speed =100,yaw=0;

 //Shared Timer Flags (defined in timer ISR)
extern volatile uint8_t time_100ms;
extern volatile uint8_t time_1s;

//global variables for raw sensor readings
//TODO: consider removing (prof pointed out too many global variables last time)
unsigned int ir_sensor_raw   = 0;
unsigned int battery_adc_raw = 0;

//State Handlers
typedef enum {
    ROBOT_STATE_HALTED             = 0,
    ROBOT_STATE_MOVING             = 1,
    ROBOT_STATE_OBSTACLE_AVOIDANCE = 2
} RobotState;

RobotState enter_halted_state(void)
{
    motor_move(0, 0);
    low_intensity_lights(OFF);
    return ROBOT_STATE_HALTED;
}

RobotState run_moving_state(void)
{
    UART1_SendString("Moving state ..\n");
            //TODO: delete this debug message TOO MUCH UART OUTPUT

    motor_move(speed-yaw, speed+yaw);
    left_side_lights(OFF);
    right_side_lights(OFF);
    low_intensity_lights(ON);

    float distance_cm = adc_ir_to_cm(ir_sensor_raw);

    if (distance_cm < OBSTACLE_DISTANCE_THRESHOLD_CM) {
        //TODO: delete this debug message TOO MUCH UART OUTPUT
        UART1_SendString("Obstacle within threshold, avoiding ..\n");
        return ROBOT_STATE_OBSTACLE_AVOIDANCE;
    }

    UART1_SendString("Path clear, continuing ..\n");
    return ROBOT_STATE_MOVING;
}

RobotState run_obstacle_avoidance_state(void)
{
    left_side_lights(OFF);
            //TODO: delete this debug message TOO MUCH UART OUTPUT

    UART1_SendString("Obstacle detected! ..\n");
    return ROBOT_STATE_OBSTACLE_AVOIDANCE;
}

//Initialization
void system_init(int baudrate)
{
    //heartbeat LED RA0 digital output and turn off LED
    LED_Init();
    //button RE8 digital input
    ANSELEbits.ANSE8 = 0;
    TRISEbits.TRISE8 = 1;
    //IR sensor enable (MIKRObus socket 1, RST = RB4)
    ANSELBbits.ANSB4 = 0;
    TRISBbits.TRISB4 = 0;
    LATBbits.LATB4   = 1;

    //RB3 enable 
    ANSELBbits.ANSB3 = 0;
    TRISBbits.TRISB3 = 0;
    LATBbits.LATB3   = 1;

    //TIMER4=1Hz and TIMER2=100 ms 
    tmr_setup_period(TIMER4, 1000);
    tmr_setup_period(TIMER2, 100);

    //Peripherals
    UART1_Init(baudrate);
    ADC_Init_ScanMode(0x0820); //scan AN5 and AN11
    pwm_init();
    SPI_Init();

    //Lights
    left_side_lights_init();     // RB8 
    right_side_lights_init();    // RF1 
    low_intensity_lights_init();
}



int main(void)
{
    int   accel_x, accel_y, accel_z;
    float   roll_deg, pitch_deg;
    char  uart_tx_buf[48];
    float distance_cm, battery_voltage_v;

    //Initialization
    system_init(9600);
    RobotState current_state = ROBOT_STATE_HALTED;
    enter_halted_state();

  
    while (1)
    {
        ADC_Start_ScanMode(&ir_sensor_raw, &battery_adc_raw);
        
        /* --- 1-second tasks: battery report + status lights --- */
        if (time_1s)
        {
            time_1s = 0;

            battery_voltage_v = adc_battery_voltage(battery_adc_raw);
            sprintf(uart_tx_buf, "$MBATT,%.2f*\n", (double)battery_voltage_v);
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
            time_100ms = 0;

            distance_cm = adc_ir_to_cm(ir_sensor_raw);
            sprintf(uart_tx_buf, "$MDIST,%.2f*\n", (double)distance_cm);
            UART1_SendString(uart_tx_buf);

            //heartbeat LED toggle
            LED_Toggle();

            ACC_ReadAxes(&accel_x, &accel_y, &accel_z);
            ACC_ComputeAngles(accel_x, accel_y, accel_z, &roll_deg, &pitch_deg);
            //sprintf(uart_tx_buf, "$MANGLE,ROLL:%d,PITCH:%d*\n", roll_deg, pitch_deg);
            //UART1_SendString(uart_tx_buf);
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





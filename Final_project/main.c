
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
#include <libpic30.h>       //Literally holds the delay function
#pragma GCC optimize("O0")
//constants
#define MAX_TASKS  8
#define OBSTACLE_DISTANCE_THRESHOLD_CM  30.0f
//latest motion reference from the PC ($PCREF), -100..100, updated in every state
int speed = 0, yawrate = 0;
int   mag_x =0, mag_y =0, mag_z =0; //magnetometer axes values
char  uart_tx_buf[48];
//global variables for raw sensor readings
unsigned int ir_sensor_raw   = 0;
unsigned int battery_adc_raw = 0;
//States
typedef enum {
    ROBOT_STATE_HALTED             = 0,
    ROBOT_STATE_MOVING             = 1,
    ROBOT_STATE_OBSTACLE_AVOIDANCE = 2
} RobotState;
static RobotState current_state = ROBOT_STATE_HALTED; //global, so it's shared between the main and the task

typedef struct{
    int n; // heartbeat counter, to decrement every HB
    int N; //number of HBs before executing a task
    int enabled; //task enabled flag
    void (*func)(); //task function pointer
} heartbeat_task;

enum {
    TASK_CONTROL,   
    TASK_UART_RX,   
    TASK_SENSORS,
    TASK_DISTANCE, 
    TASK_BATTERY,
    TASK_LED,
    TASK_LIGHTS,
    TASK_BUTTONS
};

heartbeat_task schedInfo[MAX_TASKS];

//global variables for avoidance logic
typedef enum {AVOID_ROTATION_CLOCKWISE, AVOID_FORWARD, AVOID_ROTATION_ACLOCKWISE } AvoidState;//avoidance sub-states
static AvoidState avoid_state;
static int avoid_attempts;
static int avoid_Forward_ticks;
static float avoid_initial_heading;
//Utils functions (for heading)
static float current_heading(void){
    int mx, my, mz; float h;
    Mag_ReadAxes(&mx, &my, &mz);
    Mag_ComputeHeading(&mx, &my, &h);
    return h;
}
static float angle_difference(float target, float current){
    float delta = target - current;
    while (delta > 180.0f)  delta -= 360.0f;
    while (delta < -180.0f) delta += 360.0f;
    return delta;
}
static int still_rotating(float target){
    float err = angle_difference(target, current_heading());
    if (err < 0) err = -err;               
    return err > 3.0f;     
}
//States functions definitions
void run_halted_state(void)
{   current_state = ROBOT_STATE_HALTED;
    motor_stop();
    low_intensity_lights(OFF);
    schedInfo[TASK_LIGHTS].enabled = 1; //enabling the task
}

void run_moving_state(void)
{
    current_state = ROBOT_STATE_MOVING;
    left_side_lights(OFF);
    right_side_lights(OFF);
    low_intensity_lights(ON);
    schedInfo[TASK_LIGHTS].enabled = 0;
}

void run_obstacle_avoidance_state(){
    current_state = ROBOT_STATE_OBSTACLE_AVOIDANCE;
    motor_stop();
    avoid_state = AVOID_ROTATION_CLOCKWISE;
    avoid_attempts = 0;
    avoid_initial_heading = current_heading();
    left_side_lights(OFF);
    low_intensity_lights(ON);
    schedInfo[TASK_LIGHTS].enabled = 1;
}

void avoidance_step(float dist){            
    switch (avoid_state){
        case AVOID_ROTATION_CLOCKWISE:
            if(still_rotating(avoid_initial_heading + 90.0f)){  //if we haven't rotated until 90 deg yet, renew the command
                motor_move(100, -100);
            }
            else{                                            //else stop moving and go forward
                motor_stop();
                avoid_Forward_ticks = 0;
                avoid_state = AVOID_FORWARD;
            }
            break;
        case AVOID_FORWARD:
            if(dist < OBSTACLE_DISTANCE_THRESHOLD_CM){
                run_halted_state();
                return;
            }
            motor_move(100,100);
            avoid_Forward_ticks ++; //increment the forward ticks counter so we move exactly for 2s
            if(avoid_Forward_ticks >= 1000){
                motor_stop();
                avoid_state = AVOID_ROTATION_ACLOCKWISE; //at this point we have gone 2s forward and we can get back to the original heading
            }
            break;
        case AVOID_ROTATION_ACLOCKWISE:
            if(still_rotating(avoid_initial_heading)){
                motor_move(-100,100); //keep rotating counterclockwise
            }
            else{
                motor_stop();
                if(dist >= OBSTACLE_DISTANCE_THRESHOLD_CM){
                    run_moving_state();
                }
                else{
                    avoid_attempts ++;
                    if(avoid_attempts < 3){
                        avoid_initial_heading = current_heading();
                        avoid_state = AVOID_ROTATION_CLOCKWISE;
                    }
                    else{
                        run_halted_state(); //3 times failed, stops definitely
                    }
                }
            }
            break;

    }
}
//scheduler declaration and tasks
void scheduler(){
    for(int i=0; i<MAX_TASKS; i++){
        schedInfo[i].n++;
        if (schedInfo[i].enabled && schedInfo[i].n >= schedInfo[i].N) {
            schedInfo[i].func();
            schedInfo[i].n = 0;
        }
    }
}
//task definitions
void task_led()
{
    LED_Toggle();
}
void task_battery(){
    float battery_voltage = adc_battery_voltage(battery_adc_raw);
    sprintf(uart_tx_buf, "$MBATT,%.2f*", (double)battery_voltage);
    UART1_SendString(uart_tx_buf);
}
void task_distance(){
    int distance_cm = adc_ir_to_cm(ir_sensor_raw);
    sprintf(uart_tx_buf, "$MDIST,%d*", (int)distance_cm);
    UART1_SendString(uart_tx_buf);
}
void task_lights(){
    if (current_state == ROBOT_STATE_HALTED) {
        left_side_lights_toggle();
        right_side_lights_toggle();
    }
    if (current_state == ROBOT_STATE_OBSTACLE_AVOIDANCE) {
        right_side_lights_toggle();
    }
}
void task_sensors(){
    int   accel_x, accel_y, accel_z;
    float roll_deg, pitch_deg, heading;
    //accelerometer
    ACC_ReadAxes(&accel_x, &accel_y, &accel_z);
    ACC_ComputeAngles(accel_x, accel_y, accel_z, &roll_deg, &pitch_deg);
    //magnetometer
    Mag_ReadAxes(&mag_x, &mag_y, &mag_z);
    Mag_ComputeHeading(&mag_x, &mag_y, &heading);
    sprintf(uart_tx_buf, "$MANGLE,%.1f,%.1f,%.1f*",(double)roll_deg, (double) pitch_deg, (double) heading);
    UART1_SendString(uart_tx_buf);
}
void task_uart_rx(){
    UART1_ParsePCREF(&speed, &yawrate);
}
void task_button_debouncing(){
    static int btn_e8_previous = 1;
    static int btn_e9_previous = 1;
    int btn_e8 = PORTEbits.RE8;
    int btn_e9 = PORTEbits.RE9;
    if(btn_e8_previous == 1 && btn_e8 == 0){//detecting edge
        if(current_state ==ROBOT_STATE_HALTED)
            run_moving_state();
        else
            run_halted_state();
    }
    if(btn_e9_previous == 1 && btn_e9 == 0){
        sprintf(uart_tx_buf, "$MBUF,%d,%d*", UART1_TxCount(), UART1_RxCount());
        UART1_SendString(uart_tx_buf);
    }
    btn_e8_previous = btn_e8; //save the previous state of the button
    btn_e9_previous = btn_e9;
}
void task_control(){
    ADC_Start_ScanMode(&ir_sensor_raw, &battery_adc_raw);
    float dist = adc_ir_to_cm(ir_sensor_raw);
    switch(current_state){
        case ROBOT_STATE_MOVING:
            motor_move((speed - yawrate),(speed + yawrate));
            if (dist < OBSTACLE_DISTANCE_THRESHOLD_CM) run_obstacle_avoidance_state();
            break;
        case ROBOT_STATE_OBSTACLE_AVOIDANCE:
            avoidance_step(dist);
            break;
        case ROBOT_STATE_HALTED:
            motor_stop();
            break;
        default:
            motor_stop();
            break;
    }
}

//Initialization function
void system_init(int baudrate)
{
    //heartbeat LED RA0 digital output and turn off LED
    LED_Init();
    //button RE8 digital input
    ANSELEbits.ANSE8 = 0;
    TRISEbits.TRISE8 = 1;
    //button RE9 digital input
    ANSELEbits.ANSE9 = 0;
    TRISEbits.TRISE9 = 1;
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


    //Peripherals
    UART1_Init(baudrate);
    ADC_Init_ScanMode(0x4800); //scan AN14 and AN11
    pwm_init();
    SPI_Init();
    Mag_Init();
    //scheduler heartbeat timer setup
    tmr_setup_period(TIMER1,2);
    //Lights
    left_side_lights_init();     // RB8 
    right_side_lights_init();    // RF1 
    low_intensity_lights_init();
}



int main(void)
{  
    //Initialization
    system_init(9600); //no validity check for the baudarate values
    run_halted_state();
  
    schedInfo[TASK_CONTROL] = (heartbeat_task){0, 1, 1, task_control}; //control task 500HZ, read IR, update PWM
    schedInfo[TASK_SENSORS] = (heartbeat_task){0, 50, 1, task_sensors}; //read sensors 10HZ acc+mag and send $MANGLE(N=50)
    schedInfo[TASK_DISTANCE] = (heartbeat_task){0, 50, 1, task_distance}; //calculate distance 10HZ and send $MDIST(N=50)
    schedInfo[TASK_BATTERY] = (heartbeat_task){0, 500, 1, task_battery}; //read battery 1HZ and send $MBATT(N=500)
    schedInfo[TASK_UART_RX] = (heartbeat_task){0, 1, 1, task_uart_rx};
    schedInfo[TASK_LED] = (heartbeat_task){0, 250, 1, task_led}; //toggle LED 1HZ
    schedInfo[TASK_LIGHTS] = (heartbeat_task){0, 250, 1, task_lights}; //toggle lights 1HZ
    schedInfo[TASK_BUTTONS] = (heartbeat_task){0, 10, 1, task_button_debouncing}; //read buttons 50HZ

   while(1){
    scheduler();
    tmr_wait_period(TIMER1);
   }
    return 0;
}


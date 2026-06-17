/* ============================================================
 * Robot Control — Main Application
 * Target: PIC33EP512MU810
 * ============================================================ */

#include <xc.h>
#include <p33EP512MU810.h>
#include <stdio.h>

#include "UART/uart.h"
#include "ADC/adc.h"
#include "PWM/pwm.h"
#include "Lights/lights.h"
#include "TIMER/timer.h"
#include "SPI/acc.h"

uint8_t speed =100,yaw=0;
/* ============================================================
 * Shared Timer Flags (defined in timer ISR)
 * ============================================================ */
extern volatile uint8_t time_100ms;
extern volatile uint8_t time_1s;

/* ============================================================
 * Global Sensor Raw ADC Readings
 * ============================================================ */
unsigned long ir_sensor_raw   = 0;
unsigned long battery_adc_raw = 0;

/* ============================================================
 * Robot State Machine
 * ============================================================ */
typedef enum {
    ROBOT_STATE_HALTED             = 0,
    ROBOT_STATE_MOVING             = 1,
    ROBOT_STATE_OBSTACLE_AVOIDANCE = 2
} RobotState;

/* ============================================================
 * Constants
 * ============================================================ */
#define ADC_MAX_VALUE        4095.0f
#define ADC_VREF             3.3f
#define BATTERY_DIVIDER_RATIO 3.0f
#define OBSTACLE_DISTANCE_THRESHOLD_CM  30.0f

/* ============================================================
 * Function Prototypes
 * ============================================================ */

/* Initialization */
void system_init(void);

/* Utilities */
void  blocking_delay_ms(unsigned int milliseconds);
float adc_to_ir_distance_cm(unsigned long adc_raw);
float adc_to_battery_voltage(unsigned long adc_raw);

/* State Handlers */
RobotState enter_halted_state(void);
RobotState run_moving_state(void);
RobotState run_obstacle_avoidance_state(void);

/* ============================================================
 * Main
 * ============================================================ */
int main(void)
{
    int   accel_x, accel_y, accel_z;
    int   roll_deg, pitch_deg;
    char  uart_tx_buf[48];
    float distance_cm, battery_voltage_v;

    /* --- GPIO: IR sensor enable (MIKRObus socket 1, RST = RB4) --- */
    ANSELBbits.ANSB4 = 0;
    TRISBbits.TRISB4 = 0;
    LATBbits.LATB4   = 1;

    /* --- GPIO: RB3 enable --- */
    ANSELBbits.ANSB3 = 0;
    TRISBbits.TRISB3 = 0;
    LATBbits.LATB3   = 1;

    system_init();

    /* --- Initial state --- */
    RobotState current_state = ROBOT_STATE_HALTED;
    enter_halted_state();

    /* ---- Main Loop ---- */
    while (1)
    {
        ADC_read(&ir_sensor_raw, &battery_adc_raw);

        /* --- 1-second tasks: battery report + status lights --- */
        if (time_1s)
        {
            time_1s = 0;

            battery_voltage_v = adc_to_battery_voltage(battery_adc_raw);
            sprintf(uart_tx_buf, "$MBATT,%.2f*\n", battery_voltage_v);
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

            distance_cm = adc_to_ir_distance_cm(ir_sensor_raw);
            sprintf(uart_tx_buf, "$MDIST,%.2f*\n", distance_cm);
            UART1_SendString(uart_tx_buf);

            /* Heartbeat LED */
            LATAbits.LATA0 = !LATAbits.LATA0;

            /* Accelerometer + angle output */
            ACC_ReadAxes(&accel_x, &accel_y, &accel_z);
            ACC_ComputeAngles(accel_x, accel_y, accel_z, &roll_deg, &pitch_deg);
            //sprintf(uart_tx_buf, "$MANGLE,ROLL:%d,PITCH:%d*\n", roll_deg, pitch_deg);
            //UART1_SendString(uart_tx_buf);
        }

        /* --- Button RE8: toggle HALTED <-> MOVING --- */
        if (PORTEbits.RE8 == 0)
        {
            if (current_state == ROBOT_STATE_HALTED)
                current_state = run_moving_state();
            else
                current_state = enter_halted_state();

            while (PORTEbits.RE8 == 0); /* Wait for button release */
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

/* ============================================================
 * System Initialization
 * ============================================================ */
void system_init(void)
{
    ANSELA = 0;

    /* Heartbeat LED */
    TRISAbits.TRISA0 = 0;

    /* Timers: 1 Hz (TIMER4) and 100 ms (TIMER2) */
    tmr_setup_period(TIMER4, 1000);
    tmr_setup_period(TIMER2, 100);

    /* Button RE8 as digital input */
    ANSELEbits.ANSE8 = 0;
    TRISEbits.TRISE8 = 1;

    /* Peripherals */
    UART1_Init();
    ADC_Init();
    pwm_init();
    SPI1_Init();

    /* Lights */
    left_side_lights_init();     /* RB8 */
    right_side_lights_init();    /* RF1 */
    low_intensity_lights_init();

    __builtin_enable_interrupts();
}

/* ============================================================
 * Utilities
 * ============================================================ */

/**
 * Busy-wait delay. Avoid using in time-critical ISR contexts.
 */
void blocking_delay_ms(unsigned int milliseconds)
{
    unsigned int i, j;
    for (i = 0; i < milliseconds; i++)
        for (j = 0; j < 7200; j++);
}

/**
 * Convert a raw IR sensor ADC reading to distance in centimetres.
 * Uses a 4th-order polynomial fit to the sensor's voltage-distance curve.
 */
float adc_to_ir_distance_cm(unsigned long adc_raw)
{
    float v = (float)adc_raw * ADC_VREF / ADC_MAX_VALUE;

    return (  2.34f
            - 4.74f * v
            + 4.06f * v*v
            - 1.60f * v*v*v
            + 0.24f * v*v*v*v ) * 100.0f;
}

/**
 * Convert a raw battery ADC reading to actual battery voltage.
 * Accounts for the 3× resistor divider on the battery sense pin.
 */
float adc_to_battery_voltage(unsigned long adc_raw)
{
    return (float)adc_raw * ADC_VREF / ADC_MAX_VALUE * BATTERY_DIVIDER_RATIO;
}

/* ============================================================
 * State Handlers
 * ============================================================ */

RobotState enter_halted_state(void)
{
    motor_move(0, 0);
    low_intensity_lights(OFF);
    return ROBOT_STATE_HALTED;
}

RobotState run_moving_state(void)
{
    UART1_SendString("Moving state ..\n");

    motor_move(speed-yaw, speed+yaw);
    left_side_lights(OFF);
    right_side_lights(OFF);
    low_intensity_lights(ON);

    float distance_cm = adc_to_ir_distance_cm(ir_sensor_raw);

    if (distance_cm < OBSTACLE_DISTANCE_THRESHOLD_CM) {
        UART1_SendString("Obstacle within threshold, avoiding ..\n");
        return ROBOT_STATE_OBSTACLE_AVOIDANCE;
    }

    UART1_SendString("Path clear, continuing ..\n");
    return ROBOT_STATE_MOVING;
}

RobotState run_obstacle_avoidance_state(void)
{
    left_side_lights(OFF);
    UART1_SendString("Obstacle detected! ..\n");
    return ROBOT_STATE_OBSTACLE_AVOIDANCE;
}
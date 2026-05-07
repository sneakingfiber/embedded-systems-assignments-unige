#include <p33EP512MU810.h>
#include <xc.h>
#include <string.h>
#include <stdlib.h>
#include "timer.h"
#include "uart.h"

// Global state variables
volatile int bandwidth = 0;
volatile int uart_freq = 10;
static int hz_counter = 0;
static char rx_buf[16];
static int rx_idx = 0;
int ret;

// Helper function to validate HZ frequency
static int is_valid_hz(int val) {
    return val == 0 || val == 1 || val == 2 || val == 5 || val == 10;
}

// Simulated algorithm (blocks for 7 ms)
void algorithm() {
    tmr_wait_ms(TIMER2, 7);
}

// Write accelerometer bandwidth via SPI
void set_bandwidth(unsigned char bw_value) {
    unsigned char trash;

    LATBbits.LATB3 = 0; // ACC_CS = 0 (activate accelerometer)

    while (SPI1STATbits.SPITBF == 1); // wait TX buffer free
    SPI1BUF = 0x10;                   // PMU_BW register address (MSB=0 for write)
    while (SPI1STATbits.SPIRBF == 0); // wait response byte
    trash = SPI1BUF;                  // read to prevent overrun

    while (SPI1STATbits.SPITBF == 1); // wait TX buffer free
    SPI1BUF = bw_value;               // value to write (8..15)
    while (SPI1STATbits.SPIRBF == 0); // wait response byte
    trash = SPI1BUF;                  // read to prevent overrun

    LATBbits.LATB3 = 1; // ACC_CS = 1 (deactivate)
}

// Parse incoming UART messages ($CMD,VAL* format)
void parse_uart() {
    char c;
    char *val_str;
    int val, valid, i;

    // Handle buffer overflow
    if (uart_rx_overflow) {
        uart_rx_overflow = 0;
        rx_idx = 0; // discard incomplete frame
        UART1_SendString("$ERR,1*");
    }

    // Drain RX FIFO one character at a time
    while (UART1_HasData()) {
        c = UART1_ReceiveChar();
        if (c == '$') {
            rx_idx = 0;
            rx_buf[rx_idx++] = c;
        } else if (rx_idx > 0) {
            if (rx_idx < (int)(sizeof(rx_buf) - 1)) {
                rx_buf[rx_idx++] = c;
            }
            // Message end delimiter
            if (c == '*') {
                rx_buf[rx_idx] = '\0';

                // Validate format: $XX,YY*
                if (rx_buf[0] == '$' && rx_buf[3] == ',') {
                    val_str = &rx_buf[4];
                    valid = 1;

                    // Validate: digits only, no signs or decimals
                    for (i = 0; val_str[i] != '\0' && val_str[i] != '*'; i++) {
                        if (val_str[i] < '0' || val_str[i] > '9') {
                            valid = 0;
                            break;
                        }
                    }

                    if (valid) {
                        val = atoi(val_str);
                        if (rx_buf[1] == 'B' && rx_buf[2] == 'W') {
                            // $BW,xx* — set bandwidth
                            if (val >= 8 && val <= 15) {
                                set_bandwidth(val);
                            } else {
                                valid = 0;
                            }
                        } else if (rx_buf[1] == 'H' && rx_buf[2] == 'Z') {
                            // $HZ,yy* — set UART output frequency
                            if (is_valid_hz(val)) {
                                uart_freq = val;
                                hz_counter = 0;
                            } else {
                                valid = 0;
                            }
                        } else {
                            valid = 0;
                        }
                    }

                    if (!valid) {
                        UART1_SendString("$ERR,1*");
                    }
                }
                rx_idx = 0;
            }
        }
    }
}

int main(int argc, char** argv) {
    // disabling all analog ports
    ANSELA = ANSELB = ANSELC = ANSELD = ANSELE = ANSELG = 0X0000;

    // configuring output pins for LEDs
    TRISAbits.TRISA0 = 0; // Set RA0 as output (LD1)
    TRISGbits.TRISG9 = 0; // Set RG9 as output (LD2)

    // initializing UART communication
    UART1_Init();

    // setting up periodic timers
    tmr_setup_period(TIMER1, 10);   // 100 Hz main loop
    tmr_setup_period(TIMER4, 500);  // 1 Hz blink period for LD2

    // enabling global interrupts
    __builtin_enable_interrupts();

    // turning off LEDs at startup
    LATAbits.LATA0 = 0; // Turn off LD1
    LATGbits.LATG9 = 0; // Turn off LD2

    while(1) {
        // execute simulated algorithm (7 ms)
        algorithm();

        // parse any incoming UART messages
        parse_uart();

        // conditionally transmit data based on frequency
        if (uart_freq > 0) {
            hz_counter++;
            if (hz_counter >= (100 / uart_freq)) {
                hz_counter = 0;
                UART1_SendString("$DATA*");
            }
        }

        // wait for main loop period (10 ms)
        ret = tmr_wait_period(TIMER1);
    }

    return 0;
}

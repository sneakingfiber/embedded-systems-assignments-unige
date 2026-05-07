#include <p33EP512MU810.h>
#include <xc.h>
#include "timer.h"
#include "uart.h"
#include <string.h>
#include <stdlib.h>
int ret;
volatile int bandwidth = 0;
volatile int uart_freq = 10;
static int hz_counter = 0;

static char rx_buf[16];
static int rx_idx = 0;

static int is_valid_hz(int val) {
    return val == 0 || val == 1 || val == 2 || val == 5 || val == 10;
}

void parse_uart(void) {
    char c;
    char *val_str;
    int val, valid, i;

    if (uart_rx_overflow) {
        uart_rx_overflow = 0;
        rx_idx = 0; /* discard incomplete frame */
        UART1_SendString("$ERR,OVF*");
    }

    while (UART1_HasData()) {
        c = UART1_ReceiveChar();
        if (c == '$') {
            rx_idx = 0;
            rx_buf[rx_idx++] = c;
        } else if (rx_idx > 0) {
            if (rx_idx < (int)(sizeof(rx_buf) - 1)) {
                rx_buf[rx_idx++] = c;
            }
            if (c == '*') {
                rx_buf[rx_idx] = '\0';

                if (rx_buf[0] == '$' && rx_buf[3] == ',') {
                    val_str = &rx_buf[4];
                    valid = 1;

                    /* validate: digits only, no sign, no decimal */
                    for (i = 0; val_str[i] != '\0' && val_str[i] != '*'; i++) {
                        if (val_str[i] < '0' || val_str[i] > '9') {
                            valid = 0;
                            break;
                        }
                    }

                    if (valid) {
                        val = atoi(val_str);
                        if (rx_buf[1] == 'B' && rx_buf[2] == 'W') {
                            if (val >= 8 && val <= 15) {
                                bandwidth = val;
                                LATGbits.LATG9 = 1;
                            } else {
                                valid = 0;
                            }
                        } else if (rx_buf[1] == 'H' && rx_buf[2] == 'Z') {
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

void algorithm() {
  tmr_wait_ms(TIMER2, 7);
}

int main() {
 // make  LD1 RA0 
  TRISAbits.TRISA0 = 0; // Set RA0 as output (LD1)
// make LD2 RG9 
  TRISGbits.TRISG9 = 0; // Set RG9 as output (LD2)
  UART1_Init();
  tmr_setup_period(TIMER1, 10);
  tmr_setup_period(TIMER4, 500);
  //enable global interrupts
 __builtin_enable_interrupts();
 // make led 0
 LATAbits.LATA0 = 0; // Turn off LD1
 LATGbits.LATG9 = 0; // Turn off LD2
 
  
  while(1) {
    algorithm();
    parse_uart();

    if (uart_freq > 0) {
        hz_counter++;
        if (hz_counter >= (100 / uart_freq)) {
            hz_counter = 0;
            UART1_SendString("$DATA*");
        }
    }

    ret = tmr_wait_period(TIMER1);
  }

}

// UART driver for dsPIC33E - 256000 baud, RX interrupt-driven, TX polling
#include <p33EP512MU810.h>
#include <xc.h>
#include <stdlib.h>
#include "acc.h"
#define FCY 72000000UL

#define UART1_RX_BUF_SIZE 32
static volatile char _rx_buf[UART1_RX_BUF_SIZE];
static volatile int _rx_head = 0;
static volatile int _rx_tail = 0;
volatile int uart_rx_overflow = 0;


// Drain hardware FIFO into circular buffer on RX interrupt
void __attribute__((__interrupt__, __auto_psv__)) _U1RXInterrupt(void) {
    char c;
    int next;
    IFS0bits.U1RXIF = 0;

    while (U1STAbits.URXDA) {
        if (U1STAbits.OERR) {
            U1STAbits.OERR = 0;
        }
        c = U1RXREG;
        next = (_rx_head + 1) % UART1_RX_BUF_SIZE;
        if (next != _rx_tail) {
            _rx_buf[_rx_head] = c;
            _rx_head = next;
        } else {
            uart_rx_overflow = 1;
        }
    }
}

int UART1_HasData(void) {
    return _rx_head != _rx_tail;
}

void UART1_SendChar(unsigned c) {
    while(U1STAbits.UTXBF == 1);
    U1TXREG = c;
}

char UART1_ReceiveChar(void) {
    char c;
    while (!UART1_HasData());
    c = _rx_buf[_rx_tail];
    _rx_tail = (_rx_tail + 1) % UART1_RX_BUF_SIZE;
    return c;
}

// Init UART1 at 256000 baud, RD0=TX, RD11=RX
void UART1_Init(void) {
    TRISDbits.TRISD0 = 0;
    TRISDbits.TRISD11 = 1;

    RPINR18bits.U1RXR = 75;
    RPOR0bits.RP64R = 1;

    U1BRG = (FCY / 4 / 256000) - 1;
    U1MODEbits.BRGH = 1;
    U1MODEbits.PDSEL = 0;
    U1MODEbits.STSEL = 0;

    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN = 1;

    IPC2bits.U1RXIP = 4;
    IFS0bits.U1RXIF = 0;
    IEC0bits.U1RXIE = 1;
}

void UART1_SendString(const char* str) {
    while(*str != '\0') {
        UART1_SendChar(*str++);
    }
}

char* UART1_ReceiveString(char* buffer, int maxLength) {
    int i = 0;
    char c;
    while (i < maxLength - 1) {
        c = UART1_ReceiveChar();
        if (c == '\n') {
            break;
        }
        buffer[i++] = c;
    }
    buffer[i] = '\0';
    return buffer;
}

// Command parser for $BW,xx* (bandwidth) and $HZ,yy* (output rate)

#define RX_BUF_SIZE 16
#define BW_MIN 8
#define BW_MAX 15

static char g_rx_buf[RX_BUF_SIZE];
static int g_rx_idx = 0;

extern volatile int g_uart_output_hz;
extern int g_hz_counter;

static int is_valid_output_hz(int hz) {
    return hz == 0 || hz == 1 || hz == 2 || hz == 5 || hz == 10;
}

static void UART_ProcessFrame(void) {
    char *p_value_str;
    int cmd_value;
    int is_valid;
    int char_idx;

    // Check frame structure: $X,Y*
    if (g_rx_buf[0] != '$' || g_rx_buf[3] != ',') {
        return;
    }

    p_value_str = &g_rx_buf[4];
    is_valid = 1;

    // Validate digit-only payload
    for (char_idx = 0;
         p_value_str[char_idx] != '\0' && p_value_str[char_idx] != '*';
         char_idx++) {
        if (p_value_str[char_idx] < '0' || p_value_str[char_idx] > '9') {
            is_valid = 0;
            break;
        }
    }

    // Dispatch to handler
    if (is_valid) {
        cmd_value = atoi(p_value_str);

        if (g_rx_buf[1] == 'B' && g_rx_buf[2] == 'W') {
            // Bandwidth code 8-15
            if (cmd_value >= BW_MIN && cmd_value <= BW_MAX) {
                ACC_SetBandwidth((unsigned char)cmd_value);
            } else {
                is_valid = 0;
            }
        } else if (g_rx_buf[1] == 'H' && g_rx_buf[2] == 'Z') {
            // Output rate: 0, 1, 2, 5, or 10 Hz
            if (is_valid_output_hz(cmd_value)) {
                g_uart_output_hz = cmd_value;
                g_hz_counter = 0;
            } else {
                is_valid = 0;
            }
        } else {
            is_valid = 0;
        }
    }

    if (!is_valid) {
        UART1_SendString("$ERR,1*");
    }
}

// Called each main loop iteration to process UART command frames
void UART_ParseCommands(void) {
    char rx_char;

    // Handle RX overflow
    if (uart_rx_overflow) {
        uart_rx_overflow = 0;
        g_rx_idx = 0;
        UART1_SendString("$ERR,1*");
    }

    while (UART1_HasData()) {
        rx_char = UART1_ReceiveChar();

        if (rx_char == '$') {
            g_rx_idx = 0;
            g_rx_buf[g_rx_idx++] = rx_char;
        } else if (g_rx_idx > 0) {
            if (g_rx_idx < sizeof(g_rx_buf) - 1) {
                g_rx_buf[g_rx_idx++] = rx_char;
            }

            if (rx_char == '*') {
                g_rx_buf[g_rx_idx] = '\0';
                UART_ProcessFrame();
                g_rx_idx = 0;
            }
        }
    }
}

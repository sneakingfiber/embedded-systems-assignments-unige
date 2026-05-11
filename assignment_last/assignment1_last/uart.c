#include <p33EP512MU810.h>
#include <xc.h>
#include <stdlib.h>
#include "acc.h"
#define FCY 72000000UL

// Circular RX buffer for interrupt-driven receive
#define UART1_RX_BUF_SIZE 32
static volatile char _rx_buf[UART1_RX_BUF_SIZE];
static volatile int _rx_head = 0;
static volatile int _rx_tail = 0;
volatile int uart_rx_overflow = 0;

// UART1 RX interrupt handler — drain hardware FIFO into circular buffer
void __attribute__((__interrupt__, __auto_psv__)) _U1RXInterrupt(void) {
    char c;
    int next;
    IFS0bits.U1RXIF = 0; // clear interrupt flag

    while (U1STAbits.URXDA) {
        if (U1STAbits.OERR) {
            U1STAbits.OERR = 0; // clear overrun error
        }
        c = U1RXREG;
        next = (_rx_head + 1) % UART1_RX_BUF_SIZE;
        if (next != _rx_tail) {
            _rx_buf[_rx_head] = c;
            _rx_head = next;
        } else {
            uart_rx_overflow = 1; // buffer full, byte dropped
        }
    }
}

// Check if data is available in circular receive buffer
int UART1_HasData(void) {
    return _rx_head != _rx_tail;
}

// Send a single character, wait until TX FIFO is not full
void UART1_SendChar(unsigned c) {
    while(U1STAbits.UTXBF == 1); // wait until transmit buffer is not full
    U1TXREG = c;
}

// Receive a single character from circular buffer, blocking
char UART1_ReceiveChar(void) {
    char c;
    while (!UART1_HasData()); // wait until data available
    c = _rx_buf[_rx_tail];
    _rx_tail = (_rx_tail + 1) % UART1_RX_BUF_SIZE;
    return c;
}

// Initialize UART1: 256000 baud, 8-N-1, RD0=TX, RD11=RX
void UART1_Init(void) {
    // configuring pins for UART
    TRISDbits.TRISD0 = 0;  // Set RD0 as output (U1TX)
    TRISDbits.TRISD11 = 1; // Set RD11 as input (U1RX)

    // peripheral pin select (PPS) remapping
    RPINR18bits.U1RXR = 75; // RD11 as U1RX
    RPOR0bits.RP64R = 1;    // RD0 as U1TX

    // configuring UART1 module
    U1BRG = (FCY / 4 / 256000) - 1; // baud rate 256000bps (BRGH=1, /4 prescaler)
    U1MODEbits.BRGH = 1;           // high speed mode (divide-by-4)
    U1MODEbits.PDSEL = 0;          // 8-bit data, no parity
    U1MODEbits.STSEL = 0;          // 1 stop bit

    // enable UART 
    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN = 1; // enable transmit 

    // configuring RX interrupt
    IPC2bits.U1RXIP = 4;  // UART1 RX interrupt priority
    IFS0bits.U1RXIF = 0;  // clear interrupt flag
    IEC0bits.U1RXIE = 1;  // enable UART1 RX interrupt
}

// Send a null-terminated string over UART
void UART1_SendString(const char* str) {
    while(*str != '\0') {
        UART1_SendChar(*str++);
    }
}

// Receive a string until newline or buffer full
char* UART1_ReceiveString(char* buffer, int maxLength) {
    int i = 0;
    char c;
    while (i < maxLength - 1) { // leave space for null terminator
        c = UART1_ReceiveChar();
        if (c == '\n') { // stop on newline
            break;
        }
        buffer[i++] = c;
    }
    buffer[i] = '\0'; // null-terminate the string
    return buffer;
}

/* ===========================================================================
 * UART COMMAND PARSER
 * =========================================================================== */

#define RX_BUF_SIZE         16                           /* UART frame assembly buffer size */
#define BW_MIN              8                            /* Minimum valid bandwidth code    */
#define BW_MAX              15                           /* Maximum valid bandwidth code    */

// Frame assembly buffer (persistent across calls)
static char  g_rx_buf[RX_BUF_SIZE];
static int   g_rx_idx = 0;

// External dependencies from main.c
extern volatile int  g_uart_output_hz;
extern int  g_hz_counter;

// Helper: returns non-zero if hz is one of the allowed $HZ values
static int is_valid_output_hz(int hz)
{
    return hz == 0 || hz == 1 || hz == 2 || hz == 5 || hz == 10;
}

/* ---------------------------------------------------------------------------
 * UART_ProcessFrame  (private)
 * Validates and dispatches a fully received frame stored in g_rx_buf.
 * --------------------------------------------------------------------------- */
static void UART_ProcessFrame(void)
{
    char *p_value_str;  /* Points to the value substring (after ',') */
    int   cmd_value;    /* Parsed numeric value                       */
    int   is_valid;     /* 1 = frame and value passed all checks      */
    int   char_idx;     /* Loop index for digit validation            */

    /*Check minimum frame structure ($X,Y at positions 0 and 3) */
    if (g_rx_buf[0] != '$' || g_rx_buf[3] != ',') {
        //TODO: we could send an ERR code here as well, but the spec only requires error reporting for invalid values
        //also sending through the uart takes time, so we might want to avoid doing it for every malformed frame
        return;  /* Silently discard */
    }

    p_value_str = &g_rx_buf[4];  /* Value string begins immediately after ',' */
    is_valid    = 1;

    /* Step 2: Reject non-digit characters in the value field */
    for (char_idx = 0;
         p_value_str[char_idx] != '\0' && p_value_str[char_idx] != '*';
         char_idx++)
    {
        if (p_value_str[char_idx] < '0' || p_value_str[char_idx] > '9') {
            is_valid = 0;
            break;
        }
    }

    /* Step 3: Dispatch to the matching command handler */
    if (is_valid) {
        cmd_value = atoi(p_value_str);

        if (g_rx_buf[1] == 'B' && g_rx_buf[2] == 'W') {
            /* $BW,xx* — BMA280 bandwidth filter code (8..15) */
            if (cmd_value >= BW_MIN && cmd_value <= BW_MAX) {
                ACC_SetBandwidth((unsigned char)cmd_value);
            } else {
                is_valid = 0;
            }

        } else if (g_rx_buf[1] == 'H' && g_rx_buf[2] == 'Z') {
            /* $HZ,yy* — ACC data output rate (0, 1, 2, 5, 10 Hz) */
            if (is_valid_output_hz(cmd_value)) {
                g_uart_output_hz = cmd_value;
                g_hz_counter     = 0;  /* Prevent stale tick count */
            } else {
                is_valid = 0;
            }

        } else {
            is_valid = 0;  /* Unrecognised command code */
        }
    }

    /* Step 4: Report error if validation or dispatch failed */
    if (!is_valid) {
        UART1_SendString("$ERR,1*");
    }
}

/* ---------------------------------------------------------------------------
 * UART_ParseCommands  (public — called every main-loop iteration)
 *
 * Drains the circular RX buffer character-by-character and assembles frames
 * into g_rx_buf. On '*' reception the frame is passed to UART_ProcessFrame().
 *
 * --------------------------------------------------------------------------- */
void UART_ParseCommands(void)
{
    char rx_char;

    /* RX overflow: ISR dropped a byte; discard partial frame and report */
    if (uart_rx_overflow) {
        uart_rx_overflow = 0;
        g_rx_idx = 0;
        UART1_SendString("$ERR,1*");
    }

    while (UART1_HasData()) {
        rx_char = UART1_ReceiveChar();

        if (rx_char == '$') {
            /* Start-of-frame: discard anything buffered previously */
            g_rx_idx = 0;
            g_rx_buf[g_rx_idx++] = rx_char;

        } else if (g_rx_idx > 0) {
            /* Mid-frame: accumulate if buffer space remains */
            if (g_rx_idx < (int)(sizeof(g_rx_buf) - 1)) {
                g_rx_buf[g_rx_idx++] = rx_char;
            }

            if (rx_char == '*') {
                /* End-of-frame: null-terminate, process, reset */
                g_rx_buf[g_rx_idx] = '\0';
                UART_ProcessFrame();
                g_rx_idx = 0;
            }
        }
        /* Characters before the first '$' are silently discarded */
    }
}

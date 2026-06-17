#include <p33EP512MU810.h>
#include <xc.h>
#include "uart.h"
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

// Initialize UART1: 9600 baud, 8-N-1, RD0=TX, RD11=RX
void UART1_Init(void) {
    // configuring pins for UART
    TRISDbits.TRISD0 = 0;  // Set RD0 as output (U1TX)
    TRISDbits.TRISD11 = 1; // Set RD11 as input (U1RX)

    // peripheral pin select (PPS) remapping
    RPINR18bits.U1RXR = 75; // RD11 as U1RX
    RPOR0bits.RP64R = 1;    // RD0 as U1TX

    // configuring UART1 module
    U1BRG = (FCY / 4 / 9600) - 1; // baud rate 9600bps (BRGH=1, /4 prescaler)
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





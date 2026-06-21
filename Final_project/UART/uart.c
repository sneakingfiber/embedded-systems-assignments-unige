#include <p33EP512MU810.h>
#include <xc.h>
#include "uart.h"
#define FCY 72000000UL

//circular buffer for interrupt-driven rx
#define UART1_RX_BUF_SIZE 32
static volatile char rx_buffer[UART1_RX_BUF_SIZE];
static volatile int rx_head = 0;
static volatile int rx_tail = 0;
static volatile int rx_overflow = 0;


//UART1 RX ISR
void __attribute__((__interrupt__, __auto_psv__)) _U1RXInterrupt(void) {
    char c; //received character (one at a time)
    int next; //next head index in circular buffer

    IFS0bits.U1RXIF = 0; //clear interrupt flag
    while (U1STAbits.URXDA) { //while data is available
        if (U1STAbits.OERR) {U1STAbits.OERR = 0;} // clear overrun error
        c = U1RXREG; //read the char from the rx register
        next = (rx_head + 1) % UART1_RX_BUF_SIZE; //calculate next head index
        if (next != rx_tail) {//checks if the head of the buffer is not equal to the tail of the buffer (buffer full)
            rx_buffer[rx_head] = c; //write the new char into the buffer
            rx_head = next; //update the head index to the next position
        } 
        else {
            rx_overflow = 1; //buffer full, dropped byte
        }
    }
}

//check if data is available in circular receive buffer
int UART1_HasData(void) {
    return rx_head != rx_tail;
}
//read char from the buffer
char UART1_ReadChar(void) {
    char c;
    while (!UART1_HasData()); //if the current position of the buffer is not empty -> read
    c = rx_buffer[rx_tail]; //read the char from the buffer array at the tail index
    rx_tail = (rx_tail + 1) % UART1_RX_BUF_SIZE; //update the tail index to the next position in the circular buffer
    return c;
}

//composing the received characters into a char array until newline is received
char* UART1_ReceiveString(char* buffer, int maxLength) {
    int i = 0;
    char c;
    while (i < maxLength - 1) { //leave space for null terminator
        c = UART1_ReadChar();
        if (c == '\n') {break;} //stop on newline
        buffer[i++] = c;
    }
    buffer[i] = '\0'; //put the null terminator at the end of the char array
    return buffer;
}

//single character send over UART 
void UART1_SendChar(unsigned c) {
    while(U1STAbits.UTXBF == 1); //write into the tx register until transmit buffer is not full
    U1TXREG = c;
}

//send string over UART
void UART1_SendString(const char* str) {
    while(*str != '\0') { //loop till null terminator is reached
        UART1_SendChar(*str++);
    }
}

//Initialize UART1: 9600 baudrate, RD0=TX, RD11=RX
void UART1_Init(int baudrate) {
    // configuring pins for UART
    TRISDbits.TRISD0 = 0;  // Set RD0 as output (U1TX)
    TRISDbits.TRISD11 = 1; // Set RD11 as input (U1RX)

    //PPS remapping
    RPINR18bits.U1RXR = 75; // RD11 as U1RX
    RPOR0bits.RP64R = 1;    // RD0 as U1TX

    //configuring UART1 module
    U1BRG = (FCY / 4 / baudrate) - 1; // baud rate 9600bps (BRGH=1, /4 prescaler)
    U1MODEbits.BRGH = 1;         
    U1MODEbits.PDSEL = 0;          // 8-bit data, no parity
    U1MODEbits.STSEL = 0;          // 1 stop bit

    //configuring RX interrupt
    IPC2bits.U1RXIP = 4;  // UART1 RX interrupt priority
    IFS0bits.U1RXIF = 0;  // clear interrupt flag
    IEC0bits.U1RXIE = 1;  // enable UART1 RX interrupt
    
    //enable UART 
    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN = 1; // enable transmit 
}








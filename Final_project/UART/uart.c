#include <p33EP512MU810.h>
#include <xc.h>
#include <stdio.h>
#include "uart.h"
#define FCY 72000000UL

#define UART1_TX_BUF_SIZE 256
#define UART1_RX_BUF_SIZE 128
#define PCREF_BUF_SIZE 24
static volatile char rx_buffer[UART1_RX_BUF_SIZE];
static volatile int rx_head = 0;
static volatile int rx_tail = 0;
static volatile int rx_overflow = 0;
static volatile char tx_buffer[UART1_TX_BUF_SIZE];
static volatile int tx_head = 0;
static volatile int tx_tail = 0;
static char pcref_buf[PCREF_BUF_SIZE]; //collects the chars between $ and *
static int pcref_idx = 0;              //current write index inside pcref_buf

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
//UART TX ISR
void __attribute__((__interrupt__, __auto_psv__)) _U1TXInterrupt(void) 
{
    IFS0bits.U1TXIF = 0; //clear interrupt flag first thing
    while (!U1STAbits.UTXBF && tx_tail != tx_head) {
        U1TXREG = tx_buffer[tx_tail]; //push one byte to the transmit register
        tx_tail = (tx_tail + 1) % UART1_TX_BUF_SIZE; //advance tail
    }
    if(tx_tail == tx_head){     //if those two indexes are equal the buffer is empty
        IEC0bits.U1TXIE = 0;    //disable tx interrupts
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

//non-blocking parser for the $PCREF,speed,yawrate* commands sent by the PC
int UART1_ParsePCREF(int* speed, int* yawrate) {
    int msg = 0; //becomes 1 when a complete valid message is decoded
    while (UART1_HasData()) { //consume every byte waiting in the circular buffer
        char c = UART1_ReadChar();
        if (c == '$') { //start of a new message
            pcref_idx = 0; //drop anything half received before (best effort method :))
        }
        else if (c == '*') { //end of the message, parse what we collected
            pcref_buf[pcref_idx] = '\0'; //close the string
            int s, y; //received parsed values in tmp variables
            if (sscanf(pcref_buf, "PCREF,%d,%d", &s, &y) == 2) { //check the format
                *speed = s; //hand back the latest reference
                *yawrate = y;
                msg = 1;
            }
            pcref_idx = 0; //ready for the next message
        }
        else { //a normal character, store it if there is room
            if (pcref_idx < PCREF_BUF_SIZE - 1) {
                pcref_buf[pcref_idx++] = c;
            }
        }
    }
    return msg; //if more than one message was buffered, speed/yawrate hold the newest
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
//RX buffer counter for the button output
int UART1_RxCount(void){ return (rx_head - rx_tail + UART1_RX_BUF_SIZE) % UART1_RX_BUF_SIZE; }

//single character send over UART 
void UART1_SendChar(unsigned c) {
    int next = (tx_head + 1) % UART1_TX_BUF_SIZE;
    while (next == tx_tail); //buffer full: wait for the ISR to free a slot
    tx_buffer[tx_head] = (char)c; //write the byte into the buffer
    tx_head = next; //increment head
    IEC0bits.U1TXIE = 1; //we have data to send so we re-enable the TX interrupt
}

//send string over UART
void UART1_SendString(const char* str) {
    while(*str != '\0') { //loop till null terminator is reached
        UART1_SendChar(*str++);
    }
}
//TX buffer counter for the button
int UART1_TxCount(void){ return (tx_head - tx_tail + UART1_TX_BUF_SIZE) % UART1_TX_BUF_SIZE; }


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
    //configuring TX interrupts
    IPC3bits.U1TXIP = 3;
    IFS0bits.U1TXIF = 0;
    //enable UART 
    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN = 1; // enable transmit 
}








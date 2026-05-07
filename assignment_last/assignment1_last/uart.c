#include <p33EP512MU810.h>
#include <xc.h>
#define FCY 72000000UL
void UART1_SendChar(unsigned c){
    while(U1STAbits.UTXBF == 1);  // Wait until TX buffer is not full
    U1TXREG = c;
}
char UART1_ReceiveChar(void){
    while(U1STAbits.URXDA == 0);  // Wait until data is available
    return U1RXREG;
}
void UART1_Init(void){
    TRISDbits.TRISD0 = 0; // Set RD0 as output (U1TX)
    TRISDbits.TRISD11 = 1; // Set RD11 as input (U1RX)
   // configure UART1 for RD11
    RPINR18bits.U1RXR = 75; // RD11 as U1RX 
    RPOR0bits.RP64R = 1; // RD0 as U1TX
    // configure UART1
    U1BRG = (FCY / 16 / 9600) - 1; // Set baud rate to 9600
    U1MODEbits.BRGH = 0; // Standard speed mode
    U1MODEbits.PDSEL = 0; // 8-bit data, no parity
    U1MODEbits.STSEL = 0; // 1-stop bit


    U1MODEbits.UARTEN = 1; // enable UART

    U1STAbits.UTXEN = 1; // enable U1TX (must be after UARTEN)
}

void UART1_SendString(const char* str){
    while(*str != '\0'){
        UART1_SendChar(*str++);
    }
}

char* UART1_ReceiveString(char* buffer, int maxLength){
    int i = 0;
    char c;
    while (i < maxLength - 1) { // Leave space for null terminator
        c = UART1_ReceiveChar();
        if (c == '\n') { // Stop on newline
            break;
        }
        buffer[i++] = c;
    }
    buffer[i] = '\0'; // Null-terminate the string
    return buffer;
    
}

#include <p33EP512MU810.h>
#include <xc.h>
#define FCY 72000000UL

#define UART1_RX_BUF_SIZE 32
static volatile char _rx_buf[UART1_RX_BUF_SIZE];
static volatile int _rx_head = 0;
static volatile int _rx_tail = 0;
volatile int uart_rx_overflow = 0;

void __attribute__((__interrupt__, __auto_psv__)) _U1RXInterrupt(void) {
    char c;
    int next;
    IFS0bits.U1RXIF = 0;
    while (U1STAbits.URXDA) {
        if (U1STAbits.OERR) { U1STAbits.OERR = 0; }
        c = U1RXREG;
        next = (_rx_head + 1) % UART1_RX_BUF_SIZE;
        if (next != _rx_tail) {
            _rx_buf[_rx_head] = c;
            _rx_head = next;
        } else {
            uart_rx_overflow = 1; /* buffer full, byte dropped */
        }
    }
}

int UART1_HasData(void) {
    return _rx_head != _rx_tail;
}

void UART1_SendChar(unsigned c){
    while(U1STAbits.UTXBF == 1);  // Wait until TX buffer is not full
    U1TXREG = c;
}
char UART1_ReceiveChar(void){
    char c;
    while (!UART1_HasData());
    c = _rx_buf[_rx_tail];
    _rx_tail = (_rx_tail + 1) % UART1_RX_BUF_SIZE;
    return c;
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

    IPC2bits.U1RXIP = 4;  // UART1 RX interrupt priority
    IFS0bits.U1RXIF = 0;  // clear flag
    IEC0bits.U1RXIE = 1;  // enable UART1 RX interrupt
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

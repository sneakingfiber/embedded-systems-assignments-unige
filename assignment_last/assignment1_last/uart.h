
#ifndef UART_H
#define UART_H
// Declarations for UART module go here.
#define FCY 72000000UL
void UART1_SendChar(unsigned c); // Sends a single character over UART1. This function
// blocks until the character is sent.
char UART1_ReceiveChar(void); // Receives a single character from UART1. This
// function blocks until a character is received.
void UART1_Init(void); // Initializes UART1 with the following settings:
// Baud rate: 9600
// Data bits: 8
// Stop bits: 1
// Parity: None

void UART1_SendString(const char* str); // Sends a null-terminated string over UART1. This function


char* UART1_ReceiveString(char* buffer, int maxLength); // Receives a string from UART1 until a newline character is received or the maximum length is reached. The received string is stored in the provided buffer, which should be large enough to hold the incoming data. The function returns a pointer to the buffer containing the received string.

#endif // UART_H
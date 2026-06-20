#ifndef UART_H
#define UART_H

#define FCY 72000000UL

void UART1_SendChar(unsigned c);
char UART1_ReadChar(void);
int  UART1_HasData(void);
void UART1_Init(void);
void UART1_SendString(const char* str);
char* UART1_ReceiveString(char* buffer, int maxLength);

#endif // UART_H

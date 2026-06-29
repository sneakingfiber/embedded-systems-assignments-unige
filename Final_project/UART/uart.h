#ifndef UART_H
#define UART_H

#define FCY 72000000UL

void UART1_SendChar(unsigned c);
char UART1_ReadChar(void);
int  UART1_HasData(void);
void UART1_Init(int baudrate);
void UART1_SendString(const char* str);
char* UART1_ReceiveString(char* buffer, int maxLength);
int  UART1_ParsePCREF(int* speed, int* yawrate);
int UART1_TxCount(void);
int UART1_RxCount(void);

#endif 

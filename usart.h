#ifndef USART_H
#define USART_H

void USARTInit(unsigned long int baudrate);
void USARTSendChar(unsigned char data);
void USARTSendString(unsigned char * data);
unsigned char USART_TSR_control(void);
void USART_print(unsigned  char *string);
unsigned char USARTIsAvailable(void);
char USARTReadChar(void);
unsigned char USARTGetByte(void);
unsigned char USARTGetFERR(void);
unsigned char USARTCheckOERR(void);

#endif

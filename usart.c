#include "system.h"

void USARTInit( unsigned long speed )
{
	unsigned char spbrg = ((int)(_XTAL_FREQ/(16 * speed) - 1)); // BRGH = 1
    BRGH = 1;
	TRISC4 = 1 ;
	TRISC5 = 1 ;
	SPBRG = spbrg ;
	RCSTA = 0b10010000 ;
	TXSTA = 0b00100100 ;
}

void USARTSendChar(unsigned char data)
{
    while(!TRMT);
    TXREG = data;
}

unsigned char USART_TSR_control(void)
{
    return TRMT;
}

void USARTSendString(unsigned char *string)
{
    int i = 0;

    for(i; string[i] != '\0'; i++)
    {
        USARTSendChar(string[i]);
    }
}

unsigned char USARTIsAvailable(void)
{
    return RCIF;
}

char USARTReadChar(void)
{
    while(!RCIF);
    return RCREG;
}

unsigned char USARTCheckOERR(void)
{
        if(OERR)        /* re-enable after overrun error */
        {
           CREN = 0;
           CREN = 1;
           return 1;
        }
        return 0;
}

unsigned char USARTGetFERR(void)
{
        while(!RCIF)
                continue;
        return FERR;    /* RCIF is not cleared until RCREG is read */
}

unsigned char USARTGetByte(void)
{
        while(!RCIF)
          continue; /* set when register is not empty */
        return RCREG;   /* RXD9 and FERR are gone now */
}
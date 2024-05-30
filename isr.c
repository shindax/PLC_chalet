#include "system.h"

static void interrupt isr( void )
{
	static unsigned char tick = 0;
	static unsigned char ch = 0;
	static unsigned int heartBeatTick = 0;
 
	if( RCIF && RCIE ){
		ch = RCREG;
		RCIF = 0;
		usartData[ usartDataPtr ++ ] = ch;
		LED_ON;
		heartBeatTick = 0;
	}

    if( TMR0IF && TMR0IE )        // Was it a timer 0 overflow?
    {
    	TMR0IF = 0 ;
    	TMR0   = TMR0_PRELOAD;
		tick ++;
		heartBeatTick ++;

		if( heartBeatTick == 2 && usartDataPtr == 0 ) 
			LED_OFF;

		if( heartBeatTick >= 200 ){ 
			heartBeatTick = 0;
			if( usartDataPtr == 0 )
				LED_ON;
			dataUpdateNeeded = 1;
			usartDataPtr = 0;
		}
		if( tick == 25 ){
				displayUpdateNeeded = 1;
				tick = 0;
		}

  	}
}

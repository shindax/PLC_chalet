#include "system.h"

static void interrupt isr( void )
{
	static unsigned char tick = 0, heartBeatTick = 0;
	static unsigned char addr = 0;
    
	if( RCIF && RCIE ){
		usartData = RCREG;
	    while(!TRMT);
    	TXREG = usartData;
	}

    if( TMR0IF && TMR0IE )        // Was it a timer 0 overflow?
    {
    	TMR0IF = 0 ;
    	TMR0   = TMR0_PRELOAD;
		tick ++;
		heartBeatTick ++;
		if( heartBeatTick == 2 )	
				HEART_BEAT = 1;
		if( heartBeatTick >= 100 ){ 
			heartBeatTick = 0;
			HEART_BEAT = 0;
			dataUpdateNeed = 1;
		}
		if( tick == 25 ){
				displayUpdateNeed = 1;
				tick = 0;
		}

  	}
}

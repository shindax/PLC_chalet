#include "system.h"

void InitPorts( void )
{
  ADCON1 = 0B10000110 ; // All ports are digital
  PORTA = 0b00010000;
  PORTB = 0xFF ; //
  PORTC = 0 ;
  TRISA  = 0;
  TRISB  = 0xFF ;
  TRISC  = 0xFF ;
}

void InitSFRS( void )
{
  OPTION_REG = 0B00000111 ; // Prescaler is 1:256, before TMR0
  TMR0 = TMR0_PRELOAD ;     // (256-176)*( 1/((4096000/4)/256)) = 20mS
  T0IE = 1 ;
  T0IF   = 0 ;
 }


void checkTimeSettings( void )
{
	unsigned char channel, state, porta, tmp;
	unsigned int i ;
	date_time locDateTime;

	porta = PORTA;
	for( i = 0 ; i < 256; i += 4 ){
		getEepromTimeSetting(i, & locDateTime);
		channel = locDateTime.second >> 4;
		state = locDateTime.second & 0x0F;
		if( channel > 4 || locDateTime.day & 0x80 == 0 )// channel is too big or setting is disabled
			continue;
		if( locDateTime.day & ( 1 << time.day - 1 ) && locDateTime.day & SETPOINT_ENABLED )
			if(	
				locDateTime.hour == time.hour 
					&& 
				locDateTime.minute == time.minute 
			  ){
					tmp = 1 << channel - 1;
					if( state )
						porta |= tmp;
							else
								porta &= ~ tmp;
			  }
	}
	PORTA = porta;
}

void getEepromTimeSetting( unsigned char addr, date_time * _time )
{
	_time -> second = eeprom_read( addr );
	_time -> hour = eeprom_read( addr + 1 );
	_time -> minute = eeprom_read( addr + 2 );
	_time -> day = eeprom_read( addr + 3 );
}

void getData( void )
{
}
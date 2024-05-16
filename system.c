#include "system.h"

void InitPorts( void )
{
  ADCON1 = 0B10000110 ; // All ports are digital
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
	unsigned char channel, porta, tmp, day, stateChanged = 0;
	unsigned int i = 0;
	date_time locDateTimeFrom;
	date_time locDateTimeTo;

	porta = PORTA;

	for( i = 0 ; i < 0x256; i += 8 ){
		getEepromTimeSetting( i, & channel, & day, & locDateTimeFrom, & locDateTimeTo );

		if( channel > 4 )// channel is too big or setting is empty
			continue;

		if( day & ( 1 << time.day - 1 ) && ( day & ( 1 << time.day - 1 )))
		{
			tmp = 1 << channel - 1;

// Проверка времени включения
			if(	time.hour == locDateTimeFrom.hour && time.minute == locDateTimeFrom.minute ){
				stateChanged = 1;
				porta |= tmp;
			}
// Проверка времени выключения
			if( locDateTimeTo.hour == time.hour && locDateTimeTo.minute == time.minute ){
				stateChanged = 1;
				porta &= ~ tmp;
			}
  		}     
	}// for( i = 0 ; i < 256; i += 8 ){
	
	if( stateChanged ){
		PORTA = porta;
		time.year = ( porta << 4 ) | 0x04;
		write_time( ( date_time * ) & time );
	}
}

void getEepromTimeSetting( unsigned char addr, unsigned char * channel, unsigned char * day, date_time * from, date_time * to )
{
	* channel = eeprom_read( addr );
	* day = eeprom_read( addr + 1 );
	from -> hour = eeprom_read( addr + 2 );
	from -> minute = eeprom_read( addr + 3 );
	to -> hour = eeprom_read( addr + 4 );
	to -> minute = eeprom_read( addr + 5 );
}

void getData( void )
{
}

unsigned char checkInRange( unsigned char min, unsigned char max, unsigned char value )
{
	return min <= value && value <= max ;
}

/*
unsigned char checkInRangeTimeSettings( void )
{
	date_time dateTimeFrom;
	date_time dateTimeTo;
	unsigned char channel = 0, porta = 0, tmp = 0, day = 0;

	unsigned int i = 0;

	for( i = 0 ; i < 256; i += 8 ){
		getEepromTimeSetting( i, & channel, & day, & dateTimeFrom, & dateTimeTo );

		if( channel > 4 )// channel is too big or setting is empty
			continue;

		if( day & ( 1 << time.day - 1 ) && ( day & ( 1 << time.day - 1 ))){
			tmp = 1 << channel - 1;

			if(	
				checkInRange( dateTimeFrom.hour, dateTimeTo.hour, time.hour )  // Внутри дипазона по часам 
					&&
				checkInRange( dateTimeFrom.minute, dateTimeTo.minute, time.minute )	// Внутри дипазона по минутам 
			  )
				porta |= tmp;
					
	   }
	}
	return porta;
}
*/
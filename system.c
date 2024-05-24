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
  
  RCREG;
  RCIF = 0;
  RCIE = 0;

  PEIE = 1;
  RBIE = 0;
  INTE = 0;
  SSPIE = 0;
 }

unsigned char checkTimeSettings( unsigned char * porta )
{
	unsigned char channel, tmp, day, stateChanged = 0;
	unsigned char i = 0;
	date_time locDateTimeFrom;
	date_time locDateTimeTo;

	* porta = PORTA;

	for( i = 0 ; i < 0xF8; i += 8 ){
		getEepromTimeSetting( i, & channel, & day, & locDateTimeFrom, & locDateTimeTo );

		if( channel > 4 )// channel is too big or setting is empty
			continue;

		if( day & ( 1 << time.day - 1 ) && ( day & ( 1 << time.day - 1 )))
		{
			tmp = 1 << channel - 1;

// Проверка времени включения
			if(	time.hour == locDateTimeFrom.hour && time.minute == locDateTimeFrom.minute ){
				stateChanged = 1;
				* porta |= tmp;
			}
// Проверка времени выключения
			if( locDateTimeTo.hour == time.hour && locDateTimeTo.minute == time.minute ){
				stateChanged = 1;
				* porta &= ~ tmp;
			}
  		}     
	}// for( i = 0 ; i < 256; i += 8 ){
	
	return stateChanged ;

}// unsigned char checkTimeSettings( unsigned char )

void getEepromTimeSetting( unsigned char addr, unsigned char * channel, unsigned char * day, date_time * from, date_time * to )
{
	* channel = eeprom_read( addr );
	* day = eeprom_read( addr + 1 );
	from -> hour = eeprom_read( addr + 2 );
	from -> minute = eeprom_read( addr + 3 );
	to -> hour = eeprom_read( addr + 4 );
	to -> minute = eeprom_read( addr + 5 );
}

unsigned int getMinutes( date_time var )
{ 
	return (( int )var.hour) * 60 + ( int )var.minute; 
}

unsigned char checkInRangeTimeSettings( void )
{
	date_time dateTimeFrom;
	date_time dateTimeTo;
	unsigned char channel = 0, porta = 0, tmp = 0, day = 0;
	unsigned int i = 0, minutes;

	for( i = 0 ; i < 0xFF; i += 8 ){
		getEepromTimeSetting( i, & channel, & day, & dateTimeFrom, & dateTimeTo );

		if( channel > 4 )// channel is too big or setting is empty
			continue;

		if( 
			( day & ( 1 << time.day - 1 )) 
				&& 
			( day & ( 1 << time.day - 1 ))
		  ){
			tmp = 1 << channel - 1;

			minutes = getMinutes( time );
			if( 
				( // Переход через 00:00
					( dateTimeFrom.hour > dateTimeTo.hour ) 
						&& 
					( minutes >= getMinutes( dateTimeFrom ) || minutes < getMinutes( dateTimeTo ))
				)
					||
				( getMinutes( dateTimeFrom ) <= minutes && getMinutes( dateTimeTo ) > minutes )
			  )
				porta |= tmp;

	   }
	}// for( i = 0 ; i < 0xFF; i += 8 ){

	for( i = 0; i < 4; i ++ ){
		if( outputsMode[i] == 0 )
			porta &= ~( 1 << i );
			else
				if( outputsMode[i] == 1 )
						porta |= ( 1 << i );
	}

	return porta;
}

unsigned char checkBarrelSensor( unsigned char porta )
{
	if( LED ) 
		porta |= 1 << 4; // Сохранить состояние бита LED
	return BARREL_IS_FULL ? porta & 0xFE : porta ; // Маскируем выходной бит PORTA управляющий клапаном бочки
}

void sendBufToUsart( void )
{
	LED_ON;
	for( unsigned char i = 0; i < 8; i ++ ){
		while(!TRMT);
    	TXREG = usartData[i];
	}
	LED_OFF;
}

void updateOutputsMode( void )
{
}

unsigned char portaCheck( unsigned char * porta )
{
	* porta = checkInRangeTimeSettings();
	PORTA = 0b00010000 | checkBarrelSensor( * porta );
	displayUpdateNeeded = 1;
	return 0xFF;
}

void getOutputsMode( void )
{
	outputsMode[0] = ext_eeprom_read_byte( 0 );
	outputsMode[1] = ext_eeprom_read_byte( 1 );
	outputsMode[2] = ext_eeprom_read_byte( 2 );
	outputsMode[3] = ext_eeprom_read_byte( 3 );
}

void putOutputsMode( void )
{
    ext_eeprom_write_byte( 0, usartData[1] );
    ext_eeprom_write_byte( 1, usartData[2] );
    ext_eeprom_write_byte( 2, usartData[3] );
    ext_eeprom_write_byte( 3, usartData[4] );

	outputsMode[0] = usartData[1];
	outputsMode[1] = usartData[2];
	outputsMode[2] = usartData[3];
	outputsMode[3] = usartData[4];
}

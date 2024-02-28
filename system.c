#include "system.h"

void InitPorts( void )
{
  ADCON1 = 0B10000110 ; // All ports are digital
  PORTA = 0 ; // Clear all traps
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

void updateDisplay( void )
{
	displayTime();
	displayTemp();
	displayDate();
	displayInputs();
	displayOutputs();
}

void displayOutputs( void )
{
	unsigned char pos = 40;
	display_char_8x16( pos		, OUTPUT_LINE, 23 + OUTPUT_1 );
	display_char_8x16( pos += 16, OUTPUT_LINE, 23 + OUTPUT_2 );
	display_char_8x16( pos += 16, OUTPUT_LINE, 23 + OUTPUT_3 );
	display_char_8x16( pos += 16, OUTPUT_LINE, 23 + OUTPUT_4 );
}

void displayInputs( void )
{
	unsigned char pos = 4;

	display_char_8x16( pos		, INPUT_LINE, 23 + INPUT_1 );
	display_char_8x16( pos += 16, INPUT_LINE, 23 + INPUT_2 );
	display_char_8x16( pos += 16, INPUT_LINE, 23 + INPUT_3 );
	display_char_8x16( pos += 16, INPUT_LINE, 23 + INPUT_4 );
	display_char_8x16( pos += 16, INPUT_LINE, 23 + INPUT_5 );
	display_char_8x16( pos += 16, INPUT_LINE, 23 + INPUT_6 );
	display_char_8x16( pos += 16, INPUT_LINE, 23 + INPUT_7 );
	display_char_8x16( pos += 16, INPUT_LINE, 23 + INPUT_8 );
}

void displayTime( void )
{
	unsigned char pos = 8;
	display_char_8x16( pos, TIME_TEMP_LINE, 26 + ( time.hour >> 4 ) );
	display_char_8x16( pos += 8, TIME_TEMP_LINE, 26 + ( time.hour & 0x0F ) );
	display_char_8x16( pos += 8, TIME_TEMP_LINE, 25 );
	display_char_8x16( pos += 8, TIME_TEMP_LINE, 26 + ( time.minute >> 4 ) );
	display_char_8x16( pos += 8, TIME_TEMP_LINE, 26 + ( time.minute & 0x0F ) );
	display_char_8x16( pos += 8, TIME_TEMP_LINE, 25 );
	display_char_8x16( pos += 8, TIME_TEMP_LINE, 26 + ( time.second >> 4 ) );
	display_char_8x16( pos += 8, TIME_TEMP_LINE, 26 + ( time.second & 0x0F ) );
}

void displayTemp( void )
{
	unsigned char pos = 80;
	unsigned long temp = DS18B20CalculateTemperature();
	unsigned int whole = temp / 1000;
    unsigned int fraction = temp % 1000;

	display_char_8x16( pos, TIME_TEMP_LINE, 26 + whole / 10 );
	display_char_8x16( pos += 8, TIME_TEMP_LINE, 26 + whole % 10 );
	display_char_8x16( pos += 8, TIME_TEMP_LINE, 37); // .
	display_char_8x16( pos += 8, TIME_TEMP_LINE, 26 + fraction / 100 );
//	display_char_8x16( pos += 8, TIME_TEMP_LINE, 38);// C
	display_char_8x16( pos += 8, TIME_TEMP_LINE, 36);// deg
}

void displayDate( void )
{
	unsigned char i, pos = 0;
	display_char_8x16( pos	   , DATE_LINE,  time.date >> 4 ? 26 + ( time.date >> 4 ) : 0 );
	display_char_8x16( pos += 8, DATE_LINE, 26 + ( time.date & 0x0F ) );
	display_char_8x16( pos += 8, DATE_LINE, 0 );
	
	i = time.month - 1;
	display_char_8x16( pos += 8, DATE_LINE, monthes[i][0] );
	display_char_8x16( pos += 8, DATE_LINE, monthes[i][1] );
	display_char_8x16( pos += 8, DATE_LINE, monthes[i][2] );

	display_char_8x16( pos += 8, DATE_LINE, 0 );
//	display_char_8x16( pos += 8, DATE_LINE, 0 );

	i = time.day - 1;
	display_char_8x16( pos += 8, DATE_LINE, days[i][0] );
	display_char_8x16( pos += 8, DATE_LINE, days[i][1] );
	display_char_8x16( pos += 8, DATE_LINE, days[i][2] );

	display_char_8x16( pos += 8, DATE_LINE, 0 );
	display_char_8x16( pos += 8, DATE_LINE, 28 ); 
	display_char_8x16( pos += 8, DATE_LINE, 26 );

	display_char_8x16( pos += 8, DATE_LINE, 26 + ( time.year >> 4 ) );
	display_char_8x16( pos += 8, DATE_LINE, 26 + ( time.year & 0x0F ));
}

void checkTimeSettings( void )
{
	unsigned char i = 0, channel, state, porta, tmp;
	date_time locDateTime;
	porta = PORTA;
	for( i = 0; i < 64; i += 4 ){
		getEepromTimeSetting(i, & locDateTime);
		channel = locDateTime.second >> 4;
		state = locDateTime.second & 0x0F;
		if( locDateTime.day & ( 1 << time.day - 1 ) )
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

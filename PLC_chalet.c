#include "system.h"

volatile unsigned char displayUpdateNeed = 0;
volatile unsigned char dataUpdateNeed = 0;
volatile unsigned char WebServerReady = 0;
volatile unsigned char usartData = 0;
volatile date_time time;
volatile unsigned long temp;

const unsigned char monthes[][3] = {{22,12,3},{19,6,3},{11,1,15},{1,14,15},{11,1,8},{7,21,12},{7,21,10},{1,3,4},{16,6,12},{13,9,17},{12,13,22},{5,6,9}};
const unsigned char days[][3] = {{14,13,12},{3,17,15},{16,15,5},{20,17,3},{14,22,17},{16,18,2},{3,16,9}};

#define hour1 	0x10
#define minute1 0x30
#define minute2 0x31

__EEPROM_DATA(0x11,hour1,minute1,0b10000010,0x10,0x08,0x00,0b10000100);
__EEPROM_DATA(0x21,hour1,minute1,0b10000010,0x20,hour1,minute2,0b10000010);


void main()
{
  unsigned char minute = 0xFF;
  unsigned char hour = 0xFF;

  InitPorts();
  InitSFRS();
  USARTInit(9600);

  RCIF = 0;
  RCIE = 0;
  PEIE = 0;
    
	display_init();
    __delay_ms(100);
    display_clear(0);

/*
	write_byte( 0,0x00 );
	__delay_ms( 1 );
	eeprom_write( 0x30,	read_byte( 0 ));
*/

	read_time( ( date_time * ) & time );

//		time.minute = 0x10;
//		time.day = 5;
//		time.date = 0x28;
//		time.month = 0x02;
//		time.year = 0x24;
//	write_time( ( date_time * ) & time );

  GIE  = 1 ;

  while(1){
		if( displayUpdateNeed ){
			read_time( ( date_time * ) & time );
			if( time.minute != minute ){// check settings one time per minute only
				minute = time.minute;
				checkTimeSettings();
				displayOutputs();
			}

			if( time.hour != hour ){// check settings one time per hour only
				hour = time.hour;
				displayDate();
			}
	
			displayTime();
			displayTemp();
			displayInputs();

			displayUpdateNeed = 0;
	}
		if( dataUpdateNeed ){
			DS18B20Convert();
			temp = DS18B20CalculateTemperature();
			dataUpdateNeed = 0;
	}
  }
}

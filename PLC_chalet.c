#include "system.h"

volatile unsigned char displayUpdateNeed = 0;
volatile unsigned char WebServerReady = 0;
volatile unsigned char usartData = 0;
volatile date_time time;

const unsigned char monthes[][3] = {{22,12,3},{19,6,3},{11,1,15},{1,14,15},{11,1,8},{7,21,12},{7,21,10},{1,3,4},{16,6,12},{13,9,17},{12,13,22},{5,6,9}};
const unsigned char days[][3] = {{3,16,9},{14,13,12},{3,17,15},{16,15,5},{20,17,3},{14,22,17},{16,18,2}};

__EEPROM_DATA(0x11,0x09,0x31,0xFF,0x10,0x09,0x34,0xFF);
__EEPROM_DATA(0x21,0x09,0x32,0xFF,0x20,0x09,0x33,0xFF);
__EEPROM_DATA(0x31,0x09,0x52,0xFF,0x30,0x09,0x54,0xFF);
__EEPROM_DATA(0x41,0x09,0x52,0xFF,0x40,0x09,0x54,0xFF);

void main()
{

/*
	time.minute = 0x41;
	time.hour = 0x20;
	time.day = 3;
	time.date = 0x27;
	time.month = 0x02;
	time.year = 0x24;
	write_time( ( date_time * ) & time );
*/

  InitPorts();
  InitSFRS();
  USARTInit(9600);

  RCIF = 0;
  RCIE = 0;
  PEIE = 0;
  GIE  = 1 ;
    
	display_init();
    __delay_ms(100);
    display_clear(0);

  while(1){
		if( displayUpdateNeed ){
			DS18B20Convert();
			read_time( ( date_time * ) & time );
			displayUpdateNeed = 0;
			checkTimeSettings();
			updateDisplay();
	}
  }
}

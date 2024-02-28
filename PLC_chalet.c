#include "system.h"

volatile unsigned char displayUpdateNeed = 0;
volatile unsigned char WebServerReady = 0;
volatile unsigned char usartData = 0;
volatile date_time time;

const unsigned char monthes[][3] = {{22,12,3},{19,6,3},{11,1,15},{1,14,15},{11,1,8},{7,21,12},{7,21,10},{1,3,4},{16,6,12},{13,9,17},{12,13,22},{5,6,9}};
const unsigned char days[][3] = {{14,13,12},{3,17,15},{16,15,5},{20,17,3},{14,22,17},{16,18,2},{3,16,9}};

#define hour 	0x13
#define minute1 0x22
#define minute2 0x23

__EEPROM_DATA(0x11,hour,minute1,0b00000100,0x10,hour,minute2,0b00000100);
__EEPROM_DATA(0x21,hour,minute1,0b00000000,0x20,hour,minute2,0b00000000);
__EEPROM_DATA(0x31,hour,minute1,0b00000000,0x30,hour,minute2,0b00000000);
__EEPROM_DATA(0x41,hour,minute1,0b00000100,0x40,hour,minute2,0b00000100);

void main()
{
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

/*
	read_time( ( date_time * ) & time );
		time.minute = 0x20;
		time.day = 3;
		time.date = 0x28;
		time.month = 0x02;
		time.year = 0x24;
	write_time( ( date_time * ) & time );
*/

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

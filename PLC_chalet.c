#include "system.h"

volatile unsigned char displayUpdateNeeded = 0;
volatile unsigned char dataUpdateNeeded = 0;
volatile unsigned char WebServerReady = 0;
volatile unsigned char usartData = 0;
volatile date_time time;
volatile unsigned long temp;

const unsigned char monthes[][3] = {{22,12,3},{19,6,3},{11,1,15},{1,14,15},{11,1,8},{7,21,12},{7,21,10},{1,3,4},{16,6,12},{13,9,17},{12,13,22},{5,6,9}};
const unsigned char days[][3] = {{14,13,12},{3,17,15},{16,15,5},{20,17,3},{14,22,17},{16,18,2},{3,16,9}};

void main()
{
  unsigned char minute = 0xFF;
  unsigned char hour = 0xFF;
  unsigned char porta, portaChanged = 0;

  InitPorts();
  InitSFRS();
  USARTInit(9600);

  RCIF = 0;
  RCIE = 0;

  PEIE = 0;
  RBIE = 0;
  INTE = 0;
  SSPIE = 0;

read_time( ( date_time * ) & time );

if( 0 ){ // Установка времени
			time.hour = 0x13;
			time.minute = 0x15;
			time.day = 1;
			time.date = 0x20;
			time.month = 0x05;
			time.year = CURRENT_YEAR;
			write_time( ( date_time * ) & time );
}

// Если напряжение отключалось, восстанавливаем состояние выходов
	porta = checkInRangeTimeSettings();
	PORTA = 0b00010000 | porta;

   display_init();
   __delay_ms(100);
   display_clear(0);

  GIE  = 1 ;

  while(1){
		if( displayUpdateNeeded ){
			PORTA = checkBarrelSensor( porta );
			read_time( ( date_time * ) & time );
			if( ( time.minute != minute && time.minute <= 0x59 ) ){// check settings one time per minute only
				minute = time.minute;
				portaChanged = checkTimeSettings( & porta );
			}

			if( time.hour != hour && time.hour <= 0x23 ){// check settings one time per hour only
				hour = time.hour;
				displayDate();
			}
	
			displayTime();
			displayTemp();
			displayInputs();

			if( portaChanged ){
				PORTA = porta;
				portaChanged = 0;
			}

			displayOutputs();
			displayUpdateNeeded = 0;
	
	}// if( displayUpdateNeeded ){

		if( dataUpdateNeeded ){
			DS18B20Convert();
			temp = DS18B20CalculateTemperature();
			dataUpdateNeeded = 0;
		}
	}
}

#include "system.h"

volatile unsigned char displayUpdateNeeded = 0;
volatile unsigned char dataUpdateNeeded = 0;
volatile unsigned char WebServerReady = 0;

volatile unsigned char usartData[ USART_PACKET_SIZE ];
volatile unsigned char usartDataPtr = 0;
volatile unsigned char outputsMode[4];
volatile date_time time;
volatile unsigned long temp;

const unsigned char monthes[][3] = {{22,12,3},{19,6,3},{11,1,15},{1,14,15},{11,1,8},{7,21,12},{7,21,10},{1,3,4},{16,6,12},{13,9,17},{12,13,22},{5,6,9}};
const unsigned char days[][3] = {{14,13,12},{3,17,15},{16,15,5},{20,17,3},{14,22,17},{16,18,2},{3,16,9}};

void main()
{
  unsigned char minute = 0xFF;
  unsigned char hour = 0xFF;
  unsigned char porta = 0, portaChanged = 0;
  unsigned int tempAddr;

  InitPorts();
  InitSFRS();

  read_time( ( date_time * ) & time );
  getOutputsMode();

   display_init();
   __delay_ms(100);
   display_clear(0);

  usartDataPtr = 0;
  dataUpdateNeeded = 1;
  displayUpdateNeeded = 1;
  LED_OFF;

//  USARTInit(9600);
  USARTInit(19200);

  RCIE = 1;

  porta = checkInRangeTimeSettings();
  PORTA = 0x10 | checkBarrelSensor( porta );
	
  GIE  = 1 ;

  while(1){
		if( displayUpdateNeeded ){
			//porta = checkInRangeTimeSettings();
			read_time( ( date_time * ) & time );

			if( ( time.minute != minute && time.minute <= 0x59 ) && portaChanged == 0 ){// check settings one time per minute only
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
				porta = checkOutputsMode( porta );	
				PORTA = 0x10 | checkBarrelSensor( porta );
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

	if( usartDataPtr == USART_PACKET_SIZE ){
		usartDataPtr = 0;

		switch( usartData[0] ){

			case USART_TIME_SETTING : // Установка времени
								time.hour = usartData[4];
								time.minute = usartData[5];
								time.day = usartData[6];
								time.date = usartData[3];
								time.month = usartData[2];
								time.year = usartData[1];
								write_time( ( date_time * ) & time );
								hour = 0xFF;
								minute = 0xFF;
								displayUpdateNeeded = 1;
								portaChanged = 1;
								break;

			case USART_OUTPUTS_MODE_REQUEST : // Запрос на выдачу режима выходов
								getOutputsMode();
								usartData[1] = outputsMode[0];
								usartData[2] = outputsMode[1];
								usartData[3] = outputsMode[2];
								usartData[4] = outputsMode[3];
								sendBufToUsart();
								break;

			case USART_GET_OUTPUTS_STATE: // Запрос на выдачу состояния выходов
								getOutputsMode();
								usartData[0] = outputsMode[0];
								usartData[1] = outputsMode[1];
								usartData[2] = outputsMode[2];
								usartData[3] = outputsMode[3];
								usartData[4] = RA0;
								usartData[5] = RA1;
								usartData[6] = RA2;
								usartData[7] = RA3;
								sendBufToUsart();
								break;

			case USART_OUTPUTS_ALARM_SETTINGS_REQUEST : // Запрос на выдачу уставок выходов
								tempAddr = usartData[1]; // Адрес банка уставок
								usartData[7] = tempAddr;
								usartData[0] = eeprom_read( tempAddr ++ );
								usartData[1] = eeprom_read( tempAddr ++ );
								usartData[2] = eeprom_read( tempAddr ++ );
								usartData[3] = eeprom_read( tempAddr ++ );
								usartData[4] = eeprom_read( tempAddr ++ );
								usartData[5] = eeprom_read( tempAddr ++ );			
								usartData[6] = eeprom_read( tempAddr ++ );
								sendBufToUsart();
								break;

			case USART_OUTPUT_ALARM_SETTINGS_SET : // Запись уставок выходов
								tempAddr = usartData[1]; // Адрес банка уставки
								eeprom_write( tempAddr ++, usartData[2] );
								eeprom_write( tempAddr ++, usartData[3] );
								eeprom_write( tempAddr ++, usartData[4] );
								eeprom_write( tempAddr ++, usartData[5] );
								eeprom_write( tempAddr ++, usartData[6] );
								eeprom_write( tempAddr ++, usartData[7] );
								eeprom_write( tempAddr ++, 0xFF );
								eeprom_write( tempAddr ++, 0xFF );
					
								displayUpdateNeeded = 1;
								portaChanged = 1;
								break;

			case USART_OUTPUTS_MODE_SET : // Изменение режима управления выходами
								putOutputsMode();
					  			porta = checkInRangeTimeSettings();
					 			PORTA = 0x10 | checkBarrelSensor( porta );
								break;

			case USART_DISPLAY_REFRESH : // Обновить дисплей
							    display_clear(0);
					  			minute = 0xFF;
					  			hour = 0xFF;
								break;

			case USART_DISPLAY_ON :// Включить дисплей
							    display_on();
								portaChanged = 1;
								displayUpdateNeeded = 1;
					  			minute = 0xFF;
					  			hour = 0xFF;
								break;

			case USART_DISPLAY_OFF : // Вылючить дисплей
								display_off();
								break;

			case USART_GET_TIME_REQUEST : // Запрос времени
								usartData[0] = time.year;
								usartData[1] = time.month;
								usartData[2] = time.date;
								usartData[3] = time.hour;
								usartData[4] = time.minute;
								usartData[5] = time.day;
								sendBufToUsart();
								break;
		}// switch( usartData[0] ){
		LED_OFF;		
	  }// if( usartDataPtr == USART_PACKET_SIZE ){
	}// while(1){
}// void main()

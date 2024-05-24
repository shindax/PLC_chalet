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

  InitPorts();
  InitSFRS();

  read_time( ( date_time * ) & time );
  getOutputsMode();

// Если напряжение отключалось, восстанавливаем состояние выходов
	portaCheck( & porta );

   display_init();
   __delay_ms(100);
   display_clear(0);

  usartDataPtr = 0;
  dataUpdateNeeded = 1;
  LED_OFF;

  USARTInit(9600);
  RCIE = 1;
  GIE  = 1 ;

  while(1){
		if( displayUpdateNeeded ){
			portaCheck( & porta );
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

	if( usartDataPtr == USART_PACKET_SIZE ){
		usartDataPtr = 0;
		if( usartData[0] == USART_TIME_SETTING ){ // Установка времени
			time.hour = usartData[4];
			time.minute = usartData[5];
			time.day = usartData[6];
			time.date = usartData[3];
			time.month = usartData[2];
			time.year = usartData[1];
			write_time( ( date_time * ) & time );
			minute = hour = portaCheck( & porta );
			LED_OFF;

		}// if( usartData[0] == USART_TIME_SETTING ){ // Установка времени

		if( usartData[0] == USART_OUTPUTS_MODE_REQUEST ){ // Запрос на выдачу режима выходов
			usartData[0] = USART_OUTPUTS_MODE_RESPONSE;
			usartData[1] = outputsMode[0];
			usartData[2] = outputsMode[1];
			usartData[3] = outputsMode[2];
			usartData[4] = outputsMode[3];
			sendBufToUsart();
			LED_OFF;
		}// if( usartData[0] == USART_OUTPUTS_MODE_REQUEST  ){ // Запрос на выдачу состояний выходов

		if( usartData[0] == USART_OUTPUTS_MODE_SET ){ // Изменение режима управления выходами
			putOutputsMode();
			minute = hour = portaCheck( & porta );
			LED_OFF;
		}// if( usartData[0] == USART_OUTPUTS_SET ){ // Изменение состояний выходов
	  }// if( usartDataPtr == USART_PACKET_SIZE ){
	}// while(1){
}




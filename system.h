#ifndef SYSTEM_H
#define	SYSTEM_H

#include <xc.h>
#include "i2c.h"
#include "rtc.h"
#include "usart.h"
#include "sh1106.h"
#include "18b20.h"
#include "24c32.h"
#include "display_data.h"

#define _XTAL_FREQ	4096000UL
#define I2C_SPEED	100000

#define CURRENT_YEAR	0x24

#define LED			RA4
#define LED_ON		LED = 0
#define LED_OFF		LED = 1

#define TMR0_PRELOAD 176

#define FILL_BARREL_OUTPUT	RA0
#define OUTPUT_2			RA1
#define OUTPUT_3			RA2
#define OUTPUT_4			RA3

#define INPUT_1				!RB7
#define INPUT_2				!RB6
#define INPUT_3				!RB5
#define INPUT_4				!RB4
#define INPUT_5				!RB3
#define BARREL_IS_FULL		!RB2
#define INPUT_7				!RB1
#define INPUT_8				!RB0

#define	MON			0x01
#define	TUE			0x02
#define	WED			0x04
#define	THU			0x08
#define	FRI			0x10
#define	SAT			0x20
#define	SUN			0x40
#define EVERYDAY	MON|TUE|WED|THU|FRI|SAT|SUN
#define	DISABLED	0x00

#define	FILL_BARREL_1	0x01
#define	WATERING_2		0x02
#define	FAN_3			0x03
#define	UV_LAMP_4		0x04

#define INPUT_SYMBOL_TABLE_SHIFT		23
#define OUTPUT_SYMBOL_TABLE_SHIFT		53
#define COLON_SYMBOL					25
#define DOT_SYMBOL						37
#define BOLD_DOT_SYMBOL					49
#define DEGREE_SYMBOL					36
#define BOLD_DEGREE_SYMBOL				48
#define SPACE_SYMBOL					0

#define MANUAL_ON						50
#define MANUAL_OFF						51
#define MANUAL_ON_DISABLED				52

#define DIGIT_TABLE_SHIFT				26
#define BOLD_DIGIT_TABLE_SHIFT			38
#define SYMBOL_SIZE						16

#define INPUT_LINE						0
#define TIME_TEMP_LINE					1
#define DATE_LINE						2
#define OUTPUT_LINE						3

#define USART_PACKET_SIZE				8
#define USART_OUTPUTS_MODE_REQUEST				0x44
#define USART_OUTPUTS_MODE_SET					0x33
#define USART_TIME_SETTING						0x55
#define USART_OUTPUTS_ALARM_SETTINGS_REQUEST	0x66
#define USART_OUTPUT_ALARM_SETTINGS_SET			0x77
#define USART_DISPLAY_ON						0x88
#define USART_DISPLAY_OFF						0x99
#define USART_DISPLAY_REFRESH					0xCC
#define USART_GET_OUTPUTS_STATE					0xAA
#define USART_GET_TIME_REQUEST          		0xBB

extern volatile unsigned char displayUpdateNeeded;
extern volatile unsigned char dataUpdateNeeded;
extern volatile unsigned char WebServerReady;
extern volatile unsigned char usartData[];
extern volatile unsigned char outputsMode[];
extern volatile unsigned char usartDataPtr;
extern volatile date_time time;
extern volatile unsigned long temp;

extern const unsigned char monthes[][3];
extern const unsigned char days[][3];

void InitPorts( void );
void InitSFRS( void );
unsigned char checkTimeSettings( unsigned char * );
unsigned char ds18b20_start( void );
void ds18b20_write_bit( unsigned char );
void ds18b20_write_byte( unsigned char );
unsigned char ds18b20_read_bit( void );
unsigned char ds18b20_read_byte( void );
unsigned char ds18b20_read( unsigned int * );
void readTemperature( void );
void displayOutputs( void );
void displayInputs( void );
void displayTime( void );
void displayTemp( void );
void displayDate( void );
void getData( void );
unsigned char getDisplayableDigit( unsigned char digit );
unsigned char checkInRangeTimeSettings( void );
unsigned char checkBarrelSensor( unsigned char );
void sendBufToUsart( void );
void updateOutputsMode( void );
unsigned char portaCheck( unsigned char * );
void getOutputsMode( void );
void putOutputsMode( void );
unsigned char checkOutputsMode( unsigned char );

__CONFIG ( XT & WDTDIS & PWRTEN & BORDIS & LVPDIS & DUNPROT & WRTDIS & DEBUGDIS & UNPROTECT);

#endif	/* SYSTEM_H */


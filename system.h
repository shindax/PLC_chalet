#ifndef SYSTEM_H
#define	SYSTEM_H

#include <xc.h>
#include "i2c.h"
#include "rtc.h"
#include "usart.h"
#include "sh1106.h"
#include "18b20.h"

#define _XTAL_FREQ	4096000UL

#define HEART_BEAT	RA4
#define TMR0_PRELOAD 176

#define OUTPUT_1	RA0
#define OUTPUT_2	RA1
#define OUTPUT_3	RA2
#define OUTPUT_4	RA3

#define INPUT_1		RB7
#define INPUT_2		RB6
#define INPUT_3		RB5
#define INPUT_4		RB4
#define INPUT_5		RB3
#define INPUT_6		RB2
#define INPUT_7		RB1
#define INPUT_8		RB0

#define INPUT_LINE		0
#define TIME_TEMP_LINE	1
#define DATE_LINE		2
#define OUTPUT_LINE		3

extern volatile unsigned char displayUpdateNeed;
extern volatile unsigned char WebServerReady;
extern volatile unsigned char usartData;
extern volatile date_time time;

extern const unsigned char monthes[][3];
extern const unsigned char days[][3];

extern unsigned int raw_temp;
extern unsigned char temp[];

void InitPorts( void );
void InitSFRS( void );
void updateDisplay( void );
void checkTimeSettings( void );
void getEepromTimeSetting( unsigned char addr, date_time * _time );

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

__CONFIG ( XT & WDTDIS & PWRTEN & BORDIS & LVPDIS & DUNPROT & WRTDIS & DEBUGDIS & UNPROTECT);

#endif	/* SYSTEM_H */

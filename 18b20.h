#ifndef DS18B20_H
#define	DS18B20_H
#include <xc.h>

#define DATA            	RC2
#define DATA_out        	TRISC2 = 0
#define DATA_in         	TRISC2 = 1

#define nop()           	asm("nop");

#define READ_ROM			0x33
#define SKIP_ROM			0xCC
#define MATCH_ROM			0x55
#define READ_SCRATCHPAD		0xBE
#define WRITE_SCRATCHPAD	0x4E
#define CONVERT_TEMP		0x44
#define ALARM_SEARCH		0xEC

// Scratchpad locations
#define TEMP_LSB        0
#define TEMP_MSB        1
#define HIGH_ALARM_TEMP 2
#define LOW_ALARM_TEMP  3
#define CONFIGURATION   4
#define INTERNAL_BYTE   5
#define COUNT_REMAIN    6
#define COUNT_PER_C     7
#define SCRATCHPAD_CRC  8

unsigned char DS18B20Reset(void);
unsigned char DS18B20ReadBit(void);
unsigned char DS18B20ReadByte(void);
void DS18B20WriteByte(unsigned char tx_buff);
unsigned char DS18B20CalCRC( unsigned char * codes );
void DS18B20Init( void );
void DS18B20Convert( void );
unsigned long DS18B20CalculateTemperature( void );
void DS18B20ReadScratchpad( unsigned char * scratch );

#endif
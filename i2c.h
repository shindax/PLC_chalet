#ifndef i2c_H
#define	i2c_H

#include <xc.h>
#define _XTAL_FREQ 4096000UL

#define I2C_SPEED	100000

#define ACK			1
#define NOACK		0

#define I2C_READ	1
#define I2C_WRITE	0

void i2c_Init(const unsigned long);
void i2c_Wait( void );
void i2c_Start( void );
void i2c_RepeatedStart( void );
void i2c_Stop( void );
void i2c_Write( unsigned short );
unsigned short i2c_Read( unsigned short );

#endif
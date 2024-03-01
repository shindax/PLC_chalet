#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H
#include <xc.h>

#define ACK			1
#define NOACK		0

#define I2C_READ	1
#define I2C_WRITE	0

void i2c_wait(void);
void i2c_init(unsigned long);
void i2c_stop(void);
void i2c_start(void);
void i2c_write(unsigned char data);
unsigned char i2c_read(unsigned char);
void i2c_repeated_start( void );

#endif	/* XC_HEADER_TEMPLATE_H */

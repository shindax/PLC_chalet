#include "system.h"

unsigned char ext_eeprom_read_byte( unsigned int addr )
{
	unsigned char byte = 0xFF;
	i2c_init( I2C_SPEED );
    i2c_start();           				
    i2c_write( AT24C32ADDR | I2C_WRITE);
    i2c_write( addr >> 8 );
    i2c_write( addr );
	i2c_repeated_start();
    i2c_write( AT24C32ADDR | I2C_READ );
	byte = i2c_read(NOACK);
    i2c_stop();            				// stop I2C
	return byte;
}

void ext_eeprom_write_byte( unsigned int addr, unsigned char byte )
{
	i2c_init( I2C_SPEED );
    i2c_start();           				
    i2c_write( AT24C32ADDR | I2C_WRITE );
    i2c_write( addr >> 8 );
    i2c_write( addr );
    i2c_write( byte );
    i2c_stop();            				// stop I2C
}
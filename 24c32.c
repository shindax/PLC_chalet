#include "system.h"

unsigned char ext_eeprom_read_byte( unsigned int addr )
{
	unsigned char byte = 0xFF;
	i2c_Init( I2C_SPEED );
    i2c_Start();           				
    i2c_Write( AT24C32ADDR | I2C_WRITE);
    i2c_Write( addr >> 8 );
    i2c_Write( addr );
	i2c_RepeatedStart();
    i2c_Write( AT24C32ADDR | I2C_READ );
	byte = i2c_Read(NOACK);
    i2c_Stop();            				// stop I2C
	return byte;
}

void ext_eeprom_write_byte( unsigned int addr, unsigned char byte )
{
	i2c_Init( I2C_SPEED );
    i2c_Start();           				
    i2c_Write( AT24C32ADDR | I2C_WRITE );
    i2c_Write( addr >> 8 );
    i2c_Write( addr );
    i2c_Write( byte );
    i2c_Stop();            				// stop I2C
}
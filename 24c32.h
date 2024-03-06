#ifndef AT_24C32_H
#define	AT_24C32_H
#include <xc.h>

#include "i2c.h"

#define AT24C32ADDR		0xA0

unsigned char ext_eeprom_read_byte( unsigned int addr );
void ext_eeprom_write_byte( unsigned int addr, unsigned char byte);

#endif
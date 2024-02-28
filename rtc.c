#include "system.h"

void read_time( date_time * time )
{
	i2c_init(100000);
    i2c_start();           				// start I2C
    i2c_write(0xD0);       				// RTC chip address
    i2c_write(0);          				// send register address
    i2c_repeated_start();  				// restart I2C
    i2c_write(0xD1);       				// initialize data read
    time -> second = i2c_read(ACK);  	// read seconds from register 0
    time -> minute = i2c_read(ACK);  	// read minutes from register 1
    time -> hour   = i2c_read(ACK);  	// read hour from register 2
    time -> day    = i2c_read(ACK);     // read day from register 3 (not used)
    time -> date   = i2c_read(ACK);  	// read date from register 4
    time -> month  = i2c_read(ACK);  	// read month from register 5
    time -> year   = i2c_read(NOACK);	// read year from register 6
    i2c_stop();            				// stop I2C
}

void write_time( date_time * time )
{
  i2c_init(100000);
  i2c_start();         		// start I2C
  i2c_write(0xD0);     		// RTC chip address
  i2c_write(0);        		// send register address
  i2c_write(0);        		// reset seconds and start oscillator
  i2c_write(time -> minute);// write minute value to RTC chip
  i2c_write(time -> hour);  // write hour value to RTC chip
  i2c_write(time -> day);   // write day value (not used)
  i2c_write(time -> date);  // write date value to RTC chip
  i2c_write(time -> month); // write month value to RTC chip
  i2c_write(time -> year);  // write year value to RTC chip
  i2c_stop();          		// stop I2C
}
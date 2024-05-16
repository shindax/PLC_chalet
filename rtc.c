#include "system.h"

void read_time( date_time * time )
{
	i2c_Init( I2C_SPEED );
    i2c_Start();           				// start I2C
    i2c_Write( DS1307ADDR | I2C_WRITE );       				// RTC chip address
    i2c_Write(0);          				// send register address
    i2c_RepeatedStart();  				// restart I2C
    i2c_Write( DS1307ADDR | I2C_READ );       				// initialize data read
    time -> second = i2c_Read(ACK);  	// read seconds from register 0
    time -> minute = i2c_Read(ACK);  	// read minutes from register 1
    time -> hour   = i2c_Read(ACK);  	// read hour from register 2
    time -> day    = i2c_Read(ACK);     // read day from register 3 (not used)
    time -> date   = i2c_Read(ACK);  	// read date from register 4
    time -> month  = i2c_Read(ACK);  	// read month from register 5
    time -> year   = i2c_Read(NOACK);	// read year from register 6
    i2c_Stop();            				// stop I2C
}

void write_time( date_time * time )
{
  i2c_Init( I2C_SPEED );
  i2c_Start();         		// start I2C
  i2c_Write( DS1307ADDR | I2C_WRITE );     		// RTC chip address
  i2c_Write(0);        		// send register address
  i2c_Write(0);        		// reset seconds and start oscillator
  i2c_Write(time -> minute);// write minute value to RTC chip
  i2c_Write(time -> hour);  // write hour value to RTC chip
  i2c_Write(time -> day);   // write day value (not used)
  i2c_Write(time -> date);  // write date value to RTC chip
  i2c_Write(time -> month); // write month value to RTC chip
  i2c_Write(time -> year);  // write year value to RTC chip
  i2c_Stop();          		// stop I2C
}
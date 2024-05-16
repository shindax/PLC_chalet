#include "i2c.h"

//Initialize I2C Module as Master
void i2c_Init(const unsigned long speed)
{
  SSPCON = 0b00101000;            				//SSP Module as Master
  SSPCON2 = 0;
  SSPADD = ( _XTAL_FREQ / ( 4 * speed ) ) - 1; 	//Setting Clock Speed
  SSPSTAT = 0;
  TRISC3 = 1;                   				//Setting as input as given in datasheet
  TRISC4 = 1;                   				//Setting as input as given in datasheet
}

//For Waiting
void i2c_Wait( void )
{
  while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F)); //Transmit is in progress
}

//Start Condition
void i2c_Start( void )
{
  i2c_Wait();    
  SEN = 1;             //Initiate start condition
}

//Repeated Start
void i2c_RepeatedStart( void )
{
  i2c_Wait();
  RSEN = 1;           	//Initiate repeated start condition
}

//Stop Condition
void i2c_Stop( void )
{
  i2c_Wait();
  PEN = 1;           	//Initiate stop condition
}

//Write Data
void i2c_Write( unsigned short data )
{
  i2c_Wait();
  SSPBUF = data;        //Write data to SSPBUF
}

// Read Data
unsigned short i2c_Read( unsigned short ack )
{
  unsigned short data;
  i2c_Wait();
  RCEN = 1;
  i2c_Wait();
  data = SSPBUF;      	//Read data from SSPBUF
  i2c_Wait();
  ACKDT = ack ? 0 : 1;  //Acknowledge bit
  ACKEN = 1;          	//Acknowledge sequence
  return data;
}

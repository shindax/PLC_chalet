#include "system.h"

unsigned char DS18B20Reset(void)
{
	unsigned char res = 0;
	DATA_out;
	DATA = 0;
	__delay_us(250);
	__delay_us(250);
	DATA = 1;
	__delay_us(30);
	DATA_in;
	nop();
	if (DATA)
	    res = 1;
	__delay_us(250);
	__delay_us(200);
	DATA_out;
	return res;
}

unsigned char DS18B20ReadBit(void)
{
	unsigned char res ;
	DATA_out;
	DATA=1;
	DATA=0;
	nop();
	DATA_in;
	DATA=1;
	res=DATA;
	__delay_us(10);
	return res;
}


unsigned char DS18B20ReadByte(void)
{
	unsigned char data;
	unsigned char i;
	data = 0;
	for( i = 0 ; i <= 7 ; i++ )
	        data += DS18B20ReadBit() << i;
	return data;
}



void DS18B20WriteByte(unsigned char tx_buff)
{
unsigned char i;
for ( i = 0 ; i < 8 ; i++ )
if ((tx_buff>>i)&0x01)
{
DATA = 1;
nop();
DATA = 0;
__delay_us(10);
DATA = 1;
__delay_us(100);
DATA = 1;
}
else
{
DATA = 1 ;
nop();
DATA = 0;
__delay_us(10);
DATA = 0;
__delay_us(100);
DATA = 1;
}
}


unsigned char DS18B20CalCRC( unsigned char * scratch )
{
        unsigned char n , data , count , crc = 0 , i ;

        for( i = 0 ; i < 8 ; i ++ )
        {
	        count = 8 ;
	        data = scratch[i] ;
	        do
	        {
	        	n = 1 & ( data ^ crc );
	        	crc >>=  1;
	        	data >>= 1;
	          	if(n)
	             	crc^=0x8C;
	         }while(--count);
        }

return crc;

}

void DS18B20ReadScratchpad( unsigned char * scratch )
{
	for( char i = 0 ; i < 9 ; i ++ )
       scratch[i] = DS18B20ReadByte();
}

void DS18B20Init( void )
{
    DS18B20Reset();
    DS18B20WriteByte( SKIP_ROM );
    DS18B20WriteByte( WRITE_SCRATCHPAD );
    DS18B20WriteByte(0);
    DS18B20WriteByte(0);
    DS18B20WriteByte(0b00011111);
    DS18B20Reset();
}

void DS18B20Convert( void )
{
    DS18B20Reset();
    DS18B20WriteByte( SKIP_ROM );
    DS18B20WriteByte( CONVERT_TEMP );
    DS18B20Reset();
}

unsigned long DS18B20CalculateTemperature( void )
{
		unsigned long fpTemperature = 0;
		unsigned long neg = 0x0;
        unsigned char scratchPad[9], crc;
        
        DS18B20WriteByte( SKIP_ROM );
        DS18B20WriteByte( READ_SCRATCHPAD );
		DS18B20ReadScratchpad( scratchPad );
        crc = DS18B20CalCRC( scratchPad );

        if( crc == scratchPad[8])
        {
			// looking thru the spec sheets of all supported devices, bit 15 is always the signing bit
			// Detected if signed
			if (scratchPad[TEMP_MSB] & 0x80)
				neg = 0xFFF80000;
		
			fpTemperature = ((( unsigned long ) scratchPad[TEMP_MSB]) << 11)
				                | 	((( unsigned long ) scratchPad[TEMP_LSB]) << 3)
				                | 	neg;
		
			/*
			 DS1820 and DS18S20 have a 9-bit temperature register.
		
			 Resolutions greater than 9-bit can be calculated using the data from
			 the temperature, and COUNT REMAIN and COUNT PER °C registers in the
			 scratchpad.  The resolution of the calculation depends on the model.
		
			 While the COUNT PER °C register is hard-wired to 16 (10h) in a
			 DS18S20, it changes with temperature in DS1820.
		
			 After reading the scratchpad, the TEMP_READ value is obtained by
			 truncating the 0.5°C bit (bit 0) from the temperature data. The
			 extended resolution temperature can then be calculated using the
			 following equation:
		
			                                  COUNT_PER_C - COUNT_REMAIN
			 TEMPERATURE = TEMP_READ - 0.25 + --------------------------
			                                         COUNT_PER_C
		
			 Hagai Shatz simplified this to integer arithmetic for a 12 bits
			 value for a DS18S20, and James Cameron added legacy DS1820 support.
		
			 See - http://myarduinotoy.blogspot.co.uk/2013/02/12bit-result-from-ds18s20.html
			 */
		
				fpTemperature = (((fpTemperature & 0xfff0) << 3) - 32
				                + (((scratchPad[COUNT_PER_C] - scratchPad[COUNT_REMAIN]) << 7)
				                   / scratchPad[COUNT_PER_C])) | neg;
		
			return fpTemperature;

        }
        else
    
	return 0xFFFFFFFF ;
}

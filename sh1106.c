#include "sh1106.h"
#include "i2c.h"
#include <string.h>

void display_command(uint8_t command){
    i2c_start();
    i2c_write(SSD1106_I2C_ADDR);
    i2c_write(0x00);
    i2c_write(command);
    i2c_stop();
}

void display_data(uint8_t data){
    i2c_start();
    i2c_write(SSD1106_I2C_ADDR);
    i2c_write(0x40);
    i2c_write(data);
    i2c_stop();
}



void display_init(void){
    i2c_init(100000);   	/*Display Off*/
    display_command(0xAE);  /*Set Lower Column Address*/ 
    display_command(0x02);  /*set higher column address*/  
    display_command(0x10);  /*set display start line*/ 
    display_command(0x40);  /*set page address*/ 
    display_command(0xB0);  /*contrast control*/  
    display_command(0x81);  /*set segment re-map*/
    display_command(0xFF);   
    display_command(0xA1);  /*normal - reverse*/   
    display_command(0xA6);  /*multiplex ratio*/  
    display_command(0xA8);  /*1/64 duty*/  
    display_command(0x3F);  /*enable charge pump*/  
    display_command(0xAD);  /*set Vpp 8V 0x30 - 0x33*/
    display_command(0x8B);    
    display_command(0x32);  /*COM scan direction*/    
    display_command(0xC8);  /*set display off-set*/    
    display_command(0xD3);
    display_command(0x00);  /*0x20*/
    display_command(0xD5);  /*set oscillator division*/
    display_command(0x80);
    display_command(0xD9);  /*set pre-charge period*/
    display_command(0x1F);  /*0x22*/
    display_command(0xDA);  /*set COM pin*/
    display_command(0x12);
    display_command(0xDB);  //set VCOMH
    display_command(0x40);
    display_command(0xAF);  //display ON
}

void display_xy(uint8_t x, uint8_t y){
    display_command(YLevel+y);
    display_command((((x+2)&0xF0)>>4)|0x10);
    display_command((x+2)&0x0F);
}

/*
void display_char(char ch){
    if((ch<32)||(ch>127)) ch=' ';
    const uint8_t *base = &font_1[ch-32][0];
    uint8_t bytes[9];
    bytes[0]=0x40;
    memmove(bytes+1,base,8);
    
    i2c_start();
    i2c_write(SSD1106_I2C_ADDR);
    i2c_write(0x40);
    for(uint8_t i=1;i<=8;i++){ i2c_write(bytes[i]);}
    i2c_stop();
}
*/

void display_clear(uint8_t data){
    for(uint8_t i=0;i<PAGE_SIZE;i++){
        display_command(YLevel+i);
        display_command(XLevelL);
        display_command(XLevelH);
        for(uint8_t n=0;n<WIDTH;n++){
            display_data(data);
            //while(SSPCONbits.WCOL==1);          
        }
    }
}

void display_text(char *txt){
    /*
    while(*txt){
        display_char(*txt++);
        while(SSPCONbits.WCOL==1);
    }
    */
    for(uint8_t i=0;i<strlen(txt);i++)
        display_char(txt[i]);
}

void display_char_8x16(uint8_t x, uint8_t y, char ch){
    y=y*2;
    display_xy(x,y);
    uint16_t temp;
//    ch=ch-32;
    temp=ch*16;
    uint8_t i=0;
    for(i=0;i<8;i++)
        display_data(font_8x16[temp+i]);
    display_xy(x,y+1);
    for(i=8;i<16;i++)
        display_data(font_8x16[temp+i]);
}

void display_text_8x16(uint8_t x, uint8_t y, unsigned char *txt, unsigned char len )
{
	unsigned char i = 0;
	while( i < len ){
    	display_char_8x16(x,y,txt[i++]);
		x += 8;
	}
}
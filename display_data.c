#include "system.h"

void displayOutputs( void )
{
	unsigned char pos = 40;
	display_char_8x16( pos				 , OUTPUT_LINE, INPUT_OUTPUT_SYMBOL_TABLE_SHIFT + FILL_BARREL_OUTPUT );
	display_char_8x16( pos += SYMBOL_SIZE, OUTPUT_LINE, INPUT_OUTPUT_SYMBOL_TABLE_SHIFT + OUTPUT_2 );
	display_char_8x16( pos += SYMBOL_SIZE, OUTPUT_LINE, INPUT_OUTPUT_SYMBOL_TABLE_SHIFT + OUTPUT_3 );
	display_char_8x16( pos += SYMBOL_SIZE, OUTPUT_LINE, INPUT_OUTPUT_SYMBOL_TABLE_SHIFT + OUTPUT_4 );
}

void displayInputs( void )
{
	unsigned char pos = 0;
	display_char_8x16( pos				 , INPUT_LINE, INPUT_OUTPUT_SYMBOL_TABLE_SHIFT + INPUT_1 );
	display_char_8x16( pos += SYMBOL_SIZE, INPUT_LINE, INPUT_OUTPUT_SYMBOL_TABLE_SHIFT + INPUT_2 );
	display_char_8x16( pos += SYMBOL_SIZE, INPUT_LINE, INPUT_OUTPUT_SYMBOL_TABLE_SHIFT + INPUT_3 );
	display_char_8x16( pos += SYMBOL_SIZE, INPUT_LINE, INPUT_OUTPUT_SYMBOL_TABLE_SHIFT + INPUT_4 );
	display_char_8x16( pos += SYMBOL_SIZE, INPUT_LINE, INPUT_OUTPUT_SYMBOL_TABLE_SHIFT + INPUT_5 );
	display_char_8x16( pos += SYMBOL_SIZE, INPUT_LINE, INPUT_OUTPUT_SYMBOL_TABLE_SHIFT + BARREL_IS_FULL  );
	display_char_8x16( pos += SYMBOL_SIZE, INPUT_LINE, INPUT_OUTPUT_SYMBOL_TABLE_SHIFT + INPUT_7 );
	display_char_8x16( pos += SYMBOL_SIZE, INPUT_LINE, INPUT_OUTPUT_SYMBOL_TABLE_SHIFT + INPUT_8 );
}

void displayTime( void )
{
	unsigned char pos = 0;

	if( time.hour > 0x23 || time.minute > 0x59 || time.second > 0x59 )
		return;

	display_char_8x16( pos, OUTPUT_LINE, 0 );
	display_char_8x16( pos += SYMBOL_SIZE, OUTPUT_LINE, 0 );
	pos = 24;
	display_char_8x16( pos, OUTPUT_LINE, 0 );

	pos = 12;
	display_char_8x16( pos, TIME_TEMP_LINE, getDisplayableDigit(( unsigned char )( time.hour >> 4 )) );
	display_char_8x16( pos += 8, TIME_TEMP_LINE, getDisplayableDigit( ( unsigned char ) ( time.hour & 0x0F )));
	display_char_8x16( pos += 8, TIME_TEMP_LINE, COLON_SYMBOL );
	display_char_8x16( pos += 8, TIME_TEMP_LINE, getDisplayableDigit( ( unsigned char )( time.minute >> 4 )));
	display_char_8x16( pos += 8, TIME_TEMP_LINE, getDisplayableDigit( ( unsigned char )( time.minute & 0x0F )));
	display_char_8x16( pos += 8, TIME_TEMP_LINE, COLON_SYMBOL );
	display_char_8x16( pos += 8, TIME_TEMP_LINE, getDisplayableDigit( ( unsigned char )( time.second >> 4 )));
	display_char_8x16( pos += 8, TIME_TEMP_LINE, getDisplayableDigit( ( unsigned char )( time.second & 0x0F )));
}

void displayTemp( void )
{
	unsigned char pos = 80;
	unsigned int whole = temp / 1000;
    unsigned int fraction = temp % 1000;

	display_inv_char_8x16( pos , TIME_TEMP_LINE, ( unsigned char ) SPACE_SYMBOL);
	display_inv_char_8x16( pos += 8, TIME_TEMP_LINE,      getDisplayableBoldDigit( ( unsigned char )( whole / 10 )));
	display_inv_char_8x16( pos += 8, TIME_TEMP_LINE, getDisplayableBoldDigit( ( unsigned char )( whole % 10 )));
	display_inv_char_8x16( pos += 8, TIME_TEMP_LINE, ( unsigned char ) BOLD_DOT_SYMBOL ); // .
	display_inv_char_8x16( pos += 8, TIME_TEMP_LINE, getDisplayableBoldDigit( ( unsigned char )( fraction / 100 )));
	display_inv_char_8x16( pos += 8, TIME_TEMP_LINE, ( unsigned char ) BOLD_DEGREE_SYMBOL);// deg
	display_char_8x16( pos += 8, TIME_TEMP_LINE, ( unsigned char ) SPACE_SYMBOL);
}

unsigned char getDisplayableDigit( unsigned char digit )
{
	return digit < 10 ? DIGIT_TABLE_SHIFT + digit : SPACE_SYMBOL;
}

unsigned char getDisplayableBoldDigit( unsigned char digit )
{
	return digit < 10 ? BOLD_DIGIT_TABLE_SHIFT + digit : SPACE_SYMBOL;
}


void displayDate( void )
{
	unsigned char i, pos = 0;
	display_char_8x16( pos	   , DATE_LINE, ( time.date >> 4 ) == 0 ? ( unsigned char ) SPACE_SYMBOL : getDisplayableDigit( ( unsigned char )( time.date >> 4 )));
	display_char_8x16( pos += 8, DATE_LINE, getDisplayableDigit( ( unsigned char )( time.date & 0x0F )));
	display_char_8x16( pos += 8, DATE_LINE, ( unsigned char ) SPACE_SYMBOL );
	
	i = time.month - 1;
	display_char_8x16( pos += 8, DATE_LINE, ( unsigned char ) monthes[i][0] );
	display_char_8x16( pos += 8, DATE_LINE, ( unsigned char ) monthes[i][1] );
	display_char_8x16( pos += 8, DATE_LINE, ( unsigned char ) monthes[i][2] );

	display_char_8x16( pos += 8, DATE_LINE, ( unsigned char ) SPACE_SYMBOL );

	i = time.day - 1;
	display_char_8x16( pos += 8, DATE_LINE, ( unsigned char ) days[i][0] );
	display_char_8x16( pos += 8, DATE_LINE, ( unsigned char ) days[i][1] );
	display_char_8x16( pos += 8, DATE_LINE, ( unsigned char ) days[i][2] );

	display_char_8x16( pos += 8, DATE_LINE, ( unsigned char ) SPACE_SYMBOL );
	display_char_8x16( pos += 8, DATE_LINE, ( unsigned char ) 28 ); // 2
	display_char_8x16( pos += 8, DATE_LINE, ( unsigned char ) 26 ); // 0

	display_char_8x16( pos += 8, DATE_LINE, getDisplayableDigit( ( unsigned char )( time.year >> 4 )));
	display_char_8x16( pos += 8, DATE_LINE, getDisplayableDigit( ( unsigned char )( time.year & 0x0F )));
}


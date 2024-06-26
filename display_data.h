#ifndef DISPLAY_DATA_H
#define	DISPLAY_DATA_H

void displayOutputs( void );
void displayInputs( void );
void displayTime( void );
void displayTemp( void );
unsigned char getDisplayableDigit( unsigned char digit );
unsigned char getDisplayableBoldDigit( unsigned char digit );
void displayDate( void );
void getEepromTimeSetting( unsigned char, unsigned char *, unsigned char *, date_time *, date_time * );

#endif
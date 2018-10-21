/*
 * myemLcd.h
 *
 * Created: 18-02-2018 06:08:34
 *  Author: maaply
 */ 


#ifndef MYEMLCD_H_
#define MYEMLCD_H_

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include "myemAvr8Gpio.h"

#ifndef CLEAR
#define CLEAR 0
#endif

#ifndef SET
#define SET 1
#endif

/**************************************************************************
* PLATFORM DEPENTENT PARAMETERS 
**************************************************************************/
#define LCDPORT PORTD		//!< Define here data line used port.
#define LCDPORT_DIR DDRD

#define LCDRS_PORT PORTC
#define LCDRW_PORT PORTC
#define LCDEN_PORT PORTC

#define LCDRS_PORT_DIR DDRC
#define LCDRW_PORT_DIR DDRC
#define LCDEN_PORT_DIR DDRC


#define	LCDRS	PORTC4			//!< LCD reset connected port pin number.
#define LCDRW   PORTC2
#define	LCDEN	PORTC3			//!< LCD Enable connected port pin number.
/**************************************************************************
****************	DEPENTENCY ENDS HERE **********************************
**************************************************************************/


#define LCDEN_HIGH  SETPIN(LCDEN_PORT, LCDEN)
#define LCDEN_LOW   CLEARPIN(LCDEN_PORT, LCDEN)

#define LCDRS_HIGH  SETPIN(LCDRS_PORT, LCDRS)
#define LCDRS_LOW   CLEARPIN(LCDRS_PORT, LCDRS)

#define FIRSTLINE_ADDRESS 0x80
#define SECONDLINE_ADDRESS 0xC0
#define THIRDLINE_ADDRESS 0x94
#define FOURTHLINE_ADDRESS 0xD4

#define FIRSTPOSITION 1

enum
{
    LCD_NOLINE,
    LCD_FIRSTLINE,
    LCD_SECONDLINE,
    LCD_THIRDLINE,
    LCD_FOURLINE,
    LCD_MAXLINE
};


//void lcdInit( void );
//void lcdChar( uint8_t byte );
//void lcdstr( uint8_t * str, uint8_t size );
//void lcdClear( void );


void lcdInit( void );
void lcdWriteStr( uint8_t lcdLine, uint8_t lcdPosi, char *lcdStr );
void lcdWriteChar(uint8_t lcdCharLine, uint8_t lcdCharPosi, char byteToWrite );
void lcdWriteDecimal(uint8_t line, uint8_t posi, uint8_t digi, uint16_t hexNum);
void lcdClear(void);
void lcdMoveCursor( uint8_t line, uint8_t posi );
void lcdCursorOnOff( uint8_t stat );
void lcdDisplayDigit( uint16_t digi );



#endif /* MYEMLCD_H_ */
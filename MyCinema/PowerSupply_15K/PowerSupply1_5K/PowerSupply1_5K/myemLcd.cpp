/* 
 * File:   lcd4pin.h
 * Author: maaply
 * Info : This is source code for lcd display . 
 * The initial file creation wrote for PIC 8 bit family.
 * Need to consider corresponding  pin connections  
 * Created on 25 October, 2017, 10:53 AM
 */
#include <util/delay.h>
#include "myemLcd.h"

#define MSBNIBBLE_MASK (0xF0)
#define LSBNIBBLE_MASK  (0x0F)
#define ASCII_MASK 0x30
#define LEFTSHIFT_NIBBLE(x) (x<<4)
#define DECIMAL_DIV 10

#define LCD_CMD_LENGTH 9

static const uint8_t lcdInitCmd[ LCD_CMD_LENGTH ]= {0x30, 0x30, 0x30, 
                                                    0x20, 
                                                    0x28, 
                                                    0x0C, 
                                                    0x04,
                                                    0x85, 
                                                    0x06 } ;

static void lcdWriteCmd( uint8_t cmd );
static void lcdWriteData( uint8_t data );   
//static void lcdMoveCursor(uint8_t line, uint8_t posi); 

/*******************************************************************************
 * Name     : intiLcd
 * Para     : NULL
 * Return   : NULL
 * Info     : tO INITIALIZE THE 2X16 LCD MODULE
 ******************************************************************************/
void lcdInit( void )
{
    uint8_t i = 0;
    
    LCDPORT_DIR = 0xF0;
    LCDRS_PORT_DIR |= ( 1<< LCDRS );
    LCDRW_PORT_DIR |= ( 1<< LCDRW );
    LCDEN_PORT_DIR |= ( 1<< LCDEN );
    
    
    _delay_ms(200);  // module Power ON time
    
    for( i=0; i<LCD_CMD_LENGTH; i++)
    {
        lcdWriteCmd(lcdInitCmd[i]);
        _delay_ms(5);
    }  
     
    lcdClear();
    return;
}
/*******************************************************************************
 * Name     : lcdWriteStr
 * Para 1   : LCD line number
 * Para 2   : LCD writing position
 * Para 3   : pointer to string want to write
 * Return   : NULL
 * Info     : Write a string in required line and position.
 ******************************************************************************/
void lcdWriteStr( volatile unsigned char lcdLine, uint8_t lcdPosi, char *lcdStr )
{
    //uint8_t temp = 0;
    char *strPtr = lcdStr;
    
    lcdMoveCursor(lcdLine, lcdPosi );   //!< Move Cursor to required place.
    
    do
    {
        lcdWriteData(*strPtr++);    //!< Assumption : Initialize LCD with increment address config 
        
    }while(*strPtr != '\0');        //!< Write data to lcd untill reach null terminator. 
    
    return;
}
/*******************************************************************************
 * Name     : lcdWriteChar
 * Para 1   : LCD line number
 * Para 2   : LCD writing position
 * Para 3   : Byte to be write
 * Return   : NULL
 * Info     : Write a string in required line and position.
 ******************************************************************************/
void lcdWriteChar(uint8_t lcdCharLine, uint8_t lcdCharPosi, char byteToWrite )
{
    lcdMoveCursor(lcdCharLine, lcdCharPosi );   //!< Move Cursor to required place.
    lcdWriteData(byteToWrite);
}
/*******************************************************************************
 * Name     : lcdClear
 * Para     : NULL
 * Return   : NULL
 * Info     : Just clear entire display and bring control to 1st line 1st position.
 ******************************************************************************/

void lcdClear(void)
{
    lcdWriteCmd(0x01);
	_delay_ms(20);
    return;
}

/*******************************************************************************
 * Name     : lcdWriteCmd
 * Para     : comment to write
 * Return   : NULL
 * Info     : write an comment to LCD
 ******************************************************************************/
static void lcdWriteCmd( uint8_t cmd )
{
    uint8_t tempy = cmd;
    
    LCDRS_LOW;
    tempy &= MSBNIBBLE_MASK;		//!<  Mask Lower 4 Bits
    LCDPORT &= LSBNIBBLE_MASK;	//!<  Make No Affect on 0ther Port Pins
    LCDPORT |= tempy;				//!<  Send Higher Nibble to LCDPORT
    LCDEN_HIGH;
    _delay_us(200);
    LCDEN_LOW;
	
    tempy = (uint8_t) LEFTSHIFT_NIBBLE(cmd);	//!< Left Shift Byte Four Times
    tempy &= MSBNIBBLE_MASK;      //!<  Mask Higher 4 Bits
    LCDPORT &= LSBNIBBLE_MASK;	//!<  Make No Affect on 0ther Port Pins
    LCDPORT |= tempy;				//!<  Send Lower Nibble to LCDPORT
    LCDEN_HIGH;
    _delay_us(200);
    LCDEN_LOW;    
     _delay_ms(2);

    return;
}
/*******************************************************************************
 * Name     : lcdWriteData
 * Para     : data to be write in LCD
 * Return   : NULL
 * Info     : Write a data to display in screen in current location
 ******************************************************************************/
static void lcdWriteData( uint8_t data )
{
    uint8_t tempy = data;    
    
    LCDRS_HIGH;
    tempy &= MSBNIBBLE_MASK;		//!<  Mask Lower 4 Bits
    LCDPORT &= LSBNIBBLE_MASK;	//!<  Make No Affect on other Port Pins
    LCDPORT |= tempy;				//!<  Send Higher Nibble to LCDPORT
    LCDEN_HIGH;
    _delay_us(200);
    LCDEN_LOW;
    
    tempy = (uint8_t) LEFTSHIFT_NIBBLE(data);	//!< Left Shift Byte Four Times
    tempy &= MSBNIBBLE_MASK;		//!< Mask Higher 4 Bits
    LCDPORT &= LSBNIBBLE_MASK;	//!< Make No Affect on other Port Pins
    LCDPORT |= tempy;				//!< Send Lower Nibble to LCDPORT
    LCDEN_HIGH;
    _delay_us(200);
    LCDEN_LOW;
    
    _delay_us(20);             //!< Just delay between two char
    
    return;
}
/*******************************************************************************
 * Name     : lcdMoveCursor
 * Para1    : line to move cursor
 * Para2    : position in that line 
 * Return   : NULL
 * Info     : Move cursor to needed place 
 ******************************************************************************/
void lcdMoveCursor(uint8_t line, uint8_t posi)
{
    uint8_t temp = 0;
    
    if( LCD_FIRSTLINE == line )
    {   
        temp = (uint8_t) (FIRSTLINE_ADDRESS + posi); 
    }
    else if ( LCD_SECONDLINE == line )
    {   
        temp = (uint8_t) ( SECONDLINE_ADDRESS + posi ); 
    }
    
    lcdWriteCmd(temp);
    
    return;
}

/*******************************************************************************
 * Name     : lcdWriteDecimal
 * Para1    : line to move cursor
 * Para2    : position in that line 
 * Return   : NULL
 * Info     : Move cursor to needed place 
 ******************************************************************************/
void lcdWriteDecimal( uint8_t line, uint8_t posi, uint8_t digi, uint16_t hexNum )
{
    unsigned int cnt = 0;
    unsigned int digitChar = 0;
    unsigned int temp = hexNum;    
        
    for( cnt = 1; cnt <= digi; cnt++ )
    {
        lcdMoveCursor(line, (posi + (digi - cnt) ) );   //!< Move Cursor to required place.
        digitChar = temp % DECIMAL_DIV;
        digitChar |= ASCII_MASK;
        lcdWriteData( digitChar );
        temp = temp / DECIMAL_DIV;        
    }  
    
    return;
}
/* END OF LCD4PIN.C */
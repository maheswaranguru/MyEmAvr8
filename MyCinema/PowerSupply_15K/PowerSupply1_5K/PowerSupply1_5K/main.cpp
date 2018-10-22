/*
 * PowerSupply1_5K.cpp
 *
 * Created: 16-10-2018 06:13:07
 * Author : maaply
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "myemAvr8Gpio.h"
#include "myemLcd.h"
#include "myEmAvrTimer.h"

#define NANO_LED PORTB4

myEmAvrTimer myTimer;

void refreshLcd( void );


static bool lcdUpdate = false;

int main(void)
{
    DDRC |= 0x01;     
    PORTC |= (1<<PORTC0);
    
    _delay_ms(200);
     
   PORTC &= ~(1<<PORTC0);
    
    lcdInit();
    myTimer.Init ( ONE_SEC );
    PORTC |= (1<<PORTC0);
    
    lcdWriteChar(1, 5, 'M' );
   
    lcdWriteStr(1, 1, "Hi Dai");
    
     lcdWriteStr(2, 0, "Ithu rendu");
     
     sei();
    while (1) 
    {
        if(lcdUpdate)
        {
            refreshLcd();
            lcdUpdate = false;
        }
        _delay_ms(1);
    }
}

ISR(TIMER0_OVF_vect)
{
    static uint16_t timerCnt = 0;
    
    TCNT0 = 24;
    
   if( timerCnt++ >= 1000 )
   {
       timerCnt = 0;
       gpioTogglePin( &PORTC, PORTC0 );       
       lcdUpdate = true;
    }
  
}
/*******************************************************************************
 * Name     : lcdMoveCursor
 * Para1    : line to move cursor
 * Para2    : position in that line 
 * Return   : NULL
 * Info     : Move cursor to needed place 
 ******************************************************************************/
void refreshLcd( void )
{
    static uint8_t secCnt = 0;
    
    lcdClear();
    
    lcdWriteDecimal(2, 3, 4, secCnt++ );
    
}    
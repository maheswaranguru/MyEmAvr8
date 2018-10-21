/*
 * PowerSupply1_5K.cpp
 *
 * Created: 16-10-2018 06:13:07
 * Author : maaply
 */ 

#include <avr/io.h>
#include "myemAvr8Gpio.h"
#include "myemLcd.h"

#define NANO_LED PORTB4


int main(void)
{
     _delay_ms (500);
    lcdInit();
    _delay_ms (1000);
    
    lcdWriteChar(1, 5, 'M' );
    _delay_ms (1000);
    
    lcdWriteStr(1, 1, "Hi Dai");
     _delay_ms (1000);
     
     lcdWriteStr(2, 0, "Ithu rendu");
     
    while (1) 
    {
        //gpioSetPin( &PORTB, NANO_LED );
        _delay_ms(1000);
    }
}


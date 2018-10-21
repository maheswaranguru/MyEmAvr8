/*
 * myemAvr8Gpio.h
 *
 * Created: 16-02-2018 19:25:46
 *  Author: maaply
 */ 


#ifndef MYEMAVR8GPIO_H_
#define MYEMAVR8GPIO_H_

#include <stdint.h>
#include <avr/io.h>

#if defined (NOT_NOW)
#define DIRECTION_INDEPENTENT 
#endif

#define SETPIN(port, pin) (port|=(1<<pin))
#define CLEARPIN(port, pin) ( port&=~(1<<pin))

void gpioSetPin( volatile unsigned char *port, uint8_t pin );
void gpioClearPin ( volatile unsigned char  *port, uint8_t pin );
void gpioTogglePin ( volatile unsigned char *port, uint8_t pin );
void gpioPinDirection ( volatile unsigned char *port, uint8_t pin );

void gpioThrowPort( volatile unsigned char *port, uint8_t value );
void gpioDirectionPort(  volatile unsigned char *port, uint8_t value );

void gpioDisableAllPullUp( void );

#endif /* MYEMAVR8GPIO_H_ */
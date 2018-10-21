/*
 * myemAvr8Gpio.c
 *
 * Created: 16-02-2018 19:57:59
 *  Author: maapl
 */ 
 #include "myemAvr8Gpio.h"
 /*******************************************************************************
 * Name     : gpioSetPin
 * Para1    : port  - port to chage pin
 * Para2	: pin  - pin need to change.
 * Return   : NULL
 * Info     : Set the one of the pin in a port.
 ******************************************************************************/
 void gpioSetPin( volatile unsigned char *port, uint8_t pin )
 {

#ifdef DIRECTION_INDEPENTENT 
	*(port-1) |= (1<<pin);  //!< Direction of pin confirmed
#endif

	*port|=(1<<pin);  //!< set the pin HIGH
	return;
 }
 /*******************************************************************************
 * Name     : gpioClearPin
 * Para1    : port  - port to change pin
 * Para2	: pin  - pin need to change.
 * Return   : NULL
 * Info     : Clear a pin in the port.
 ******************************************************************************/
 void gpioClearPin ( volatile unsigned char *port, uint8_t pin )
 {
	*port &= ~(1<<pin);  //!< set the pin HIGH
	return;
 }
 /*******************************************************************************
 * Name     : gpioTogglePin
 * Para1    : port  - port to change pin
 * Para2	: pin  - pin need to change.
 * Return   : NULL
 * Info     : Toggle a pin in a port
 ******************************************************************************/
 void gpioTogglePin ( volatile unsigned char *port, uint8_t pin )
 {

#ifdef DIRECTION_INDEPENTENT 
	*(port-1) |= (1<<pin);  //!< Direction of pin confirmed
 #endif

	*port^=(1<<pin);  //!< toggle the pin HIGH
	return;

 }
 /*******************************************************************************
 * Name     : gpioPinDirection
 * Para1    : port  - port to change direction
 * Para2	: pin  - pin need to change .
 * Return   : NULL
 * Info     : Initialize UART
 ******************************************************************************/
 void gpioPinDirection ( volatile unsigned char *port, uint8_t pin )
 {

#ifdef DIRECTION_INDEPENTENT 
	*(port-1) |= (1<<pin);  //!< Direction of pin confirmed
#endif

 }
 /*******************************************************************************
 * Name     : gpioSetPin
 * Para1    : port  - port to chage pin
 * Para2	: pin  - pin need to change.
 * Return   : NULL
 * Info     : Initialize UART
 ******************************************************************************/
  void gpiosetPort( volatile unsigned char *port, uint8_t value )
  {
#ifdef DIRECTION_INDEPENTENT 
	*(port-1) |= value;  //!< Direction of pin confirmed
#endif

    *port = value;

    return;
  }
  /*******************************************************************************
 * Name     : gpioSetPin
 * Para1    : port  - port to chage pin
 * Para2	: pin  - pin need to change.
 * Return   : NULL
 * Info     : Initialize UART
 ******************************************************************************/
 void gpioThrowPort( volatile unsigned char *port, uint8_t value )
 {
#ifdef DIRECTION_INDEPENTENT 
	*(port-1) |= value;  //!< Direction of pin confirmed
 #endif

	 *port = value;  //!< set the pin HIGH
	 return;
 }
 /*******************************************************************************
 * Name     : gpioDirectionPort
 * Para1    : port  - port to change pin direction.
 * Para2	: pin  - value to change for port direction.
 * Return   : NULL
 * Info     : Initialize UART
 ******************************************************************************/
void gpioDirectionPort( volatile unsigned char *port, uint8_t value )
{

#ifdef DIRECTION_INDEPENTENT 
	 *(port-1) = value;  //!< Direction of pin confirmed
 #endif

}
/*******************************************************************************
 * Name     : gpioSetPin
 * Para1    : port  - port to chage pin
 * Para2	: pin  - pin need to change.
 * Return   : NULL
 * Info     : Initialize UART
 ******************************************************************************/
void gpioDisableAllPullUp( void )
{
	MCUCR |= ( 1<<PUD );
}



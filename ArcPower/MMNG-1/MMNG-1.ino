#include <LiquidCrystal.h>
#include "timer.h"

#define RELAY 6

#define MIN_REQ_VOLT 200
#define MAX_REQ_VOLT 250


#define SAMPLE_COUNT  1000


typedef struct
{
  unsigned int voltInput;
  unsigned int current;
}adc_t;

typedef struct
{
    unsigned int peakMax;
    unsigned int lowMin;
    unsigned int meanValue;    
    float realWorldValue;    
}data_t;

enum{
  IDEL,
  OFF,
  ON,
  MINIMUM_VOLT,
  MAXIMUM_VOLT,
  OVER_LOAD,
  NO_LOAD
};

adc_t adc;
data_t volt;
data_t current;
unsigned int looptime = 0;
bool ledUpdate_f;

const int rs = 12, en = 11, d4 = 10, d5 = 9, d6 = 8, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const unsigned int ADC_VOLT  = A4;
const unsigned int ADC_CURRENT = A6;

void setup() 
{
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, LOW);
  lcd.begin(16, 2);

  delay(100);
  Init_LCD();
  timer1Init();


}

void loop()
{

 for( looptime=0;looptime<= SAMPLE_COUNT; looptime++ )
  {    
        //!< VOLTAGE READING
        adc.voltInput = analogRead(ADC_VOLT); 
         adc.voltInput &= 0x3FF;
        if( adc.voltInput > volt.peakMax )
        {
        volt.peakMax = adc.voltInput;
        }
        if( adc.voltInput < volt.lowMin )
        {
          volt.lowMin = adc.voltInput;
        }

        //!< CURRENT READING
        adc.current = analogRead(ADC_CURRENT); 
        adc.current &= 0x3FF;
        if( adc.current > current.peakMax )
        {
        current.peakMax = adc.current;
        }
        if( adc.current < current.lowMin )
        {
          current.lowMin = adc.current;
        }
  }
  
  volt.meanValue = volt.peakMax - volt.lowMin;
  volt.realWorldValue = volt.meanValue * 0.449657;                   //  n=(311/1023)*m;0.30400782
  volt.realWorldValue = (volt.realWorldValue/1.414);   

  if( ledUpdate_f )
  {
      ledUpdate_f = false;
      updateLcd();
  }

  if(  ( volt.realWorldValue < MIN_REQ_VOLT ) || ( volt.realWorldValue > MAX_REQ_VOLT )  ) 
  {
    digitalWrite(RELAY, LOW);
  }else
  {
    digitalWrite(RELAY, HIGH);
  }

   
  volt.peakMax = 0;
  volt.lowMin = 0x3FF;
  volt.meanValue = 0;
  volt.realWorldValue = 0;  

  
}


/********************************************************************************
Name        : updateLcd
para 1      : N/A
return      : N/A
Discription : The LCD display will update with the real world value.
**********************************************************************************/
void Init_LCD( void )
{

  lcd.clear();
  lcd.print("--- WELCOME ---");
  lcd.setCursor(0,1);
  lcd.print("**** MyEm **** ");
  delay(2000);
  
  lcd.clear();
  lcd.print("   MMNG-1    ");
  lcd.setCursor(0,1);
  lcd.print("* Starting... *");
  delay(2000);
}
/********************************************************************************
Name        : updateLcd
para 1      : N/A
return      : N/A
Discription : The LCD display will update with the real world value.
**********************************************************************************/
void updateLcd( void )
{
    lcd.clear();
    lcd.print("      MMNG-1    ");
    lcd.setCursor(0,1);
    lcd.print( "VOLT:" );
    lcd.print( volt.realWorldValue, DEC );
    lcd.print( " I :" );
    lcd.print( volt.realWorldValue, DEC );
    
    
}


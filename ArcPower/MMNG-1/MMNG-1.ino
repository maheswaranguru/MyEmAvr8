#include <LiquidCrystal.h>
#include "config.h"
#include "timer.h"



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


adc_t adc;
data_t volt;
data_t current;

int16_t inputVolt;
int16_t power;
float loadCurrent;
bool ledUpdate_f;
systemState_t systemState = IDEL;
systemError_t systemError = NO_ERROR;

uint16_t wattsLUT[] = { 100 , 200 , 300 , 400 , 500 , 600 , 700 , 800 , 900 , 1000 , 1100 , 1200 , 1300 , 1400 , 1500 , 1600 , 1700 , 1800 , 1900 , 2000 , 2100 , 2200 , 2300 , 2400 , 2500 , 2600 , 2700 };
//float ivolt[] = {0.087, 0.151, 0.241, 0.301, 0.394, 0.487, 0.58, 0.673, 0.766, 0.859, 0.952, 1.045, 1.138, 1.231, 1.324, 1.417, 1.51, 1.603, 1.696, 1.789, 1.882, 1.975, 2.068, 2.161 } ;
uint16_t iAdcloadLUT[] = { 79, 138, 217, 275, 352, 429, 506 , 583 , 660 , 737 , 814 , 891 , 968 , 1045, 1122, 1199, 1276, 1353, 1430, 1507, 1584, 1661, 1738, 1815, 1892, 1969, 2046 };
uint8_t sizeiAdcloadLUT = sizeof(iAdcloadLUT)/ sizeof(uint16_t);
uint8_t sizewattsLUT = sizeof(wattsLUT)/ sizeof(uint8_t);


const int rs = 12, en = 11, d4 = 10, d5 = 9, d6 = 8, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void setup() 
{
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, LOW);
  lcd.begin(16, 2);

  delay(100);
  //Init_LCD();
  timer1Init();


  volt.peakMax = 0;
  volt.lowMin = 0x3FF;
  volt.meanValue = 0;
  volt.realWorldValue = 0;  

 }

void loop()
{


 readSample();
 calculateRealWorlValue();

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
  
  current.peakMax = 0;
  current.lowMin = 0x3FF;
  current.meanValue = 0;
  current.realWorldValue = 0; 

  power = 0;
  loadCurrent = 0;
  
}



/********************************************************************************
Name        : readSample
para 1      : N/A
return      : N/A
Discription : Read ADC chennals for requier number of times. And update voltage levels
**********************************************************************************/
void readSample( void )
{
  unsigned int looptime = 0;
  
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
}
/********************************************************************************
Name        : calculateRealWorlValue
para 1      : N/A
return      : N/A
Discription : Calculate real world value from adc read counts.
**********************************************************************************/
void calculateRealWorlValue( void )
{
  volt.meanValue = volt.peakMax - volt.lowMin;
  volt.realWorldValue = volt.meanValue * 0.449657;                   //  n=(311/1023)*m;0.30400782
  volt.realWorldValue = (volt.realWorldValue/1.414);
  inputVolt = (uint16_t)volt.realWorldValue;
  
  current.meanValue = current.peakMax - current.lowMin;
 
  power = findLinearyValue( current.meanValue );
  loadCurrent =  ((float)power/(float)inputVolt);  
 
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
    lcd.print("V:");
    lcd.print( inputVolt, DEC );
    lcd.print(" 'I':");
    lcd.print( loadCurrent, DEC );
 
    lcd.setCursor(0,1);
    lcd.print( "adc:" );
    lcd.print( current.meanValue, DEC );
    lcd.print( " P :" );
    lcd.print( power, DEC );    
}
/********************************************************************************
Name        : findyValue
para 1      : adc count read from CT
return      : Watts for corresponting adc count.
Discription : 
**********************************************************************************/
uint16_t findLinearyValue( uint16_t x3 )
{
    uint8_t x1p = 0, x2p = 0;
    uint8_t loop = 0;
    float m = 0;
    float y3 = 0;
       
    x1p = 0;
    x2p = sizeiAdcloadLUT - 1;
    for(loop = 0;loop<sizeiAdcloadLUT-1;loop++)
    {
        if( x3 > iAdcloadLUT[x1p] )
            x1p = loop;
        if( x3 < iAdcloadLUT[x2p] )
            x2p = sizeiAdcloadLUT-(loop+1);
    }
    if(x1p != 0)
    x1p--;
    x2p++;

    m = ( wattsLUT[x2p] - wattsLUT[x1p] )/ ( iAdcloadLUT[x2p] - iAdcloadLUT[x1p] );
    y3 = ((m*(x3-iAdcloadLUT[x1p]))+ wattsLUT[x1p]);

    return( y3 );
}

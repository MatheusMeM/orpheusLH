
#include <Wire.h>
#include <avr/pgmspace.h>
#include <Adafruit_MCP4725.h>

Adafruit_MCP4725 dac;
uint16_t m = 0;
int sensor = 0;

//Blanking wire: White goes to Digital Pin 8, Blue goes to ground.
const int blanking = 8; 


/* For a faster responde of the Arduino values are
   stored at the Program memory                    */
PROGMEM uint16_t galvo_Position[18] = 
  {
    // DAC can only  outputs 0 to 4095 analog values (12bits)
    0, 500, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 
    3500, 3000, 2500, 2000, 1500, 1000, 500, 0
  } ;



void setup() 
  {
      /* For Adafruit MCP4725A1 the address is 0x62 (default) 
         or 0x63 (ADDR pin tied to VCC)                       */
       pinMode(blanking, OUTPUT);    // blanking White pin 
       digitalWrite(blanking, LOW);
       dac.begin(0x62);

  }

void loop()
  {
      digitalWrite(blanking, LOW);
      
      dac.setVoltage(pgm_read_word(&(galvo_Position[m])), false);  
     
      delayMicroseconds(390);    
      
      digitalWrite(blanking, HIGH);
    
      delayMicroseconds(900);
      m++ ;
      if (m > 19)
      {
      m = 1;
      }
      sensor = digitalRead(2);
      if(sensor == HIGH)
      {
        digitalWrite(13, HIGH);
      }
      else
      {
        digitalWrite(13, LOW);
      }
  }
    



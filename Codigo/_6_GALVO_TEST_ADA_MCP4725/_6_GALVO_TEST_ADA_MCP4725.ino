
#include <Wire.h>
#include <avr/pgmspace.h>
#include <Adafruit_MCP4725.h>

Adafruit_MCP4725 dac;
uint16_t m = 0;

/* For a faster responde of the Arduino values are
   stored at the Program memory                    */
PROGMEM uint16_t galvo_Position[10] = 
  {
    // DAC can only  outputs 0 to 4095 analog values (12bits)
    0, 500, 1000, 1500, 2000, 2500, 3000, 3500, 4000
  } ;



void setup() 
  {
      /* For Adafruit MCP4725A1 the address is 0x62 (default) 
         or 0x63 (ADDR pin tied to VCC)                       */
      dac.begin(0x62);
      Serial.begin(9600);
      Serial.println("Galvo Test");
  }

void loop()
  {
      dac.setVoltage(pgm_read_word(&(galvo_Position[m])), false);   
      m++ ;
      if (m > 9)
      {
      m = 1;
      }
   }

    



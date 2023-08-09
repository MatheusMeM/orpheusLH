
#include <Wire.h>
#include <avr/pgmspace.h>
#include <Adafruit_MCP4725.h>

Adafruit_MCP4725 dac;
uint16_t m = 0;

//Blanking wire: White goes to Digital Pin 8, Blue goes to ground.
const int blanking = 12; 

int sensorState  = 0; 
const int sensorPin = 2;
int notesonoff[9] = {   0,   0,    0,    0,    0,    0,    0,    0,    0 }; // note status. Keeps track if the note is on or off

/* For a faster responde of the Arduino values are
   stored at the Program memory                    */
PROGMEM uint16_t galvo_Position[18] = 
  {
    // DAC can only  outputs 0 to 4095 analog values (12bits)
    0, 500, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 
    3500, 3000, 2500, 2000, 1500, 1000, 500, 0
  } ;


//  plays a MIDI note.  
void noteOn(int cmd, int pitch, int velocity)
{
  Serial.write(cmd);
  Serial.write(pitch);
  Serial.write(velocity);
}


void setup() 
  {
      /* For Adafruit MCP4725A1 the address is 0x62 (default) 
         or 0x63 (ADDR pin tied to VCC)                       */
       pinMode(blanking, OUTPUT);    // blanking White pin 
       digitalWrite(blanking, LOW);
       dac.begin(0x62);
       Serial.begin(31250);
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
      
      
          // *** Beam Detection ***
    sensorState = digitalRead(sensorPin);
    
      
          // *** Beam Detection Evaluation ***
    // Beam interupted
    if (sensorState == HIGH)
       {
       if (notesonoff[beam] == 0)    // note status was off
          {           
           noteOn(mchannel, playnote, 0x7F);
           
           notesonoff[beam] = 1;     // sets note status to on 
          }
       }
       
       // Beam not interupted   
       if (sensorState == LOW)
          {
               noteOn(mchannel, playnote, 0x00);
               
               notesonoff[beam] = 0;     // sets note status to on
              
          }   
  }
    





#include <Wire.h>

#include <avr/pgmspace.h>

#include <Adafruit_MCP4725.h>



Adafruit_MCP4725 dac;

uint16_t m = 0;


//laserPinPinPin wire: White goes to Digital Pin 8, Blue goes to ground.

const int laserPin = 12; 

const int mchannel = 0x90;

const int sensorPin = 2;

const int ledPin = 13;

int sensorState;

void noteOn(int cmd, int pitch, int velocity)
{
  Serial.write(cmd);
  Serial.write(pitch);
  Serial.write(velocity);
}
 
 
int noteStatus[9] = {   0,   0,    0,    0,    0,    0,    0,    0,    0 }; // note status. Keeps track if the note is on or off

//Note position       01  02  03  04  05  06  07  08  09    10   11  12  13  14  15  16  17  18  19    20  21  22  23  24  25  26  27  28  29    30
int note[60]     = {  95, 95, 95, 95, 95, 95, 95, 95, 95 , 666 , 57, 60, 62, 64, 65, 67, 69, 71, 72 , 666, 36, 41, 48, 53, 60, 65, 72, 77, 84 , 666 }; 
//Note layout  bank0  B6  B6  B6  B6  B6  B6  B6  B6  B6  bank1  A3  C4  D4  E4  F4  G4  A4  B4  C5  bank2 C2  F2  C3  F3  C4  F4  C5  F5  C6

int notebank     = 10;    // set initial note bank to center bank1
int playnote     = 0;



/* For a faster responde of the Arduino values are

   stored at the Program memory                    */

int galvo_Position[18] = 

  {

    // DAC can only  outputs 0 to 4095 analog values (12bits)

    0, 500, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 

    3500, 3000, 2500, 2000, 1500, 1000, 500, 0

  };







void setup() 
{
       pinMode(laserPin, OUTPUT);    // laserPinPinPin White pin 
       digitalWrite(laserPin, LOW);
       dac.begin(0x62);
       pinMode( sensorPin, INPUT );
       pinMode(ledPin, OUTPUT);
       Serial.begin(31250);
  }



void loop()

   {

      digitalWrite(laserPin, LOW);
      dac.setVoltage(galvo_Position[m], false);  
      delayMicroseconds(390);    
      digitalWrite(laserPin, HIGH);
      delayMicroseconds(900);

      m++ ;

      if (m > 19)

      {

      m = 1;

      }

         //*** m Detection ***//
    sensorState = digitalRead(sensorPin);
    
    // *** m Detection Evaluation ***
    // m interupted
    if (sensorState == HIGH)
       {
       if (noteStatus[m] == 0)    // note status was off
          {
           digitalWrite(ledPin, HIGH);    // m interupt indicator
           
           playnote = note[m + notebank]; //+ (12*octave)
           noteOn(mchannel, playnote, 0x7F);
           
           noteStatus[m] = 1;     // sets note status to on 
          }
       }
       
       // m not interupted   
       if (sensorState == LOW)
          {
           if (noteStatus[m] == 1)    // note status was on
              {
               digitalWrite(ledPin, LOW);     // m interupt indicator

               playnote = note[m + notebank]; //+ (12*octave)
               noteOn(mchannel, playnote, 0x00);
               
               noteStatus[m] = 0;     // sets note status to on
              }
          }    
         
   }

    





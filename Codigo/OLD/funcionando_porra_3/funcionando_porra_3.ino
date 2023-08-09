#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(3, 4, 5, 6, 7, 8);

#include <Wire.h>

#include <avr/pgmspace.h>

#include <Adafruit_MCP4725.h>



Adafruit_MCP4725 dac;

uint16_t m = 0;


//Blanking wire: White goes to Digital Pin 8, Blue goes to ground.

const int blanking = 12; 

const int mchannel = 0x90;

const int lightSensorPin = 2;

int lightSensor;

int a;

void noteOn(int cmd, int pitch, int velocity)
{
  Serial.write(cmd);
  Serial.write(pitch);
  Serial.write(velocity);
}


/*int notesonoff[9] = {   
  0,   0,    0,    0,    0,    0,    0,    0,    0 }; // note status. Keeps track if the note is on or off

int note[60]     = {  
  95, 95, 95, 95, 95, 95, 95, 95, 95 , 666 , 57, 60, 62, 64, 65, 67, 69, 71, 72 , 666, 36, 41, 48, 53, 60, 65, 72, 77, 84 , 666 }; 

int notebank     = 10;    // set initial note bank to center bank1
*/

/* For a faster responde of the Arduino values are
 
 stored at the Program memory                    */

PROGMEM uint16_t galvo_Position[18] = 

{

  // DAC can only  outputs 0 to 4095 analog values (12bits)

  0, 500, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 

  3500, 3000, 2500, 2000, 1500, 1000, 500, 0

} 
;



//int nota[] = { 57, 60, 62, 64, 65, 67, 69, 70, 72};



void setup() 
{
  pinMode(blanking, OUTPUT);    // blanking White pin 
  digitalWrite(blanking, LOW);
  dac.begin(0x62);
  pinMode( lightSensorPin, INPUT );
  Serial.begin(31250);
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Orpheus");
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


  lightSensor = digitalRead(lightSensorPin);



  if(lightSensor == HIGH)
  {

    switch (m) 
//  57, 60, 62, 64, 65, 67, 69, 71, 72
    {

    case 1:
      a =1;
      noteOn(mchannel, 57, 0x7f);
      break;

    case 2:
      a=2;
      noteOn(mchannel, 60, 0x7f);
      break;

    case 3:
      a=3;
      noteOn(mchannel, 62, 0x7f);
      break;

    case 4:
      a=4;
      noteOn(mchannel, 64, 0x7f);
      break;

    case 5:
      a=5;
      noteOn(mchannel, 65, 0x7f);
      break;

    case 6:
      a=6;
      noteOn(mchannel, 67, 0x7f);
      break;

    case 7:
      a=7;
      noteOn(mchannel, 69, 0x7f);
      break;

    case 8:
      a=8;
      noteOn(mchannel, 71, 0x7f);
      break;

    case 9:
      a=9;
      noteOn(mchannel, 72, 0x7f);
      break;

    }

   /* if(lightSensor == LOW);
    {
      noteOn(mchannel,nota[a], 0x00);
    }
*/
  }


}








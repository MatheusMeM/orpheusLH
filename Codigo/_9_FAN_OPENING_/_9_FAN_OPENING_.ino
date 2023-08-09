#include <Wire.h>
#include <avr/pgmspace.h>
#include <Adafruit_MCP4725.h>

Adafruit_MCP4725 dac;

int positionCurrent   = 0;
int positionDirection = 1;
int positionMiddle = 5;

//Blanking wire: White goes to Digital Pin 8, Blue goes to ground.
const int blanking = 12;

bool bootingUp = true;

/* For a faster responde of the Arduino values are
 stored at the Program memory                    */
// PROGMEM uint16_t galvo_Position[18] =
// {
//   // DAC can only  outputs 0 to 4095 analog values (12bits)
//   0, 500, 1000, 1500, 2000, 2500, 3000, 3500, 4000,
//   4000, 3500, 3000, 2500, 2000, 1500, 1000, 500, 0
// };

int galvo_Position[9] =
{0, 500, 1000, 1500, 2000, 2500, 3000, 3500, 4000};

int galvo_Middle = 2045;


void setup() 
{
  /* For Adafruit MCP4725A1 the address is 0x62 (default) 
   or 0x63 (ADDR pin tied to VCC)                       */
  pinMode(blanking, OUTPUT);    // blanking White pin 
  digitalWrite(blanking, LOW);
  dac.begin(0x62);
  Serial.begin(9600);
}

void fire(int pos, int mirrorDelay, int laserDelay){
  digitalWrite(blanking, LOW);
  dac.setVoltage(pos, false);
  delayMicroseconds(mirrorDelay);
  digitalWrite(blanking, HIGH);
  delayMicroseconds(laserDelay);
}

void loop()
{
  if(bootingUp)
  {
    bootingUp = false;
    fanout();
  }
  fire(galvo_Position[positionCurrent], 390, 900);
  setNextPosition();
}

void setNextPosition(){
  positionCurrent += positionDirection;
  if (positionCurrent > 9 || positionCurrent < 1){
    positionDirection = positionDirection * -1;
    positionCurrent += 2 * positionDirection;
  }
}

void fanout(){
  positionCurrent = positionMiddle;

  for (float zoom=0; zoom <= 1; zoom += 0.01)
  {
   // int position = galvo_Position[positionCurrent] * zoom;

    int pos =
        galvo_Position[positionMiddle] +
        (galvo_Position[positionCurrent] - galvo_Position[positionMiddle])*zoom;

    fire(pos, 390*zoom, 900*zoom);
    setNextPosition();
  }
}


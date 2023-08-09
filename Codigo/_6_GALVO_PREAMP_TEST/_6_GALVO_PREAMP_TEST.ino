
#include <Wire.h>

int sintab[10] = {
  0, 500, 1000, 1500, 2000, 2500, 3000, 3500, 4000
} ;
const int blanking      = 8;   // laser blanking 
int value = 0;
int sp=0;
int time = 0;
int beams = 9;
#define MCP4725_DEVICE 96 
void setup()
{
  Serial.begin(9600); 
  Wire.begin() ;
  //pinMode(2, OUTPUT);
  //pinMode(3, OUTPUT);
  pinMode(blanking, OUTPUT);
  digitalWrite(blanking, HIGH);
}

void loop()
{
  digitalWrite(13, LOW);
  value = sintab[sp];
  //Serial.println(" ");
  //Serial.print("sp: ");
  //Serial.println(sp);
  //Serial.print("value: ");
  //Serial.println(value);
  
  
  Wire.beginTransmission(MCP4725_DEVICE);
  Wire.write(64);                    // cmd to update the DAC
  Wire.write(value >> 4);            // the 8 most significant bits...
  Wire.write((value & 15) << 4);     // the 4 least significant bits...
  Wire.endTransmission();
digitalWrite(13, HIGH);
  sp = sp + 1;
  if (sp > beams )
     {
      sp=1;
     }


  delay(1000);
 }

    



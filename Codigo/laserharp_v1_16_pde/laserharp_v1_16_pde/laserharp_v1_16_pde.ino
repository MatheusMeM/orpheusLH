#include <Wire.h>
#define MCP4725_DEVICE 96 

////beam number        01   02    03    04    05    06    07    08    09 
int beamloc[10]   = {   0, 500, 1000, 1500, 2000, 2500, 3000, 3500, 4000 }; // position of the beams in the beam fan
int notesonoff[9] = {   0,   0,    0,    0,    0,    0,    0,    0,    0 }; // note status. Keeps track if the note is on or off

//Note position       01  02  03  04  05  06  07  08  09    10   11  12  13  14  15  16  17  18  19    20  21  22  23  24  25  26  27  28  29    30
int note[60]     = {  95, 95, 95, 95, 95, 95, 95, 95, 95 , 666 , 57, 60, 62, 64, 65, 67, 69, 71, 72 , 666, 36, 41, 48, 53, 60, 65, 72, 77, 84 , 666 }; 
//Note layout  bank0  B6  B6  B6  B6  B6  B6  B6  B6  B6  bank1  A3  C4  D4  E4  F4  G4  A4  B4  C5  bank2 C2  F2  C3  F3  C4  F4  C5  F5  C6

int notebank     = 10;    // set initial note bank to center bank1

float zoom       = 0;
int playnote     = 0;
int bank         = 2;     // set notebank to the center
int octave       = 0;     // set octave to the center
int mchannelset   = 0;
int mchannel     = 0x90;  // set midichannel base to channel 0 
int mirrorpause  = 1;
int transpose    = 0;
int beampos      = 0;
int beam         = 0;     // is nul??
int times        = 0;
int steps        = 1;     // resolution of the opening/closing of the fan 
int middle       = 2000;  // middle position of the beam fan
int beams        = 9;     // number of beams
int openspeed    = 1;     // minimum 1. Higher number is faster

int startstop    = 0;     // harp status
int harprun      = 0;     

int sensorState  = 0; 
int swipin2val   = 0;
int swipin3val   = 0;
int reads        = 0;

// pin definitions

const int sensorPin     = 2;  // beam interrupt sensor
const int beambroken    = 4;  // beam interrupt LED

const int redPin_note   = 6;  // note bank RGB_LED
const int greenPin_note = 5;  // 0 = color on, 255 is color off 
const int bluePin_note  = 3;  // (this is for a commom anode rgb-led)

const int redPin_oct    = 11;  // octave RGB-LED
const int greenPin_oct  = 10;  // 0 = color on, 255 is color off 
const int bluePin_oct   =  9;  // (this is for a commom anode rgb-led)

const int blanking      = 8;   // laser blanking 
const int startstopPin  = 13;  // start/stop button  

int switchPin2 = A2;    // ANALOG PIN 2 
int switchPin3 = A3;    // ANALOG PIN 3

void setup()
{
  //Serial.begin(9600);  // debugging speed
  Serial.begin(31250);   // midi speed
  Wire.begin() ;
  pinMode(blanking, OUTPUT);
  pinMode(startstopPin, INPUT); 
  pinMode(beambroken, OUTPUT);  // beam interupt indicator
  
// set start position of laser beam
beampos = middle;
Wire.beginTransmission(MCP4725_DEVICE);
Wire.send(64);                      // cmd to update the DAC
Wire.send(beampos >> 4);            // the 8 most significant bits...
Wire.send((beampos & 15) << 4);     // the 4 least significant bits...
Wire.endTransmission();
digitalWrite(6, LOW);               // Beam interupt indicator  

pinMode(redPin_note, OUTPUT);
pinMode(greenPin_note, OUTPUT);
pinMode(bluePin_note, OUTPUT);

pinMode(redPin_oct, OUTPUT);
pinMode(greenPin_oct, OUTPUT);
pinMode(bluePin_oct, OUTPUT);

analogWrite(redPin_note, 255);
analogWrite(greenPin_note, 0);     // Set RGB-LED to GREEN
analogWrite(bluePin_note, 255);

analogWrite(redPin_oct, 255);
analogWrite(greenPin_oct, 0);      // Set RGB-LED to GREEN 
analogWrite(bluePin_oct, 255);

digitalWrite(blanking, LOW);

delay(mirrorpause);

setrgb_note();
setrgb_oct();


} // end void setup

void loop()
{

// ***********************************************************
// ************************ HARP CORE ************************
// ***********************************************************

if (harprun == 1)
   {
    // *** mirror positioning ***  
    beampos = middle + (middle - beamloc[beam]);
    Wire.beginTransmission(MCP4725_DEVICE);
    Wire.send(64);                      // cmd to update the DAC
    Wire.send(beampos >> 4);            // the 8 most significant bits...
    Wire.send((beampos & 15) << 4);     // the 4 least significant bits...
    Wire.endTransmission();
    delay(mirrorpause);
    delay(mirrorpause);
    digitalWrite(blanking, HIGH);
    delay(mirrorpause);
    digitalWrite(blanking, LOW);        //  make this line a comment if you to turn blanking off    

    // *** Beam Detection ***
    sensorState = digitalRead(sensorPin);
    
    // *** Beam Detection Evaluation ***
    // Beam interupted
    if (sensorState == HIGH)
       {
       if (notesonoff[beam] == 0)    // note status was off
          {
           digitalWrite(beambroken, HIGH);    // Beam interupt indicator
           
           playnote = note[beam + notebank]+ (12*octave);
           playnote = playnote + transpose;
           noteOn(mchannel, playnote, 0x7F);
           
           notesonoff[beam] = 1;     // sets note status to on 
          }
       }
       
       // Beam not interupted   
       if (sensorState == LOW)
          {
           if (notesonoff[beam] == 1)    // note status was on
              {
               digitalWrite(beambroken, LOW);     // Beam interupt indicator

               playnote = note[beam + notebank]+ (12*octave);
               playnote = playnote + transpose;
               noteOn(mchannel, playnote, 0x00);
               
               notesonoff[beam] = 0;     // sets note status to on
              }
          }    
         
        beam = beam + 1;
        if (beam > beams )
           {
            beam = 0;
           }
   }  
         
// ***************************************************************
// ************************ end HARP CORE ************************
// ***************************************************************

// *** Button read out ***

startstop = digitalRead(startstopPin);

if (startstop == HIGH)
   {
    // harp is on
    if (harprun == 1)
       {
        //*** HARP STOP *** 
        harprun = 0;
        steps = 1;
        close_harp();
        do
          {
           sensorState = digitalRead(sensorPin);
           if (sensorState == HIGH)
              {
               digitalWrite(blanking, LOW);
              }
          } while (sensorState == LOW); 
        return;
       }
       
    // harp is off
    if (harprun == 0)
       {
        //*** HARP START ***
        delay(2000);
        harprun = 1;
        steps = 1;
        delay(10);
        digitalWrite(blanking, HIGH);
        delay(10);
        digitalWrite(blanking, HIGH);   
        do
         {
          sensorState = digitalRead(sensorPin);
          if (sensorState == HIGH)
            {
             open_harp();
            }
         } while (sensorState == LOW); 
       }
   }

swipin2val = analogRead(switchPin2);
if (swipin2val > 100)
   {
    evaluate_buttonsA2();
   }

swipin3val = analogRead(switchPin3);  // read the value from the analog pin:  
if (swipin3val > 100)
   {
    evaluate_buttonsA3(); 
   }

// *** end Button read out ***

} // end void loop

// ***********************************************************
// ************************ FUNCTIONS ************************
// ***********************************************************

//  play the MIDI note. Or switch the note off. 
void noteOn(int cmd, int pitch, int velocity) 
{
 Serial.write(cmd);
 Serial.write(pitch);
 Serial.write(velocity);
}

// *** Buttons on Pin A2
void  evaluate_buttonsA2()
{
 delay(20);
 for (reads = 1; reads < 5; reads++)
     {
      swipin2val = swipin2val + analogRead(switchPin2);
     }
 swipin2val = swipin2val/5;

 // *** NOTE BANK UP ***
 if (swipin2val > 980)
    {
     if (notebank < 20)
        {
         steps = 20;
         notebank = notebank + 10;
         digitalWrite(blanking, HIGH);
         tonebank_animation();
         setrgb_note();
        }
        
    }
 
 // *** OCTAVE UP ***   
 if (swipin2val > 900 && swipin2val < 960)
    {
     if (octave < 1 )
        {
         steps = 20;
         octave = octave + 1;
         digitalWrite(blanking, HIGH);
         tonebank_animation();
         setrgb_oct();
        }
  
    }
 
 // *** TRANSPOSE UP ***  
 if (swipin2val > 820 && swipin2val < 880)
    {
     digitalWrite(beambroken, HIGH); 
     if (transpose < 8 )
        {
         transpose = transpose + 1;
        }  
     delay(200);
    }
 
  // *** MIDI CHANNEL UP ***  
  if (swipin2val > 600 && swipin2val < 810)
     {
      if (mchannelset < 16 )
         {
          mchannelset = mchannelset + 1; 
          set_midi_channel();
          delay(200);
         }
     }
   
 swipin2val =0; 
}

// *** Buttons on Pin A3
void  evaluate_buttonsA3()
{
 delay(20);
 for (reads = 1; reads < 5; reads++)
     {
      swipin3val = swipin3val + analogRead(switchPin3);
     }
 swipin3val = swipin3val / 5;

 // *** NOTE BANK DOWN ***
 if (swipin3val > 980)
    {
     if (notebank > 0)
        {
         steps = 20;
         notebank = notebank - 10;
         digitalWrite(blanking, HIGH);
         tonebank_animation();
         setrgb_note();
        }
        
    }
 
 // *** OCTAVE DOWN ***  
 if (swipin3val > 900 && swipin3val < 960)
    {
      if (octave > -1 )
        {
         steps = 20;
         octave = octave - 1;
         digitalWrite(blanking, HIGH);
         tonebank_animation();
         setrgb_oct();
        }
    }
 
 // *** TRANSPOSE DOWN ***  
 if (swipin3val > 820 && swipin3val < 880)
    {
     digitalWrite(beambroken, HIGH); 
     if (transpose > -7 )
        {
         transpose = transpose - 1;
        }
     delay(200);   
    }
 
 // *** MIDI CHANNEL DOWN ***  
 if (swipin3val > 600 && swipin3val < 810)
    {
     if (mchannelset > 0 )
         {
          mchannelset = mchannelset - 1; 
          set_midi_channel();
          delay(200);
         }

    }
   
 swipin3val =0; 
}


void tonebank_animation() 
{
 digitalWrite(blanking, HIGH); 
 // ******* close beams *******  
 for (times = 2000; times >= 0; times=times-steps) 
    {
     float zoom = 2000.0/times; //zoom -0.05;
     if (zoom <= 1)
        {
         zoom=1;
        }
     beampos = middle + (middle - beamloc[beam])/zoom;
     delay(openspeed);
     Wire.beginTransmission(MCP4725_DEVICE);
     Wire.send(64);                      // cmd to update the DAC
     Wire.send(beampos >> 4);            // the 8 most significant bits...
     Wire.send((beampos & 15) << 4);     // the 4 least significant bits...
     Wire.endTransmission();
     beam = beam + 1;
     if (beam > beams)
        {
         beam=0;
        }
     } 
 // ******* open beams *******
 for (times = 1; times <= 2000; times=times+steps) 
     {
      float zoom = 2000.0/times; //zoom -0.05;
      if (zoom <= 1)
         {
          //zoom=1;
          return;
         }
      beampos = middle + (middle - beamloc[beam])/zoom;
      delay(openspeed);
      Wire.beginTransmission(MCP4725_DEVICE);
      Wire.send(64);                      // cmd to update the DAC
      Wire.send(beampos >> 4);            // the 8 most significant bits...
      Wire.send((beampos & 15) << 4);     // the 4 least significant bits...
      Wire.endTransmission();
      beam = beam + 1;
      if (beam > beams)
         {
          beam=1;
         }
      }
} // end tonebank_animation() 

void open_harp() 
{
 digitalWrite(blanking, HIGH); 
 for (times = 1; times <= 2000; times=times+steps) 
     {
      float zoom = 2000.0/times; //zoom -0.05;
      if (zoom <= 1)
         {
          //zoom=1;
          return;
         }
      beampos = middle + (middle - beamloc[beam])/zoom;
      delay(openspeed);
      Wire.beginTransmission(MCP4725_DEVICE);
      Wire.send(64);                      // cmd to update the DAC
      Wire.send(beampos >> 4);            // the 8 most significant bits...
      Wire.send((beampos & 15) << 4);     // the 4 least significant bits...
      Wire.endTransmission();
      beam = beam + 1;
      if (beam > beams)
         {
          beam=1;
         }
      }
} // end open_harp() 

void close_harp() 
{
 digitalWrite(blanking, HIGH); 
 for (times = 2000; times >= 0; times=times-steps) 
     {
      float zoom = 2000.0/times; //zoom -0.05;
      if (zoom <= 1)
         {
          zoom=1;
         }
      beampos = middle + (middle - beamloc[beam])/zoom;
      delay(openspeed);
      Wire.beginTransmission(MCP4725_DEVICE);
      Wire.send(64);                      // cmd to update the DAC
      Wire.send(beampos >> 4);            // the 8 most significant bits...
      Wire.send((beampos & 15) << 4);     // the 4 least significant bits...
      Wire.endTransmission();
      beam = beam + 1;
      if (beam > beams)
         {
          beam=0;
         }
      } 
} // end close_harp() 

void setrgb_note()
{
 if (notebank == 0)  
    {
     //blue
     analogWrite(redPin_note  , 255);
     analogWrite(greenPin_note, 255);
     analogWrite(bluePin_note , 0);
    } 
 if (notebank == 10)  
    {
     //green
     analogWrite(redPin_note  , 255);
     analogWrite(greenPin_note, 0);
     analogWrite(bluePin_note , 255);
    }   
 if (notebank == 20)  
    {
     //red
     analogWrite(redPin_note  , 0);
     analogWrite(greenPin_note, 255);
     analogWrite(bluePin_note , 255);
    }   
 } // end setrgb_note()

void setrgb_oct()
{
 if (octave == -1)  
    {
     //blue
     analogWrite(redPin_oct  , 255);
     analogWrite(greenPin_oct, 255);
     analogWrite(bluePin_oct , 0);
    } 
 if (octave == 0)  
    {
     //green
     analogWrite(redPin_oct  , 255);
     analogWrite(greenPin_oct, 0);
     analogWrite(bluePin_oct , 255);
    }   
 if (octave == 1)  
    {
     //red
     analogWrite(redPin_oct  , 0);
     analogWrite(greenPin_oct, 255);
     analogWrite(bluePin_oct , 255);
    }   
 } // end setrgb_oct()

void set_midi_channel()
{ 
  if (mchannelset == 1)
     {
      mchannel = 0x90;
     }   
  if (mchannelset == 2)
     {
      mchannel = 0x91;
     }   
  if (mchannelset == 3)
     {
      mchannel = 0x92;
     }   
  if (mchannelset == 4)
     {
      mchannel = 0x93;
     }   
  if (mchannelset == 5)
     {
      mchannel = 0x94;
     }       
  if (mchannelset == 6)
     {
      mchannel = 0x95;
     }   
  if (mchannelset == 7)
     {
      mchannel = 0x96;
     }   
  if (mchannelset == 8)
     {
      mchannel = 0x97;
     }       
  if (mchannelset == 9)
     {
      mchannel = 0x98;
     }       
  if (mchannelset == 10)
     {
      mchannel = 0x99;
     }   
  if (mchannelset == 11)
     {
      mchannel = 0x9a;
     }   
  if (mchannelset == 12)
     {
      mchannel = 0x9b;
     }   
  if (mchannelset == 13)
     {
      mchannel = 0x9c;
     }       
  if (mchannelset == 14)
     {
      mchannel = 0x9d;
     }       
  if (mchannelset == 15)
     {
      mchannel = 0x9e;
     }   
  if (mchannelset == 16)
     {
      mchannel = 0x9f;
     }
}  // end set_midi_channel()


laserharp_v1_16

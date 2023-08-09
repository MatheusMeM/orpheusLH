
//////////Includes & Definitions//////////
#include <Wire.h>
#include <avr/pgmspace.h>
#include <LiquidCrystal.h>
#include <Adafruit_MCP4725.h>

Adafruit_MCP4725 dac;

#define buttonBas A1
#define buttonDroite A0
#define buttonGauche A7
#define buttonHaut A6

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(3, 4, 5, 6, 7, 8);


////////// Variables & Arrays//////////

const int blanking = 12; //blanking wire: White goes to Digital Pin 12, Blue goes to ground.
const int sensorPin = 2;
const int ledPin = 13;


int sensorState;
int up;
int down;
int right;
int left;


int item = 0;                             //sys var, LCD menu
int oldUP, oldDOWN, oldRIGHT,oldLEFT;     //sys var, LCD menu
int n, avr, maxBeams, index, MIDIchannel, mig;


boolean flag;         //sys var, galvo positoin (says yes or no to the code to change the position array)
boolean backFlag;     //sys var, galvo positoin
boolean lcdFlag = false;
boolean galvoFlag = true;    //sys var, galvo positoin  (this is to control if the galvo indexing and movement is on or off)


byte laserState = 1;


int array[23] = {};      //galvo position Array

int noteStatus[9] = {   0,   0,    0,    0,    0,    0,    0,    0,    0 }; // note status. Keeps track if the note is on or off
int note[128];
int octave, playNote, startNote, scale;


char* menuList[7] = 
{
  "MIDI ch:",
  "No Strings:",
  "Scale:",
  "Octave:",
  "Start Note:",
  "Laser:", 
  "Animation:"
};

char* noteList[12] =
{
  "C",
  "C#",
  "D",
  "D#",
  "E",
  "F",
  "F#",
  "G",
  "G#",
  "A",
  "A#",
  "B"
};

char* numberList[17] =
{
  "0",
  "1",
  "2",
  "3",
  "4",
  "5",
  "6",
  "7",
  "8",
  "9",
  "10",
  "11",
  "12",
  "13",
  "14",
  "15",
  "16"
};

char* asnwerList[2] =
{
  "OFF",
  "ON"
};


void setup() 
{
  //////////Defining PinModes//////////
       pinMode(blanking,    OUTPUT);    // blanking - White pin 
       pinMode(ledPin,      OUTPUT);
       pinMode(sensorPin,    INPUT);
       pinMode(buttonHaut,   INPUT);
       pinMode(buttonBas,    INPUT);
       pinMode(buttonGauche, INPUT);
       pinMode(buttonDroite, INPUT);


       
    //////////Communication//////////
       
       dac.begin(0x62);             //Calling the routine to start - I2C addr of the DAC
       Serial.begin(31250);
       //Serial.begin(9600);     //debug


      //////////Setups//////////
       digitalWrite(blanking, LOW);
       
       lcd.begin(16, 2);
       lcd.clear();
       lcd.setCursor(0, 0);
       lcd.print("ORPHEUS");
       
       n = 8;
       scale = 1;
       octave = 5;
       maxBeams = 12;
       startNote = 0;
       MIDIchannel = 1;

       fillNoteArray();
       refreshArray();
       index = maxBeams-n;
       

 }



void loop()
  {  
      //////////LCD Menu//////////
      
   refreshPad();                    // reads all buttons inputs
   char* atributesList[7] =                                        
   {
     numberList[MIDIchannel],   //MIDIchannel
     numberList[n],           //No Strings/beams
     numberList[item],          //Scale
     numberList[octave],        //Octave
     noteList[startNote],       //StartNote
     asnwerList[laserState],    //Laser
     asnwerList[item]           //Animations
   };
   
   if(lcdFlag)
  { 
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ORPHEUS");
    lcd.setCursor(0, 1);
    lcd.print(menuList[item]);
    lcd.print(atributesList[item]);      


    
    //////////Interface//////////
    
    if(up < 42){item = item - 1;}  //up and down buttons - makes this to rotate through the options (itens)
    if(down < 42){item++;}
    if(item < 0){item = 6;}   //
    if(item > 6){item = 0;}
    
    if(right < 42)                //if it was pressed
  {
    switch(item)
    {
      case 0:                   //MIDI channel
        MIDIchannel++;
        if(MIDIchannel >= 17)
        { MIDIchannel = 16; }
        break;
         
      case 1:                   //No of Strings                     
        n++;
        if(n > 12)
        { n = 12; }
        else
        { flag = true; }
        break;
        
      case 2:
        break;
        
      case 3:                        //Octave
        octave++;
        if(octave > 9)
        {
          octave = 0;
        }
        Serial.println(octave);
        break;
        
      case 4:                        //Start Note
        startNote++;
        if(startNote > 11)
        {
          startNote = 0;
        }
        break;
        
      case 5:                       //laser ON/OFF
        digitalWrite(blanking, HIGH);
        galvoFlag = true;
        laserState = 1;
        break;
        
      case 6:
        break;
    }
    
  }
  
  if(left < 42)
  {
    switch(item)
    {
      case 0:                       //MIDI channel
        MIDIchannel = MIDIchannel-1;
        if(MIDIchannel <= 1)
        { MIDIchannel = 1; }
        break;
         
      case 1:                      //No of strings
        n--;
        if(n < 3)
        {n = 3;}
        else
        { flag = true;            //<-flag to change the N of strings           
          backFlag = true; }      // backFlag flag to overwrite the old array with 0's before updating
        break;
        
      case 2:
        break;
      case 3:                 //Octave
        octave--;
        if(octave < 0)
        {
          octave = 9;
        }
        Serial.println(octave);
        break;
        
      case 4:                 //Start Note
        startNote--;
        if(startNote < 0)
        {
          startNote = 11;
        }
        break;
        
      case 5:                   //laser ON/OFF
        digitalWrite(blanking, LOW);
        galvoFlag = false;
        laserState = 0;
        break;
        
      case 6:
        break;
    }  

 
   }
   
   lcdFlag = false;             //Sets te lcdFlag to a not changed state again
  }
 //////////Galvo Positioning//////////
 
    if(flag == true)        //to change the N of strings (the numbers in the galvo position array)
    {
      refreshArray();
      index = maxBeams-n;
      flag = false;
    }


    if(galvoFlag == true)
    {
      index++;
      if(index == ((2*n)-1) + maxBeams-n)                         //the last significative number in the position array
      {
        index = maxBeams-n;
      }
  
      digitalWrite(blanking, LOW);               //turns laser off
      dac.setVoltage(array[index], false);       // all the magic happens this line changes indeed the galvo position (set 2nd parameter always false)

                /*    
      Serial.print(index);           //debug
      Serial.print(", ");
      Serial.println(array[index]);
      delay(50);
              */      
      
      delayMicroseconds(2800/pow(n, 1.14));      //Inercia Babe...
      digitalWrite(blanking, HIGH);              //Turn laser On
      delayMicroseconds(6400/pow(n, 1.14));      //Complain with Newton \m/
     
    }

     //////////Sensor Detection//////////
    sensorState = digitalRead(sensorPin);
    
    //////////Sensor Evaluation//////////
  if(array[index] > 4090){
            mig = index - 8;      }
  else{
      mig = index - 4;
   }

           
    if (sensorState == HIGH)      // m interupted
       {
       if (noteStatus[index] == 0)    // if note status was off
         {
           digitalWrite(ledPin, HIGH);    //just an easy indicator
           
           playNote = note[mig+startNote+(12*octave)]; 

           
           noteON(MIDIchannel, playNote, 0x7F);
           
           noteStatus[index] = 1;     // sets note status to on 
         }
       }  
       
        
       if (sensorState == LOW)      //if laser not interupted  
          {
           if (noteStatus[index] == 1)    // note status was on
              {
               digitalWrite(ledPin, LOW);     // m interupt indicator

               playNote = note[mig+startNote+(12*octave)]; 
               noteOFF(MIDIchannel, playNote, 0x00);
               
               noteStatus[index] = 0;     // sets note status to off
              }
          }    
    }

   

    

//////////Functions//////////

void refreshPad()
{ 
  up = analogRead(buttonHaut);
  down = analogRead(buttonBas);
  right = analogRead(buttonDroite);
  left = analogRead(buttonGauche);
  
  if(up != oldUP || down != oldDOWN || right != oldRIGHT || left != oldLEFT)
  {
    lcdFlag = true;
  }
  oldUP     = up;
  oldDOWN   = down;
  oldRIGHT  = right;
  oldLEFT   = left;
}

void refreshArray()
{
  if(backFlag == true)   // if the left button was pressed, it refil the array with zeros 
  {
    memset(array,0,sizeof(array)); 
    array[11] = 4095;   //sets the middle position as the highest as the DAC can handle
  }
  
  avr = (4096/(n-1));   //total DAC value divided by the number of beams - 1, because there is always zero.
                        //The "avr" is the number that corespond in lenght between the beams. it is the "standart yard" for the No of strings determined by N.
                        
  for(int i = 1; i < (n-1); i++)    //i=1 because there is always a zero starting the array
  {
    array[(i + (maxBeams - n) )] = avr*i;         //fills the array in an ascending form. (from 0 towards the middle)
    array[((maxBeams - 1) + ((n-1)-i))] = avr*i;  //fills the array in an descending form.
  }
  array[11] = 4095;
  
                /*   
     for(int i=0; i<=22; i++)         //debug
     {
       Serial.print(array[i]);
       Serial.print(", ");
       if(i==22)
       {Serial.println();}
     } 
               */
                
}

void fillNoteArray()
{
  for(int i=0; i<128; i++)
  {
    note[i]=i;    
  }
}

void noteON(byte channel, byte pitch, byte velocity)
{
  byte midiMessage= 0x90 + (channel - 1);
  Serial.write(midiMessage);
  Serial.write(pitch);
  Serial.write(velocity);
}

void noteOFF(byte channel, byte pitch, byte velocity)
{
  byte midiMessage= 0x80 + (channel - 1);
  Serial.write(midiMessage);
  Serial.write(pitch);
  Serial.write(velocity);
}

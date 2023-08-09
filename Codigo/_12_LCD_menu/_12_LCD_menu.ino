
// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(3, 4, 5, 6, 7, 8);

#define buttonBas A1
#define buttonDroite A0
#define buttonGauche A7
#define buttonHaut A6

int up;
int down;
int right;
int left;

int item = 0;
int beams = 12;
int MIDIchannel = 1;

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
  "C"
  "C#"
  "D"
  "D#"
  "E"
  "F"
  "F#"
  "G"
  "G#"
  "A"
  "A#"
  "B"
};

char* numberList[17] =
{
  "NAO USADO"
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

char* atributesList[7] = 
{
  numberList[item],     //MIDIchannel
  numberList[item],     //No Strings/beams
  numberList[item],     //Scale
  numberList[item],     //Octave
  noteList[item],   //StartNote
  asnwerList[item],     //Laser
  asnwerList[item]      //Animations
};



void setup()
{
  pinMode(buttonHaut,   INPUT);
  pinMode(buttonBas,    INPUT);
  pinMode(buttonGauche, INPUT);
  pinMode(buttonDroite, INPUT);

  lcd.begin(16, 2);
  Serial.begin(9600);
}




void loop()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ORPHEUS");
  lcd.setCursor(0, 1);
  lcd.print(menuList[item]);
  lcd.print(atributesList[item]);
  delay(100);
  refreshPad();
    
  if(up < 42){item = item - 1;}
  if(down < 42){item++;}
  if(item < 0){item = 6;}
  if(item > 6){item = 0;}
  delay(100);
  
  if(right < 42)
  {
    switch(item)
    {
      case 0:
        break;
         
      case 1:
        beams++;
        if(beams > 12){
          lcd.setCursor(13, 1);
          lcd.print("MAX");
          beams = 12;
        }
        
        delay(500); //preciso retirar isso aqui
        break;
        
      case 2:
        break;
      case 3:
         break;
      case 4:
         break;
      case 5:
         break;
      case 6:
         break;
    }
    
  }
  
  if(left < 42)
  {
    switch(item)
    {
      case 0:
      
        break;
         
      case 1:
        beams = beams - 1;
        if(beams < 1)
        {
          lcd.setCursor(13, 1);
          lcd.print("MIN");
          beams = 1;
        }
        delay(500); //preciso retirar isso aqui
        break;
        
      case 2:
        break;
      case 3:
         break;
      case 4:
         break;
      case 5:
         break;
      case 6:
         break;
    }  

 
  }
}





void refreshPad()
{
  up = analogRead(buttonHaut);
  down = analogRead(buttonBas);
  right = analogRead(buttonDroite);
  left = analogRead(buttonGauche);
}


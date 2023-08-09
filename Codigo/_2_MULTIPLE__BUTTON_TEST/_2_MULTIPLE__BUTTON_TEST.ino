
int swipin2val   = 0;
int swipin3val   = 0;
int reads        = 0;

const int redPin_note   = 3;  // note bank RGB_LED
const int greenPin_note = 5;  // 0 = color on, 255 is color off 
const int bluePin_note  = 6;  // (this is for a commom anode rgb-led)

const int bluePin_oct   = 9;  // octave RGB-LED
const int greenPin_oct  = 10; // 0 = color on, 255 is color off 
const int redPin_oct    = 11; // (this is for a commom anode rgb-led)

int switchPin2 = A2;    // ANALOG PIN 2 
int switchPin3 = A3;    // ANALOG PIN 3

void setup()
{

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

} // end void setup

void loop()
{

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


} // end void loop

// ***********************************************************
// ************************ FUNCTIONS ************************
// ***********************************************************


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
     //blue
     analogWrite(redPin_note  , 255);
     analogWrite(greenPin_note, 255);
     analogWrite(bluePin_note , 0);
    }
 
 // *** OCTAVE UP ***   
 if (swipin2val > 900 && swipin2val < 960)
    {
     //green
     analogWrite(redPin_note  , 255);
     analogWrite(greenPin_note, 0);
     analogWrite(bluePin_note , 255);
    }
 
 // *** TRANSPOSE UP ***  
 if (swipin2val > 820 && swipin2val < 880)
    {
     //red
     analogWrite(redPin_note  , 0);
     analogWrite(greenPin_note, 255);
     analogWrite(bluePin_note , 255);
    }
 
  // *** MIDI CHANNEL UP ***  
  if (swipin2val > 600 && swipin2val < 810)
     {
     //white
     analogWrite(redPin_note  , 0);
     analogWrite(greenPin_note, 0);
     analogWrite(bluePin_note , 0);
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
     //blue
     analogWrite(redPin_oct  , 255);
     analogWrite(greenPin_oct, 255);
     analogWrite(bluePin_oct , 0);      
    }
 
 // *** OCTAVE DOWN ***  
 if (swipin3val > 900 && swipin3val < 960)
    {
     //green
     analogWrite(redPin_oct  , 255);
     analogWrite(greenPin_oct, 0);
     analogWrite(bluePin_oct , 255);
    }
     
 // *** TRANSPOSE DOWN ***  
 if (swipin3val > 820 && swipin3val < 880)
    {
     //red
     analogWrite(redPin_oct  , 0);
     analogWrite(greenPin_oct, 255);
     analogWrite(bluePin_oct , 255);
    }
 
 // *** MIDI CHANNEL DOWN ***  
 if (swipin3val > 600 && swipin3val < 810)
    {
     //white
     analogWrite(redPin_oct  , 0);
     analogWrite(greenPin_oct, 0);
     analogWrite(bluePin_oct , 0);
    }
   
 swipin3val =0; 
}







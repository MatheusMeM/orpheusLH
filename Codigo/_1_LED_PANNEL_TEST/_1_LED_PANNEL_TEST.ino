
int bluePinnote   = 6;
int greenPinnote  = 5;
int redPinnote    = 3;

int redPin_oct    = 11;
int greenPin_oct  = 10;
int bluePin_oct   = 9;

int beam_interupt = 4;

int pause = 300;


void setup()
{
  pinMode(bluePinnote, OUTPUT);
  pinMode(greenPinnote, OUTPUT);
  pinMode(redPinnote, OUTPUT);

  pinMode(redPin_oct, OUTPUT);
  pinMode(greenPin_oct, OUTPUT);
  pinMode(bluePin_oct, OUTPUT);

  pinMode(beam_interupt, OUTPUT);


  
  analogWrite(bluePinnote, 255);
  analogWrite(greenPinnote, 255);
  analogWrite(redPinnote, 255);
  
  analogWrite(bluePin_oct, 255);
  analogWrite(greenPin_oct, 255);
  analogWrite(redPin_oct, 255);
  delay(pause);  
  
  digitalWrite(beam_interupt, LOW); 
  
}

void loop()
{
  // cycles the notebank RGB LED BLUE - GREEN - RED - WHITE 

  analogWrite(bluePinnote, 0);
  analogWrite(greenPinnote, 255);
  analogWrite(redPinnote, 255);
  delay(pause);  
  
  analogWrite(bluePinnote, 255);
  analogWrite(greenPinnote, 0);
  analogWrite(redPinnote, 255);
  delay(pause);
  
  analogWrite(bluePinnote, 255);
  analogWrite(greenPinnote, 255);
  analogWrite(redPinnote, 0);
  delay(pause);
  
  analogWrite(bluePinnote, 0);
  analogWrite(greenPinnote, 0);
  analogWrite(redPinnote, 0);
  delay(pause);

  analogWrite(bluePinnote, 255);
  analogWrite(greenPinnote, 255);
  analogWrite(redPinnote, 255);
  delay(pause);


  // cycles the octave RGB LED BLUE - GREEN - RED - WHITE 
  
  analogWrite(bluePin_oct, 0);
  analogWrite(greenPin_oct, 255);
  analogWrite(redPin_oct, 255);
  delay(pause);  
  
  analogWrite(bluePin_oct, 255);
  analogWrite(greenPin_oct, 0);
  analogWrite(redPin_oct, 255);
  delay(pause);
  
  analogWrite(bluePin_oct, 255);
  analogWrite(greenPin_oct, 255);
  analogWrite(redPin_oct, 0);
  delay(pause);
  
  analogWrite(bluePin_oct, 0);
  analogWrite(greenPin_oct, 0);
  analogWrite(redPin_oct, 0);
  delay(pause);
  
  analogWrite(bluePin_oct, 255);
  analogWrite(greenPin_oct, 255);
  analogWrite(redPin_oct, 255);
  delay(pause);

  // blinks the beam interupt indicator 3 times

  digitalWrite(beam_interupt, HIGH); 
  delay(pause);
  digitalWrite(beam_interupt, LOW);
  delay(pause);  
  digitalWrite(beam_interupt, HIGH); 
  delay(pause);
  digitalWrite(beam_interupt, LOW);
  delay(pause);  
  digitalWrite(beam_interupt, HIGH); 
  delay(pause);
  digitalWrite(beam_interupt, LOW);
  delay(pause);  
  
}





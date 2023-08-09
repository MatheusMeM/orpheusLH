/*
  Blink Laser
  Uses the TTL modulation of the laser to turn it on or off.
  Pin 13 has an LED connected on most Arduino boards.
  Blanking wire: White goes to Digital Pin 8, Blue goes to ground.
*/

int led = 13;
int blank = 12;


void setup() 
{                
  pinMode(led, OUTPUT);      // internal led pin
  pinMode(blank, OUTPUT);    // blanking pin 
  digitalWrite(blank, LOW);
}


void loop() {
  digitalWrite(led, HIGH);
  digitalWrite(blank, HIGH);
  delay(500);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(blank, LOW);
  delay(500);               
}

//Math

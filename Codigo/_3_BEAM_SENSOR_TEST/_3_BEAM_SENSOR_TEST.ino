
int beambroken  = 4;      // beam interrupt indicator
int sensorState = 0; 
const int sensorPin = 2;  // sensor pin

void setup()
{
pinMode(beambroken, OUTPUT);  
digitalWrite(beambroken, HIGH); 
delay(200);
digitalWrite(beambroken, LOW); 
delay(200);
} // end void setup

void loop()
{
sensorState = digitalRead(sensorPin);
// Beam interupted
if (sensorState == HIGH)
   {
    digitalWrite(beambroken, HIGH); // Beam interupt indicator
   }
// Beam not interupted   
if (sensorState == LOW)
   {
    digitalWrite(beambroken, LOW);  // Beam interupt indicator
   }    
} // end void loop




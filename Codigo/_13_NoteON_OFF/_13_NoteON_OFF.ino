
byte lastNote[1];

void noteON(byte channel, byte pitch, byte velocity)
{
  byte midiMessage= 0x90 + (channel - 1);
  Serial.write(midiMessage);
  Serial.write(pitch);
  lastNote[0] = pitch;
  Serial.write(velocity);
  lastNote[1] = velocity;
}

void noteOFF(byte channel)
{
  byte midiMessage= 0x80 + (channel - 1);
  Serial.write(midiMessage);
  Serial.write(lastNote[0]);
  Serial.write(lastNote[1]);
}


/*Teste*/

void setup() 
  {
       Serial.begin(31250);
  }
  
  
void loop()
  {
    noteON(1, 72, 127);
    delay(1000);
    noteOFF(1);
    delay(1000); 
}

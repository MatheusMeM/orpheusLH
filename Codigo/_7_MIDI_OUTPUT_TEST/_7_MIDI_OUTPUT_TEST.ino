
byte mchannel = 0x90;
int mchannelset = 1;
int note = 60; // C4

void setup() {
//  Set MIDI baud rate:
Serial.begin(31250);
  
}

void loop() {
  // plays notes from C4 to B4 through channel 1 to 16
  
    //Note on channel 1 (0x90), some note value (note), middle velocity (0x45):
    noteOn(mchannel, note, 0x7f); // 0x00 = note on at max velocity
    delay(150);
    
    noteOn(mchannel, note, 0x00);  // 0x00 = note off   
    delay(50);
    
    note = note + 1;
    
    if (note  > 71) 
    {
      note = 60;
      mchannelset = mchannelset + 1; 
      midichannelset();
    }
}
//  plays a MIDI note.  
void noteOn(int cmd, int pitch, int velocity)
{
  Serial.write(cmd);
  Serial.write(pitch);
  Serial.write(velocity);
}

void midichannelset()
{
     if (mchannelset > 16)
        { 
         mchannelset = 1;
        }
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
}




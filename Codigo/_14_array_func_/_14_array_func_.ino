

int n = 7; // numero de raios
int avr;
int maxbeams = 12;
int array[22] = {0,0,0,0,0,0,0,0,0,0,0,4095,0,0,0,0,0,0,0,0,0,0};

void setup()
{
  Serial.begin(9600);
}

void loop()
{
 avr = (4096/(n-1)); //total DAC value divided by the number of beams - 1, because there is always zero.
 for(int i = 1; i < (n-1); i++)
 {
   array[(i + (12-n)/*<---constant*/)] = avr*i;
   array[((maxbeams - 1) + ((n-1)-i))] = avr*i;
 }
 
 for(int i=0; i<=21; i++)
 {
   Serial.print(array[i]);
   Serial.print(", ");
 }
 while(1){}
}

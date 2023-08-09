#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int estados[5] = {
  0, 1, 2, 3, 4
};

int t=0;
int valor;
int i=0;
int es=0;
int x=0;
int botao1=6;
int botao2=7;
int botao3=8;
int botao4=9;


void setup(){
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Ola, eu sou");
  lcd.setCursor(0,1);
  lcd.print("Orpheus");
  Serial.print("Ola, eu sou Orpheus");
  pinMode(botao1, INPUT);
  pinMode(botao2, INPUT);
  pinMode(botao3, INPUT);
  pinMode(botao4, INPUT);
}

void loop(){
  valor = estados[es];
  if(es=0){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Ola, eu sou");
    lcd.setCursor(0,1);
    lcd.print("Orpheus");
    Serial.print("Ola, eu sou Orpheus");
    if(botao4=HIGH){
      es = 1;
      }
  }
  delay(1000);
  if(es=1){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Escolha sua oitava");
    Serial.print("Escolha sua oitava");
    lcd.setCursor(0,1);
    lcd.print(i);
    
    if(botao2=HIGH){
      es = 2;
    }
    
    if(botao4=HIGH){
      es = 3;
    }    
  }
    delay(1000);
  
  if(es=2){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Escolha sua escala");
    Serial.print("Escolha sua escala");
    lcd.setCursor(0,1);
    lcd.print(x);
    if(botao3=HIGH){
      es = 1;
    }
    if(botao4=HIGH){
      es = 4;
    }
  }
    delay(1000);
  if(es=3){
    if(botao1=HIGH){
      es = 1;
    }
    if(botao2=HIGH){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Escolha sua oitava");
      Serial.print("Escolha sua oitava");
      lcd.setCursor(0,1);
      lcd.print(i++);
    }
     if(botao2=HIGH){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Escolha sua oitava");
      Serial.print("Escolha sua oitava");
      lcd.setCursor(0,1);
      lcd.print(i--);
    }
  }
    delay(1000);
  if(es=4){
    if(botao1=HIGH){
      es = 2;
    }
    if(botao2=HIGH){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Escolha sua escala");
      Serial.print("Escolha sua escala");
      lcd.setCursor(0,1);
      lcd.print(x++);
    }
     if(botao2=HIGH){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Escolha sua escala");
      Serial.print("Escolha sua escala");
      lcd.setCursor(0,1);
      lcd.print(x--);
    }
  }
  delay(1000);
}


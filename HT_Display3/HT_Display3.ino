#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
DHT dht0(13, DHT11);
char b[17];
int f;
long int t;
short int p = 0;
LiquidCrystal_I2C lcd(0x3f, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

void PrintLcd(int x, int y, char * str) {
  int l;
  for (l = 0; l < 16; l++) {
    if (str[l] == '\0') {
      break;
    }
  }
  for (int i = 0; i < l; i++) {
    lcd.setCursor(x + i, y);
    lcd.print((char)str[i]);
  }
}

void setup() {
  lcd.init();                      // initialize the lcd
  // Print a message to the LCD.
  lcd.backlight();
  //Serial.begin(9600);
  dht0.begin();
  pinMode(18,OUTPUT);
  pinMode(19,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(11,OUTPUT);
  digitalWrite(18,LOW);
  digitalWrite(19,HIGH);
  digitalWrite(11,LOW);
  digitalWrite(12,HIGH);
  //pinMode(13, OUTPUT);
}
void loop() {
  t=millis();
  f = (int)dht0.readHumidity();
  sprintf(b, "H0 %02d%%", f);
  PrintLcd(0, 1, b);
  
  f = (int)dht0.readTemperature();
  sprintf(b, "T0 %02d*C", f);
  PrintLcd(0, 0, b);

  if (p == 0 ) {
    PrintLcd(7,0," ");
    p = 1;
  } else {
    PrintLcd(7,0,":");
    p = 0;    
  }
  
  delay(200);
  
}

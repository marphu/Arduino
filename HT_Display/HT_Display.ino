#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
int dhtPin1 = 11;
int dhtPin0 = 4;
DHT dht0(dhtPin0, DHT11);
DHT dht1(dhtPin1, DHT11);
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
  dht1.begin();
  pinMode(13, OUTPUT);
  pinMode(dhtPin0 - 1, OUTPUT);
  pinMode(dhtPin0 - 2, OUTPUT);
  digitalWrite(dhtPin0 - 1, HIGH);
  digitalWrite(dhtPin0 - 2, LOW);
  pinMode(dhtPin1 - 1, OUTPUT);
  pinMode(dhtPin1 - 2, OUTPUT);
  digitalWrite(dhtPin1 - 1, HIGH);
  digitalWrite(dhtPin1 - 2, LOW);
}
void loop() {
  t=millis();
  f = (int)dht0.readHumidity();
  sprintf(b, "H0 %02d%%", f);
  PrintLcd(0, 1, b);
  
  f = (int)dht1.readHumidity();
  sprintf(b, "H1 %02d%%", f);
  PrintLcd(8, 1, b);

  f = (int)dht0.readTemperature();
  sprintf(b, "T0 %02d*C", f);
  PrintLcd(0, 0, b);

  f = (int)dht1.readTemperature();
  sprintf(b, "T1 %02d*C", f);
  PrintLcd(8, 0, b);
  
  digitalWrite(13, HIGH);
  delay(200);
  digitalWrite(13, LOW);
  delay(200);
  if (p == 0 ) {
    PrintLcd(7,0,"|");
    p = 1;
  } else {
    PrintLcd(7,0,"-");
    p = 0;    
  }
  
  //Serial.println(millis()-t);
  
}

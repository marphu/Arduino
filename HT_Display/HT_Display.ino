#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#define DHTPIN A0
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);
char b[17];
char c[17];
int f;
long int t;
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
  dht.begin();
  // Запуск часов
  Serial.begin(9600);
}
void loop() {
  t=millis();
  f = (int)dht.readHumidity();
  //dtostrf(f, 3, 2, c);
  sprintf(b, "Humidity %02d%%", f);
  PrintLcd(2, 1, b);
  f = (int)dht.readTemperature();
  //dtostrf(f, 3, 2, c);
  sprintf(b, "Temperature %02d*C", f);
  PrintLcd(0, 0, b);
  delay(490-(millis()-t));
  //Serial.println(millis()-t);
  
}

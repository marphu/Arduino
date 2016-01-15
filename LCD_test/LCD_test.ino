#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include <DS1302.h> // Подключение библиотеки часов.
#define DHTPIN 2
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);
char b[16];
char c[16];
int f;
long int t;
LiquidCrystal_I2C lcd(0x3f, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display
// Set the appropriate digital I/O pin connections. These are the pin
// assignments for the Arduino as well for as the DS1302 chip. See the DS1302
// datasheet:
//
//   http://datasheets.maximintegrated.com/en/ds/DS1302.pdf
const int kCePin   = 5;  // Chip Enable
const int kIoPin   = 6;  // Input/Output
const int kSclkPin = 7;  // Serial Clock

// Create a DS1302 object.
DS1302 rtc(kCePin, kIoPin, kSclkPin);

String dayAsString(const Time::Day day) {
  switch (day) {
    case Time::kSunday: return "Sunday";
    case Time::kMonday: return "Monday";
    case Time::kTuesday: return "Tuesday";
    case Time::kWednesday: return "Wednesday";
    case Time::kThursday: return "Thursday";
    case Time::kFriday: return "Friday";
    case Time::kSaturday: return "Saturday";
  }
  return "(unknown day)";
}

void printTime(int x, int y) {
  // Get the current time and date from the chip.
  Time t = rtc.time();

  // Name the day of the week.
  const String day = dayAsString(t.day);

  // Format the time and date and insert into the temporary buffer.
  char buf[50];
  snprintf(buf, sizeof(buf), "%02d-%02d %02d:%02d:%02d",
           //day.c_str(),
           //t.yr,
           t.mon, t.date,
           t.hr, t.min, t.sec);

  // Print the formatted string to serial so we can see the time.
  //Serial.println(buf);
  PrintLcd(x,y,buf);
  snprintf(buf, sizeof(buf), "%04d", t.yr);
  PrintLcd(12,0,buf);
}
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
  //lcd.backlight();
  //Serial.begin(9600);
  dht.begin();
  // Запуск часов
  rtc.halt(false);
  rtc.writeProtect(true);
  Serial.begin(9600);
}
void loop() {
  t=millis();
  f = (int)dht.readHumidity();
  //dtostrf(f, 3, 2, c);
  sprintf(b, "H %02d%%", f);
  PrintLcd(6, 0, b);
  f = (int)dht.readTemperature();
  //dtostrf(f, 3, 2, c);
  sprintf(b, "T %02dC", f);
  PrintLcd(0, 0, b);
  printTime(1,1);
  delay(490-(millis()-t));
  //Serial.println(millis()-t);
  
}

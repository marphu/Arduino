#include <FreeRTOS_AVR.h>
// Экран
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3f, 16, 2);
// Термометр
#include <Wire.h>
#include "DHT.h"
DHT dht(4, DHT11);
// The LED is attached to pin 13 on Arduino.
//const uint8_t LED_PIN = 13;

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
// Declare a semaphore handle.
//SemaphoreHandle_t sem;
QueueHandle_t dhtTemp, dhtHumd;
//------------------------------------------------------------------------------
/*
   Thread 1, turn the LED off when signalled by thread 2.
*/
// Declare the thread function for thread 1.
void DHTsensor(void* arg) {
  dht.begin();
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  digitalWrite(2,LOW);
  digitalWrite(3,HIGH);
  int Temp = 2, Humd = 2;
  while (1) {
    xQueueReset(dhtTemp);
    xQueueReset(dhtHumd);
    Temp = (int)dht.readTemperature();
    Serial.println(Temp);
    Humd = (int)dht.readHumidity();
    Serial.println(Humd);
    xQueueSend(dhtTemp, &Temp, (TickType_t)1);
    xQueueSend(dhtHumd, &Humd, (TickType_t)1);
    vTaskDelay((500L * configTICK_RATE_HZ) / 1000L);
  }
}
//------------------------------------------------------------------------------
/*
   Thread 2, turn the LED on and signal thread 1 to turn the LED off.
*/
// Declare the thread function for thread 2.
void LCDisplay(void* arg) {
  char b[17];
  int Temp = 1, Humd = 1;
  int Humq = 3;
  lcd.init();                      // initialize the lcd
  // Print a message to the LCD.
  lcd.backlight();
  while (1) {
    xQueuePeek(dhtTemp, &Temp, 0);
    if (dhtHumd != 0 ) {
      xQueueReceive(dhtHumd, &Humq, 1);
      Humd = Humq;
    }
    sprintf(b, "T %02d*C H %02d%%", Temp, Humd);
    PrintLcd(0, 0, b);
    Serial.println(2);
    vTaskDelay((200L * configTICK_RATE_HZ) / 1000L);
  }
}
//------------------------------------------------------------------------------
void setup() {
  portBASE_TYPE s1, s2;
  Serial.begin(9600);
  dhtTemp = xQueueCreate( 1, sizeof( int ));
  dhtHumd = xQueueCreate( 1, sizeof( int ));
  Serial.println(1);
  // create task at priority two
  s1 = xTaskCreate(DHTsensor, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);
  // create task at priority one
  s2 = xTaskCreate(LCDisplay, NULL, configMINIMAL_STACK_SIZE, NULL, 1, NULL);
  // check for creation errors
  if ( s1 != pdPASS || s2 != pdPASS ) {
    Serial.println(F("Creation problem"));
    while (1);
  }
  // start scheduler
  vTaskStartScheduler();
  Serial.println(F("Insufficient RAM"));
  while (1);
}
//------------------------------------------------------------------------------
// WARNING idle loop has a very small stack (configMINIMAL_STACK_SIZE)
// loop must never block
void loop() {
  // Not used.
}

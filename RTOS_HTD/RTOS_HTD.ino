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
char d = ':';
//------------------------------------------------------------------------------
/*
   Thread 1, turn the LED off when signalled by thread 2.
*/
// Declare the thread function for thread 1.
void DHTsensor(void* arg) {
  //*
  dht.begin();
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  digitalWrite(2, LOW);
  digitalWrite(3, HIGH);
  int Temp = (int)22, Humd = (int)50;//*/
  while (1) {
    xQueueReset(dhtTemp);
    xQueueReset(dhtHumd);
    xQueueSend(dhtTemp, &Temp, (TickType_t)1);
    xQueueSend(dhtHumd, &Humd, (TickType_t)1);
    //Temp++;
    vTaskDelay((2000L * configTICK_RATE_HZ) / 1000L);
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
  lcd.init();                      // initialize the lcd
  // Print a message to the LCD.
  //lcd.backlight();
  while (1) {
    xQueuePeek(dhtTemp, &Temp, 0);
    xQueuePeek(dhtHumd, &Humd, 0);
    /*if (d == ' ') {
      d = ':';
    } else {
      d = ' ';
    }*/
    sprintf(b, "%02d* %02d%%", Temp, Humd);
    PrintLcd(0, 0, b);
    //Serial.println(b);
    //Serial.println(d);
    Serial.println(5);
    vTaskDelay((200L * configTICK_RATE_HZ) / 1000L);
  }
}
//------------------------------------------------------------------------------
void setup() {
  portBASE_TYPE s1, s2;
  Serial.begin(9600);
  Serial.println(configTICK_RATE_HZ);
  dhtTemp = xQueueCreate( 1, sizeof( int ));
  dhtHumd = xQueueCreate( 1, sizeof( int ));
  //Serial.println(configMINIMAL_STACK_SIZE);
  // create task at priority two
  s1 = xTaskCreate(DHTsensor, NULL, configMINIMAL_STACK_SIZE*25, NULL, 1, NULL);
  // create task at priority one
  s2 = xTaskCreate(LCDisplay, NULL, configMINIMAL_STACK_SIZE*25, NULL, 2, NULL);

  // check for creation errors
  if ( s1 != pdPASS || s2 != pdPASS ) {
    Serial.println(9);
    while (1);
  }
  // start scheduler
  vTaskStartScheduler();
  Serial.println(8);
  while (1);
}
//------------------------------------------------------------------------------
// WARNING idle loop has a very small stack (configMINIMAL_STACK_SIZE)
// loop must never block
void loop() {
  // Not used.
}

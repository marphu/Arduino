#include <stdio.h>
//#include <Arduino.h>
int Freq = 20000;
int val;
int BuffLen = 1000;
int data[7][3300];
int InPins[6] = {A0, A1, A2, A3, A4, A5};
unsigned long Interval = (unsigned long)(1000000 / Freq);
unsigned long b, n, bb;
void setup() {
  Serial.begin(9600);
  b = micros();
  for (int i = 0; i < BuffLen; i++) {
    for (int k = 0; k < 6; k++) {
      data[k][i] = analogRead(InPins[k]);
    }
  }
  n = micros();
  //  Serial.println(b);
  //  Serial.println(n-b);
  Serial.println(Interval - (n - b)/BuffLen);
}

void loop() {
  /*  bb = micros();
    for (int k=0;k<512;k++) {
      b = micros();
      data[1][k] = b - bb;
      data[1][k] = analogRead(1);
      data[2][k] = analogRead(2);
      data[3][k] = analogRead(3);
      data[4][k] = analogRead(4);
      data[5][k] = analogRead(5);
      data[6][k] = analogRead(6);
      data[8][k] = analogRead(7);
      delayMicroseconds(Interval-micros()+b);
    }*/
}

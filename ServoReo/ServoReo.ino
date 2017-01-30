int ledPin = 11;      // LED connected to digital pin 9
int aPx = A1;   // potentiometer connected to analog pin 3
int aPy = A0;   // potentiometer connected to analog pin 3
int val = 0;         // variable to store the read value
#include <Servo.h>

Servo myservo;  // create servo object to control a servo
void setup(){
  pinMode(ledPin, OUTPUT);   // sets the pin as output
  Serial.begin(9600);
  myservo.attach(ledPin);  // attaches the servo on pin 9 to the servo object
}

void loop(){
  Serial.print((analogRead(aPy))/1);
  Serial.print(" ");
  Serial.println((analogRead(aPx))/1);
  delay(20);
}

#include <Stepper.h>

const int IN1 = 8;
const int IN2 = 9;
const int IN3 = 10;
const int IN4 = 11;

const int stepsPerRevolution = 32; // шагов за один оборот

Stepper myStepper(stepsPerRevolution, IN1, IN2, IN3, IN4); 

void setup() {
  myStepper.setSpeed(5); // скорость 5 об/минуту
}

void loop() {
  myStepper.step(stepsPerRevolution); // шаг в одном направлении
  delay(500);
 
  Serial.println("counterclockwise");
  myStepper.step(-stepsPerRevolution); // шаг в обратную сторону
  delay(500);
}

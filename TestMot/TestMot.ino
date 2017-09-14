// Тестировалось на Arduino IDE 1.0.5
int IN1 = 7; // Input1 подключен к выводу 5
int IN2 = 6;
int ENB = 6;

void setup() {
  // put your setup code here, to run once:
  pinMode (ENB, OUTPUT);
  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
}
void RunMotor(int Speed) {
  if (Speed > 0) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite( ENB, Speed);
  } else if (Speed < 0 ) {
    digitalWrite(IN2, LOW);
    digitalWrite(IN1, HIGH);
    analogWrite( ENB, -Speed);
  } else {
    digitalWrite(IN2, LOW);
    digitalWrite(IN1, LOW);
    analogWrite( ENB, 0);
  }

}
void loop() {
  // put your main code here, to run repeatedly:
RunMotor(255);
delay(5000);
RunMotor(0);
delay(1000);
RunMotor(-255);
delay(5000);
RunMotor(0);
delay(1000);
}

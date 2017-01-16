// Тестировалось на Arduino IDE 1.0.5
int IN1 = 5; // Input1 подключен к выводу 5 
int IN2 = 4;
int ENB = 3;
int i;
void setup()
{
  pinMode (ENB, OUTPUT);
  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
}
void RunMotor(int Speed){
  if (Speed > 0){
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
void loop(){
  RunMotor(0);
  delay(3000);
  RunMotor(128);
  delay(3000);
  RunMotor(255);
  delay(3000);
  RunMotor(128);
  delay(3000);
  RunMotor(0);
  delay(3000);
  RunMotor(-128);
  delay(3000);
  RunMotor(-255);
  delay(3000);
  RunMotor(-128);
  delay(3000);
}

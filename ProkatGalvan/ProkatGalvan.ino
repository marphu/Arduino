// Тестировалось на Arduino IDE 1.0.5
int IN1 = 6; // Input1 подключен к выводу 5
int IN2 = 7;
int ENB = 2;
int LIM[] = {8, 4, 5, 6, 12};
bool BR[5], BO[5];
bool RUN, UP;
short unsigned int TYPE = 1;
int Speed = 0, Speedo = 0;
short unsigned int i;
int V = 255;
void setup()
{
  pinMode (ENB, OUTPUT);
  pinMode (IN1, OUTPUT);
  pinMode (IN2, OUTPUT);
  for(i = 0; i < 5 ; i++ ){
    pinMode (LIM[i], INPUT);
    digitalWrite(LIM[i], HIGH);
    BR[i] = ButDown(LIM[i]);
    BO[i] = BR[i];
  }
  RUN = false;
  UP = true;
  Serial.begin(9600);
}
bool ButDown (int Button) {
  if (digitalRead(Button) != HIGH){
    return true;
  } else {
    return false;
  }
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
  if (RUN) {
    if (UP) {
      Speed = V;
    } else {
      Speed = -V;
    }
  } else {
    Speed = 0;
  }
  if (Speed != Speedo ){
    RunMotor(Speed);
    Speedo = Speed;
    Serial.println(Speed);
  }
  for( i = 0; i < 5 ; i++ ){
    BR[i] = ButDown(LIM[i]);
  }
  if(BR[4] != BO[4]) {
    if (BR[4]) {
      Serial.println("Go!");
      RUN = !RUN;
    }
    BO[4] = BR[4];
  }
  if(BR[0] != BO[0]) {
    if (BR[0]) {
      Serial.println("Stop 1!");
      RUN = false;
      UP = true;
    }
    BO[0] = BR[0];
  }
  if(BR[TYPE] != BO[TYPE]) {
    if (BR[TYPE]) {
      Serial.println("Stop 2!");
      RUN = false;
      UP = false;
    }
    BO[TYPE] = BR[TYPE];
  }
 /* for( i = 0; i < 5 ; i++ ){
    BO[i] = BR[i];
    //Serial.print(BR[i]);
  }
  delay(100);*/
}

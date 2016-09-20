int R1[3] = {11,12,13};
int R2[3] = { 5, 4, 3};
long int Time = 10*1000;
int Reverse = 30;

void setup() {
  pinMode(R1[0],OUTPUT);
  pinMode(R1[1],OUTPUT);
  pinMode(R1[2],OUTPUT);
  pinMode(R2[0],OUTPUT);
  pinMode(R2[1],OUTPUT);
  pinMode(R2[2],OUTPUT);
  digitalWrite(R1[0],LOW);
  digitalWrite(R2[0],LOW);
  digitalWrite(R1[1],HIGH);
  digitalWrite(R2[1],HIGH);
  digitalWrite(R1[2],LOW);
  digitalWrite(R2[2],LOW);
  Serial.begin(9600);
}

void loop() {
  long int tim = millis();
  Serial.print("LOW\t");
  delay(Time*(Reverse)/100);
  digitalWrite(R1[2],HIGH);
  digitalWrite(R2[2],HIGH);
  Serial.println(millis() - tim);
  Serial.print("HIGHT\t");
  delay(Time*(100-Reverse)/100);
  digitalWrite(R1[2],LOW);
  digitalWrite(R2[2],LOW);
  Serial.println(millis() - tim);
}

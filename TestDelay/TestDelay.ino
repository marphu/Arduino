
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println(sizeof(micros()));
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(micros());
  delay(10000);
}

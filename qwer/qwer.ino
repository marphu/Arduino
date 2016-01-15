void setup() {
  pinMode(13, OUTPUT);
  pinMode(A0, INPUT);
}
void loop() {
  analogWrite(13, analogRead(A0));
  delay(50);
}

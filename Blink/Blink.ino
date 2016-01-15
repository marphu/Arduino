int k = 0;
int d = 1;

void setup() {
  pinMode(13, OUTPUT);
}

void loop() {
  analogWrite(13, k);   // turn the LED on (HIGH is the voltage level
  if (k == 0) {
    d = 1;
  } else if (k == 255) {
    d = -1;
  }
  k = k + d;
  delay(4);              // wait for a second
}

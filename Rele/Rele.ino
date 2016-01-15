/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://www.arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
*/
int rele[] = {13, 12};
int i;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  // initialize digital pin 13 as an output.
  for (int i = 0; i < 2 ; i++) {
    pinMode(rele[i], OUTPUT);
    Serial.println(rele[i]);
  }
  Serial.flush();
}

// the loop function runs over and over again forever
void loop() {
  i = 0;
  Serial.print(rele[i]);
  Serial.print(":\t");
  digitalWrite(rele[i], HIGH);   // turn the LED on (HIGH is the voltage
  delay(300);
  digitalWrite(rele[i], LOW);    // turn the LED off by making the voltage LOW
  Serial.print("Off.\n");
  Serial.flush();
  delay(700);              // wait for a second
  i = 1;
  Serial.print(rele[i]);
  Serial.print(":\t");
  digitalWrite(rele[i], HIGH);   // turn the LED on (HIGH is the voltage
  delay(300);
  digitalWrite(rele[i], LOW);    // turn the LED off by making the voltage LOW
  Serial.print("Off.\n");
  Serial.flush();
  delay(700);              // wait for a second
}

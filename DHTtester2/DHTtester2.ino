#include "DHT.h"
DHT dht0( 2, DHT11);
int t, h;
void setup() {
  Serial.begin(9600);
  Serial.println("DHTxx test!");
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  digitalWrite(3,HIGH);
  digitalWrite(4,LOW);
  dht0.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  h = (int)dht0.readHumidity();
  t = (int)dht0.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT 0 sensor!");
    return;
  }
  Serial.print(' ');
  Serial.print(h);
  Serial.print(' ');
  Serial.print(t);
  Serial.println("");
  //Serial.print(f);
  //Serial.print(" *F\t");
  //Serial.print("Heat index: ");
  //Serial.print(hic);
  //Serial.print(" *C ");
  //Serial.print(hif);
  //Serial.println(" *F");
}

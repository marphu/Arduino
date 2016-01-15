#include "DHT.h"
DHT dht0( 2, DHT11);
DHT dht1( 3, DHT11);
DHT dht2( 4, DHT11);
int t[3], h[3];
//float ts[3]={24,24,24}, hs[3]={35,35,35};
void setup() {
  Serial.begin(9600);
  Serial.println("DHTxx test!");

  dht1.begin();
  dht2.begin();
  dht0.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  h[0] = (int)dht0.readHumidity();
  t[0] = (int)dht0.readTemperature();
  if (isnan(h[0]) || isnan(t[0])) {
    Serial.println("Failed to read from DHT 0 sensor!");
    return;
  }
  h[1] = (int)dht1.readHumidity();
  t[1] = (int)dht1.readTemperature();
  if (isnan(h[1]) || isnan(t[1])) {
    Serial.println("Failed to read from DHT 0 sensor!");
    return;
  }
  h[2] = (int)dht2.readHumidity();
  t[2] = (int)dht2.readTemperature();
  if (isnan(h[2]) || isnan(t[2])) {
    Serial.println("Failed to read from DHT 0 sensor!");
    return;
  }
  for (int i = 0; i < 3; i++) {
    Serial.print(' ');
    Serial.print(h[i]);
  }
  for (int i = 0; i < 3; i++) {
    Serial.print(' ');
    Serial.print(t[i]);
  }
  Serial.println("");
  //Serial.print(f);
  //Serial.print(" *F\t");
  //Serial.print("Heat index: ");
  //Serial.print(hic);
  //Serial.print(" *C ");
  //Serial.print(hif);
  //Serial.println(" *F");
}

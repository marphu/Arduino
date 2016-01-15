const float freq = 0.333;
const int zap = 67;
const int OnOff[2] = {11, 10}; //Pin On|off switch
const int Polar[2] = {8, 9}; // Pin to switch polar
const int PlusMinus[2] = {10*zap/freq, 10*(100-zap)/freq}; // Длительность импульсов
unsigned long int start;
void setup() {
  //Serial.begin(9600);
  pinMode(13,OUTPUT);
  for ( int i = 0; i < 2 ; i++) {
    pinMode(OnOff[i], OUTPUT);
    digitalWrite(OnOff[i], LOW);
    pinMode(Polar[i], OUTPUT);
    digitalWrite(Polar[i], LOW);
  }
  digitalWrite(OnOff[0],HIGH);
}

void loop() {
  start = millis();
  digitalWrite(Polar[1], LOW);
  digitalWrite(Polar[0], HIGH);
  digitalWrite(13, HIGH);
  delay(PlusMinus[0]-(int)(millis()-start));
  start = millis();
  digitalWrite(Polar[0], LOW);
  digitalWrite(Polar[1], HIGH);
  digitalWrite(13, LOW);
  delay(PlusMinus[1]-(int)(millis()-start));
}

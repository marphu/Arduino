int pinI = 2;
int pinO = 12;
unsigned long duration, timeout;
 
void setup()
{
  timeout = 1000000;
  pinMode(pinI, INPUT);
  pinMode(pinO, OUTPUT);
  Serial.begin(9600);
}
 
void loop()
{
  digitalWrite(pinO, HIGH);
  delay(timeout/1000);
  digitalWrite(pinO, LOW );
  duration = pulseIn(pinI, LOW);
  Serial.println(duration);
  delay((timeout)/1000);
}

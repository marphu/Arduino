//объявляем переменные с номерами пинов
int r =  9;
int g = 10;
int b = 11;
int i = 0;
int d = 1;
void setup() //процедура setup
{
  //объявляем используемые порты
  pinMode(r, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(12, OUTPUT);
  digitalWrite(12,LOW);
  //*Serial.begin(9600);*/
}
void loop() {
  i = i + d;
  if ( i == 255) {
    d=-1;
  }
  if (i==0) {
    d=1;
  }
  analogWrite(g, i);
  analogWrite(b, 255 - i);
  analogWrite(r, abs(i * 2 - 255));
  /*
  Serial.print(i);
  Serial.print('\t');
  Serial.print(255 - i);
  Serial.print('\t');
  Serial.println(abs(i * 2 - 255));
  //*/
  delay(40);
}


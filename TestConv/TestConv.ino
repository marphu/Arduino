unsigned short int NChan = 6;
unsigned short int BSize= (NChan+1)*20+1;
char val[BSize];
unsigned short int mes = 300;
char buff[32];

void setup() {
  Serial.begin(9600);
  for (unsigned short int i = 0 ; i < BSize/(NChan+1) ; ++i ) {
    val[i*(NChan+1)] = '\n';
  }
}

void loop() {
  val[0] = mes >> 8;
  val[1] = mes;
  Serial.write(val);
  Serial.println(1);
  delay(10000);
}

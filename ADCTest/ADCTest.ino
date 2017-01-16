
unsigned int PeriodVub = 0;
unsigned long int time_current_micros, time_last_micros;
unsigned long int o;
short unsigned int NChanel = 6;
short unsigned int k;
char val[15][10];
short unsigned int tmp_val;
char buff[32];
//*
static inline uint16_t analogReadFast(byte analog_pin) {
  ADMUX = _BV(REFS0) | analog_pin;
  ADCSRA = _BV(ADSC) | _BV(ADEN) | _BV(ADPS2) | _BV(ADPS0);   //500 kHz
  while (ADCSRA & _BV(ADSC))
    ;
  uint16_t ret;
  asm volatile(
    "lds %A0,  %1\n\t"
    "lds %B0,  %2"
    : "=w" (ret): "n"(&ADCL), "n"(&ADCH));
  return ret;
}//*/

void setup() {
  Serial.begin (115200); // 9600, 19200, 38400, 57600 and 115200 bit/s
  //Serial.begin (9600);
  //
//  val[12] = '\r';
  //val[13] = '\n';
}

void loop() {
  o = 0;
  unsigned long int start = millis();
  time_last_micros = micros();
  while (o < 1000) {
    time_current_micros = micros();
    if ((time_current_micros - time_last_micros) >= PeriodVub ) {
      time_last_micros = time_current_micros;
      for ( k = 0 ; k < NChanel ; k ++) {
        tmp_val = analogRead(k);
        val[k*2+1] = tmp_val; 
        val[k*2] = tmp_val >> 8; 
      }
      //Serial.write(val,14);
      o++;
    }
  }
  Serial.println((float)((millis()-start)));
  delay(10000);
}

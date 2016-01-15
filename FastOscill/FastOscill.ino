#define MAX_ADC_RESOLUTION 12 //changing resolution doesn't seem to affect sampling rate speed in this test
static int pinAdc[15] = {A0, A1, A3, A4, A5, A6, A7, A8, A9, A10, A11};
#define numAdc 7
unsigned long Vtime;
unsigned int val[numAdc];
#define freq 20000;
#define cykle 1e6/freq;
//#define TRACKING_TIME 1
//#define ADC_SETTLING_TIME_3 17
//#define TRANSFER_PERIOD  1
long int startLoop ;
char buff[4+numAdc*2+2];
void setup() {
  Serial.begin(250000);
  SerialUSB.begin(250000);
  //check adc.h in the /sam folder
  analogReadResolution(MAX_ADC_RESOLUTION); //
  adc_init(ADC, SystemCoreClock, ADC_FREQ_MAX * 2, 2); //~47000 samples per second, from the name, I was tempted to think this would perform better. But I know nothing.
  // adc_configure_timing(ADC, TRACKING_TIME, ADC_SETTLING_TIME_3, TRANSFER_PERIOD);
  for ( int k = 0; k < numAdc; k++) {
    pinMode(pinAdc[k], INPUT);
  }
  pinMode(13,OUTPUT);
  //Serial.println(printf("%d",sizeof(val)));
}

void Recording(int Length) {
  digitalWrite(13,HIGH);
  int start = millis();
  int num_samples = 0;
  startLoop = micros();
  while ( num_samples < Length ) {
    Vtime = (micros() - startLoop);
    for ( int k = 0; k < numAdc; k++) {
      val[k] = analogRead(pinAdc[k]);
    }
    buff[0] = Vtime>>24;
    buff[1] = (Vtime>>16)%256;
    buff[2] = (Vtime>> 8)%256;
    buff[3] = Vtime%256;
    for ( int k = 0; k < numAdc; k++) {
      buff[4+2*k] = val[k]>>8;
      buff[5+2*k] = val[k]%256;
    }
    buff[4+numAdc*2+1] = 10;
    SerialUSB.write(buff,4+numAdc*2+2);
    num_samples++;
  }
  for(int i=0;i<4+numAdc*2+2;i++) {
    buff[i]=0;
  }
  SerialUSB.write(buff,4+numAdc*2+2);
  digitalWrite(13,LOW);
  Serial.print("samples read: ");
  Serial.println(Vtime);
}

void loop() {
  Recording(1024);
  delay(1000);
}


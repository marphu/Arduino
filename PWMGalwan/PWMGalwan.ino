int IN1 = 13;
int IN2 = 11;
float Freq = 100; // Hz
int perekr = 10 ;
unsigned long int Period = 1000000 / Freq;
unsigned long int Periodp = Period * perekr / 100;
unsigned long int t, t0;
unsigned long int dt0, dt1;
unsigned int i;
bool state, stateOld;
void setup() {
  //*
  Serial.begin(9600);
  /* 
  Serial.println(Freq);
  Serial.println(Period);
  Serial.println(Periodp);//*/
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN1, LOW);
  t = millis();
  state = true;
  stateOld = true;
}

void loop() {
  t0 = micros();
  //*
  if (t > t0) {
    t = micros();
  }//*/
  if ((t0 - t) < Periodp) {
    state = true;
  } else {
    state = false;
  }
  //if (state ^ stateOld ) {
  //} else {
    stateOld = state;
    if (state) {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      dt0 = t0 - t;
    } else {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      dt1 = t0 - t;
    }
  //}
  if ((t0 - t) > Period) {
    t = micros();
    //*
    i++;
    if (i == 100) {
      Serial.println(((float)(dt1)) / ((float)dt0));
      //Serial.print("\r");
      i = 0;
    }//*/
    /*
      Serial.print(t0);
      Serial.print("\t");
      Serial.println(t);//*/
  }
}

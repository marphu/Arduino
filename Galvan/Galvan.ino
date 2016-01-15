#define AnalogResolutionDAC01 12
#define MaxVoltage 3.3
#define OutputVoltage 0.7
#define freq 100;
int maxVal = pow(2, AnalogResolutionDAC01);
int medVal = pow(2, AnalogResolutionDAC01 - 1);
const float minRange = 1*MaxVoltage/6;
const float maxRange = 5*MaxVoltage/6;
int val = 0;
int d = 1;
int vala;
void SetVoltage(float Volt) {
  int Val1, Val0;
  if (Volt >= OutputVoltage) {
    Volt = OutputVoltage;
  } else if ( Volt <= -OutputVoltage) {
    Volt = -OutputVoltage;
  }
  Val0 = medVal * ( 1 + Volt / MaxVoltage);
  Val1 = medVal * ( 1 - Volt / MaxVoltage);
  analogWrite(DA0, Val0);
  analogWrite(DA1, Val1);
  Serial.print(Val0);
  Serial.print("\t");
  Serial.println(Val1);
}
void setup() {
  Serial.begin(115200);
  pinMode(DAC0, OUTPUT);
  pinMode(DAC1, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(13, OUTPUT);
  analogWriteResolution(AnalogResolutionDAC01);
  analogReadResolution(AnalogResolutionDAC01);
}

void loop() {

}

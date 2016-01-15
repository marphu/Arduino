
void testi1(char * str) {
  Serial.println("qwe");
  Serial.println(str);
  Serial.println("qwe");
//  return "&str";
}
char IntToStr(int i) {
  char b[16];
  sprintf(b, "%d", i);
  return *b;
}
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  testi1("tesa");
}

void loop() {

}

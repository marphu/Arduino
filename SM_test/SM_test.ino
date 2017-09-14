int IN[]= {11,10,9,8};
  int i;
  int k;
  bool s;
  char q[]="0000";
void setup() {

  for(i=0;i<4;i++) {
    pinMode(IN[i],OUTPUT);
    digitalWrite(IN[i],LOW);
  }
  Serial.begin(9600);
  // put your setup code here, to run once:
}

void loop() {
  
  for (i=0;i<8;i++){
    for (k=0;k<4;k++) {
        if(i>=k && i<2*k) {
          s=true;
        } else {
          s=false;
        }
        //digitalWrite(IN[k],s);
        if(s==true){
          q[k]='1';
        } else {
          q[k]='0';
        }
    }
    Serial.println(q);
    delay(50);
  }
  // put your main code here, to run repeatedly:

}

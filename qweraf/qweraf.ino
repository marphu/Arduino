// This Arduino example demonstrates bidirectional operation of a 
// 28BYJ-48, which is readily available on eBay for $4.25 inc shipping,
// using a ULN2003 interface board to drive the stepper. The 28BYJ-48 
// motor is a 4-phase, 8-beat motor, geared down by a factor of 64. One 
// bipolar winding is on motor pins 1,3 and the other on motor pins 2,4. 
// Refer to the manufacturer's documentation of Changzhou Fulling 
// Motor Co., Ltd., among others.  The step angle is 5.625/64 and the 
// operating Frequency is 100pps. Current draw is 92mA.  
// Vin w USB power is 4.5v too slow for testing use 5v pin.
//#include <Narcoleptic.h>

#define dw digitalWrite
#define dm delayMicroseconds

const int mp1 = 4;  // Blue   - 28BYJ48 pin 1
const int mp2 = 5;  // Pink   - 28BYJ48 pin 2
const int mp3 = 6;  // Yellow - 28BYJ48 pin 3
const int mp4 = 7;  // Orange - 28BYJ48 pin 4
                        // Red    - 28BYJ48 pin 5 VCC
int motorSpeed=3000;    // set stepper speed, period actually
int stepnum=0;          // current microstep 0-7
float err=0;            // running total of movedeg - requested move, always negative in degrpm()
int ar[8];              // assigned in ccw() only, to detect abnormal torque load
int movecnt=0;          // running total of steps taken in move(), &=4095 in moveto
long m;                 // millis()
int step2,d;            // nowait
int ii,k;

void setup() {
  pinMode(mp1, OUTPUT);
  pinMode(mp2, OUTPUT);
  pinMode(mp3, OUTPUT);
  pinMode(mp4, OUTPUT);
  pinMode(13, OUTPUT);  //onboard LED
  //Serial.begin(115200);
  //degrpmslowCool4_nowait(0,180*100,1000); //10RPH
  //degrpm_nowait(1,180*100,1500);
}
void loop(){
//calloften(); delay(500);
//off();while(1); //halt
}
void unramp(boolean bcw, long deg100, int rpm100, int percslow){
//to prevent bouncing at end of move with DSLR camera mounted on shaft directly
degrpm8(bcw,deg100*(100-percslow*2)/100,rpm100); //limited to 17.00 800 12v?
degrpm8(bcw,deg100*percslow/100,400);
degrpm8(bcw,deg100*percslow/100,100);
}
void unramp1(boolean bcw, long deg100, int rpm100, int percslow){
//to prevent bouncing at end of move with DSLR camera mounted on shaft directly
//faster 20rpm for 8v, 30rpm for 12v
degrpm1(bcw,deg100*percslow/100,1500); //change for 12v
degrpm1(bcw,deg100*(100-percslow*3)/100,rpm100);
degrpm1(bcw,deg100*percslow/100,400);
degrpm1(bcw,deg100*percslow/100,100);
//off for 12v
}
const float STEPERDEG=float(64)*64/360;
//call like this moveto(90.0*STEPERDEG);
//code decides which direction to move is shortest
void moveto(int steppos){
steppos&=4095;
movecnt&=4095; //this will not happen in move() can be >4096
int bcw=0;
if(steppos>movecnt)bcw=1;
if(abs(steppos-movecnt)>2048)bcw=!bcw;
movetodir(bcw,steppos);
}
void movetodir(boolean bcw, int steppos){
//absolute position you decide direction
steppos&=4095;
movecnt&=4095; //this will not happen in move() can be >4096 or <0
while(movecnt!=steppos) {
  move(bcw);
  movecnt&=4095;
  }
//can turn off()
}
void midspeedcool_12v(boolean bcw, long steps){
//use const STEPERDEG
//no reason to use this with 5-8v
//perfect for running motor all day long
//delay 1200us already in move, dm(1200) is 50% duty cycle
//dm(400) is strong, 1200 is like 5v torque?
//<5000 smoother ok with 12v only but Cool4 is better
//change 1200 to fit your desired speed
for(long i=0;i<steps;i++){move(bcw); off(); dm(1200);} 
}
void ramp(boolean bcw, int rpm100) {
//past 1200 it ramps fast in cwss() also, thats bad, so dont do it
for(int i=50;i<1200;i+=100) //1200 or rpm100 if <1200, can change step 100
degrpmEZ(bcw,900,i); //<9.00deg ignores speed
revRestart(bcw,2,rpm100,1); //can change these
}  
void revRestart(boolean bcw, long revo, int rpm100, int xinrev) {
//int j=0;
//xinrev small only
//restarts ramp in case it gets stuck due to torque load spike
if(xinrev) //>0 restarts, 0 no restart
for(long i=0;i<revo*xinrev;i++){
if(i%xinrev==0){
  //Serial.print(j++);
  //Serial.write(32);  
  }
//EZ does not delay(10) at end
degrpmEZ(bcw,36000/xinrev,rpm100); //no max
} //for
else
degrpmEZ(bcw,36000*revo,rpm100); //64 max or use rev()
}
void rev(boolean bcw, long revo) {  
long step2=revo*64*8-12; //not exactly right?
int rpm100=500; //can change
if(bcw) cwss(12,rpm100); //ramp up speed
else   ccwss(12,rpm100);
for(long i=0;i<step2;i++)
  if(bcw)cw(); else ccw();
off();
}
void degrpmslowCool4(boolean bcw, long deg100, int rph100) {  
//ccw only, more torque, less current, less heat, most efficient code for maH
//but 4x as jerky
motorSpeed=1500; //1000 or 1500 needed to pull camera 5v
int step2=deg100*64*8/360/100/2; //rounded down
int d=long(351500)*2/rph100; //see stepper.xls was /2 now *2
for(int i=0;i<step2;i++) {
ccw(); ccw(); off(); 
//can comment out line below sleeps 10ma Aroboto from 5v regulator 40ma Uno
//Narcoleptic.
delay(d);
//can hardcode cw() instead
//if(i%16==0)Serial.println(float(i+1)*360/64/8*2);
} //for
off(); //redundant
}
void degrpm_nowait(boolean bcw, long deg100, int rpm100) {  
//bcw not used pass to calloften_micro() instead
motorSpeed=1; //0?
step2=deg100*64*64/360/100;
d=long(1463600)/rpm100-20; 
m=micros();
ii=0;
}
int calloften_micro(boolean bcw){
//at least every d micros
if(ii>=step2)return(0); //saves time when done
//if(m+d-micros()<200)  //can change this
//while(micros()< m+d){}; //wait until time can change //disable this line
long ms=micros();
//if(ms>=m+d*3)Serial.println("Double-step"); //2 is double, 3 is triple
if((ms>=m+d)&&(ii<step2)){ //while would be too soon, ii< redundant
if(bcw)stepnum++; else stepnum--;
//if(bcw)movecnt++; else movecnt--;
st07();
//off(); 
m+=d; ii++;
} //if
//steps left until destination
return(step2-ii);
}
void degrpmslowCool4_nowait(boolean bcw, long deg100, int rph100) {  
//ccw only for now
motorSpeed=1500; //1000 or 1500 needed to pull camera 5v
//int or global below
step2=deg100*64*8/360/100/2;
d=long(351500)*2/rph100;
m=millis();
ii=0;
//can use global d to determine process time in loop()
//Serial.println(step2);Serial.println(d);
}
int calloften(){
//at least every d ms 
//will catch up if behind
if(ii>=step2)return(0); //saves time when done
long ms=millis();
//if(ms>=m+d*3)Serial.println("Double-step"); //2 is double, 3 is triple
while((ms>=m+d)&&(ii<step2)){
ccw(); ccw(); off(); m+=d; ii++;
//if(ii%16==1)Serial.println(float(ii)*360/64/8*2);
} //while
//steps left until destination
return(step2-ii);
}
void degrpmslowCool(boolean bcw, long deg100, int rph100) {  
//ccw only, more torque, less current, less heat
motorSpeed=1500; //why 1500 needed?
int step2=deg100*64*8/360/100; //rounded down
int d=long(351500)/2/rph100; //div 2? see stepper.xls
for(int i=0;i<step2;i++) {
//can comment out line below sleeps 10ma from 5v regulator
//Narcoleptic.delay
ccw4st1(); off(); delay(d); //cools while off
ccw4st2(); off(); delay(d);
//if(i%16==0)Serial.println(float(i+1)*360/64/8);
} //for
off(); //redundant
}
void degrpmslowHot(boolean bcw, long deg100, int rph100) {  
//ccw only, more torque, hot w 12v, use for 5v
motorSpeed=1000; //difference#1
int step2=deg100*64*8/360/100; //rounded down
int d=long(351500)/2/rph100; //div 2? see stepper.xls
for(int i=0;i<step2;i++) {
ccw4st1(); delay(d);  //difference#2 off()
ccw4st2(); delay(d);
//if(i%16==0)Serial.println(float(i+1)*360/64/8);
} //for
off();
}
void degrpmslow2(boolean bcw, long deg100, int rph100) {  
//ccw only, more torque, less current, less heat, 50% duty cycle
//compromise Hot,Cool
motorSpeed=1000; //12v
int step2=deg100*64*8/360/100; //rounded down
int d=long(351500)/2/rph100; //div 2? see stepper.xls
for(int i=0;i<step2;i++) { //only difference is below
ccw4st1(); off(); delay(d/4); st1(); delay(d/2); off(); delay(d/4);
ccw4st2(); off(); delay(d/4); st2(); delay(d/2); off(); delay(d/4);
} //for
off();
}
void degrpm1(boolean bcw, long deg100, int rpm100) {
//do your own ramping past 17RPM
long step2=deg100*64*8/360/100; //rounded down no limit
if(rpm100<50)rpm100=50; //minimum should use degrpmslow()
motorSpeed=long(1463600)/rpm100-20;  //see stepper.xls
for(long i=0;i<step2;i++)
  if(bcw)cw(); else ccw();
//can move() here to get better resolution
}
void degrpmEZ(boolean bcw, long deg100, int rpm100) {
//no delay(10) at end
//first 5 lines of degrpm  
//max 64 turns
//max 3500 rpm
int step2=deg100*64*8/360/100; //rounded down
if(rpm100<50)rpm100=50; //minimum should use degrpmslow()
rpm100=long(1463600)/rpm100-20;  //see stepper.xls
if(bcw) cwss(step2,rpm100);
else   ccwss(step2,rpm100);
}
void degrpm(boolean bcw, long deg100, int rpm100) {  
//max 64 turns or 23,000 deg or 2,300,000 deg100 long is bigger
//max 3500 rpm100 with 12v
int step2=deg100*64*8/360/100; //rounded down
if(rpm100<50)rpm100=50; //minimum should use degrpmslow()
rpm100=long(1463600)/rpm100-20;  //see stepper.xls
if(bcw) cwss(step2,rpm100);
else   ccwss(step2,rpm100);

//with this code you can step by 2.00 deg 180x will be 360+-1
//0.50 deg 720x works to 360
//even though step size is >0.50 or 0.72?
float movedeg=float(step2)*360/64/8;  //float library adds 2K size to sketch
//Serial.println(movedeg);
//Serial.println(movedeg-(float)deg100/100);  //moved too little only? never too much
err+=(movedeg-(float)deg100/100);
if(err<-1) {
motorSpeed=1200;
if(bcw) cw(); else ccw();
err+=(float(360)/64/8);
//Serial.print("err=");Serial.println(err);
} //if err
//soft stop moves further than it should
//if(bcw) cwss(15,2000);
//else   ccwss(15,2000);
delay(10); //so it stops and holds before off in loop
//off();
}
void degrpm8(boolean bcw, long deg100, int rpm100) {  
//max 8 turns
//max <<3500 rpm100 with 12v cuz no ramping ~17RPM
const int min2start=800; //800 12v, 1200 5v
long deg100a=deg100; //deg100 used later dont change
if(deg100a<300)deg100a +=4; //now err can be positive, check %=0 instead?
int step2=deg100a*64*64/360/100; //rounds up/down by 1/2 cuz +4 above, error 1/22nd of degree
if(rpm100<50)rpm100=50; //minimum should use degrpmslow()
rpm100=long(1463600)/rpm100-20;  //see stepper.xls same
motorSpeed=rpm100;
if(motorSpeed<min2start)motorSpeed=min2start;
for(int i=0;i<step2;i++) {
  if(bcw)stepnum++; else stepnum--;
  st07();
  }
//was if(bcw)cwss(step2,rpm100);
//does not adjust position for err like degrpm cuz step 0.044 or 1/22 deg small
float movedeg=float(step2)*360/64/64;  //float library adds 2K size to sketch
//Serial.println(movedeg);
err+=(movedeg-(float)deg100/100);  //not used
//Serial.println(err);
//if(err<-1) {motorSpeed=1200; if(bcw) cw(); else ccw(); err+=(float(360)/64/8);}
//off();
}
void move(boolean bcw){
motorSpeed=1200;
if(bcw)stepnum++; else stepnum--;
if(bcw)movecnt++; else movecnt--;
st07();
//if(movecnt%16==1)Serial.println(float(movecnt)*360/64/64);
}
void ccwss(int steps, int speed) {
int norm=0,last=0,nalarm=0,nar=0;
long sum=0;
//900 self starting 5v motor 5v ps
//800 most of 100% torque
//700 less torque
//600 almost none
//5v motor 12v supply:
//700 self starting
//400 decent torque
//6.6v 20RPM
//8.3v load 8.5v float 6x NiMh
//20RPM strong 
//28RPM weak
//15RPM 100% torque
//^does not overheat
//5V 10RPM strong
//5V 20RPM weak
//5V 24RPM no torque
//12V 20RPM self start 35 max speed
//if(speed<700) steps-=10;
//ramp up speed
//const int readadj=100;
motorSpeed=1000; //<12 steps, 800 for 12v
if(steps>=12) {
motorSpeed=3000; if(speed<1200) {ccw(); ccw(); steps-=2;}
motorSpeed=1200; if(speed<1200) {ccw(); ccw(); steps-=2;}
motorSpeed=800;  if(speed<800 ) {ccw(); ccw(); steps-=2;}
motorSpeed=700;  if(speed<700 ) for(int i=0;i<4;i++) {ccw(); steps--;} 
motorSpeed=speed; 
} //if
//torque load detect code to prevent destroying motor
for(int i=0;i<steps;i++){
  if(nar>=1024){ //lower for slow?
    k=sum*5/nar; //why not 6? div 0? fixed
    if((k==last)&&(!norm)&&(millis()>5*1000)) norm=k; //since program start
    if((k==last)&&(k==norm+1)) norm=k; //within 40 sec when if k>norm+0 below
    //if((k==last)&&(k==norm-1)) norm=k;
    digitalWrite(13,LOW);
    if(!norm)digitalWrite(13,HIGH); //ON until normal is found
//10,-10 for 8v level ground
// 0,-10 for 12v >30RPM only
    if(((k>norm+20)&&norm)||((k<norm-20)&&norm)){ //0,-10 can change 2B less sensitive with high load
      digitalWrite(13,HIGH);
      //will never print past 1 min
      //Serial.println();Serial.print(millis()/1000/60);Serial.print(" Min ");Serial.print(nalarm+1);Serial.println(" count");
      if(millis()>long(40)*1000){ off(); while(1);} //infinite loop
      if(nalarm++>10){ off(); while(1);} //can change 10
    } //if load
    last=k;
    sum=nar=0;
  } //if 1024 
  ccw();
  //if(i%16==0)Serial.println(float(i+1)*360/64/8);
  //can detect individual outliers here
  nar+=8; for(int j=0;j<8;j++) sum+=ar[j];
} //for
} //ccwss()

void cwss(int steps, int speed) {
motorSpeed=1000; //<12 steps, 800 for 12v
if(steps>=12) {
motorSpeed=3000; if(speed<1200) {cw(); cw(); steps-=2;}
motorSpeed=1200; if(speed<1200) {cw(); cw(); steps-=2;}
motorSpeed=800;  if(speed<800 ) {cw(); cw(); steps-=2;}
motorSpeed=700;  if(speed<700 ) for(int i=0;i<4;i++) {cw(); steps--;}
motorSpeed=speed; 
} //if
for(int i=0;i<steps;i++) cw();  //64*8 is 1 rev
//load detect code is ccwss only for now
}
void ccw4st1(){
  dw(mp1, HIGH);
  dw(mp2, LOW);
  dw(mp3, LOW);
  dw(mp4, LOW);
  dm(motorSpeed);
  //dw(mp1, HIGH);
  dw(mp2, HIGH);
  //dw(mp3, LOW);
  //dw(mp4, LOW);
  dm(motorSpeed);
  dw(mp1, LOW);
  //dw(mp2, HIGH);
  //dw(mp3, LOW);
  //dw(mp4, LOW);
  dm(motorSpeed);
  //dw(mp1, LOW);
  //dw(mp2, HIGH);
  dw(mp3, HIGH);
  //dw(mp4, LOW);
  dm(motorSpeed);
}
void st1(){
  dw(mp1, LOW);
  dw(mp2, HIGH);
  dw(mp3, HIGH);
  dw(mp4, LOW);
}
void ccw4st2(){
  dw(mp1, LOW);
  dw(mp2, LOW);
  dw(mp3, HIGH);
  dw(mp4, LOW);
  dm(motorSpeed);
  //dw(mp1, LOW);
  //dw(mp2, LOW);
  //dw(mp3, HIGH);
  dw(mp4, HIGH);
  dm(motorSpeed);
  //dw(mp1, LOW);
  //dw(mp2, LOW);
  dw(mp3, LOW);
  //dw(mp4, HIGH);
  dm(motorSpeed);
  dw(mp1, HIGH);
  //dw(mp2, LOW);
  //dw(mp3, LOW);
  //dw(mp4, HIGH);
  dm(motorSpeed);
}
void st2(){
  dw(mp1, HIGH);
  dw(mp2, LOW);
  dw(mp3, LOW);
  dw(mp4, HIGH);
}
void off(){
  dw(mp1, LOW);
  dw(mp2, LOW);
  dw(mp3, LOW);
  dw(mp4, LOW);
}
//////////////////////////////////////////////////////////////////////////////
//set pins to ULN2003 high in sequence from 1 to 4
void ccw (){
//may throw off rev() and slow() by 14us
//for j loop in ccwss() takes <2ms/rev
int ms=motorSpeed-114; //114 cuz ar 100 calibrated inc code in ccwss()>cwss()
  // 1
  dw(mp1, HIGH);
  dw(mp2, LOW);
  dw(mp3, LOW);
  dw(mp4, LOW);
ar[0]=analogRead(0);
  dm(ms);
  // 2
  dw(mp1, HIGH);
  dw(mp2, HIGH);
  dw(mp3, LOW);
  dw(mp4, LOW);
ar[1]=analogRead(0);
  dm(ms);
  // 3
  dw(mp1, LOW);
  dw(mp2, HIGH);
  dw(mp3, LOW);
  dw(mp4, LOW);
ar[2]=analogRead(0);
  dm(ms);
  // 4
  dw(mp1, LOW);
  dw(mp2, HIGH);
  dw(mp3, HIGH);
  dw(mp4, LOW);
ar[3]=analogRead(0);
  dm(ms);
  // 5
  dw(mp1, LOW);
  dw(mp2, LOW);
  dw(mp3, HIGH);
  dw(mp4, LOW);
ar[4]=analogRead(0);
  dm(ms);
  // 6
  dw(mp1, LOW);
  dw(mp2, LOW);
  dw(mp3, HIGH);
  dw(mp4, HIGH);
ar[5]=analogRead(0);
  dm(ms);
  // 7
  dw(mp1, LOW);
  dw(mp2, LOW);
  dw(mp3, LOW);
  dw(mp4, HIGH);
ar[6]=analogRead(0);
  dm(ms);
  // 8
  dw(mp1, HIGH);
  dw(mp2, LOW);
  dw(mp3, LOW);
  dw(mp4, HIGH);
ar[7]=analogRead(0);
  dm(ms);
}
//////////////////////////////////////////////////////////////////////////////
//set pins to ULN2003 high in sequence from 4 to 1
void cw(){
  // 1
  dw(mp4, HIGH);
  dw(mp3, LOW);
  dw(mp2, LOW);
  dw(mp1, LOW);
  dm(motorSpeed);
  // 2
  //dw(mp4, HIGH);
  dw(mp3, HIGH);
  //dw(mp2, LOW);
  //dw(mp1, LOW);
  dm(motorSpeed);
  // 3
  dw(mp4, LOW);
  //dw(mp3, HIGH);
  //dw(mp2, LOW);
  //dw(mp1, LOW);
  dm(motorSpeed);
  // 4
  //dw(mp4, LOW);
  //dw(mp3, HIGH);
  dw(mp2, HIGH);
  //dw(mp1, LOW);
  dm(motorSpeed);
  // 5
  //dw(mp4, LOW);
  dw(mp3, LOW);
  //dw(mp2, HIGH);
  //dw(mp1, LOW);
  dm(motorSpeed);
  // 6
  //dw(mp4, LOW);
  //dw(mp3, LOW);
  //dw(mp2, HIGH);
  dw(mp1, HIGH);
  dm(motorSpeed);
  // 7
  //dw(mp4, LOW);
  //dw(mp3, LOW);
  dw(mp2, LOW);
  //dw(mp1, HIGH);
  dm(motorSpeed);
  // 8
  dw(mp4, HIGH);
  //dw(mp3, LOW);
  //dw(mp2, LOW);
  //dw(mp1, HIGH);
  dm(motorSpeed);
}
void st07(){
if(stepnum==-1)stepnum=7;
if(stepnum== 8)stepnum=0;
switch(stepnum){
case 0:
  dw(mp4, HIGH);
  dw(mp3, LOW);
  dw(mp2, LOW);
  dw(mp1, LOW);
  break;
case 1:
  dw(mp4, HIGH);
  dw(mp3, HIGH);
  dw(mp2, LOW);
  dw(mp1, LOW);
  break;
case 2:
  dw(mp4, LOW);
  dw(mp3, HIGH);
  dw(mp2, LOW);
  dw(mp1, LOW);
  break;
case 3:
  dw(mp4, LOW);
  dw(mp3, HIGH);
  dw(mp2, HIGH);
  dw(mp1, LOW);
  break;
case 4:
  dw(mp4, LOW);
  dw(mp3, LOW);
  dw(mp2, HIGH);
  dw(mp1, LOW);
  break;
case 5:
  dw(mp4, LOW);
  dw(mp3, LOW);
  dw(mp2, HIGH);
  dw(mp1, HIGH);
  break;
case 6:
  dw(mp4, LOW);
  dw(mp3, LOW);
  dw(mp2, LOW);
  dw(mp1, HIGH);
  break;
case 7:
  dw(mp4, HIGH);
  dw(mp3, LOW);
  dw(mp2, LOW);
  dw(mp1, HIGH);
  break;
}
//for all cases
dm(motorSpeed);
}

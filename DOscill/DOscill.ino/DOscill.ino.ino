//
// Oscilloscope
// http://accrochages.drone.ws/en/node/90
//

#define ANALOGA_IN 0
#define ANALOGB_IN 1
#define ANALOGC_IN 2
#define ANALOGD_IN 3
#define ANALOGE_IN 4
#define ANALOGF_IN 5

int counter = 0;

void setup() 
{
  Serial.begin(38400);
}

void loop() 
{
  int val[5];

  val[0] = analogRead(ANALOGA_IN);
  val[1] = analogRead(ANALOGB_IN);
  val[2] = analogRead(ANALOGC_IN);
  val[3] = analogRead(ANALOGD_IN);
  val[4] = analogRead(ANALOGE_IN);
  val[5] = analogRead(ANALOGF_IN);


  Serial.print( "A" );
  Serial.print( val[0] );

  Serial.print( "B" );
  Serial.print( val[1] );
  Serial.print( "C" );
  Serial.print( val[2] );
  Serial.print( "D" );
  Serial.print( val[3] );
  Serial.print( "E" );
  Serial.print( val[4] );
  Serial.print( "F" );
  Serial.println( val[5] );
}


#include <MINDSi.h>
#include <Servo.h>

/***************************************************
/ Example provided by MINDS-i
/ Try checking out our arduino resource guide at
/ http://mindsieducation.com/programming-resources
/ Questions? Concerns? Bugs? email code@mymindsi.com
/
/ This example expects 2 servos in pins 4 and 5
/ three QTI sensors in pins A0, A1, and A2
/ and a forward facing ping sensor on pin 9
/***************************************************/

Servo left, right;

float Lout, Rout;
float Lset, Rset;

const double rxtime    = 0.25;
const int neutral      = 90;
const int leftCenter   = 0;
const int rightCenter  = 0;
const int outsideSpeed = 45;
const int insideSpeed  = 12;
const int forwardSpeed = 35;
const int hazardDist   = 750;

void setup() {
  Serial.begin(9600);

  left.attach(5);
  right.attach(4);

  left.write(neutral);
  right.write(neutral);

  Rout = neutral;
  Lout = neutral;
}

void loop() {
  bool left   = QTI(A0) > 120;
  bool middle = QTI(A1) > 120;
  bool right  = QTI(A2) > 120;

  if (left) {
    Lset = neutral + insideSpeed;
    Rset = neutral + outsideSpeed;
  }
  else if (right) {
    Lset = neutral - outsideSpeed;
    Rset = neutral - insideSpeed;
  }
  else if (middle) {
    Lset = neutral - forwardSpeed;
    Rset = neutral + forwardSpeed;
  }

  if (getPing(9) < hazardDist) {
    Rset = neutral;
    Lset = neutral;
  }

  output();
}

//all this is because the QTI Blocks for a variable amount of time
void output() {
  static uint32_t time = millis();

  double dT = ( double(millis() - time) / 100) ;
  time = millis();

  double adj = pow(rxtime, dT);

  Rout = ( Rout * adj ) + ( Rset * (1 - adj) );
  Lout = ( Lout * adj ) + ( Lset * (1 - adj) );

  left.write(Lout + leftCenter);
  right.write(Rout+ rightCenter);
}

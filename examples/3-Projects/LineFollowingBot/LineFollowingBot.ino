#include <MINDSi.h>
#include <Servo.h>

/***************************************************
/ Example provided by MINDS-i
/ Try checking out our arduino resource guide at
/ http://mindsieducation.com/programming-resources
/ Questions? Concerns? Bugs? email code@mymindsi.com
/
/ This example expects 2 servos in pins 4 and 5
/ and three QTI sensors in pins A0, A1, and A2
/***************************************************/

Servo leftServo, rightServo;
bool left, middle, right;

float Lout, Rout;
float Lset, Rset;

unsigned long time;
double dT;
double adj;

const int center =  90;

void setup() {
  leftServo.attach(5);
  rightServo.attach(4);

  leftServo.write(center);
  rightServo.write(center);

  Rout = center;
  Lout = center;
}

void loop() {
  left = QTI(A0) > 120;
  middle = QTI(A1) > 120;
  right = QTI(A2) > 120;

  if (left) {
    Lset = center + 6;
    Rset = center + 45;
  }
  else if (right) {
    Lset = center - 45;
    Rset = center - 6;
  }
  else if (middle) {
    Lset = center - 45;
    Rset = center + 45;
  }

  if (getPing(9) < 750) {
    Rset = center;
    Lset = center;
  }

  output();
}

void output() {
  //all this is because the QTI Blocks for a variable amount of time
#define rxtime .25

  dT = ( double(millis() - time) / 100) ;
  time = millis();

  adj = pow(rxtime, dT);

  Rout = ( Rout * adj ) + ( Rset * (1 - adj) );
  Lout = ( Lout * adj ) + ( Lset * (1 - adj) );

  leftServo.write(Lout);
  rightServo.write(Rout);
}



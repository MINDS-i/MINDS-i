#include <MINDSi.h>
#include <Servo.h>

/***************************************************
/ mymindsi.com
/
/ Line Following Robot - Line only code
/
/This code will follow a line using its three QTI sensors
/it uses a time-adjusted exponential running average to smooth out
/motion and prevent wobbling.
/***************************************************/

Servo leftServo,rightServo;
bool left, middle, right;

float Lout, Rout;
float Lset, Rset;

unsigned long time;
double dT;
double adj;

const int center = 90;

void setup(){
  Serial.begin(115200);

  leftServo.attach(5); //set a pin for a servo/ESC to use
  rightServo.attach(4);

  leftServo.write(center);
  rightServo.write(center);

  Rout = center;
  Lout = center;
}

void loop(){
  left = QTI(A0) > 120;
  middle = QTI(A1) > 120;
  right = QTI(A2) > 120;

  if (left) {
    Lset = center + 6;
    Rset = center + 45;
  }
  else if(right){
    Lset = center - 45;
    Rset = center - 6;
  }
  else if(middle){
    Lset = center - 45;
    Rset = center + 45;
  }

  output();
}

void output(){
  //all this is because the QTI Blocks for a variable amount of time
  #define rxtime .25

  dT = ( double(millis() - time) / 100) ;
  time = millis();

  adj = pow(rxtime, dT);

  Rout = ( Rout*adj ) + ( Rset*(1-adj) );
  Lout = ( Lout*adj ) + ( Lset*(1-adj) );

  leftServo.write(Lout);
  rightServo.write(Rout);
}


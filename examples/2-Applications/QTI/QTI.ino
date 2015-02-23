#include <MINDSi.h>
#include <Servo.h>

/***************************************************
/ MINDS-i QTI Application. mymindsi.com
/
/This QTI Application code is a basic example
/of how to incorporate your QTI sensor in the
/Arduino code.  In this code your rover will
/follow a black line.
/***************************************************/

Servo steer, drive;

const int threshold = 300;

void setup()
{
  drive.attach(4); //set a pin for the ESC/steering servo to use
  steer.attach(5);

  drive.write(90); //set the output for the ESC/servo
  steer.write(90);

  delay(2000); //delay 2 seconds for arming

  drive.write(100);
}

void loop() {
  //first check if the left sensor is on the line, if not check the right sensor
  //and finally check if the center sensor says to go strait, otherwise
  //don't change the desired direction

  if (QTI(A0) < threshold) {
    steer.write(45);
  } else if (QTI(A2) < threshold) {
    steer.write(135);
  } else if (QTI(A1) < threshold) {
    steer.write(90);
  }
}

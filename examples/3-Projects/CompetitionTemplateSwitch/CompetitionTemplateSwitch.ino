#include <MINDSi.h>
#include <Servo.h>

/***************************************************
/ Example provided by MINDS-i
/ Try checking out our arduino resource guide at
/ http://mindsieducation.com/programming-resources
/ Questions? Concerns? Bugs? email code@mymindsi.com
/
/ This example shows how to switch control between
/ two modes based on a radio switch plugged in to
/ pin 7
/***************************************************/

Servo steer, drive;

void setup() {
  drive.attach(4);
  steer.attach(5);

  drive.write(90);
  steer.write(90);

  delay(2000);
}

void loop() {
  if (getRadio(7) > 90) {
    radioControl();
  } else {
    radioOff();
  }
}

void radioControl() {
}

void radioOff() {
}

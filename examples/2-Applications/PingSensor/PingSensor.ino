#include <MINDSi.h>
#include <Servo.h>

/***************************************************
/ MINDS-i Ping Application. mymindsi.com
/
/This Ping Application code is a basic example
/of how to incorporate your PING sensor in the
/Arduino code.  In this code your rover will
/drive until it sees an object with the PING
/sensor and then it will stop.
/***************************************************/

Servo drive, steer;

void setup() {
  drive.attach(4); //set a pin for the ESC/steering servo to use
  steer.attach(5);

  drive.write(90); //set the output for the ESC/servo
  steer.write(90);

  delay(2000); //delay 2 seconds for arming

  steer.write(180);
}

void loop() {
  //if there is no room in front, stop
  if (getPing(10) < 1500) {
  	drive.write(90);
  } else {  //otherwise go forward
  	drive.write(100);
  }
}

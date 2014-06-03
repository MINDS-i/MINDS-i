#include <MINDSi.h>
#include <Servo.h>

/***************************************************
/ MINDS-i ESC Application. mymindsi.com
/
/This ESC Application code is a basic example
/of how to incorporate your ESC in the Arduino
/code.  In this code your rover will drive forward
/for a certain time, stop and drive back in reverse
/for the same amount as the previous time.
/***************************************************/

Servo drive, steer;

void setup() {
  drive.attach(4); //set a pin for the ESC/steering servo to use
  steer.attach(5);

  drive.write(90); //set the output for the ESC/servo
  steer.write(90);

  delay(2000); //delay 2 seconds for arming
}

void loop() {
  drive.write(100); //drive forward one second
  delay(1000);

  drive.write(0); //brake and wait
  delay(1000);

  drive.write(95); //dissengage brakes
  delay(150);

  drive.write(80); //drive backward one second
  delay(1500);
}

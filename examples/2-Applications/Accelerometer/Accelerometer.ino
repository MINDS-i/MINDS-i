#include <MINDSi.h>
#include <Servo.h>

/***************************************************
/MINDS-i Accelerometer Application. mymindsi.com
/
/This Accelerometer Application code is a basic example
/of how to incorporate your Accelerometer in the Arduino
/code. In this code your rover will drive straight until
/it tips over in which case it will stop.
/***************************************************/

const int downReading = 700;
Servo drive, steer;
float Z;

void setup() {
  drive.attach(4); //set a pin for the ESC/steering servo to use
  steer.attach(5);

  drive.write(90); //set the output for the ESC/servo
  steer.write(90);

  delay(2000); //delay 2 seconds for arming
}

void loop() {
  Z = analogRead(A2)/1024;  //store the value read

  //if less than half of normal G's is read on the Z axis (up/down), stop
  if (Z < downReading) {
  	drive.write(90);
  } else {
  	drive.write(100); //otherwise drive
  }
}

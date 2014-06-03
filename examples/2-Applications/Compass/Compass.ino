#include <Wire.h>  //necessary only for I2C devices
#include <MINDSi.h>
#include "Compass.h"
#include <Servo.h>

/***************************************************
/ MINDS-i Compass Application. mymindsi.com
/
/This Compass Application code is a basic example
/of how to incorporate your Compass in the Arduino
/code.  In this code your rover will drive and turn
/until it faces north.
/***************************************************/

Servo drive, steer;

void setup() {
  beginCompass();

  drive.attach(4); //set a pin for the ESC/steering servo to use
  steer.attach(5);

  drive.write(90); //set the output for the ESC/servo
  steer.write(90);

  delay(2000); //delay 2 seconds for arming

  steer.write(135); //turn wheels right
}

void loop() {
  if (fabs( getHeading() ) < .05) {
  	drive.write(90); //stop when near north
  } else {
  	drive.write(100); //otherwise drive (turning)
  }
}

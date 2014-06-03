#include <MINDSi.h>
#include <Servo.h>

/***************************************************
/ MINDS-i Radio Drive Application. mymindsi.com
/
/This Simple Radio Drive Application code
/is a basic example of how to incorporate
/your Radio Transmitter and Receiver in
/the Arduino code.  In this code your rover
/will be controlled manually by your radio
/transmitter.
/***************************************************/

Servo drive, steer;
int driveSig, steerSig;

void setup() {
  drive.attach(4); //set a pin for the ESC/steering servo to use
  steer.attach(5);

  drive.write(90); //set the output for the ESC/servo
  steer.write(90);

  delay(2000); //delay 2 seconds for arming
}

void loop() {
  driveSig = getRadio(2);
  steerSig = getRadio(3);

  drive.write( driveSig );  //set the servo to the inbound radio value
  steer.write( steerSig );
}

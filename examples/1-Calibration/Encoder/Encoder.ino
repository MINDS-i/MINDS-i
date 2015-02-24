#include <Servo.h>
#include <MINDSi.h>
#include "Encoder.h"

/***************************************************
/MINDS-i Encoder Calibration. mymindsi.com
/
/This Encoder Calibration code is used to test
/and adjust your encoder as well as to establish
/the measurement that it’s reading.
/
/To learn more about the encoder visit:
/http://www.usdigital.com/products/encoders/incremental/rotary/kit/e2
/***************************************************/

float rpm;
float rev;
float mph;

const int encoderPins[2] = {2,3};

//Since these conversions vary based on measurments of the robot, these must be calculated here
//(wheel Diameter*PI) * minutes per hour / (inches per mile * gear ratio)
float RPMtoMPH = ((5. *PI) * 60.) / (63360.*(37. / 13.));

void setup() {
  Serial.begin(9600);  //start a serial connection
  encoder::begin(encoderPins[0], encoderPins[1]);
}

void loop() {
  rpm = encoder::getRPM();
  rev = encoder::getRev();
  mph = rpm * RPMtoMPH;
  
  Serial.print("RPM: "); 
  Serial.print(rpm);
  Serial.print("\tMPH: ");
  Serial.print(mph);
  Serial.print("\tREV: ");
  Serial.print(rev);
  Serial.print("\n");
}

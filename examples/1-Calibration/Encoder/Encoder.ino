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

float val;
float mph;
int dist;
float feet;

//Since these conversions vary based on measurments of the robot, these must be calculated here
//(wheel Diameter*PI) * minutes per hour / (inches per mile * gear ratio)
float RPMtoMPH = ((5. *PI) * 60.) / (63360.*(37. / 13.));

//(wheel Diameter*PI) / (inches to foot * steps per rotation * gear ratio)
float STPtoFeet = (5. *PI) / (12.*100.*(37. / 13.));

void setup() {
  Serial.begin(9600);  //start a serial connection
  startEncoder();  //start the encoder counter
}

void loop() {
  val = getRPM();  //read the average rpm of the last 50th of a second
  mph = val * RPMtoMPH;
  dist = getSteps();  //read the total steps the encoder has moved
  feet = dist * STPtoFeet;


  Serial.print("RPM: ");  //send a string or value on the serial connection
  Serial.print(val);
  Serial.print("\tMPH: ");
  Serial.print(mph);

  Serial.print("\tSteps: ");
  Serial.print(dist);
  Serial.print("\tFeet: ");
  Serial.println(feet);
}

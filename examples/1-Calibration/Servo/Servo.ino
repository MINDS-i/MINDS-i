#include <Servo.h>

/***************************************************
/MINDS-i Calibration. mymindsi.com
/
/Servo Calibration
/
/This Servo Calibration code is used to test
/and center your servo.
/
/a servo on pin 5 will be moved to its center position
/
/To learn more about our stock servos visit:
/http://www.hitecrcd.com/products/analog/standard-sport/hs-485hb.html
/***************************************************/

Servo myservo;

void setup(){
  myservo.attach(5); //set a pin for a servo/ESC to use
}

void loop(){
  myservo.write (90); //set the output to 90 degrees (center)
}



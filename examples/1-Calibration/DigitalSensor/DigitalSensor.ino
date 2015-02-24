#include <MINDSi.h>
#include <Servo.h>

/***************************************************
/MINDS-i Digital Sensor Calibration. mymindsi.com
/
/This Digital Sensor Calibration code is used to test
/and adjust various digital sensors. The sensors should
/be plugged into pin 3
/
/To learn more about the digital sensors visit:
/http://arduino.cc/playground/Main/InterfacingWithHardware
/***************************************************/

int val;

void setup() {
  Serial.begin(9600);  //start a serial connection
}

void loop() {
  val = digitalRead(3);  //read in the digital value on pin 3
  Serial.println(val);  //send a string or value on the serial connection
}

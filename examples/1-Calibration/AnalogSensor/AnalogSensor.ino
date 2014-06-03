#include <MINDSi.h>
#include <Servo.h>

/***************************************************
/MINDS-i Analog Sensor Calibration. mymindsi.com
/
/This code will send an analog sensor's value to the serial terminal
/the sensor should be plugged in to pin A0
/
/To learn more about the analog sensors visit:
/http://arduino.cc/playground/Main/InterfacingWithHardware
/***************************************************/

int val;

void setup() {
  Serial.begin(9600);  //start a serial connection
}

void loop() {
  val = analogRead(A0); //read in the value on analog pin 0
  Serial.println(val);  //send a string or value on the serial connection
}

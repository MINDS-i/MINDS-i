#include <MINDSi.h>
#include <Servo.h>

/***************************************************
/MINDS-i Radio Calibration. mymindsi.com
/
/
/This Radio Test Calibration code is used to test
/and adjust your Radio transmitter and to establish
/the signal that your radio is sending.
/due to hardware limitations, receiving radio signals
/on pins other than 2 and/or 3 will be more twitchy than
/usual
/***************************************************/

int val;

void setup() {
  Serial.begin(9600); //start a serial connection
}

void loop() {
  val = getRadio(2);  //read the value being sent on pin 2
  Serial.println(val);  //send a string or value on the serial connection
}

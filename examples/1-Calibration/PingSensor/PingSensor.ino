#include <MINDSi.h>
#include <Servo.h>

/***************************************************
/MINDS-i Ping Calibration. mymindsi.com
/
/
/This Ping Calibration code is used to test
/and adjust your ping sensor as well as to establish
/the measurement that it’s reading. Because some sensors
/vary, this calibration is recommended before applying
/it in your code to avoid malfunctions.
/
/To learn more about the ping sensor visit:
/http://arduino.cc/en/Tutorial/Ping
/***************************************************/

void setup() {
  Serial.begin(9600); //start a serial connection
}

void loop() {
  //send the value read by the ping sensor out serial
  Serial.println( getPing(10) );

  //optional code for displaying three QTI sensors, make sure the previous code
  //is commented out when using the code below.
  /*
  	Serial.print(getPing(9));
  	Serial.print("\t"); //insert some tabs
  	Serial.print(getPing(10));
  	Serial.print("\t");
  	Serial.println(getPing(11));
  */
}

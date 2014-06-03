#include <Wire.h> //necessary only for I2C devices
#include <MINDSi.h>
#include "Compass.h"

/***************************************************
/MINDS-i Compass Calibration. mymindsi.com
/
/
/This Compass Calibration code is used give feedback
/on the compass's reading.
/
/Compasses are very sensative to ferrous and/or magnetic
/materials. Without finding the calibration constants
/for a particular application and sensor,
/the value could be very skewed.
/
/in addition to the regular includes, the compass sensor
/requires Wire.h to be included before MINDSi.h
/
/To learn more about our standard compass visit:
/http://www.parallax.com/Store/Sensors/CompassGPS/tabid/173/CategoryID/48/List/0/SortField/0/Level/a/ProductID/779/Default.aspx
/***************************************************/

float val;

void setup() {
  Serial.begin(9600); //start a serial connection

  beginCompass();     //turn on the compass sensor
}

void loop() {
  val = getHeading(); //read the current heading from the sensor

  Serial.println(val,DEC); //send a string or value on the serial connection
}

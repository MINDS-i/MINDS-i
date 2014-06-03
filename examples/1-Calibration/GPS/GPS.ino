#include <MINDSi.h>
#include <NMEA.h>

/***************************************************
/MINDS-i GPS Calibration. mymindsi.com
/
/This GPS Calibration code is used to test
/and adjust your GPS as well as to establish
/the measurement that it’s reading.
/
/To learn more about the GPS visit:
/http://www.parallax.com/Store/Sensors/CompassGPS/tabid/173/CategoryID/48/List/0/Level/a/ProductID/757/Default.aspx?SortField=ProductName%2cProductName
/***************************************************/

NMEA nmea(Serial1);

void setup() {
  Serial.begin(9600);
  Serial1.begin(38400);
}

void loop() {
  nmea.update();
  if(nmea.getWarning()==false){
  	Serial.print(nmea.getLatitude(), DEC);
  	Serial.print('\t');
  	Serial.print(nmea.getLongitude(), DEC);
  	Serial.print('\t');
  } else {
  	Serial.println("No lock");
  }

  delay(100);
}

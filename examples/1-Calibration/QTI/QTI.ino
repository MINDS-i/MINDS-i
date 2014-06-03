#include <MINDSi.h>
#include <Servo.h>

/***************************************************
/MINDS-i QTI Calibration. mymindsi.com
/
/This QTI Calibration code is used to test
/and adjust your QTI sensor as well as to establish
/the measurement that it’s reading. Because each sensor
/varies, this calibration is recommended before applying
/it in your code to avoid malfunctions.
/
/To learn more about the QTI sensor visit:
/http://www.parallax.com/StoreSearchResults/tabid/768/txtSearch/QTI/List/0/SortField/4/ProductID/100/Default.aspx
/***************************************************/

int val;

void setup() {
  Serial.begin(9600); //start a serial connection
}

void loop() {
  val = QTI(A0);  //save the value read by the QTI sensor on analog pin 0
  Serial.println(val);  //send a string or value on the serial connection

  //optional code for displaying three QTI sensors, make sure the previous code
  //is commented out when using the code below.
  /*
  	Serial.print(QTI(A0));
  	Serial.print("\t"); //insert some tabs
  	Serial.print(QTI(A1));
  	Serial.print("\t");
  	Serial.println(QTI(A2));
  */
}

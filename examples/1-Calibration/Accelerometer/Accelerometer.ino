#include <MINDSi.h>
#include <Servo.h>

/***************************************************
/MINDS-i Calibration. mymindsi.com
/
/Accelerometer Calibration
/
/This Accelerometer Calibration code is used to test
/and adjust your accelerometer Because each sensor
/varies, this calibration is necessary before applying
/it in your code to avoid malfunctions.
/
/To learn more about our standard accelerometer visit:
/http://www.freescale.com/files/sensors/doc/data_sheet/MMA7260QT.pdf
/***************************************************/

float X,Y,Z;

void setup() {
  Serial.begin(9600);  //start a serial connection
}

void loop() {
  X = analogRead(A0); //read in the value on analog pin 0
  Y = analogRead(A1);
  Z = analogRead(A2);

  Serial.print("X:"); //send title and then value over serial
  Serial.print(X);
  Serial.print("\tY:"); // \t inserts a "tab" to keep everything easy to read
  Serial.print(Y);
  Serial.print("\tZ:");
  Serial.print(Z);

  Serial.println(); //shift down a line
}

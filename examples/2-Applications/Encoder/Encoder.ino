#include <Servo.h>
#include <MINDSi.h>
#include "Encoder.h"

/***************************************************
/MINDS-i Encoder Application. mymindsi.com
/
/This Encoder Application code is a basic example
/of how to incorporate Cruise Control in the Arduino
/code.  In this code your rover will drive forward
/at a certain speed (set in RPM) and adapt motor power
/to maintain that speed. Stall protection is also implimented
/in the case of a stall move the wheels forward to
/re-engage the power
/***************************************************/

//declare a Cruise object; just like servos
Cruise cruise;
float val;
float mph;
int dist;
float feet;

//these conversions vary based on measurments of the robot
//(wheel Diameter*PI) * minutes per hour / (inches per mile * gear ratio)
float RPMtoMPH = ((5. *PI)*60.)/(63360.*(37./13.));

//(wheel Diameter*PI) / (inches to foot * steps per rotation * gear ratio)
float STPtoFeet = (5. *PI)/(12.*100.*(37./13.));

void setup() {
  cruise.attach(4);
  cruise.write(90); //send neutral; here cruise acts exactly like a Servo
  delay(2000);
  cruise.setControl(100); //set RPM control to 100 RPM
  cruise.bounds(45, 135); //set upper and lower bound the outputs will approach
  Serial.begin(9600);

  Serial.print("STP to Feet: ");
  Serial.print(STPtoFeet, DEC);
  Serial.print(" RPM to MPH:");
  Serial.println(RPMtoMPH,DEC);

}

void loop() {
  cruise.update(); // update the output value; this must be called often

  val = getRPM();  //read the average rpm of the last 50th of a second
  mph = val * RPMtoMPH;
  dist = getSteps();  //read the total steps the encoder has moved
  feet = dist*STPtoFeet;


  Serial.print("RPM: ");  //send a string or value on the serial connection
  Serial.print(val);
  Serial.print("\tMPH: ");
  Serial.print(mph);

  Serial.print("\tSteps: ");
  Serial.print(dist);
  Serial.print("\tFeet: ");
  Serial.println(feet);
 }

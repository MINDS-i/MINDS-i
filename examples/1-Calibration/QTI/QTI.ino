#include <MINDSi.h>
#include <Servo.h>

/***************************************************
/ Example provided by MINDS-i
/ Try checking out our arduino resource guide at
/ http://mindsieducation.com/programming-resources
/ Questions? Concerns? Bugs? email code@mymindsi.com
/
/ This code expects a QTI sensor plugged into pin A0
/ Or, Swap out the comments below to read three sensors
/ on pins A0, A1, and A2
/***************************************************/

int val;

void setup() {
  //start a serial connection
  Serial.begin(9600);
}

void loop() {
  //save the value read by the QTI sensor on analog pin 0; then send
  val = QTI(A0);
  Serial.println(val);

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

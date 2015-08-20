#include <MINDSi.h>
#include <Servo.h>

/***************************************************
/ Example provided by MINDS-i
/ Try checking out our arduino resource guide at
/ http://mindsieducation.com/programming-resources
/ Questions? Concerns? Bugs? email code@mymindsi.com
/
/ This example expects 2 servos in pins 4 and 5
/ and a ping sensor in pin 9
/***************************************************/

Servo left, right;

const int center     =  90;
const int hazardDist = 750;

void setup() {
  left.attach(5);
  right.attach(4);

  //driving straight
  left.write(0);
  right.write(180);
}

void loop() {
  if (getPing(9) < hazardDist) {
    //liberal coasting to prevent brownouts
    left.write(center);
    right.write(center);
    delay(500);

    left.write(180);
    right.write(center);
    delay(1000);

    left.write(center);
    right.write(center);
    delay(250);
  } else {
    //resume driving straight
    left.write(0);
    right.write(180);
  }

}

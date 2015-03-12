#include <MINDSi.h>
#include <Servo.h>

/***************************************************
/ Example provided by MINDS-i
/ Try checking out our arduino resource guide at
/ http://mindsieducation.com/programming-resources
/ Questions? Concerns? Bugs? email code@mymindsi.com
/
/ This example expects 2 servos in pins 4 and 5
/ and a ping sensor in pin 10
/***************************************************/

Servo leftServo, rightServo;

const int center = 93;
const int hazardDist = 750;

void setup() {
  leftServo.attach(5);
  rightServo.attach(4);

  leftServo.write(0);
  rightServo.write(180);
}

void loop() {
  if (getPing(10) < hazardDist) {
    //liberal coasting to prevent brownouts
    leftServo.write(center);
    rightServo.write(center);
    delay(500);

    leftServo.write(180);
    rightServo.write(center);
    delay(1000);

    leftServo.write(center);
    rightServo.write(center);
    delay(250);
  } else {
    //resume driving straight
    leftServo.write(0);
    rightServo.write(180);
  }

}

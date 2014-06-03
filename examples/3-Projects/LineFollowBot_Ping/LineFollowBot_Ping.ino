#include <MINDSi.h>
#include <Servo.h>

/***************************************************
/ mymindsi.com
/
/ Line Following Robot - Ping stop
/
/ This code will Stop and turn whenever something blocks its path
/***************************************************/

Servo leftServo,rightServo;

const int center = 93;
const int hazardDist = 750;

void setup(){
  leftServo.attach(5);
  rightServo.attach(4);

  leftServo.write(0);
  rightServo.write(180);
}

void loop(){
  if (getPing(10) < hazardDist) {

    leftServo.write(center); //liberal coasting to prevent brownouts
    rightServo.write(center);
    delay(500);

    leftServo.write(180);
    rightServo.write(center);
    delay(1000);

    leftServo.write(center);
    rightServo.write(center);
    delay(250);
  } else { //resume driving straight
    leftServo.write(0);
    rightServo.write(180);
  }

}

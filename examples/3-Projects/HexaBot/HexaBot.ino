#include <Servo.h>
#include <MINDSi.h>
/***************************************************
/MINDS-i Calibration. mymindsi.com
/
/Hex walker robot example
/
/this code will walk until one of its sensors hits something
/Then it will back up, turn, and march forward again.
/***************************************************/
Servo leftServo, rightServo;


void setup() {
  leftServo.attach(4);
  rightServo.attach(5);
  pinMode(12, INPUT);
  pinMode(11, INPUT);
}

void loop() {
  leftServo.write(0);
  rightServo.write(180);

  if (!digitalRead(12)) {
    leftServo.write(90);
    rightServo.write(90);
    delay(250);
    leftServo.write(180);
    rightServo.write(0);
    delay(2000);
    leftServo.write(0);
    rightServo.write(0);
    delay(2000);
  }
  if (!digitalRead(11)) {
    leftServo.write(90);
    rightServo.write(90);
    delay(250);
    leftServo.write(180);
    rightServo.write(0);
    delay(2000);
    leftServo.write(180);
    rightServo.write(180);
    delay(2000);
  }
}



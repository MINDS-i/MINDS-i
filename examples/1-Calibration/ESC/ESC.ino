#include <MINDSi.h>
#include <Servo.h>

/***************************************************
/MINDS-i ESC Calibration. mymindsi.com
/
/This ESC (Electronic Speed Controller)
/Calibration code is used to ensure your ESC is working
/properly. your ESC should use the motor to make three beeps
/when connected correctly once the program is run.
/
/To learn more about esc visit:
/https://www.google.com/search?source=ig&hl=en&rlz=&q=electronic+speed+controller+arduino&oq=Elect&aq=0p&aqi=p-p1g9&aql=&gs_sm=1&gs_upl=3573l5250l0l6903l5l5l0l0l0l0l223l808l1.1.3l5l0
/***************************************************/

Servo motor;

void setup() {
  motor.attach(4); //set a pin for a servo/ESC to use
  motor.write(90); //set the output for a servo/ESC
  delay(2000); //wait for 2 seconds
}

void loop() {
}

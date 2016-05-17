#include <MINDSi.h>
#include <Servo.h>

Servo steer, drive;
float driveOut, steerOut;
float driveIn, steerIn;

void setup() {
  drive.attach(4);
  steer.attach(5);

  drive.write(90);
  steer.write(90);

  // start interrupts on pin 3 so pulses will be captured before isRadioOn
  // is first called
  getRadio(3);

  delay(2000);
}

void loop() {
  if (isRadioOn(3)) {
    radioControl();
  } else {
    radioOff();
  }
}

void radioControl() {
}

void radioOff() {
}

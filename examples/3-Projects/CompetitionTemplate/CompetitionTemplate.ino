#include <MINDSi.h>
#include <Servo.h>

Servo steer, drive;
float driveOut, steerOut;
float driveIn, steerIn;

void setup(){
	drive.attach(4);
	steer.attach(5);

	drive.write(90);
	steer.write(90);

	delay(2000);
}

void loop(){
	if (isRadioOn(2)) {
		radioControl();
	} else {
		radioOff();
	}
}

void radioControl() {
}

void radioOff(){
}

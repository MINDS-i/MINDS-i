#include <Servo.h>
#include <MINDSi.h>

/***************************************************
/ MINDS-I Projects. mymindsi.com
/
/This example will drive until it sees a wall in front of it
/and sends a signal to stop. then it will back up in the direction
/with more space until it sees room to move forward.
/It also steers away from walls when approaching at a shallow angle.
/Attach the drive speed controller to pin 4
/Attach the front steering servo to pin 5
/Attach the back steering servo to pin 6 (optional)
/Attach the Pings as follow:
left:10 center:9 right:8
/***************************************************/

Servo drive, steer, backsteer;
unsigned long startTime;
int right, front, left;
int steervalue;

const int CENTER = 90;
const int THROW = 45;
const int FWDSPEED = 108;
const int REVSPEED = 75;
				//left,center,right
const int HAZARD_DIST[] = {650, 3000, 650}; //Raise these on 6x6's

void setup() {
	pinMode(13, INPUT);
	pinMode(12, INPUT);

	drive.attach(4);
	steer.attach(5);
	backsteer.attach(6);

	setSteer(CENTER);
	drive.write(90);
	delay(2000);
 }

void loop(){
	right = getPing(9);
	delay(10);
	left  = getPing(11);
	delay(10);
	front = getPing(10);

	if (	(left < HAZARD_DIST[0] ||
			front < HAZARD_DIST[1] ||
			right < HAZARD_DIST[2]) && drive.read()==FWDSPEED) {
		//wait for the vehicle to stop
		drive.write(90);
		delay(750);

		//turn in the most favorable direction
		if(left > right) setSteer(CENTER+THROW);
		else setSteer(CENTER-THROW);
		delay(50);

		//back up until enough room is found
		drive.write(REVSPEED);
		startTime = millis();
		while ((millis()-startTime)<1500 || front<5500) {
			front = getPing(10);
			delay(10);
		}
		drive.write(90);

		//center the wheels and resume driving
		setSteer(CENTER);
		delay(1000);
		drive.write(FWDSPEED);
	}
	else{
		//turn away from near walls
		bound(0, left, 4000);
		bound(0, right,4000);
		steervalue = map(left-right, -4000, 4000, CENTER+THROW, CENTER-THROW);
		setSteer(steervalue);
		drive.write(FWDSPEED);
	}
}

//methods are used to save space on simple, but often repeated lines of code
void inline setSteer(int out) {
	steer.write(out);
	backsteer.write(180-out);
}

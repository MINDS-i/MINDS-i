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
/leftmost (west):11 left:10 center:9 right:8 rightmost (east):7
/
/In addition, if a radio is plugged in and turned on this example
/ will drive manually and return to automatic when it is turned on.
/***************************************************/

Servo drive, frontsteer, backsteer;
unsigned long startTime;
int east, right, front, left, west;
int steervalue;

const int CENTER = 90;
const int TURN = 45;
const int FWDSPEED = 115;
const int REVSPEED = 70;

                        //west,left,center,right,east
const int HAZARD_DIST[] = {650, 800, 3000, 800, 650}; //Raise these on 6x6's

void setup() {
  pinMode(13, INPUT);
  pinMode(12, INPUT);

  drive.attach(4);
  frontsteer.attach(5);
  backsteer.attach(6);

  steer(CENTER);
  drive.write(90);
  delay(2000);
 }

void loop(){
  //this will give manual control if a radio is plugged in
  if (micros() - timeLastSignal() < 100000) {
    drive.write(getRadio(2));
    steer(getRadio(3));
  }
  else autodrive();
}

void autodrive(){
  east = getPing(7);
  west = getPing(11);
  delay(10);
  right = getPing(8);
  left = getPing(10);
  delay(10);
  front = getPing(9);
  delay(10);

  if (  west  < HAZARD_DIST[0] ||
        left  < HAZARD_DIST[1] ||
        front < HAZARD_DIST[2] ||
        right < HAZARD_DIST[3] ||
        east  < HAZARD_DIST[4] ) {
    //wait for the vehicle to stop
    drive.write(90);
    delay(750);

    //turn in the most favorable direction
    if(left > right) steer(CENTER+TURN);
    else steer(CENTER-TURN);
    delay(50);

    //back up until enough room is found
    drive.write(REVSPEED);
    startTime = millis();
    while ((millis()-startTime)<1500 || front<5500) {
      front = getPing(9);
      delay(10);
    }
    drive.write(90);

    //center the wheels and resume driving
    steer(CENTER);
    delay(1000);
    drive.write(FWDSPEED);
  }
  else{
    //turn away from near walls
    int lAve = (left+west/2);
    int rAve= (right+east/2);
    bound(0, lAve, 4000);
    bound(0, rAve,4000);
    steervalue = map(lAve-rAve, -4000, 4000, CENTER+TURN, CENTER-TURN);
    steer(steervalue);
    drive.write(FWDSPEED);
  }
}

//methods are used to save space on simple, but often repeated lines of code
void inline steer(int out) {
  frontsteer.write(out);
  backsteer.write(180-out);
}

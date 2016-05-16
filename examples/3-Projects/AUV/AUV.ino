#include <Servo.h>
#include <MINDSi.h>

/***************************************************
/ Example provided by MINDS-i
/ Try checking out our arduino resource guide at
/ http://mindsieducation.com/programming-resources
/ Questions? Concerns? Bugs? email code@mymindsi.com
/
/ This example expects an ESC plugged into pin 4
/ A servo plugged into pin 5
/ and ping sensors in pins 9, 10, and 11
/ optionally, IR sensors in pins 12 and 13
/***************************************************/

// Enable the IR sensors to detect obstacles when backing up
const bool IR_ENABLED    = false;
// The value that, when turned to, makes the robot drive straight
const int CENTER        = 90;
// The maximum distance to turn away from CENTER
const int TURN          = 45;
// The speed to drive when going forward
const int FWDSPEED      = 110;
// The speed to drive when going in reverse
const int REVSPEED      = 75;
// The minimum time to back away from and obstacle
const int BACKUP_TIME   = 2500;
// The amount of "force" obstacles push the robot with in avoid mode
const float AVOID_FORCE   = 8.0;
// The maximum distances that each ping sensor considers a hazard
const int  HAZARD_DIST[] = {   800,  3000,  800};
                          //  left,center,right

// The pins that each radio function are plugged in to
const int DRIVE_RADIO_PIN  = 2;
const int STEER_RADIO_PIN  = 3;
const int SWITCH_RADIO_PIN = 7;

// The list of States autonomous can be in
enum State {AVOID, BACKUP};

// Variables to use during the course of the program
Servo drive, frontsteer, backsteer;
State autoMode = AVOID;
uint32_t BackupEndTime;

void setup() {
  pinMode(13, INPUT);
  pinMode(12, INPUT);

  drive.attach(4);
  frontsteer.attach(5);
  backsteer.attach(6);

  dualSteer(CENTER);
  drive.write(90);
  delay(2000);
}

void loop(){
  if(getRadio(SWITCH_RADIO_PIN) < 90){
    autonomous();
  } else {
    drive.write(getRadio(DRIVE_RADIO_PIN));
    dualSteer(getRadio(STEER_RADIO_PIN));
    autoMode = AVOID;
  }
}

void autonomous() {
  int right = getPing(9);
  delay(10);
  int left  = getPing(11);
  delay(10);
  int front = getPing(10);

  bool hazard = (left  < HAZARD_DIST[0] ||
                 front < HAZARD_DIST[1] ||
                 right < HAZARD_DIST[2]   );

  if (autoMode == AVOID){
    // steer to avoid obstacles detected by the left and right sensors
    steerToAvoid(left, front, right);
    drive.write(FWDSPEED);

    // if a hazard is present, switch to BACKUP mode
    if(hazard){
      coast();
      primeReverse();
      BackupEndTime = millis() + BACKUP_TIME;

      // this will steer in the direction with more space while in BACKUP
      if (left > right) {
        dualSteer(CENTER + TURN);
      } else {
        dualSteer(CENTER - TURN);
      }
      autoMode = BACKUP;
    }

  } else if (autoMode == BACKUP) {
    // steer in the direction with more space and back up
    drive.write(REVSPEED);

    // if time is up or the IR sensors see an obstacle, go back to AVOID mode
    if((millis() > BackupEndTime) ||
       (IR_ENABLED && (!digitalRead(13) || !digitalRead(12))) ) {
      coast();
      autoMode = AVOID;
    }

  }
}

// Let the robot coast to a stop
void coast(){
  drive.write(90);
  dualSteer(CENTER);
  delay(2500);
}
// Prime the motor control to go in reverse
void primeReverse(){
  drive.write(85);
  delay(25);
  drive.write(90);
  delay(25);
}
// Steer the robot to avoid walls measured to `left` and `right` unit away
/*void steerToAvoid(float left, float front, float right){
  float turnForce = 1e6*(AVOID_FORCE/(left*left) - AVOID_FORCE/(right*right));
  turnForce = constrain(turnForce, -1.0, 1.0);
  dualSteer(CENTER + (int)(turnForce * (float)TURN) );
}*/

float curve(float input){
  return copysign(pow(fabs(input), 1.5f), input);
}

const float WALLDIST = 3000;

void steerToAvoid(float left, float front, float right){
  float turnForce = 0;
  turnForce += curve(WALLDIST-left)/1e6;
  //turnForce += 1e6/curve(front-3000);
  turnForce = constrain(AVOID_FORCE*turnForce, -1.0, 1.0);
  dualSteer(CENTER + (int)(turnForce * (float)TURN) );
}

// Send a signal to the front and back servo's simultaneously
void dualSteer(int out) {
  frontsteer.write(out);
  backsteer.write(180 - out);
}
/*
  int right = getPing(9);
  delay(10);
  int left  = getPing(11);
  delay(10);
  int front = getPing(10);

  if (	left  < HAZARD_DIST[0] ||
        front < HAZARD_DIST[1] ||
        right < HAZARD_DIST[2]   ) {
    //coast down for two seconds
    drive.write(90);
    delay(2000);

    //turn in the most favorable direction
    if (left > right) {
      dualSteer(CENTER + TURN);
    } else {
      dualSteer(CENTER - TURN);
    }

    //prime reverse
    drive.write(85);
    delay(50);
    drive.write(90);
    delay(50);

    //back up for BACKUP_TIME milliseconds
    drive.write(REVSPEED);
    uint32_t endTime = millis() + BACKUP_TIME;
    while (millis() < endTime) {
      //leave the loop if backup sensors see a wall
      if ( IR_ENABLED && (!digitalRead(13) || !digitalRead(12)) ) break;
    }

    //coast to a stop
    drive.write(90);
    dualSteer(CENTER);
    delay(1000);
  }
  else {
    left  = constrain(left , 0, SIDE_RANGE);
    right = constrain(right, 0, SIDE_RANGE);

    //map the difference in lAve and rAve from +/- SIDE_RANGE to +/- TURN
    long steerValue = map( (left - right),
                           -SIDE_RANGE, SIDE_RANGE,
                           -TURN,       TURN);

    dualSteer(CENTER - steerValue);
    drive.write(FWDSPEED);
  }
}
*/

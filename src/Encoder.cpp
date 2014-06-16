#include <Servo.h>
#ifdef Servo_h
#include "Encoder.h"
#include "MINDSi.h"

unsigned long _pTime;
const int timestep = 20;
long _steps, _p_steps;
float _rpm;
float _hrpm[10];
float* _tr = &_hrpm[0];

Cruise::Cruise() : time(millis()), out(90), gain(160), loss(100), set(0),
		control(false), stall(false), lower(90), upper(180), dT(0) {
}

void Cruise::attach(int pin){
	servo.attach(pin);
	startEncoder();
}

void Cruise::tune(int g, int l){
	gain = g;
	loss = l;
}

void Cruise::bounds(int lowerBound, int upperBound){
	lower = lowerBound; upper = upperBound;
}

void Cruise::setControl(int setPoint){
	if(servo.attached()){
		set = setPoint;
		control = true;
		time = millis();
	}
}

void Cruise::update(){
	if(control){

		dT = (double(millis()-time)/40);
		if(dT > 25) dT = 25;
		time = millis();

		int val = getRPM();
		if(val > 100 && stall) {
			stall = false;
			out = 90;
		}

		if(val < set) out +=  dT*(fabs(set - val)/gain);
  		else if(val > set) out /= (1 + (dT*(fabs(val - set)/(loss*100))) );
  		if(out >= upper) {
  			out = upper;
  			if(val < 10) stall = true;
  		}
  		else if(out < lower) {
  			out = lower;
  		}

  		if(!stall) servo.write((int)out);
  		else servo.write(90);

	}
}

bool Cruise::isStalled(){
	return stall;
}

void Cruise::detach(){
	servo.detach();
	control = false;
}

void Cruise::stopControl(){
	control = false;
}

void Cruise::write(int output){
	if(servo.attached()){
		servo.write(output);
		control = false;
	}
}

void startEncoder(){
	pinMode(2, INPUT);
	pinMode(3, INPUT);
	digitalWrite(2, HIGH);
	digitalWrite(3, HIGH);
	attachInterrupt(0, Aline, RISING);
	attachInterrupt(1, Bline, RISING);
	_rpm = 0;
	_steps = 0;
	interrupts();
}

long getSteps(){
	return _steps;
}

float getRPM(){
	if(millis() - _pTime >= timestep) { calcrpm(); _pTime = millis(); }
	return _rpm;
}

void Aline(){
	if(digitalRead(3)== LOW) _steps--;
	if(millis() - _pTime >= timestep) { calcrpm(); _pTime = millis(); }
}

void Bline(){
	if(digitalRead(2)== LOW) _steps++;
}

void inline calcrpm(){
	*_tr++ = _steps-_p_steps;
	if(_tr == &_hrpm[9]) _tr = &_hrpm[0];

	_rpm = (_hrpm[0]+_hrpm[1]+_hrpm[2]+_hrpm[3]+_hrpm[4]
				+_hrpm[5]+_hrpm[6]+_hrpm[7]+_hrpm[8]+_hrpm[9]);
	_rpm/= 100; //ticks per revolution
	_rpm*= (100/timestep);
	_rpm /= 1000;//millis()-_pTime;
	_rpm *= 60000;

	_p_steps = _steps;
}
#endif

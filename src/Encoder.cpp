#include "Encoder.h"
#include "MINDSi.h"

unsigned long _pTime;
const int _TIMESTEP = 20;
long _steps, _p_steps;
float _rpm;
float _hrpm[10];
float* _tr = &_hrpm[0];


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
	if(millis() - _pTime >= _TIMESTEP) { calcrpm(); _pTime = millis(); }
	return _rpm;
}

void Aline(){
	if(digitalRead(3)== LOW) _steps--;
	if(millis() - _pTime >= _TIMESTEP) { calcrpm(); _pTime = millis(); }
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
	_rpm*= (100/_TIMESTEP);
	_rpm /= 1000;//millis()-_pTime;
	_rpm *= 60000;

	_p_steps = _steps;
}

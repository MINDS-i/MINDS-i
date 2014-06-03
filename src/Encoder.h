#include <Servo.h>

#ifndef Servo_h
	#error "Encoder Lib requires Servo.h to be included first to function"
#else

#ifndef ENCODER_H
#define ENCODER_H

void startEncoder();
long getSteps();
float getRPM();
void Aline();
void Bline();
void calcrpm();
class Cruise{
public:
	Cruise();
	void attach(int pin);
	void tune(int g, int l);
	void bounds(int lowerBound, int upperBound);
	void setControl(int setPoint);
	void update();
	void detach();
	void stopControl();
	void write(int output);
	bool isStalled();
private:
	Servo servo;
	unsigned long time;
	double dT;
	float out,gain,loss;
	bool control, stall;
	int set;
	int lower, upper;
};
#endif

#endif

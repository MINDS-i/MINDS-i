#ifndef MINDSi_h
#define MINDSi_h

#include "Arduino.h"

/*
MINDSi.h ----- Library for use with mindsi products
Created by MINDS-i, INC. May 9th 2013
Released into the public domain */

int getRadioPulse(int pin, bool interrupt);
int getRadio(int pin, int min = 45, int max = 135, bool interrupt = true);
bool isRadioOn(int pin);
unsigned long timeLastSignal();
int getPing(int pin);
int QTI(int pin);

template <typename Type>
void inline bound(Type min, Type &var,Type max){
	if(var<min) var=min;
	else if(var>max) var=max;
}

//functions below should not be accessed by the user
void Achan();
void Bchan();
void setInput(int pin);
inline bool fastDigitalRead(int pin);
#endif

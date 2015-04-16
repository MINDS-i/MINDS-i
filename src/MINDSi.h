#ifndef MINDSi_h
#define MINDSi_h

#include "Arduino.h"
#include "wiring_private.h"

/*
MINDSi.h ----- Library for use with mindsi products
Created by MINDS-i, INC.
Released into the public domain */

int getPing(int pin, uint32_t maxMicros = 20000);
int QTI(int pin, uint32_t maxLoops = 10000);

bool isRadioOn(int pin, uint32_t timeoutMicros = 5000);
int getRadioPulse(int pin, bool interrupt);
int getRadio(int pin, int min = 45, int max = 135, bool interrupt = true);

bool fastDigitalRead(int pin);

#endif

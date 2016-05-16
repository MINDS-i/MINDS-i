/* Copyright 2015 MINDS-i, INC.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "MINDSi.h"

using namespace MINDSi;

namespace { //make sure these vars/functions aren't visible outside
	volatile uint32_t	pStart[EXTERNAL_NUM_INTERRUPTS];
	volatile uint32_t	pTime [EXTERNAL_NUM_INTERRUPTS];
	volatile int8_t		interruptPin[EXTERNAL_NUM_INTERRUPTS];
	boolean				interruptOn[EXTERNAL_NUM_INTERRUPTS];

	//normal digitalRead is too slow for Interrupt handlers
	inline bool fastDigitalRead(int pin){
		return *portInputRegister(digitalPinToPort(pin))
				& digitalPinToBitMask(pin);
	}

	boolean inline isIntOn(int interrupt){
		//the registers differ too much on each chip to read directly
		return interruptOn[interrupt];
	}

	template<int num>
	void inline iFunc(void){
		if(fastDigitalRead(interruptPin[num])) pStart[num] = micros();
		else pTime[num] = micros()-pStart[num];
	}

	//number of interrupts can only by 2,3,5 or 8 on current models;
	voidFuncPtr func[EXTERNAL_NUM_INTERRUPTS]={
		 iFunc<0>
		,iFunc<1>
	#if EXTERNAL_NUM_INTERRUPTS > 2
		,iFunc<2>
	#if EXTERNAL_NUM_INTERRUPTS > 3
		,iFunc<3>
	#if EXTERNAL_NUM_INTERRUPTS > 4
		,iFunc<4>
	#if EXTERNAL_NUM_INTERRUPTS > 5
		,iFunc<5>
		,iFunc<6>
		,iFunc<7>
	#endif
	#endif
	#endif
	#endif
	};
}

uint16_t getRadioPulse(int pin, bool interrupt){
	uint16_t pulse = 0;
	int iNum  = digitalPinToInterrupt(pin);

	//get signal width
	if(interrupt && iNum!=NOT_AN_INTERRUPT){
		if (!isIntOn(iNum)) {
			attachInterrupt(iNum, func[iNum], CHANGE);
			interruptPin[iNum] = pin; //there is no interruptToDigitalPin Macro
			interruptOn[iNum]  = true;
			interrupts();
		} else {
			uint32_t refresh_time = micros()-pStart[iNum];
			pulse = (refresh_time >= RADIO_PULSE_TIMEOUT)? 0 : pTime[iNum];
		}
	} else {
		pulse = pulseIn(pin, HIGH, RADIO_PULSE_TIMEOUT);
	}

	if(pulse == 0) return NO_PULSE;
	if(pulse > RADIO_PULSE_TIMEOUT) return NO_PULSE;
	return pulse;
}

uint16_t getRadio(int pin, bool interrupt){
	uint16_t pulse = getRadioPulse(pin, interrupt);
	if (pulse == NO_PULSE) {
		return DEFAULT_RADIO_SIGNAL;
	}
	const int sig = map(pulse, RADIO_MIN_US, RADIO_MAX_US, 0, 180);
	return constrain(sig, 0, 180);
}

bool isRadioOn(int pin, bool interrupt){
	return (getRadioPulse(pin, interrupt) != NO_PULSE);
}

uint16_t getPing(int pin, uint16_t maxMicros){
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
	delayMicroseconds(5);
	digitalWrite(pin, HIGH);
	delayMicroseconds(5);
	digitalWrite(pin, LOW);
	delayMicroseconds(5);
	pinMode(pin, INPUT);
	int inputpulse = pulseIn(pin, HIGH, maxMicros);
	if(inputpulse == 0) inputpulse = maxMicros;
	delayMicroseconds(200);
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
	return inputpulse;
}

uint16_t QTI(int pin, uint16_t maxLoops){
	uint16_t time = 0;
	pinMode(pin, OUTPUT);
	digitalWrite(pin, HIGH);
	delay(1);
	pinMode(pin, INPUT);
	digitalWrite(pin, LOW);
	while (digitalRead(pin)) {
		time++;
		if(time >= maxLoops) break;
	}
	return time;
}

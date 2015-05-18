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

namespace { //make sure these vars/functions aren't visible outside
	const uint32_t radio_timeout =32000;
	const uint32_t radio_min_int = 1000;
	const uint32_t radio_max_int = 2000;

	volatile uint32_t	pStart[EXTERNAL_NUM_INTERRUPTS];
	volatile uint32_t	pTime [EXTERNAL_NUM_INTERRUPTS];
	volatile int8_t		interruptPin[EXTERNAL_NUM_INTERRUPTS];
	boolean				interruptOn[EXTERNAL_NUM_INTERRUPTS];

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

//normal digitalRead is too slow for Interrupt handlers
inline bool fastDigitalRead(int pin){
	return *portInputRegister(digitalPinToPort(pin))
			& digitalPinToBitMask(pin);
}

int getRadioPulse(int pin, bool interrupt){
	int pulse = 0;
	int iNum  = digitalPinToInterrupt(pin);

	//get signal width
	if(interrupt && iNum!=NOT_AN_INTERRUPT){
		if( !isIntOn(iNum) ) {
			attachInterrupt(iNum, func[iNum], CHANGE);
			interruptPin[iNum] = pin; //there is no interruptToDigitalPin Macro
			interruptOn[iNum]  = true;
			interrupts();
		} else {
			pulse = pTime[iNum];
		}
	} else {
		pulse = pulseIn(pin, HIGH, radio_timeout);
	}

	if(pulse == 0) pulse = (radio_min_int+radio_max_int)/2;
	return pulse;
}

int getRadio(int pin, int min, int max, bool interrupt){
	int sig = map(getRadioPulse(pin,interrupt), radio_min_int, radio_max_int,
												min, 		   max);
	return constrain(sig, 0, 180);
}

bool isRadioOn(int pin, uint32_t timeoutMicros){
	int iNum = digitalPinToInterrupt(pin);
	if( iNum!=NOT_AN_INTERRUPT && isIntOn(iNum) ){
		return (micros() < pStart[iNum] + timeoutMicros);
	} else {
		uint16_t pulse = pulseIn(pin, HIGH, timeoutMicros);
		return (pulse > radio_min_int && pulse < radio_max_int);
	}
}

int getPing(int pin, uint32_t maxMicros){
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

int QTI(int pin, uint32_t maxLoops){
	unsigned int time = 0;
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

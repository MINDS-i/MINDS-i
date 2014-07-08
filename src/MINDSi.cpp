#include "MINDSi.h"
#include "wiring_private.h"

const uint16_t RADIO_CHECK_INTERVAL = 0x7550;  //loops - no interrupt timeout
const uint32_t RADIO_TIMEOUT 		= 1000000; //microseconds interrupt timeout
const uint16_t PING_MAX			    = 20000;   //microseconds until no response

volatile uint32_t pStart[EXTERNAL_NUM_INTERRUPTS];
volatile uint32_t pTime [EXTERNAL_NUM_INTERRUPTS];
int8_t interruptPin[EXTERNAL_NUM_INTERRUPTS];
boolean interruptOn[EXTERNAL_NUM_INTERRUPTS];

template<int num>
void inline iFunc(void){
	if(fastDigitalRead(interruptPin[num])) pStart[num] = micros();
	else pTime[num] = micros()-pStart[num];
}

//number of interrupts can only by 2,3,5 or 8 on current models
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

boolean inline isIntOn(int interrupt){
	//the registers differ too much on each chip to read directly
	return interruptOn[interrupt];
}

int getRadioPulse(int pin, bool interrupt){
	int pulse;
	int iNum = digitalPinToInterrupt(pin);

	//get signal width
	if(interrupt && iNum!=NOT_AN_INTERRUPT){
		if( !isIntOn(iNum) ) {
			attachInterrupt(iNum, func[iNum], CHANGE);
			interruptPin[iNum]=pin; //there is no interruptToDigitalPin Macro
			interruptOn[iNum]=true;
			interrupts();
		} else {
			pulse = pTime[iNum];
		}
	} else {
		pulse = pulseIn(pin, HIGH);
	}

	if(pulse == 0) pulse = 1500; //1500 is exactly centered ppm signal
	return pulse;
}

int getRadio(int pin, int min, int max, bool interrupt){
	int sig = map(getRadioPulse(pin,interrupt), 1020, 1980, min, max);
	bound(0, sig, 180);
	return sig;
}

unsigned long timeLastSignal(){
	unsigned long latestSignal = 0;
	for(int i=0; i<EXTERNAL_NUM_INTERRUPTS; i++){
		max(pStart[i], latestSignal);
	}
	return latestSignal;
}

bool isRadioOn(int pin){
	int iNum = digitalPinToInterrupt(pin);
	if( iNum!=NOT_AN_INTERRUPT && isIntOn(iNum) ){
		return (micros() < pStart[iNum] + RADIO_TIMEOUT);
	} else {
		uint16_t numloops;
		uint8_t lastState = fastDigitalRead(pin);
		while (fastDigitalRead(pin) == lastState) {
			if (numloops++ >= RADIO_CHECK_INTERVAL) return false;
		}
		return true;
	}
}

int getPing(int pin){
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
	delayMicroseconds(5);
	digitalWrite(pin, HIGH);
	delayMicroseconds(5);
	digitalWrite(pin, LOW);
	delayMicroseconds(5);
	setInput(pin);
	int inputpulse = pulseIn(pin, HIGH, PING_MAX);
	if(inputpulse == 0) inputpulse = PING_MAX;
	delayMicroseconds(200);
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
	return inputpulse;
}

int QTI(int pin){
	unsigned int time = 0;
	pinMode(pin, OUTPUT);
	digitalWrite(pin, HIGH);
	delay(1);
	setInput(pin);
	digitalWrite(pin, LOW);
	//fastDigitalRead is not used to keep values consistent with older versions
	while (digitalRead(pin)) {
		time++;
		if(time >= 10000) break;
	}
	return time;
}

//setting pinMode takes time; this will check if it has already been set first
void inline setInput(int pin){
	if((*portModeRegister(digitalPinToPort(pin)) & digitalPinToBitMask(pin))!=0)
		pinMode(pin, INPUT);
}

//normal digitalRead is also too slow for some applications
inline bool fastDigitalRead(int pin){
	return *portInputRegister(digitalPinToPort(pin)) & digitalPinToBitMask(pin);
}

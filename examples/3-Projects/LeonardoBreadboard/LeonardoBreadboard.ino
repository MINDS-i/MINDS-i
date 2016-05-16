#include <Wire.h>
#include <SPI.h>
#include <Servo.h>
#include <MINDSi.h>
#include <LiquidCrystal_I2C.h>
#include "logo.h"

/**
 * For use with the NewLiquidCrystal library available here:
 * https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads
 */

const int I2C_ADDR = 0x27;
const int BACKLIGHT_PIN = 3;
const int En_pin = 2;
const int Rw_pin = 1;
const int Rs_pin = 0;
const int D4_pin = 4;
const int D5_pin = 5;
const int D6_pin = 6;
const int D7_pin = 7;
LiquidCrystal_I2C lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

const int pingPins[3] = {9,10,11};
const int QTIPins[3]  = {A0,A1,A2};
const int radioPins[2] = {1,0}; //radio Drive, Steer
const int servoPins[2] = {4,5}; //Drive, steer
const int IRPins[2] = {12, 13};
const int buttonPin[2] = {6,8};
const String mindsi =   "MINDS-i  ";
const String robotics = "Robotics ";
const int msInInch = 148;
const int msInCm = 58;
float RPMtoMPH = ((5. *PI)*60.)/(63360.*(37./13.));
float STPtoFeet = (5. *PI)/(12.*100.*(37./13.));

const int numWindows = 8;
void (*window[numWindows])() = { &pingDetail, &pingExpo, &Auto, &Manual, &QTI, &IR};
enum charSet currentCharSet;
enum buttonSet currentButton;
bool onRise=false, longRise = false;
bool radioFound=false;

Servo drive, steer;

int active=1;

void setup(){
	lcd.begin (20,4);
	lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
	lcd.setBacklight(true);
	lcd.noCursor();
	lcd.backlight();
	lcd.noAutoscroll();

	Logo();

	drive.attach(servoPins[0]); drive.write(90);
	steer.attach(servoPins[1]); steer.write(90);
	getRadio(radioPins[0]);
	getRadio(radioPins[1]);

	delay(2000);
}

void checkButtons(){
	int tmpButton;
	#define INVERT true
	#if INVERT
	if(!digitalRead(buttonPin[0])) tmpButton = 4;
	else if(!digitalRead(buttonPin[1])) tmpButton = 0;
	else tmpButton = 6;
	#else
	if(digitalRead(buttonPin[0])) tmpButton = 4;
	else if(digitalRead(buttonPin[1])) tmpButton = 0;
	else tmpButton = 6;
	#endif
	onRise |= (currentButton != tmpButton);
	switch(tmpButton){
		case 0:
			currentButton = LEFT;
			break;
		case 1:
			currentButton = UP;
			break;
		case 2:
			currentButton = DOWN;
			break;
		case 4:
			currentButton = RIGHT;
			break;
		case 5:
			currentButton = START;
			break;
		default:
			currentButton = FREE;
	}
}

void loop(){
	checkButtons();
	longRise = onRise;
	if(onRise && currentButton == LEFT) {
		active -= 1;
		lcd.clear();
	}
	else if(onRise && currentButton == RIGHT) {
		active +=1;
		lcd.clear();
	}
	onRise = false;
	if(active >= numWindows) active = 0;
	if(active < 0) active = (numWindows-1);

	window[active]();

	lcd.setCursor(8,0);
		lcd.print("Pwr");
		printInPlace(3, ((drive.read()-90)/.9) );
		lcd.print("Str");
		printInPlace(3, steer.read());
}

void Logo(){
	setCustomChars(EYE);
	lcd.home();
	const int frames = 40;
	const int slide = 200;

	for(int pos=-3; pos< 5; pos++){
		for(int i=0; i<4; i++){
			if(pos+i >= 0){
				lcd.setCursor(pos+i,1);
				lcd.send(i,1);
				lcd.setCursor(pos+i,2);
				lcd.send(i+4,1);
			}
		}
		if(pos-1 >= 0){
			lcd.setCursor(pos-1,1);
			lcd.print(" ");
			lcd.setCursor(pos-1,2);
			lcd.print(" ");
		}
		delay(slide);
	}
	for(int pos=20; pos>7; pos--){
		for(int i=0; i<9; i++){
			if(pos+i < 20){
				lcd.setCursor(pos+i, 1);
				lcd.print(mindsi[i]);
				lcd.setCursor(pos+i, 2);
				lcd.print(robotics[i]);
			}
		}
		delay(slide);
	}

	for(int step = 2; step<8; step++){
		setCustomChars((charSet) step);
		delay(frames);
	}
	delay(frames);
	for(int step = 7; step>0; step--){
		setCustomChars((charSet) step);
		delay(frames);
	}

	lcd.clear();
}
/////////// Windows /////////////////////////////
void pingExpo(){
	radioDrive();
	const static int SCALE = 200;
	setCustomChars(VERT);
	lcd.home();
	lcd.setCursor(0,0);
	lcd.print("P");
	lcd.setCursor(0,1);
	lcd.print("I");
	lcd.setCursor(0,2);
	lcd.print("N");
	lcd.setCursor(0,3);
	lcd.print("G");

	int left = getPing(pingPins[0]);
	int front = getPing(pingPins[1]);
	int right = getPing(pingPins[2]);

	verticalBarGraph(1,5,  3,7, left/SCALE);
	verticalBarGraph(1,9,  3,11, front/SCALE);
	verticalBarGraph(1,13, 3,15, right/SCALE);

	delay(10);
}

void pingDetail(){
	radioDrive();
	setCustomChars(HORZ);
	int activePing = active;

	lcd.setCursor(0,0);
		lcd.print("Ping #");
		lcd.print(activePing);

	int tmp = getPing(pingPins[activePing]);
	horizontalBarGraph(1,0, 1,19, (tmp/50));

	lcd.setCursor(0,2);
		printInPlace(5,tmp);
		lcd.print(" ms");
		lcd.setCursor(15, 2);
		printInPlace(2,tmp/msInInch);
		lcd.print(" in");
	lcd.setCursor(1,3);
		printInPlace(3, tmp/msInCm);
		lcd.print(" cm");
		lcd.setCursor(13,3);
		lcd.print("up/down");

	delay(10);	lcd.setCursor(0,2);
/*		lcd.print("Mot: ");
		printInPlace(4, getRPM());
		lcd.print("RPM, ");
		printInPlace(4, getSteps()*STPtoFeet);
		lcd.print("Ft");*/
}

void IR(){
	radioDrive();
	lcd.setCursor(0,0);
		lcd.print("IR");
	lcd.setCursor(0,1);
		lcd.print("Left:  ");
		if(digitalRead(IRPins[0])) lcd.print("On ");
		else lcd.print("Off");
	lcd.setCursor(0,2);
		lcd.print("Right: ");
		if(digitalRead(IRPins[1])) lcd.print("On ");
		else lcd.print("Off");
	lcd.setCursor(0,3);
}

void Auto(){
	setCustomChars(HORZ);
	String msg;
	AutonoDrive(msg);

	lcd.setCursor(0,0);
		lcd.print("Auto");
	lcd.setCursor(0,1);
		lcd.print("Pwr:");
		printInPlace(3, ((drive.read()-90)/.9) );
		lcd.print("% Steer: ");
		printInPlace(3, steer.read());
		lcd.send(6,1);
	lcd.setCursor(0,3);
		lcd.print(msg);
}

void Manual(){
	setCustomChars(HORZ);
	radioDrive();

	lcd.setCursor(0,0);
		lcd.print("Manual");
}

void QTI(){
	lineFollow();
	lcd.setCursor(0,0);
		lcd.print("QTI ");
	lcd.setCursor(0,1);
		lcd.print("Sensor A: ");
		printInPlace(10, (int)QTI(QTIPins[0]));
	lcd.setCursor(0,2);
		lcd.print("Sensor B: ");
		printInPlace(10, (int)QTI(QTIPins[1]));
	lcd.setCursor(0,3);
		lcd.print("Sensor C: ");
		printInPlace(10, (int)QTI(QTIPins[2]));
}
/////////// Driving /////////////////////////////////
void radioDrive(){
		steer.write(getRadio(radioPins[0]));
		drive.write(getRadio(radioPins[1]));
}

void AutonoDrive(String& msg){
	const static int CENTER = 90;
	const static int TURN = 45;
	const static int FWDSPEED = 108;
	const static int REVSPEED = 75;
	static int rightc, right;
	static int leftc, left;
	static int frontc, front;
	static int backing=0;

	if((rightc = getPing(pingPins[0])) != 0) right = rightc;
	if((leftc = getPing(pingPins[2])) != 0) left = leftc;
	if((frontc = getPing(pingPins[1])) != 0) front = frontc;

	if(backing !=0){
		int delta = millis()-backing;
		if(delta < 750) {
			drive.write(0);
			msg = "Stopping            ";
		}
		else if(delta < 1000){
			steer.write( (left > right) ? (CENTER+TURN) : (CENTER-TURN) );
			drive.write(95);
			msg = "Releasing brake     ";
		}
		else{
			if( front<5500 && steer.read()!=CENTER) {
				drive.write(REVSPEED);
				backing = millis()-1000;
				msg = "aquiring clear path ";
			}
			else if(delta < 1800){
				drive.write(90);
				steer.write(CENTER);
				msg = "Stopping            ";
			}
			else {
				backing = 0;
			}
		}
	}
	else if ((front < 700 || left < 550 || right < 550) && drive.read()>90){
		backing = millis();
	}
	else{
		cap(left, 4000); cap(right,4000);
		int steervalue = map(left-right, -4000, 4000, CENTER+TURN, CENTER-TURN);
		steer.write(steervalue);
		drive.write(FWDSPEED);
		msg = "Driving Forward     ";
	}
}

void stop(){
	drive.write(90);
	steer.write(90);
}

void lineFollow(){
	static const int threshold = 400;
	drive.write(90);
	if(QTI(QTIPins[0]) < threshold) steer.write(45);
	else if(QTI(QTIPins[1]) < threshold) steer.write(90);
	else if(QTI(QTIPins[2]) < threshold) steer.write(135);
}
//////////////////////////////////////////////////
void writeToChars(byte in[][8]){
	for(int i=0; i< 8; i++) lcd.createChar(i, in[i]);
}

void setCustomChars(enum charSet set){
	if(currentCharSet != set){
		switch(set){
			case EYE:
				writeToChars(eye);
				break;
			case EYE2:
				writeToChars(eye2);
				break;
			case EYE3:
				writeToChars(eye3);
				break;
			case EYE4:
				writeToChars(eye4);
				break;
			case EYE5:
				writeToChars(eye5);
				break;
			case EYE6:
				writeToChars(eye6);
				break;
			case EYE7:
				writeToChars(eye7);
				break;
			case VERT:
				byte data[8];
				for(int i=0; i<8; i++) data[i] = 0;
				for(int row=0; row<8; row++){
					data[7-row] = 0xFF;
					lcd.createChar(row, data);
				}
				break;
			case HORZ:
				byte map[8];
				for(int i=0; i<8; i++) map[i] = 0;
				for(int clmn=0; clmn<5; clmn++){
					for(int i=0; i<8; i++) map[i] |= (0x10>>clmn);
					lcd.createChar(clmn, map);
				}
				lcd.createChar(6, degree);
				break;
			default:
				break;
		}
		currentCharSet = set;
	}
}

void verticalBarGraph(int top, int left, int bottom, int right, int value){
	for(int line=(bottom); line>=top; line--){
		lcd.setCursor(left,line);
		if(value > 8) {
			for(int i=0; i<=(right-left); i++) lcd.send(7,1);
			value -= 8;
		}
		else if(value != 0) {
			for(int i=0; i<=(right-left); i++) lcd.send((value%8 -1), 1);
			value = 0;
		}
		else {
			for(int i=0; i<=(right-left); i++) lcd.print(' ');
		}
	}
}

void horizontalBarGraph(int top, int left, int bottom, int right, int value){
	for(int colm=left; colm <= right; colm++){
		if(value > 5) {
			for(int i=top; i<=bottom; i++) {
				lcd.setCursor(colm, i);
				lcd.send(4, 1);
			}
			value -= 5;
		}
		else if(value !=0){
			for(int i=top; i<=bottom; i++) {
				lcd.setCursor(colm, i);
				lcd.send((value%5 -1), 1);
			}
			value = 0;
		}
		else {
			for(int i=top; i<=bottom; i++) {
				lcd.setCursor(colm, i);
				lcd.print(' ');
			}
		}

	}
}

void printInPlace(int places, int number){
	bool hit = false;
	long i=1;
	for(int j=1; j<places; j++) i*=10;
	if(number < 0) {
		number = abs(number);
		lcd.print('-');
		i/=10;
	}
	cap(number, (i*10)-1);
	for(; i>0; i/=10){
		if( hit |= ((number/i)%10 != 0) )
			lcd.print( (number/i)%10 );
		else lcd.print(' ');
	}
}

void printInPlace(int places, float number){
	float i=1;
	for(int j=1; j<places; j++) {
		if( abs(number/i) < 1.) break;
		i*=10;
	}
	if(number < 0) {
		number = abs(number);
		lcd.print('-');
		places--;
	}
	cap(number, (i)-1);
	for(int j=0; j<places; j++){
		if(i==1) {
			i/=10;
			lcd.print('.');
		}
		else{
			i/=10;
			if(i<1) lcd.print( (int)(number/i) %10 );
			else lcd.print( ((int)(number/i)) %10 );
		}
	}
}

void printInPlace(int places, double number){
	printInPlace(places, (float) number);
}

void cap(int&   var, long  max){ if(var > max) var = max; }
void cap(float& var, float max){ if(var > max) var = max; }

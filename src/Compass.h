#include <Math.h>
#include <Wire.h>

#ifndef TwoWire_h
	#error "Compass Lib requires Wire.h to be included first to function"
#else

#ifndef COMPASS_H
#define COMPASS_H

void beginCompass();
void rawCompass(int*x,int*y,int*z);
float getHeading();
float getHeading(int xshift,int xscalar,int yshift,int yscalar);
float getHeadingTiltComp(int xshift, int xscalar,
						 int yshift, int yscalar,
						 double pitch, double roll);

#endif

#endif

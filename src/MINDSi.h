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

#ifndef MINDSi_h
#define MINDSi_h

#include "Arduino.h"
#include "wiring_private.h"

int getPing(int pin, uint32_t maxMicros = 20000);
int QTI(int pin, uint32_t maxLoops = 10000);

bool isRadioOn(int pin, uint32_t timeoutMicros = 5000);
int getRadioPulse(int pin, bool interrupt);
int getRadio(int pin, int min = 45, int max = 135, bool interrupt = true);

bool fastDigitalRead(int pin);

#endif

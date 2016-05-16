/* Copyright 2015-16 MINDS-i, INC.

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

namespace MINDSi {
    /* maximum microseconds without a pulse before considering the signal off */
    constexpr uint32_t radio_pulse_timeout = 20000;
    /* maximum microseconds before considering a ping sensor reading lost*/
    constexpr uint32_t ping_reading_timeout = 20000;
    /* maximum loop count for a QTI sensor reading */
    constexpr uint32_t QTI_reading_timeout = 10000;
    /* high time in microseconds of a radio signal at 0 degrees */
    constexpr uint32_t radio_min_us = 600;
    /* high time in microseconds of a radio signal at 180 degrees */
    constexpr uint32_t radio_max_us = 2400;
}

/* */
int getPing(int pin, uint32_t maxMicros = MINDSi::ping_reading_timeout);
/* */
int QTI(int pin, uint32_t maxLoops = MINDSi::QTI_reading_timeout);
/* */
bool isRadioOn(int pin, uint32_t timeoutMicros = MINDSi::radio_pulse_timeout);
/* */
int getRadioPulse(int pin, bool interrupt);
/* */
int getRadio(int pin, bool interrupt = true);
/* */
bool fastDigitalRead(int pin);

#endif

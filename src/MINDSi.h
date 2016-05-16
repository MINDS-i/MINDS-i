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
    /** maximum microseconds without a pulse before considering the signal off*/
    constexpr uint32_t RADIO_PULSE_TIMEOUT = 80000;
    /** maximum microseconds before considering a ping sensor reading lost */
    constexpr uint16_t PING_READING_TIMEOUT = 20000;
    /** maximum loop count for a QTI sensor reading */
    constexpr uint16_t QTI_READING_TIMEOUT = 10000;
    /** high time in microseconds of a radio signal at 0 degrees */
    constexpr uint16_t RADIO_MIN_US = 600;
    /** high time in microseconds of a radio signal at 180 degrees */
    constexpr uint16_t RADIO_MAX_US = 2400;
    /** signal to return from getRadio when nothing is received */
    constexpr uint16_t DEFAULT_RADIO_SIGNAL = 90;
    /** returned by getRadioPulse when no radio pulse is active */
    constexpr uint16_t NO_PULSE = 0xffff;
}

/** Return the radio signal present on `pin`
  * The signal is scaled as if it were a servo signal into the range 0-180
  * If no signal is present, DEFAULT_RADIO_SIGNAL is returned
  * If `interrupt`, this will enable an interrupt based signal receiver
  *     and the call will immediatly return stored values from then on.
  * If `interrupt` is false or the pin does not support interrupt input,
  *     The call will wait until a new pulse arrives and time it.
  **/
uint16_t getRadio(int pin, bool interrupt = true);
/** Activate a parallax ping sensor and return the echo time in microseconds */
uint16_t getPing(int pin, uint16_t maxMicros = MINDSi::PING_READING_TIMEOUT);
/** Poll a QTI sensor, returning a unitless time based value
  * smaller values correspond to a higher light intensity hitting the sensor
  */
uint16_t QTI(int pin, uint16_t maxLoops = MINDSi::QTI_READING_TIMEOUT);
/** Determines if a radio signal is present on the given pin */
bool isRadioOn(int pin, bool interrupt = true);
/** Return the raw radio pulse length on a given pin, or NO_PULSE */
uint16_t getRadioPulse(int pin, bool interrupt);

#endif

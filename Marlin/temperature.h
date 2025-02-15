/*
  temperature.h - temperature controller
  Part of Marlin

  Copyright (c) 2011 Erik van der Zalm

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TEMPERATURE_H
#define TEMPERATURE_H 

#include "Marlin.h"
#include "planner.h"
#ifdef PID_ADD_EXTRUSION_RATE
  #include "stepper.h"
#endif

// public functions
void tp_init();  //initialize the heating
void manage_heater(); //it is critical that this is called periodically.

#ifdef FILAMENT_SENSOR
// For converting raw Filament Width to milimeters 
 float analog2widthFil(); 
 
// For converting raw Filament Width to an extrusion ratio 
 int widthFil_to_size_ratio();
#endif

// low level conversion routines
// do not use these routines and variables outside of temperature.cpp
extern int target_temperature[4];  
extern float current_temperature[4];
#ifdef PNEUMATICS
  extern int current_pneumatic_raw;
  extern int target_value_pneumatic;
  extern float current_pneumatic;
#endif
#ifdef SHOW_TEMP_ADC_VALUES
  extern int current_temperature_raw[4];
  extern int current_temperature_bed_raw;
#endif
extern int target_temperature_bed;
extern float current_temperature_bed;
#ifdef TEMP_SENSOR_1_AS_REDUNDANT
  extern float redundant_temperature;
#endif

#if HAS_CONTROLLERFAN
  extern unsigned char soft_pwm_bed;
#endif

#ifdef PIDTEMP

  #ifdef PID_PARAMS_PER_EXTRUDER
    extern float Kp[EXTRUDERS], Ki[EXTRUDERS], Kd[EXTRUDERS], Kc[EXTRUDERS]; // one param per extruder
    #define PID_PARAM(param,e) param[e] // use macro to point to array value
  #else
    extern float Kp, Ki, Kd, Kc; // one param per extruder - saves 20 or 36 bytes of ram (inc array pointer)
    #define PID_PARAM(param, e) param // use macro to point directly to value
  #endif // PID_PARAMS_PER_EXTRUDER	
  float scalePID_i(float i);
  float scalePID_d(float d);
  float unscalePID_i(float i);
  float unscalePID_d(float d);

#endif

#ifdef PIDTEMPBED
  extern float bedKp,bedKi,bedKd;
#endif
  
#ifdef BABYSTEPPING
  extern volatile int babystepsTodo[3];
#endif
  
//high level conversion routines, for use outside of temperature.cpp
//inline so that there is no performance decrease.
//deg=degreeCelsius

FORCE_INLINE float degHotend(uint8_t extruder) { return current_temperature[extruder]; }
FORCE_INLINE float degBed() { return current_temperature_bed; }

#ifdef SHOW_TEMP_ADC_VALUES
  FORCE_INLINE float rawHotendTemp(uint8_t extruder) { return current_temperature_raw[extruder]; }
  FORCE_INLINE float rawBedTemp() { return current_temperature_bed_raw; }
#endif

FORCE_INLINE float degTargetHotend(uint8_t extruder) { return target_temperature[extruder]; }
FORCE_INLINE float degTargetBed() { return target_temperature_bed; }

#ifdef PNEUMATICS
  FORCE_INLINE int rawPneumatic() { return current_pneumatic_raw; }
  FORCE_INLINE float pressurePneumatic() { return current_pneumatic / 10.0; }
  FORCE_INLINE float targetPneumatic() { return target_value_pneumatic / 10.0; }
  FORCE_INLINE void setTargetPressure(const float &psi) { target_value_pneumatic = psi * 10; } // Multiply by 10 to eliminate floating point #s
#endif

FORCE_INLINE void setTargetHotend(const float &celsius, uint8_t extruder) { target_temperature[extruder] = celsius; }
FORCE_INLINE void setTargetBed(const float &celsius) { target_temperature_bed = celsius; }

FORCE_INLINE bool isHeatingHotend(uint8_t extruder) { return target_temperature[extruder] > current_temperature[extruder]; }
FORCE_INLINE bool isHeatingBed() { return target_temperature_bed > current_temperature_bed; }

FORCE_INLINE bool isCoolingHotend(uint8_t extruder) { return target_temperature[extruder] < current_temperature[extruder]; }
FORCE_INLINE bool isCoolingBed() { return target_temperature_bed < current_temperature_bed; }

#define HOTEND_ROUTINES(NR) \
  FORCE_INLINE float degHotend##NR() { return degHotend(NR); } \
  FORCE_INLINE float degTargetHotend##NR() { return degTargetHotend(NR); } \
  FORCE_INLINE void setTargetHotend##NR(const float c) { setTargetHotend(c, NR); } \
  FORCE_INLINE bool isHeatingHotend##NR() { return isHeatingHotend(NR); } \
  FORCE_INLINE bool isCoolingHotend##NR() { return isCoolingHotend(NR); }
HOTEND_ROUTINES(0);
#if EXTRUDERS > 1
  HOTEND_ROUTINES(1);
#else
  #define setTargetHotend1(c) do{}while(0)
#endif
#if EXTRUDERS > 2
  HOTEND_ROUTINES(2);
#else
  #define setTargetHotend2(c) do{}while(0)
#endif
#if EXTRUDERS > 3
  HOTEND_ROUTINES(3);
#else
  #define setTargetHotend3(c) do{}while(0)
#endif

int getHeaterPower(int heater);
void disable_all_heaters();
void setWatch();
void updatePID();

void PID_autotune(float temp, int extruder, int ncycles);

void setExtruderAutoFanState(int pin, bool state);
void checkExtruderAutoFans();

FORCE_INLINE void autotempShutdown() {
  #ifdef AUTOTEMP
    if (autotemp_enabled) {
      autotemp_enabled = false;
      if (degTargetHotend(active_extruder) > autotemp_min)
        setTargetHotend(0, active_extruder);
    }
  #endif
}

#endif // TEMPERATURE_H

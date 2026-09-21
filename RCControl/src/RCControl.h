#ifndef RCControl_h
#define RCControl_h

#if defined(ARDUINO_ARCH_ESP32)
	#include "esp32/RCControl_ESP32.h"
#elif defined(ARDUINO_ARCH_MBED)
	#include "mbed/RCControl_MBED.h"
#else
	#error "RCControl: Unsupported architecture"
#endif

#endif
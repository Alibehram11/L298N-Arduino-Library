/*
 * ============================================================================
 * Arduino Car Library - Main Header
 * ============================================================================
 * 
 * This is the primary header file for the Arduino Car Control Library.
 * Include this file to access both:
 *   - Motor control (L298N H-bridge driver)
 *   - HC-06 Bluetooth communication
 * 
 * Usage:
 *   #include <arduino/arduino.h>
 * 
 * Or include individual components:
 *   #include <arduino/motor.h>
 *   #include <arduino/hc-06.h>
 */

#ifndef ARDUINO_H
#define ARDUINO_H

// Include all library components
#include "motor.h"
#include "hc-06.h"

#endif // ARDUINO_H
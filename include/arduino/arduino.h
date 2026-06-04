/*
 * ════════════════════════════════════════════════════════════════════════════
 * Arduino Car Library - Main Header
 * ════════════════════════════════════════════════════════════════════════════
 * 
 * Modular hardware abstraction library for Arduino microcontrollers.
 * 
 * Core Modules:
 *   ✓ Motor Control (L298N Dual H-bridge Driver)
 *   ✓ Bluetooth Communication (HC-06 Serial)
 *   ✓ Ultrasonic Distance Sensor (HC-SR04 enhanced with filtering)
 *   ✓ I2C Radio Link (RF receiver on I2C bus)
 *
 * Usage:
 *   #include <arduino/arduino.h>
 *   
 *   // Or include individual components:
 *   #include <arduino/motor.h>
 *   #include <arduino/hc-06.h>
 *   #include <arduino/hcsr-04.h>
 *   #include <arduino/radio-link-i2c.h>
 *
 * Design Principles:
 *   • Clean abstraction: Hardware implementation hidden, logical interface exposed
 *   • Modular: Include only what you need, no unused code
 *   • Efficient: Optimized for embedded systems with limited resources
 *   • Well-documented: Extensive comments for educational value
 *   • Professional grade: Production-ready with error handling
 *
 * Author: Arduino Car Library Contributors
 * License: MIT
 * ════════════════════════════════════════════════════════════════════════════
 */

#ifndef ARDUINO_CAR_LIBRARY_H
#define ARDUINO_CAR_LIBRARY_H

// Motor control module
#include "motor.h"

// Bluetooth communication module
#include "hc-06.h"

// Ultrasonic distance sensor (enhanced version with filtering & averaging)
#include "hcsr-04.h"

// I2C-based radio link receiver
#include "radio-link-i2c.h"

// ═══════════════════════════════════════════════════════════════════════════
// ADVANCED FEATURES (NEW)
// ═══════════════════════════════════════════════════════════════════════════

// PID Motor Speed Controller - Smooth speed regulation with feedback
#include "pid-controller.h"

// QTR Line Following Sensors - Reflectance sensor arrays for line tracking
#include "qtr-sensor.h"

// Motor Encoder Feedback - Quadrature encoders for speed/position tracking
#include "motor-encoder.h"

// Configuration Manager - Persistent storage of robot parameters
#include "config-manager.h"

// Speed Ramper - Smooth acceleration/deceleration profiles
#include "speed-ramper.h"

// Power Management - Battery monitoring and energy tracking
#include "power-management.h"

// Diagnostics Engine - System health monitoring and error logging
#include "diagnostics.h"

// Encoder - Motor rotation and distance tracking
#include "encoder.h"

#endif // ARDUINO_CAR_LIBRARY_H


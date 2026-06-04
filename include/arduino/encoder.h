/*
 * ═══════════════════════════════════════════════════════════════════════════════
 * Motor Encoder/Odometer Module - Header
 * ═══════════════════════════════════════════════════════════════════════════════
 * Track motor rotation, RPM, distance traveled, and heading
 * 
 * Features:
 *   ✓ Quadrature encoder support
 *   ✓ RPM calculation with filtering
 *   ✓ Odometry (distance/heading tracking)
 *   ✓ Interrupt-based reading for accuracy
 *   ✓ Acceleration detection
 *
 * Author: Arduino Car Library Contributors
 * License: MIT
 * ═══════════════════════════════════════════════════════════════════════════════
 */

#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>

/**
 * Encoder Class - Motor speed and distance measurement via rotary encoder
 * 
 * Measures motor RPM and tracks total distance traveled by counting encoder pulses.
 */
class Encoder {
public:
    /**
     * Constructor - Initialize encoder
     * 
     * @param pinA - Encoder A (pulse) pin
     * @param pinB - Encoder B (direction) pin
     * @param pulsesPerRevolution - Pulses per full motor rotation
     * @param wheelDiameterCM - Wheel diameter in centimeters (for odometry)
     */
    Encoder(int pinA, int pinB, int pulsesPerRevolution, float wheelDiameterCM = 6.5);

    /**
     * Begin encoder reading (setup interrupts)
     */
    void begin();

    /**
     * Get current RPM (revolutions per minute)
     * 
     * @return RPM value (0-32000), or -1 if not moving
     */
    int getRPM();

    /**
     * Get distance traveled since reset
     * 
     * @return Distance in centimeters
     */
    float getDistance();

    /**
     * Get total pulse count
     * 
     * @return Total pulses counted
     */
    long getPulseCount();

    /**
     * Get rotation direction
     * 
     * @return 1 = forward, -1 = backward, 0 = stopped
     */
    int getDirection();

    /**
     * Reset distance and pulse counters
     */
    void reset();

    /**
     * Reset distance only (keep pulse count)
     */
    void resetDistance();

    /**
     * Check if motor is accelerating
     * 
     * @return true if RPM is increasing
     */
    bool isAccelerating();

    /**
     * Get acceleration (RPM change per second)
     * 
     * @return Current acceleration in RPM/sec
     */
    float getAcceleration();

private:
    int _pinA, _pinB;
    int _pulsesPerRev;
    float _wheelDiameterCM;
    volatile long _pulseCount;
    volatile int _direction;
    unsigned long _lastRPMTime;
    int _lastRPM;
    int _currentRPM;
};

#endif // ENCODER_H

/*
 * ═════════════════════════════════════════════════════════════════════════════
 * Motor Encoder Feedback Module - Header
 * ═════════════════════════════════════════════════════════════════════════════
 * Quadrature encoder support for motor speed measurement and position tracking.
 * Essential for PID motor control and odometry calculations.
 *
 * Supported Encoders:
 *   - Standard quadrature encoders (AB channels)
 *   - Incremental rotary encoders
 *   - Hall effect sensors
 *
 * Features:
 *   ✓ Quadrature decoding (360°/tick precision)
 *   ✓ Speed calculation (RPM, rad/s, m/s)
 *   ✓ Position tracking (absolute counts, rotations)
 *   ✓ Direction detection
 *   ✓ Interrupt-driven counting for accuracy
 *
 * Author: Arduino Car Library Contributors
 * License: MIT
 * ═════════════════════════════════════════════════════════════════════════════
 */

#ifndef MOTOR_ENCODER_H
#define MOTOR_ENCODER_H

#include <Arduino.h>

/**
 * MotorEncoder Class - Quadrature Encoder Interface
 *
 * Typical Usage:
 *   MotorEncoder encoder1(2, 4, 1024);  // Pin 2 (CLK), Pin 4 (DT), 1024 counts/rev
 *
 *   while (running) {
 *       long ticks = encoder1.getTickCount();
 *       float rpm = encoder1.getRPM();
 *       encoder1.update(micros());  // Call frequently or use interrupts
 *   }
 */
class MotorEncoder {
public:
    /**
     * Constructor - Initialize encoder
     *
     * @param clkPin - Clock/A-channel pin (interrupt capable)
     * @param dtPin - Data/B-channel pin
     * @param countsPerRevolution - Encoder resolution (e.g., 1024 CPR)
     * @param wheelRadiusMM - Wheel radius in mm (for distance calculation, optional)
     *
     * Notes:
     *   - Pin must support attachInterrupt() for accuracy
     *   - CPR depends on encoder type (check datasheet)
     */
    MotorEncoder(uint8_t clkPin, uint8_t dtPin, uint16_t countsPerRevolution,
                 float wheelRadiusMM = 33.0);

    /**
     * Destructor
     */
    ~MotorEncoder();

    /**
     * Get total tick count since start/reset
     *
     * @return Accumulated encoder ticks (can be negative for reverse)
     */
    long getTickCount() const;

    /**
     * Get current rotation count (full revolutions)
     *
     * @return Number of complete rotations
     */
    long getRotationCount() const;

    /**
     * Get current speed in RPM
     *
     * @return Rotations per minute
     *
     * Requires periodic update() calls for accurate measurement
     */
    float getRPM() const;

    /**
     * Get current speed in radians per second
     *
     * @return Angular velocity (rad/s)
     */
    float getRadPerSec() const;

    /**
     * Get linear speed in mm/s (if wheel radius configured)
     *
     * @return Linear velocity (mm/s)
     */
    float getLinearSpeedMMS() const;

    /**
     * Get distance traveled in mm
     *
     * @return Total distance from reset point
     */
    float getDistanceMM() const;

    /**
     * Get current direction
     *
     * @return 1 for forward, -1 for backward, 0 for stopped
     */
    int8_t getDirection() const;

    /**
     * Reset tick counter to zero
     *
     * Useful at start of motion or for lap counting
     */
    void resetTicks();

    /**
     * Reset distance counter
     *
     * Clears accumulated distance measurement
     */
    void resetDistance();

    /**
     * Update speed calculation (call this regularly or in interrupt)
     *
     * @param currentTimeMicros - Current time in microseconds (from micros())
     *
     * Speed calculation updates at this rate. For best results, call
     * at regular intervals (e.g., every 20-50ms in loop, or via timer interrupt)
     */
    void update(unsigned long currentTimeMicros);

    /**
     * Attach interrupt handler (optional, for high-speed accuracy)
     *
     * Call this to use hardware interrupts instead of polling update()
     * Arduino Uno: Pin 2 (INT0), Pin 3 (INT1)
     */
    void attachInterrupt();

    /**
     * Detach interrupt handler
     */
    void detachInterrupt();

    /**
     * Set wheel radius for distance calculations
     *
     * @param radiusMM - Wheel radius in millimeters
     */
    void setWheelRadius(float radiusMM);

    /**
     * Get encoder resolution (counts per revolution)
     *
     * @return CPR value
     */
    uint16_t getCountsPerRevolution() const;

    /**
     * Get raw tick count from last cycle
     *
     * @return Ticks counted in last update interval
     */
    long getTicksLastCycle() const;

private:
    uint8_t _clkPin;
    uint8_t _dtPin;
    uint16_t _cpr;
    float _wheelRadiusMM;

    // State
    volatile long _tickCount;
    volatile long _lastTickCount;
    long _ticksLastCycle;

    float _rpmValue;
    int8_t _direction;
    unsigned long _lastUpdateTime;
    float _distanceAccumulator;

    /**
     * Internal: Quadrature decoder logic
     */
    void decodeQuadrature();

    /**
     * Internal: Interrupt handler stub
     */
    static void interruptHandler();
};

#endif // MOTOR_ENCODER_H

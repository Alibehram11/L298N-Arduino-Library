/*
 * ═════════════════════════════════════════════════════════════════════════════
 * Speed Ramper - Smooth Acceleration/Deceleration
 * ═════════════════════════════════════════════════════════════════════════════
 * Generates smooth speed profiles for acceleration and deceleration,
 * preventing mechanical shock and ensuring smooth motion of robot vehicles.
 *
 * Features:
 *   ✓ Linear ramp acceleration
 *   ✓ S-curve (sigmoid) acceleration (smooth start/stop)
 *   ✓ Configurable acceleration rates
 *   ✓ Non-blocking async operation
 *   ✓ Multiple independent ramps
 *
 * Author: Arduino Car Library Contributors
 * License: MIT
 * ═════════════════════════════════════════════════════════════════════════════
 */

#ifndef SPEED_RAMPER_H
#define SPEED_RAMPER_H

#include <Arduino.h>

/**
 * SpeedRamper Class - Smooth Motor Acceleration/Deceleration
 *
 * Prevents mechanical shock from sudden speed changes and ensures
 * smooth, controllable motion profiles.
 *
 * Typical Usage:
 *   SpeedRamper leftMotor(255, 50);      // Max speed 255, accel 50 units/s
 *   SpeedRamper rightMotor(255, 50);
 *
 *   // Set target speed, ramp happens gradually
 *   leftMotor.setTargetSpeed(200);
 *   rightMotor.setTargetSpeed(200);
 *
 *   while (true) {
 *       uint8_t leftPWM = leftMotor.update(millis());
 *       uint8_t rightPWM = rightMotor.update(millis());
 *       
 *       motor.drive(leftPWM, rightPWM);
 *   }
 */
class SpeedRamper {
public:
    /**
     * Acceleration profile types
     */
    enum AccelProfile {
        PROFILE_LINEAR,    // Constant acceleration (max jerk)
        PROFILE_SCURVE,    // S-curve (smooth start, smooth stop)
        PROFILE_EXPONENTIAL // Exponential approach (smooth)
    };

    /**
     * Constructor - Initialize speed ramper
     *
     * @param maxSpeed - Maximum PWM output (usually 255)
     * @param accelRate - Acceleration rate (units per second)
     *                    - 100 = reach full speed in ~2.5 seconds
     *                    - 255 = reach full speed in ~1 second (instant-ish)
     * @param profile - Acceleration profile type (default: LINEAR)
     *
     * Example:
     *   SpeedRamper(255, 50, PROFILE_SCURVE)
     *   - Max 255 PWM, smooth S-curve ramp at 50 units/sec
     */
    SpeedRamper(uint8_t maxSpeed = 255, uint8_t accelRate = 100,
                AccelProfile profile = PROFILE_LINEAR);

    /**
     * Destructor
     */
    ~SpeedRamper();

    /**
     * Set target speed
     *
     * @param targetSpeed - Desired speed (0-255 for PWM)
     *
     * Ramping will begin immediately towards this target at configured rate.
     */
    void setTargetSpeed(uint8_t targetSpeed);

    /**
     * Get current speed (may be ramping towards target)
     *
     * @return Current PWM value (0-255)
     */
    uint8_t getCurrentSpeed() const;

    /**
     * Get target speed
     *
     * @return Target PWM value being approached
     */
    uint8_t getTargetSpeed() const;

    /**
     * Update ramper state (call this regularly)
     *
     * @param currentTimems - Current time in milliseconds (from millis())
     * @return Current speed output (0-255)
     *
     * Call at regular intervals (10-50ms) for smooth ramping.
     * Returns linearly interpolated value between current and target.
     */
    uint8_t update(unsigned long currentTimeMs);

    /**
     * Check if speed ramping is complete
     *
     * @return true if current speed equals target speed
     */
    bool isRampingComplete() const;

    /**
     * Set acceleration rate
     *
     * @param accelRate - New acceleration rate (units per second)
     */
    void setAccelerationRate(uint8_t accelRate);

    /**
     * Set acceleration profile
     *
     * @param profile - Profile type (LINEAR, SCURVE, EXPONENTIAL)
     */
    void setProfile(AccelProfile profile);

    /**
     * Instantly jump to target speed (no ramping)
     *
     * Useful for emergency maneuvers or emergency stop
     */
    void jumpToTarget();

    /**
     * Stop immediately (emergency brake)
     *
     * Sets target to 0 and optionally jumps to it
     *
     * @param immediate - true to stop instantly, false to ramp down
     */
    void stop(bool immediate = false);

    /**
     * Reset ramper to zero speed
     *
     * Resets internal state without changing target
     */
    void reset();

private:
    uint8_t _currentSpeed;
    float _currentSpeedFloat;
    uint8_t _targetSpeed;
    uint8_t _rampStartSpeed;
    uint8_t _maxSpeed;
    uint8_t _accelRate;
    AccelProfile _profile;
    unsigned long _lastUpdateTime;
    unsigned long _rampStartTime;
    unsigned long _rampDurationMs;

    void restartRamp();

    /**
     * Internal: Calculate S-curve interpolation
     */
    float calculateSCurve(float t);

    /**
     * Internal: Calculate exponential interpolation
     */
    float calculateExponential(float t);
};

#endif // SPEED_RAMPER_H

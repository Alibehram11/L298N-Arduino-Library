/*
 * ═════════════════════════════════════════════════════════════════════════════
 * PID Motor Speed Controller - Header
 * ═════════════════════════════════════════════════════════════════════════════
 * Proportional-Integral-Derivative controller for smooth motor speed regulation
 * with encoder feedback, ideal for maintaining constant velocity despite load variations.
 *
 * Features:
 *   ✓ PID control loop with configurable gains (Kp, Ki, Kd)
 *   ✓ Anti-windup for integral term
 *   ✓ Derivative filter to reduce noise
 *   ✓ Output clamping (0-255 PWM)
 *   ✓ Real-time tuning of PID parameters
 *
 * Author: Arduino Car Library Contributors
 * License: MIT
 * ═════════════════════════════════════════════════════════════════════════════
 */

#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include <Arduino.h>

/**
 * PIDController Class - Motor Speed Regulation via Feedback
 *
 * Typical Usage:
 *   PIDController pid(0.5, 0.2, 0.1);  // Kp=0.5, Ki=0.2, Kd=0.1
 *   pid.setTargetSpeed(150);           // Target: 150 RPM
 *   pid.setOutputLimits(0, 255);
 *
 *   while (running) {
 *       uint16_t currentSpeed = getEncoderRPM();  // From encoder
 *       uint8_t pwm = pid.update(currentSpeed);
 *       motor.setPWM(pwm);
 *       delay(20);  // 50Hz control loop
 *   }
 */
class PIDController {
public:
    /**
     * Constructor - Initialize PID controller with gains
     *
     * @param kp - Proportional gain (typical: 0.1-2.0)
     * @param ki - Integral gain (typical: 0.01-0.5)
     * @param kd - Derivative gain (typical: 0.01-1.0)
     *
     * Tuning Guide:
     *   Kp: Larger = faster response, too large = oscillation
     *   Ki: Reduces steady-state error, too large = overshoot
     *   Kd: Smooths response, too large = noise amplification
     */
    PIDController(float kp = 1.0, float ki = 0.1, float kd = 0.05);

    /**
     * Destructor
     */
    ~PIDController();

    /**
     * Update PID controller with current measurement
     *
     * @param currentValue - Current speed/position from sensor
     * @return Control output (0-255 PWM or configured limits)
     *
     * Call this regularly in your main loop (typically 20-100Hz)
     */
    uint8_t update(float currentValue);

    /**
     * Set target setpoint (desired speed/position)
     *
     * @param target - Target value to reach
     */
    void setTarget(float target);

    /**
     * Get current error (target - measured)
     *
     * @return Error value
     */
    float getError() const;

    /**
     * Get integral term (accumulated error)
     *
     * @return Integral value
     */
    float getIntegral() const;

    /**
     * Get derivative term (rate of change)
     *
     * @return Derivative value
     */
    float getDerivative() const;

    /**
     * Set PID gains dynamically
     *
     * @param kp - Proportional gain
     * @param ki - Integral gain
     * @param kd - Derivative gain
     */
    void setGains(float kp, float ki, float kd);

    /**
     * Set output limits for clamping
     *
     * @param minOutput - Minimum output (usually 0)
     * @param maxOutput - Maximum output (usually 255 for PWM)
     */
    void setOutputLimits(float minOutput, float maxOutput);

    /**
     * Set integral windup guard
     *
     * @param limit - Maximum integral term magnitude
     */
    void setIntegralLimit(float limit = 50.0);

    /**
     * Reset PID state (clear integral, derivative history)
     *
     * Useful when starting new control cycle or changing setpoint
     */
    void reset();

    /**
     * Enable/disable PID controller
     *
     * @param enable - true to enable, false to freeze output
     */
    void setEnabled(bool enable = true);

    /**
     * Get controller status
     *
     * @return true if enabled, false if disabled
     */
    bool isEnabled() const;

private:
    // PID gains
    float _kp, _ki, _kd;

    // Control state
    float _target;
    float _lastError;
    float _integral;
    float _lastValue;

    // Output limits
    float _minOutput;
    float _maxOutput;

    // Anti-windup limit
    float _integralLimit;

    // Status
    bool _enabled;

    // Timestamps for dt calculation
    unsigned long _lastTime;

    /**
     * Internal: Clamp value between min and max
     */
    float clamp(float value, float minVal, float maxVal);
};

#endif // PID_CONTROLLER_H

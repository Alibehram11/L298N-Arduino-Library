/*
 * ═════════════════════════════════════════════════════════════════════════════
 * PID Motor Speed Controller - Implementation
 * ═════════════════════════════════════════════════════════════════════════════
 */

#include "pid-controller.h"

PIDController::PIDController(float kp, float ki, float kd)
    : _kp(kp), _ki(ki), _kd(kd),
      _target(0), _lastError(0), _integral(0), _lastValue(0),
      _minOutput(0), _maxOutput(255),
      _integralLimit(50.0),
      _enabled(true), _lastTime(millis())
{
}

PIDController::~PIDController()
{
}

uint8_t PIDController::update(float currentValue)
{
    if (!_enabled) {
        return 0;
    }

    unsigned long currentTime = millis();
    float dt = (float)(currentTime - _lastTime) / 1000.0;  // Delta time in seconds
    _lastTime = currentTime;

    if (dt <= 0) dt = 0.001;  // Prevent division by zero

    // Calculate error
    float error = _target - currentValue;

    // Proportional term
    float p = _kp * error;

    // Integral term with anti-windup
    _integral += error * dt;
    if (_integral > _integralLimit) _integral = _integralLimit;
    if (_integral < -_integralLimit) _integral = -_integralLimit;
    float i = _ki * _integral;

    // Derivative term
    float derivative = (error - _lastError) / dt;
    float d = _kd * derivative;

    // Calculate output
    float output = p + i + d;

    // Clamp output
    output = clamp(output, _minOutput, _maxOutput);

    // Store for next iteration
    _lastError = error;
    _lastValue = currentValue;

    return (uint8_t)output;
}

void PIDController::setTarget(float target)
{
    _target = target;
}

float PIDController::getError() const
{
    return _target - _lastValue;
}

float PIDController::getIntegral() const
{
    return _integral;
}

float PIDController::getDerivative() const
{
    return _lastError;
}

void PIDController::setGains(float kp, float ki, float kd)
{
    _kp = kp;
    _ki = ki;
    _kd = kd;
}

void PIDController::setOutputLimits(float minOutput, float maxOutput)
{
    _minOutput = minOutput;
    _maxOutput = maxOutput;
}

void PIDController::setIntegralLimit(float limit)
{
    _integralLimit = limit;
}

void PIDController::reset()
{
    _integral = 0;
    _lastError = 0;
    _lastValue = 0;
    _lastTime = millis();
}

void PIDController::setEnabled(bool enable)
{
    _enabled = enable;
    if (enable) {
        _lastTime = millis();  // Reset time reference
    }
}

bool PIDController::isEnabled() const
{
    return _enabled;
}

float PIDController::clamp(float value, float minVal, float maxVal)
{
    if (value < minVal) return minVal;
    if (value > maxVal) return maxVal;
    return value;
}

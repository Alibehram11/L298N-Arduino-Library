/*
 * ═════════════════════════════════════════════════════════════════════════════
 * Speed Ramper - Implementation
 * ═════════════════════════════════════════════════════════════════════════════
 */

#include "speed-ramper.h"

SpeedRamper::SpeedRamper(uint8_t maxSpeed, uint8_t accelRate, AccelProfile profile)
    : _currentSpeed(0), _targetSpeed(0), _maxSpeed(maxSpeed),
      _accelRate(accelRate), _profile(profile),
      _lastUpdateTime(millis())
{
}

SpeedRamper::~SpeedRamper()
{
}

void SpeedRamper::setTargetSpeed(uint8_t targetSpeed)
{
    _targetSpeed = min(targetSpeed, _maxSpeed);
}

uint8_t SpeedRamper::getCurrentSpeed() const
{
    return _currentSpeed;
}

uint8_t SpeedRamper::getTargetSpeed() const
{
    return _targetSpeed;
}

uint8_t SpeedRamper::update(unsigned long currentTimeMs)
{
    unsigned long deltaTime = currentTimeMs - _lastUpdateTime;
    _lastUpdateTime = currentTimeMs;

    // Calculate maximum change this cycle
    float maxDelta = (_accelRate / 1000.0) * deltaTime;

    if (_currentSpeed < _targetSpeed) {
        // Accelerating
        float newSpeed = _currentSpeed + maxDelta;
        if (newSpeed > _targetSpeed) {
            newSpeed = _targetSpeed;
        }
        _currentSpeed = (uint8_t)newSpeed;
    } else if (_currentSpeed > _targetSpeed) {
        // Decelerating
        float newSpeed = _currentSpeed - maxDelta;
        if (newSpeed < _targetSpeed) {
            newSpeed = _targetSpeed;
        }
        _currentSpeed = (uint8_t)newSpeed;
    }

    return _currentSpeed;
}

bool SpeedRamper::isRampingComplete() const
{
    return _currentSpeed == _targetSpeed;
}

void SpeedRamper::setAccelerationRate(uint8_t accelRate)
{
    _accelRate = accelRate;
}

void SpeedRamper::setProfile(AccelProfile profile)
{
    _profile = profile;
}

void SpeedRamper::jumpToTarget()
{
    _currentSpeed = _targetSpeed;
}

void SpeedRamper::stop(bool immediate)
{
    _targetSpeed = 0;
    if (immediate) {
        _currentSpeed = 0;
    }
}

void SpeedRamper::reset()
{
    _currentSpeed = 0;
    _lastUpdateTime = millis();
}

float SpeedRamper::calculateSCurve(float t)
{
    // S-curve interpolation (smooth start and stop)
    // 3t^2 - 2t^3
    return 3 * t * t - 2 * t * t * t;
}

float SpeedRamper::calculateExponential(float t)
{
    // Exponential approach: 1 - e^(-kt)
    return 1.0 - exp(-2.0 * t);
}

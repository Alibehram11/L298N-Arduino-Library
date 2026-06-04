/*
 * ═════════════════════════════════════════════════════════════════════════════
 * Speed Ramper - Implementation
 * ═════════════════════════════════════════════════════════════════════════════
 */

#include "speed-ramper.h"

SpeedRamper::SpeedRamper(uint8_t maxSpeed, uint8_t accelRate, AccelProfile profile)
    : _currentSpeed(0), _currentSpeedFloat(0), _targetSpeed(0), _rampStartSpeed(0), _maxSpeed(maxSpeed),
      _accelRate(accelRate), _profile(profile),
      _lastUpdateTime(millis()), _rampStartTime(millis()), _rampDurationMs(0)
{
}

SpeedRamper::~SpeedRamper()
{
}

void SpeedRamper::setTargetSpeed(uint8_t targetSpeed)
{
    uint8_t newTarget = min(targetSpeed, _maxSpeed);
    if (newTarget != _targetSpeed) {
        _targetSpeed = newTarget;
        restartRamp();
    }
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

    if (_currentSpeed == _targetSpeed) {
        _currentSpeedFloat = _currentSpeed;
        return _currentSpeed;
    }

    if (_accelRate == 0) {
        _currentSpeed = _targetSpeed;
        _currentSpeedFloat = _targetSpeed;
        return _currentSpeed;
    }

    if (_profile != PROFILE_LINEAR && _rampDurationMs > 0) {
        float t = (float)(currentTimeMs - _rampStartTime) / (float)_rampDurationMs;
        if (t >= 1.0f) {
            _currentSpeed = _targetSpeed;
            _currentSpeedFloat = _targetSpeed;
            return _currentSpeed;
        }
        if (t < 0.0f) {
            t = 0.0f;
        }

        float shaped = (_profile == PROFILE_SCURVE) ? calculateSCurve(t) : calculateExponential(t);
        int direction = (_targetSpeed >= _rampStartSpeed) ? 1 : -1;
        float span = abs((int)_targetSpeed - (int)_rampStartSpeed);
        _currentSpeedFloat = _rampStartSpeed + direction * span * shaped;
        _currentSpeed = (uint8_t)constrain((int)(_currentSpeedFloat + 0.5f), 0, _maxSpeed);
        return _currentSpeed;
    }

    if (_currentSpeed < _targetSpeed) {
        // Accelerating
        float maxDelta = (_accelRate / 1000.0f) * deltaTime;
        _currentSpeedFloat += maxDelta;
        if (_currentSpeedFloat > _targetSpeed) {
            _currentSpeedFloat = _targetSpeed;
        }
        _currentSpeed = (uint8_t)(_currentSpeedFloat + 0.5f);
    } else if (_currentSpeed > _targetSpeed) {
        // Decelerating
        float maxDelta = (_accelRate / 1000.0f) * deltaTime;
        _currentSpeedFloat -= maxDelta;
        if (_currentSpeedFloat < _targetSpeed) {
            _currentSpeedFloat = _targetSpeed;
        }
        _currentSpeed = (uint8_t)(_currentSpeedFloat + 0.5f);
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
    restartRamp();
}

void SpeedRamper::setProfile(AccelProfile profile)
{
    _profile = profile;
    restartRamp();
}

void SpeedRamper::jumpToTarget()
{
    _currentSpeed = _targetSpeed;
    _currentSpeedFloat = _targetSpeed;
    _rampStartSpeed = _targetSpeed;
    _rampDurationMs = 0;
}

void SpeedRamper::stop(bool immediate)
{
    _targetSpeed = 0;
    if (immediate) {
        _currentSpeed = 0;
        _currentSpeedFloat = 0;
        _rampStartSpeed = 0;
        _rampDurationMs = 0;
    } else {
        restartRamp();
    }
}

void SpeedRamper::reset()
{
    _currentSpeed = 0;
    _currentSpeedFloat = 0;
    _lastUpdateTime = millis();
    restartRamp();
}

void SpeedRamper::restartRamp()
{
    _rampStartSpeed = _currentSpeed;
    _rampStartTime = millis();

    uint8_t distance = abs((int)_targetSpeed - (int)_rampStartSpeed);
    if (_accelRate == 0 || distance == 0) {
        _rampDurationMs = 0;
        return;
    }

    _rampDurationMs = ((unsigned long)distance * 1000UL) / _accelRate;
    if (_rampDurationMs == 0) {
        _rampDurationMs = 1;
    }
}

float SpeedRamper::calculateSCurve(float t)
{
    // S-curve interpolation (smooth start and stop)
    // 3t^2 - 2t^3
    return 3 * t * t - 2 * t * t * t;
}

float SpeedRamper::calculateExponential(float t)
{
    float numerator = 1.0f - exp(-4.0f * t);
    float denominator = 1.0f - exp(-4.0f);
    return numerator / denominator;
}

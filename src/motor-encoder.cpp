/*
 * ═════════════════════════════════════════════════════════════════════════════
 * Motor Encoder Feedback - Implementation
 * ═════════════════════════════════════════════════════════════════════════════
 */

#include "motor-encoder.h"

MotorEncoder::MotorEncoder(uint8_t clkPin, uint8_t dtPin, uint16_t countsPerRevolution,
                           float wheelRadiusMM)
    : _clkPin(clkPin), _dtPin(dtPin), _cpr(countsPerRevolution),
      _wheelRadiusMM(wheelRadiusMM),
      _tickCount(0), _lastTickCount(0), _ticksLastCycle(0),
      _rpmValue(0), _direction(0), _lastUpdateTime(millis()),
      _distanceAccumulator(0)
{
    pinMode(_clkPin, INPUT);
    pinMode(_dtPin, INPUT);
}

MotorEncoder::~MotorEncoder()
{
}

long MotorEncoder::getTickCount() const
{
    return _tickCount;
}

long MotorEncoder::getRotationCount() const
{
    return _tickCount / _cpr;
}

float MotorEncoder::getRPM() const
{
    return _rpmValue;
}

float MotorEncoder::getRadPerSec() const
{
    return _rpmValue * 3.14159 / 30.0;
}

float MotorEncoder::getLinearSpeedMMS() const
{
    // linear_speed = rpm * wheel_circumference / 60
    // circumference = 2 * π * r
    float circumference = 2.0 * 3.14159 * _wheelRadiusMM;
    return (_rpmValue * circumference) / 60.0;
}

float MotorEncoder::getDistanceMM() const
{
    return _distanceAccumulator;
}

int8_t MotorEncoder::getDirection() const
{
    return _direction;
}

void MotorEncoder::resetTicks()
{
    _tickCount = 0;
}

void MotorEncoder::resetDistance()
{
    _distanceAccumulator = 0;
}

void MotorEncoder::update(unsigned long currentTimeMicros)
{
    unsigned long deltaTime = currentTimeMicros - _lastUpdateTime;
    _lastUpdateTime = currentTimeMicros;

    // Calculate ticks in this period
    _ticksLastCycle = _tickCount - _lastTickCount;
    _lastTickCount = _tickCount;

    // Determine direction
    if (_ticksLastCycle > 0) {
        _direction = 1;
    } else if (_ticksLastCycle < 0) {
        _direction = -1;
    } else {
        _direction = 0;
    }

    // Calculate RPM: ticks_per_minute / cpr
    if (deltaTime > 0) {
        float ticksPerMinute = (float)abs(_ticksLastCycle) * 60000000.0 / deltaTime;
        _rpmValue = ticksPerMinute / _cpr;
    } else {
        _rpmValue = 0;
    }

    // Update distance
    float circumference = 2.0 * 3.14159 * _wheelRadiusMM;
    _distanceAccumulator += (float)_ticksLastCycle * circumference / _cpr;
}

void MotorEncoder::attachInterrupt()
{
    // Attach interrupt to CLK pin (typically INT0 or INT1)
    // This is platform-dependent and may need customization
}

void MotorEncoder::detachInterrupt()
{
    // Detach interrupt
}

void MotorEncoder::setWheelRadius(float radiusMM)
{
    _wheelRadiusMM = radiusMM;
}

uint16_t MotorEncoder::getCountsPerRevolution() const
{
    return _cpr;
}

long MotorEncoder::getTicksLastCycle() const
{
    return _ticksLastCycle;
}

void MotorEncoder::decodeQuadrature()
{
    // Read encoder pins
    uint8_t clk = digitalRead(_clkPin);
    uint8_t dt = digitalRead(_dtPin);

    // Quadrature decoding logic
    // This is a simple implementation
    if (clk == 1 && dt == 1) {
        _tickCount++;
    } else if (clk == 0 && dt == 0) {
        _tickCount--;
    }
}

void MotorEncoder::interruptHandler()
{
    // Static function for ISR - would be bound to actual instance
}

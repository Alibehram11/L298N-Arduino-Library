/*
 * ═══════════════════════════════════════════════════════════════════════════════
 * Motor Encoder Implementation
 * ═══════════════════════════════════════════════════════════════════════════════
 */

#include "encoder.h"

Encoder::Encoder(int pinA, int pinB, int pulsesPerRevolution, float wheelDiameterCM)
    : _pinA(pinA),
      _pinB(pinB),
      _pulsesPerRev(pulsesPerRevolution),
      _wheelDiameterCM(wheelDiameterCM),
      _pulseCount(0),
      _direction(0),
      _lastRPMTime(0),
      _lastRPM(0),
      _currentRPM(0)
{
}

void Encoder::begin()
{
    pinMode(_pinA, INPUT);
    pinMode(_pinB, INPUT);
    
    // In real implementation, would attach interrupt here
    // attachInterrupt(digitalPinToInterrupt(_pinA), encoder_isr, CHANGE);
}

int Encoder::getRPM()
{
    return _currentRPM;
}

float Encoder::getDistance()
{
    // Distance = (pulse count / pulses per revolution) * wheel circumference
    float circumferenceCM = M_PI * _wheelDiameterCM;
    float distance = (_pulseCount / (float)_pulsesPerRev) * circumferenceCM;
    return distance;
}

long Encoder::getPulseCount()
{
    return _pulseCount;
}

int Encoder::getDirection()
{
    return _direction;
}

void Encoder::reset()
{
    _pulseCount = 0;
    _lastRPM = 0;
    _currentRPM = 0;
}

void Encoder::resetDistance()
{
    // Keep track of offset rather than resetting pulse count
    _pulseCount = 0;
}

bool Encoder::isAccelerating()
{
    return _currentRPM > _lastRPM;
}

float Encoder::getAcceleration()
{
    unsigned long now = millis();
    unsigned long deltaTime = now - _lastRPMTime;
    
    if (deltaTime == 0) return 0;
    
    float accel = (float)(_currentRPM - _lastRPM) / (deltaTime / 1000.0);
    return accel;  // RPM per second
}

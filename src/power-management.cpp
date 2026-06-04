/*
 * ═══════════════════════════════════════════════════════════════════════════════
 * Power Management Implementation
 * ═══════════════════════════════════════════════════════════════════════════════
 */

#include "power-management.h"

PowerManager::PowerManager(int voltagePin, int currentPin, int batteryCapacityMAh)
    : _voltagePin(voltagePin),
      _currentPin(currentPin),
      _batteryCapacityMAh(batteryCapacityMAh),
      _lastVoltage(0),
      _lastCurrent(0),
      _minVoltage(16.0),
      _maxVoltage(0),
      _lowBatteryThreshold(9.0),
      _lastUpdateTime(0),
      _totalEnergyWh(0)
{
}

void PowerManager::update()
{
    unsigned long now = millis();
    unsigned long deltaTime = now - _lastUpdateTime;
    
    // Read voltage from analog pin (0-1023 maps to 0-5V)
    int rawVoltage = analogRead(_voltagePin);
    _lastVoltage = (rawVoltage / 1023.0) * 5.0;
    
    // For 12V battery with voltage divider: actual = measured * 3
    // Adjust multiplier based on your voltage divider configuration
    _lastVoltage = _lastVoltage * 3.0;
    
    // Track min/max
    if (_lastVoltage < _minVoltage) _minVoltage = _lastVoltage;
    if (_lastVoltage > _maxVoltage) _maxVoltage = _lastVoltage;
    
    // Read current if sensor connected
    if (_currentPin >= 0)
    {
        int rawCurrent = analogRead(_currentPin);
        _lastCurrent = (rawCurrent / 1023.0) * 5.0;  // ACS712 outputs ~2.5V at 0A
        
        // For ACS712-5A: 185mV per Amp
        _lastCurrent = (_lastCurrent - 2.5) / 0.185;
        
        // Integrate energy: E = V * I * t
        if (deltaTime > 0 && _lastCurrent > 0)
        {
            float powerWatts = _lastVoltage * _lastCurrent;
            float energyWh = (powerWatts * deltaTime) / (1000.0 * 3600.0);
            _totalEnergyWh += energyWh;
        }
    }
    
    _lastUpdateTime = now;
}

float PowerManager::getVoltage()
{
    return _lastVoltage;
}

uint8_t PowerManager::getBatteryPercent()
{
    // Simple linear model: assume 12V = 100%, 9V = 0%
    // For LiPo 3S (11.1V nominal): adjust accordingly
    float percent = ((getVoltage() - 9.0) / 3.0) * 100.0;
    if (percent > 100) percent = 100;
    if (percent < 0) percent = 0;
    return (uint8_t)percent;
}

float PowerManager::getCurrentAmps()
{
    return _lastCurrent;
}

bool PowerManager::isLowBattery()
{
    return getVoltage() < _lowBatteryThreshold;
}

void PowerManager::setLowBatteryThreshold(float voltageThreshold)
{
    _lowBatteryThreshold = voltageThreshold;
}

int PowerManager::getEstimatedRuntimeMinutes()
{
    if (_lastCurrent <= 0) return -1;  // Not consuming power or no current sensor
    
    float remainingCapacityMAh = (getBatteryPercent() / 100.0) * _batteryCapacityMAh;
    float runtimeMinutes = (remainingCapacityMAh / (_lastCurrent * 1000.0)) * 60.0;
    
    return (int)runtimeMinutes;
}

float PowerManager::getEnergyConsumedWh()
{
    return _totalEnergyWh;
}

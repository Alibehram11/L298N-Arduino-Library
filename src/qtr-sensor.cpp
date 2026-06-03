/*
 * ═════════════════════════════════════════════════════════════════════════════
 * QTR Line Following Sensor - Implementation
 * ═════════════════════════════════════════════════════════════════════════════
 */

#include "qtr-sensor.h"

QTRSensor::QTRSensor(uint8_t numSensors, uint8_t analogPin)
    : _numSensors(numSensors), _analogPin(analogPin), _calibrated(false)
{
    _sensorValues = new uint16_t[numSensors];
    _whiteCalibration = new uint16_t[numSensors];
    _blackCalibration = new uint16_t[numSensors];

    // Initialize with default values
    for (uint8_t i = 0; i < numSensors; i++) {
        _sensorValues[i] = 512;
        _whiteCalibration[i] = 1000;
        _blackCalibration[i] = 0;
    }
}

QTRSensor::~QTRSensor()
{
    if (_sensorValues) delete[] _sensorValues;
    if (_whiteCalibration) delete[] _whiteCalibration;
    if (_blackCalibration) delete[] _blackCalibration;
}

void QTRSensor::calibrate()
{
    // Reset calibration values
    for (uint8_t i = 0; i < _numSensors; i++) {
        _whiteCalibration[i] = 0;
        _blackCalibration[i] = 1023;
    }

    // Collect samples during 2-second calibration
    unsigned long endTime = millis() + 2000;
    while (millis() < endTime) {
        for (uint8_t i = 0; i < _numSensors; i++) {
            uint16_t val = analogRead(_analogPin + i);

            if (val > _whiteCalibration[i])
                _whiteCalibration[i] = val;
            if (val < _blackCalibration[i])
                _blackCalibration[i] = val;
        }
    }

    _calibrated = true;
}

uint8_t QTRSensor::readSensors(uint16_t* sensorValues, uint8_t numSensors)
{
    uint8_t count = min(numSensors, _numSensors);
    for (uint8_t i = 0; i < count; i++) {
        _sensorValues[i] = analogRead(_analogPin + i);
        sensorValues[i] = _sensorValues[i];
    }
    return count;
}

uint16_t QTRSensor::readLinePosition()
{
    // Read all sensors
    readSensors(_sensorValues, _numSensors);

    // Calculate weighted position
    uint32_t weightedSum = 0;
    uint32_t sensorSum = 0;

    for (uint8_t i = 0; i < _numSensors; i++) {
        uint16_t normalized = normalizeValue(_sensorValues[i], i);
        weightedSum += (uint32_t)normalized * i * 1000 / _numSensors;
        sensorSum += normalized;
    }

    if (sensorSum == 0) {
        return 1500;  // No line detected, return center
    }

    return (uint16_t)(weightedSum / sensorSum);
}

uint16_t QTRSensor::readLinePosWithConfidence(uint8_t& outConfidence)
{
    readSensors(_sensorValues, _numSensors);

    // Calculate position
    uint32_t weightedSum = 0;
    uint32_t sensorSum = 0;

    for (uint8_t i = 0; i < _numSensors; i++) {
        uint16_t normalized = normalizeValue(_sensorValues[i], i);
        weightedSum += (uint32_t)normalized * i * 1000 / _numSensors;
        sensorSum += normalized;
    }

    // Calculate confidence (0-100%)
    if (sensorSum == 0) {
        outConfidence = 0;
        return 1500;
    }

    outConfidence = min(100, (uint8_t)(sensorSum / 10));
    return (uint16_t)(weightedSum / sensorSum);
}

bool QTRSensor::isLineDetected()
{
    readSensors(_sensorValues, _numSensors);

    // Check if any sensor detects strong signal
    for (uint8_t i = 0; i < _numSensors; i++) {
        uint16_t normalized = normalizeValue(_sensorValues[i], i);
        if (normalized > 500) {
            return true;
        }
    }
    return false;
}

void QTRSensor::setCalibrationValues(uint16_t* whiteValues, uint16_t* blackValues)
{
    for (uint8_t i = 0; i < _numSensors; i++) {
        _whiteCalibration[i] = whiteValues[i];
        _blackCalibration[i] = blackValues[i];
    }
    _calibrated = true;
}

uint8_t QTRSensor::getLastSensorValues(uint16_t* outValues)
{
    for (uint8_t i = 0; i < _numSensors; i++) {
        outValues[i] = _sensorValues[i];
    }
    return _numSensors;
}

bool QTRSensor::isCalibrated() const
{
    return _calibrated;
}

void QTRSensor::resetCalibration()
{
    for (uint8_t i = 0; i < _numSensors; i++) {
        _whiteCalibration[i] = 1000;
        _blackCalibration[i] = 0;
    }
    _calibrated = false;
}

uint16_t QTRSensor::normalizeValue(uint16_t rawValue, uint8_t sensorIndex)
{
    if (sensorIndex >= _numSensors) {
        return 512;
    }

    uint16_t white = _whiteCalibration[sensorIndex];
    uint16_t black = _blackCalibration[sensorIndex];

    if (white <= black) {
        return 512;  // Invalid calibration
    }

    // Map raw value to 0-1000 range
    uint16_t normalized = (rawValue - black) * 1000 / (white - black);
    if (normalized > 1000) normalized = 1000;

    return normalized;
}

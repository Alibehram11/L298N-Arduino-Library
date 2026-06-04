/*
 * ═════════════════════════════════════════════════════════════════════════════
 * QTR Line Following Sensor Array - Header
 * ═════════════════════════════════════════════════════════════════════════════
 * Support for Pololu QTR sensor arrays (reflectance sensors) for line tracking
 * and edge detection in autonomous robotics.
 *
 * Supported Modules:
 *   - QTR-8x (8 sensors)
 *   - QTR-8RC (8 RC sensors)
 *   - QTR-8A (8 analog sensors)
 *   - Custom arrays with 1-16 sensors
 *
 * Features:
 *   ✓ Read sensor array position/line location
 *   ✓ Calibration for surface variations
 *   ✓ Weighted position calculation
 *   ✓ Raw sensor value access
 *   ✓ Automatic/manual calibration modes
 *
 * Author: Arduino Car Library Contributors
 * License: MIT
 * ═════════════════════════════════════════════════════════════════════════════
 */

#ifndef QTR_SENSOR_H
#define QTR_SENSOR_H

#include <Arduino.h>

/**
 * QTRSensor Class - Line Following via Reflectance Sensors
 *
 * Typical Usage:
 *   QTRSensor lineSensor(8, A0);           // 8 analog sensors starting at A0
 *   lineSensor.calibrate();
 *
 *   while (running) {
 *       uint16_t linePosition = lineSensor.readLinePosition();
 *       if (linePosition < 2000) turnLeft();    // Line is left
 *       else if (linePosition > 2000) turnRight();  // Line is right
 *   }
 */
class QTRSensor {
public:
    /**
     * Constructor - Initialize QTR sensor array
     *
     * @param numSensors - Number of sensors in array (1-16)
     * @param analogPin - Starting analog pin for sensor readings
     *                    (sensors use consecutive pins: A0, A1, A2, etc.)
     *
     * Sensor readings:
     *   - High value (800-1000): White/reflective surface
     *   - Low value (0-200): Black line/absorbent surface
     */
    QTRSensor(uint8_t numSensors, uint8_t analogPin);

    /**
     * Destructor
     */
    ~QTRSensor();

    /**
     * Calibrate sensors for current surface
     *
     * Move sensors over white and black surfaces for 2 seconds each.
     * Stores min/max values for normalization.
     *
     * Simple calibration approach:
     *   lineSensor.calibrate();
     *   // Then manually move sensor across white and black areas
     */
    void calibrate();

    /**
     * Read raw sensor values
     *
     * @param sensorValues - Output array to store raw values (0-1023)
     * @param numSensors - Number of values to read
     * @return Number of sensors read
     */
    uint8_t readSensors(uint16_t* sensorValues, uint8_t numSensors);

    /**
     * Read weighted line position
     *
     * @return Position estimate (0 = leftmost, 7000 = rightmost for 8 sensors)
     *
     * Calculation:
     *   - Black line at left edge: ~0
     *   - Black line at center: ~3500 (for 8 sensors)
     *   - Black line at right edge: ~7000
     *
     * Use to detect line offset and adjust motor speeds accordingly.
     */
    uint16_t readLinePosition();

    /**
     * Read line position with confidence
     *
     * @param outConfidence - Output: Confidence value (0-100%)
     * @return Line position (same as readLinePosition)
     *
     * Confidence indicates how clear the line is:
     *   - 100% = Strong contrast between line and background
     *   - < 50% = Weak signal, may be off-line or poor contrast
     */
    uint16_t readLinePosWithConfidence(uint8_t& outConfidence);

    /**
     * Check if line is detected
     *
     * @return true if strong line detected, false otherwise
     */
    bool isLineDetected();

    /**
     * Set calibration values manually
     *
     * @param whiteValues - Sensor readings on white surface
     * @param blackValues - Sensor readings on black surface
     *
     * For precise tuning without on-site calibration
     */
    void setCalibrationValues(uint16_t* whiteValues, uint16_t* blackValues);

    /**
     * Get last read sensor values
     *
     * @param outValues - Array to receive last raw values
     * @return Number of values copied
     */
    uint8_t getLastSensorValues(uint16_t* outValues);

    /**
     * Get calibration status
     *
     * @return true if calibrated, false if raw reads
     */
    bool isCalibrated() const;

    /**
     * Reset calibration data
     */
    void resetCalibration();

private:
    uint8_t _numSensors;
    uint8_t _analogPin;
    uint16_t* _sensorValues;
    uint16_t* _whiteCalibration;
    uint16_t* _blackCalibration;
    bool _calibrated;

    /**
     * Internal: Normalize sensor value based on calibration
     */
    uint16_t normalizeValue(uint16_t rawValue, uint8_t sensorIndex);
};

#endif // QTR_SENSOR_H

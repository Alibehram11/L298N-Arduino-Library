/*
 * ═════════════════════════════════════════════════════════════════════════════
 * HC-SR04 Ultrasonic Distance Sensor Library - Enhanced Header
 * ═════════════════════════════════════════════════════════════════════════════
 * Advanced driver for HC-SR04 ultrasonic sensor with noise filtering,
 * non-blocking measurement, and multi-sample averaging.
 *
 * Features:
 *   ✓ Noise filtering with configurable threshold
 *   ✓ Multi-sample averaging for stable readings
 *   ✓ Non-blocking (asynchronous) measurement capability
 *   ✓ Signal quality metrics
 *   ✓ Measurement statistics (min/max/average)
 *   ✓ Safe distance range validation
 *
 * Author: Arduino Car Library Contributors
 * License: MIT
 * ═════════════════════════════════════════════════════════════════════════════
 */

#ifndef HCSR04_H
#define HCSR04_H

#include <Arduino.h>

/**
 * HCSR04 Class - Advanced HC-SR04 Ultrasonic Distance Sensor Interface
 *
 * This class provides professional-grade ultrasonic distance measurement
 * with noise filtering, averaging, and non-blocking operations.
 *
 * Typical ultrasonic specifications:
 *   - Speed of sound: ~343 m/s at 20°C
 *   - Measurement range: 2 cm - 400 cm
 *   - Accuracy: ±1 cm typically
 *   - Frequency: 40 kHz
 */
class HCSR04 {
public:
    /**
     * Constructor - Initialize HCSR04 with trigger and echo pins
     *
     * @param trigPin - Arduino pin connected to HC-SR04 TRIG (trigger pin)
     * @param echoPin - Arduino pin connected to HC-SR04 ECHO (echo pin)
     *
     * Example:
     *   HCSR04 sensor(9, 10); // TRIG on pin 9, ECHO on pin 10
     */
    HCSR04(int trigPin, int echoPin);

    /**
     * destructor - Clean up sensor resources
     *
     * Properly releases any allocated buffers or resources
     */
    ~HCSR04();

    /*────────────────────────────────────────────────────────────────────────
     * BLOCKING MEASUREMENT METHODS
     * Synchronous methods that wait for measurement completion
     * ────────────────────────────────────────────────────────────────────────*/

    /**
     * Measure distance to an object in centimeters (single measurement)
     *
     * @return Distance in centimeters, or -1 if invalid
     *
     * This method sends a trigger pulse and measures the echo duration,
     * converting the time to distance based on speed of sound (343 m/s).
     */
    long measureDistance();

    /**
     * Measure distance with multiple samples and averaging
     *
     * @param numSamples - Number of measurements to average (default: 5)
     * @return Averaged distance in cm, or -1 if measurement fails
     *
     * Takes multiple measurements and returns the filtered average,
     * rejecting outliers beyond the noise threshold.
     */
    long measureDistanceAveraged(uint8_t numSamples = 5);

    /*────────────────────────────────────────────────────────────────────────
     * NON-BLOCKING MEASUREMENT METHODS
     * For efficient, non-blocking measurement acquisition
     * ────────────────────────────────────────────────────────────────────────*/

    /**
     * Start an asynchronous measurement (non-blocking)
     *
     * Call this, then later call isReadyForMeasurement() and readDistance().
     * Allows other code to run while sensor is measuring.
     */
    void startAsyncMeasurement();

    /**
     * Check if an asynchronous measurement has completed
     *
     * @return true if measurement is ready to read, false otherwise
     *
     * Use in loop() to poll for completion without blocking.
     */
    bool isAsyncMeasurementReady();

    /**
     * Read result from an asynchronous measurement
     *
     * @return Distance in cm, or -1 if invalid
     *
     * Only call after isAsyncMeasurementReady() returns true.
     */
    long readAsyncDistance();

    /*────────────────────────────────────────────────────────────────────────
     * CONFIGURATION METHODS
     * Configure sensor behavior and filtering parameters
     * ────────────────────────────────────────────────────────────────────────*/

    /**
     * Set the noise threshold for filtering outliers
     *
     * @param thresholdCM - Maximum deviation from median (default: 5 cm)
     *
     * Measurements deviating more than this from the median are rejected.
     * Useful for filtering electrical noise or reflections.
     */
    void setNoiseThreshold(long thresholdCM = 5);

    /**
     * Set the valid measurement range
     *
     * @param minCM - Minimum valid distance (default: 2 cm, sensor minimum)
     * @param maxCM - Maximum valid distance (default: 400 cm, sensor maximum)
     *
     * Measurements outside this range are considered invalid.
     */
    void setValidRange(long minCM = 2, long maxCM = 400);

    /**
     * Set timeout for echo measurement
     *
     * @param timeoutMicros - Echo timeout in microseconds (default: 30000)
     *
     * Longer timeout allows measuring greater distances but uses more CPU.
     * Max distance ≈ timeout * 0.0343 / 2 centimeters
     */
    void setEchoTimeout(unsigned long timeoutMicros = 30000);

    /**
     * Enable/disable noise filtering
     *
     * @param enable - true to enable filtering, false to disable
     */
    void setNoiseFilteringEnabled(bool enable = true);

    /*────────────────────────────────────────────────────────────────────────
     * DIAGNOSTIC METHODS
     * Query sensor status and measurement statistics
     * ────────────────────────────────────────────────────────────────────────*/

    /**
     * Get the last raw echo duration in microseconds
     *
     * @return Last measured echo duration (us), or 0 if no measurement
     */
    unsigned long getLastEchoDuration();

    /**
     * Get measurement statistics (min/max/avg from last averaged read)
     *
     * @param outMin - Output: minimum value from last batch
     * @param outMax - Output: maximum value from last batch
     * @param outAvg - Output: average value from last batch
     * @return Number of valid samples in last batch
     */
    uint8_t getMeasurementStats(long& outMin, long& outMax, long& outAvg);

    /**
     * Get the number of valid measurements in the last batch
     *
     * @return Number of measurements that passed filtering
     */
    uint8_t getValidSampleCount();

    /**
     * Get signal quality metric (0-100%)
     *
     * @return Percentage of valid samples in last measurement batch
     *
     * 100% = all samples were valid
     * < 100% = some samples were filtered as noise/outliers
     */
    uint8_t getSignalQuality();

    /*────────────────────────────────────────────────────────────────────────
     * DEBUG METHODS
     * Utilities for troubleshooting and testing
     * ────────────────────────────────────────────────────────────────────────*/

    /**
     * Get debug information string about the sensor state
     *
     * @return String with sensor configuration and last measurement info
     */
    String getDebugInfo();

private:
    int _trigPin;                        // Trigger pin number
    int _echoPin;                        // Echo pin number

    // Configuration parameters
    long _noiseThresholdCM;              // Noise filter threshold in cm
    long _minValidDistanceCM;            // Minimum valid measurement range
    long _maxValidDistanceCM;            // Maximum valid measurement range
    unsigned long _echoTimeoutMicros;    // Echo measurement timeout
    bool _filteringEnabled;              // Noise filtering enable flag

    // Measurement state for async operations
    unsigned long _asyncStartTime;       // Time async measurement started
    long _lastAsyncDistance;             // Last async measurement result
    bool _asyncMeasurementInProgress;    // Async measurement active flag

    // Statistics from last measurement batch
    unsigned long _lastEchoDuration;     // Last raw echo duration (us)
    long _lastMinDistance;               // Minimum from last batch
    long _lastMaxDistance;               // Maximum from last batch
    long _lastAvgDistance;               // Average from last batch
    uint8_t _validSampleCount;           // Valid samples in last batch
    uint8_t _totalSampleCount;           // Total samples attempted

    /*────────────────────────────────────────────────────────────────────────
     * PRIVATE HELPER METHODS
     * Internal methods used for measurement processing
     * ────────────────────────────────────────────────────────────────────────*/

    /**
     * Perform raw distance measurement
     *
     * @return Raw distance in cm, or -1 if measurement invalid
     */
    long getRawDistance();

    /**
     * Check if a measurement value is valid
     *
     * @param distance - Distance value to validate
     * @return true if distance is within valid range
     */
    bool isValidDistance(long distance);

    /**
     * Sort array using insertion sort (for median calculation)
     *
     * @param arr - Array to sort
     * @param size - Number of elements
     */
    void sortArray(long* arr, uint8_t size);

    /**
     * Calculate median value from array
     *
     * @param arr - Array of values
     * @param size - Number of elements
     * @return Median value
     */
    long calculateMedian(long* arr, uint8_t size);

    /**
     * Filter outliers from measurements using median deviation
     *
     * @param samples - Array of measurements
     * @param numSamples - Number of measurements
     * @param outValidCount - Output: number of valid samples after filtering
     * @return Average of valid (non-filtered) samples
     */
    long filterOutliers(long* samples, uint8_t numSamples, uint8_t& outValidCount);
};

#endif // HCSR04_H
 *   Serial.println("Distance: " + String(distance) + " cm");
 * 
 * License: MIT
 * ============================================================================
 */

 
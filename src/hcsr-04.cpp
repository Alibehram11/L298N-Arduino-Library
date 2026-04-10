/*
 * ═════════════════════════════════════════════════════════════════════════════
 * HC-SR04 Ultrasonic Distance Sensor Library - Implementation
 * ═════════════════════════════════════════════════════════════════════════════
 */

#include "hcsr-04.h"

/**
 * CONSTRUCTOR: HCSR04::HCSR04(int trigPin, int echoPin)
 * ─────────────────────────────────────────────────────────────
 * Initializes the HCSR04 sensor with default configuration
 */
HCSR04::HCSR04(int trigPin, int echoPin)
    : _trigPin(trigPin),
      _echoPin(echoPin),
      _noiseThresholdCM(5),
      _minValidDistanceCM(2),
      _maxValidDistanceCM(400),
      _echoTimeoutMicros(30000),
      _filteringEnabled(true),
      _asyncStartTime(0),
      _lastAsyncDistance(-1),
      _asyncMeasurementInProgress(false),
      _lastEchoDuration(0),
      _lastMinDistance(0),
      _lastMaxDistance(0),
      _lastAvgDistance(0),
      _validSampleCount(0),
      _totalSampleCount(0)
{
    pinMode(_trigPin, OUTPUT);
    pinMode(_echoPin, INPUT);
}

/**
 * DESTRUCTOR: HCSR04::~HCSR04()
 * ────────────────────────────────────────
 * Cleanup (for potential future dynamic allocations)
 */
HCSR04::~HCSR04()
{
    // Nothing to clean up yet, but good practice for future expansion
}

/*═════════════════════════════════════════════════════════════════════════════
 * BLOCKING MEASUREMENT METHODS
 * ═════════════════════════════════════════════════════════════════════════════*/

/**
 * METHOD: long HCSR04::getRawDistance()
 * ──────────────────────────────────────
 * Internal: Performs a single raw measurement
 */
long HCSR04::getRawDistance()
{
    // Send 10µs trigger pulse
    digitalWrite(_trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(_trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(_trigPin, LOW);

    // Measure echo duration
    unsigned long duration = pulseIn(_echoPin, HIGH, _echoTimeoutMicros);

    // Store last echo duration for diagnostics
    _lastEchoDuration = duration;

    if (duration == 0 || duration == _echoTimeoutMicros)
    {
        return -1;
    }

    // Convert time to distance: distance = (duration / 2) / 29.1 cm
    // 29.1 microseconds per centimeter at 343 m/s speed of sound
    long distance = (duration / 2) / 29.1;

    return distance;
}

/**
 * METHOD: bool HCSR04::isValidDistance(long distance)
 * ─────────────────────────────────────────────────────
 * Internal: Validate distance against configured range
 */
bool HCSR04::isValidDistance(long distance)
{
    if (distance < 0)
    {
        return false;
    }

    if (distance < _minValidDistanceCM || distance > _maxValidDistanceCM)
    {
        return false;
    }

    return true;
}

/**
 * METHOD: long HCSR04::measureDistance()
 * ─────────────────────────────────────────
 * Single measurement with basic validation
 */
long HCSR04::measureDistance()
{
    long distance = getRawDistance();

    if (!isValidDistance(distance))
    {
        return -1;
    }

    _lastAvgDistance = distance;
    _validSampleCount = 1;
    _totalSampleCount = 1;

    return distance;
}

/**
 * METHOD: void HCSR04::sortArray(long* arr, uint8_t size)
 * ────────────────────────────────────────────────────────
 * Internal: Simple insertion sort for small arrays
 */
void HCSR04::sortArray(long* arr, uint8_t size)
{
    for (uint8_t i = 1; i < size; i++)
    {
        long key = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

/**
 * METHOD: long HCSR04::calculateMedian(long* arr, uint8_t size)
 * ──────────────────────────────────────────────────────────────
 * Internal: Calculate median value (requires sorted array)
 */
long HCSR04::calculateMedian(long* arr, uint8_t size)
{
    if (size % 2 == 1)
    {
        // Odd number of elements
        return arr[size / 2];
    }
    else
    {
        // Even number of elements - average middle two
        return (arr[size / 2 - 1] + arr[size / 2]) / 2;
    }
}

/**
 * METHOD: long HCSR04::filterOutliers(long* samples, uint8_t numSamples, uint8_t& outValidCount)
 * ───────────────────────────────────────────────────────────────────────────────────────────────
 * Internal: Filter measurements using median absolute deviation (MAD) method
 * Rejects measurements that deviate more than threshold from median
 */
long HCSR04::filterOutliers(long* samples, uint8_t numSamples, uint8_t& outValidCount)
{
    // If filtering disabled, average all samples
    if (!_filteringEnabled)
    {
        long sum = 0;
        for (uint8_t i = 0; i < numSamples; i++)
        {
            sum += samples[i];
        }
        outValidCount = numSamples;
        return sum / numSamples;
    }

    // Make a copy for sorting
    long* sortedCopy = new long[numSamples];
    for (uint8_t i = 0; i < numSamples; i++)
    {
        sortedCopy[i] = samples[i];
    }

    // Sort to find median
    sortArray(sortedCopy, numSamples);
    long median = calculateMedian(sortedCopy, numSamples);

    // Calculate deviations and filter
    long sum = 0;
    outValidCount = 0;

    for (uint8_t i = 0; i < numSamples; i++)
    {
        long deviation = samples[i] > median ? samples[i] - median : median - samples[i];

        if (deviation <= _noiseThresholdCM)
        {
            sum += samples[i];
            outValidCount++;
        }
    }

    delete[] sortedCopy;

    if (outValidCount == 0)
    {
        return -1;
    }

    return sum / outValidCount;
}

/**
 * METHOD: long HCSR04::measureDistanceAveraged(uint8_t numSamples)
 * ──────────────────────────────────────────────────────────────────
 * Measure with multiple samples and filtering
 * Returns averaged filtered distance
 */
long HCSR04::measureDistanceAveraged(uint8_t numSamples)
{
    if (numSamples == 0 || numSamples > 50)
    {
        numSamples = 5;  // Safe default
    }

    // Allocate temporary buffer for samples
    long* samples = new long[numSamples];
    long minDist = LONG_MAX;
    long maxDist = LONG_MIN;
    uint8_t validCount = 0;

    // Collect samples
    for (uint8_t i = 0; i < numSamples; i++)
    {
        long dist = getRawDistance();

        if (isValidDistance(dist))
        {
            samples[validCount] = dist;
            if (dist < minDist)
                minDist = dist;
            if (dist > maxDist)
                maxDist = dist;
            validCount++;
        }

        // Small delay between measurements
        delayMicroseconds(100);
    }

    _lastMinDistance = minDist;
    _lastMaxDistance = maxDist;
    _totalSampleCount = numSamples;

    // Filter outliers and calculate average
    long avgDistance = filterOutliers(samples, validCount, _validSampleCount);

    _lastAvgDistance = avgDistance;

    delete[] samples;

    return avgDistance;
}

/*═════════════════════════════════════════════════════════════════════════════
 * NON-BLOCKING MEASUREMENT METHODS
 * ═════════════════════════════════════════════════════════════════════════════*/

/**
 * METHOD: void HCSR04::startAsyncMeasurement()
 * ──────────────────────────────────────────────
 * Start a non-blocking measurement
 */
void HCSR04::startAsyncMeasurement()
{
    // Send trigger pulse
    digitalWrite(_trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(_trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(_trigPin, LOW);

    _asyncStartTime = micros();
    _asyncMeasurementInProgress = true;
}

/**
 * METHOD: bool HCSR04::isAsyncMeasurementReady()
 * ────────────────────────────────────────────────
 * Check if async measurement has completed
 * Uses digitalRead polling for timeout tolerance
 */
bool HCSR04::isAsyncMeasurementReady()
{
    if (!_asyncMeasurementInProgress)
    {
        return false;
    }

    // Check if echo pin has gone low (measurement complete)
    if (digitalRead(_echoPin) == LOW)
    {
        _asyncMeasurementInProgress = false;
        return true;
    }

    // Timeout check
    if ((micros() - _asyncStartTime) > _echoTimeoutMicros)
    {
        _asyncMeasurementInProgress = false;
        _lastAsyncDistance = -1;
        return true;
    }

    return false;
}

/**
 * METHOD: long HCSR04::readAsyncDistance()
 * ─────────────────────────────────────────
 * Read result from async measurement
 */
long HCSR04::readAsyncDistance()
{
    // Measure echo duration from where it should be high
    unsigned long duration = pulseIn(_echoPin, HIGH, _echoTimeoutMicros);
    _lastEchoDuration = duration;

    if (duration == 0 || duration == _echoTimeoutMicros)
    {
        _lastAsyncDistance = -1;
        return -1;
    }

    long distance = (duration / 2) / 29.1;

    if (!isValidDistance(distance))
    {
        _lastAsyncDistance = -1;
        return -1;
    }

    _lastAsyncDistance = distance;
    return distance;
}

/*═════════════════════════════════════════════════════════════════════════════
 * CONFIGURATION METHODS
 * ═════════════════════════════════════════════════════════════════════════════*/

/**
 * METHOD: void HCSR04::setNoiseThreshold(long thresholdCM)
 * ─────────────────────────────────────────────────────────
 * Configure noise filter threshold
 */
void HCSR04::setNoiseThreshold(long thresholdCM)
{
    if (thresholdCM >= 0)
    {
        _noiseThresholdCM = thresholdCM;
    }
}

/**
 * METHOD: void HCSR04::setValidRange(long minCM, long maxCM)
 * ──────────────────────────────────────────────────────────
 * Set acceptable measurement range
 */
void HCSR04::setValidRange(long minCM, long maxCM)
{
    if (minCM >= 0 && maxCM > minCM)
    {
        _minValidDistanceCM = minCM;
        _maxValidDistanceCM = maxCM;
    }
}

/**
 * METHOD: void HCSR04::setEchoTimeout(unsigned long timeoutMicros)
 * ────────────────────────────────────────────────────────────────
 * Set echo measurement timeout
 */
void HCSR04::setEchoTimeout(unsigned long timeoutMicros)
{
    if (timeoutMicros >= 10000 && timeoutMicros <= 100000)
    {
        _echoTimeoutMicros = timeoutMicros;
    }
}

/**
 * METHOD: void HCSR04::setNoiseFilteringEnabled(bool enable)
 * ──────────────────────────────────────────────────────────
 * Enable or disable outlier filtering
 */
void HCSR04::setNoiseFilteringEnabled(bool enable)
{
    _filteringEnabled = enable;
}

/*═════════════════════════════════════════════════════════════════════════════
 * DIAGNOSTIC METHODS
 * ═════════════════════════════════════════════════════════════════════════════*/

/**
 * METHOD: unsigned long HCSR04::getLastEchoDuration()
 * ────────────────────────────────────────────────────
 * Get raw echo duration from last measurement
 */
unsigned long HCSR04::getLastEchoDuration()
{
    return _lastEchoDuration;
}

/**
 * METHOD: uint8_t HCSR04::getMeasurementStats(long& outMin, long& outMax, long& outAvg)
 * ──────────────────────────────────────────────────────────────────────────────────────
 * Get statistics from last averaged measurement batch
 */
uint8_t HCSR04::getMeasurementStats(long& outMin, long& outMax, long& outAvg)
{
    outMin = _lastMinDistance;
    outMax = _lastMaxDistance;
    outAvg = _lastAvgDistance;
    return _validSampleCount;
}

/**
 * METHOD: uint8_t HCSR04::getValidSampleCount()
 * ──────────────────────────────────────────────
 * Get number of valid samples from last batch
 */
uint8_t HCSR04::getValidSampleCount()
{
    return _validSampleCount;
}

/**
 * METHOD: uint8_t HCSR04::getSignalQuality()
 * ──────────────────────────────────────────
 * Get signal quality as percentage (0-100%)
 */
uint8_t HCSR04::getSignalQuality()
{
    if (_totalSampleCount == 0)
    {
        return 0;
    }

    return (uint8_t)((_validSampleCount * 100) / _totalSampleCount);
}

/**
 * METHOD: String HCSR04::getDebugInfo()
 * ──────────────────────────────────────
 * Get detailed debug information string
 */
String HCSR04::getDebugInfo()
{
    String info = "=== HC-SR04 Debug Info ===\n";
    info += "Trigger Pin: " + String(_trigPin) + "\n";
    info += "Echo Pin: " + String(_echoPin) + "\n";
    info += "Last Echo Duration: " + String(_lastEchoDuration) + " us\n";
    info += "Last Distance: " + String(_lastAvgDistance) + " cm\n";
    info += "Valid Samples: " + String(_validSampleCount) + "/" + String(_totalSampleCount) + "\n";
    info += "Signal Quality: " + String(getSignalQuality()) + "%\n";
    info += "Min/Max/Avg: " + String(_lastMinDistance) + "/" + String(_lastMaxDistance) + "/" + String(_lastAvgDistance) + " cm\n";
    info += "Range: " + String(_minValidDistanceCM) + "-" + String(_maxValidDistanceCM) + " cm\n";
    info += "Filtering: " + String(_filteringEnabled ? "ENABLED" : "DISABLED") + "\n";
    info += "Threshold: " + String(_noiseThresholdCM) + " cm\n";

    return info;
}



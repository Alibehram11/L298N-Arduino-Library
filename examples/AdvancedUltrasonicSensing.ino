/*
 * ═════════════════════════════════════════════════════════════════════════════
 * EXAMPLE 2: AdvancedUltrasonicSensing.ino
 * HC-SR04 Advanced Distance Measurement with Filtering & Diagnostics
 * ═════════════════════════════════════════════════════════════════════════════
 *
 * OVERVIEW:
 *   This example demonstrates advanced HC-SR04 ultrasonic sensor capabilities:
 *   - Real-time distance measurement with noise filtering
 *   - Multi-sample averaging for stable readings
 *   - Signal quality metrics and statistics
 *   - Non-blocking (asynchronous) measurement mode
 *   - Configuration tuning for different environments
 *
 * PREREQUISITES:
 *   ✓ Arduino Uno/Mega/Nano microcontroller
 *   ✓ HC-SR04 Ultrasonic Sensor Module
 *   ✓ Jumper wires and breadboard
 *   ✓ Optional: LED or buzzer for range feedback
 *
 * PIN CONFIGURATION:
 *   Arduino Pin 9  ──→ HC-SR04 TRIG (signal input)
 *   Arduino Pin 10 ──→ HC-SR04 ECHO (signal output)
 *   Arduino GND ────→ HC-SR04 GND
 *   Arduino 5V  ────→ HC-SR04 VCC
 *
 * TYPICAL USE CASES:
 *   ✓ Obstacle avoidance for robots
 *   ✓ Parking distance warning systems
 *   ✓ Level/proximity monitoring
 *   ✓ Motion/presence detection
 *   ✓ Autonomous vehicle navigation
 *
 * SERIAL OUTPUT:
 *   Opens serial monitor at 9600 baud to display:
 *   - Current distance measurements
 *   - Signal quality metrics
 *   - Measurement statistics (min/max/average)
 *   - Raw echo duration (microseconds)
 *   - Diagnostic information and debug data
 *
 * ═════════════════════════════════════════════════════════════════════════════
 */

#include <arduino/arduino.h>

// ───────────────────────────────────────────────────────────────────────────────
// SENSOR CONFIGURATION
// ───────────────────────────────────────────────────────────────────────────────

// Create HC-SR04 sensor object
// Parameters: (TRIG_PIN, ECHO_PIN)
HCSR04 sensor(9, 10);

// ───────────────────────────────────────────────────────────────────────────────
// APPLICATION PARAMETERS
// ───────────────────────────────────────────────────────────────────────────────

// Measurement modes
enum MeasurementMode {
    MODE_SINGLE,      // Single measurement per cycle
    MODE_AVERAGED,    // Multi-sample averaging (5 samples)
    MODE_ASYNC,       // Non-blocking asynchronous mode
    MODE_CONTINUOUS   // Continuous streaming
};

MeasurementMode currentMode = MODE_AVERAGED;

// Averaging parameters
const uint8_t SAMPLES_PER_BATCH = 5;  // Number of samples to average

// Measurement timing
unsigned long lastMeasurementTime = 0;
const unsigned long MEASUREMENT_INTERVAL = 500;  // Milliseconds between measurements

// Async state machine
bool asyncMeasurementPending = false;
unsigned long asyncInitTime = 0;

// Statistics tracking
struct Statistics {
    long minDistance = LONG_MAX;
    long maxDistance = LONG_MIN;
    long totalDistance = 0;
    unsigned int measurementCount = 0;
    unsigned int validCount = 0;
    unsigned int invalidCount = 0;
} stats;

// ───────────────────────────────────────────────────────────────────────────────
// SETUP()
// ───────────────────────────────────────────────────────────────────────────────

void setup()
{
    // Initialize serial communication (9600 baud standard for Arduino)
    Serial.begin(9600);

    // Wait for serial connection to establish
    delay(500);

    // Print startup banner
    Serial.println("╔════════════════════════════════════════════════╗");
    Serial.println("║  HC-SR04 Advanced Ultrasonic Sensor Example   ║");
    Serial.println("║         Distance Measurement System            ║");
    Serial.println("╚════════════════════════════════════════════════╝\n");

    // Configure sensor parameters
    configureMainSensor();

    // Print initial configuration
    printSensorConfiguration();

    Serial.println("\nStarting measurements...\n");
}

// ───────────────────────────────────────────────────────────────────────────────
// LOOP()
// ───────────────────────────────────────────────────────────────────────────────

void loop()
{
    // Check if it's time for the next measurement
    unsigned long currentTime = millis();

    if (currentTime - lastMeasurementTime >= MEASUREMENT_INTERVAL)
    {
        lastMeasurementTime = currentTime;

        // Run active measurement mode
        switch (currentMode)
        {
        case MODE_SINGLE:
            performSingleMeasurement();
            break;

        case MODE_AVERAGED:
            performAveragedMeasurement();
            break;

        case MODE_ASYNC:
            performAsyncMeasurement();
            break;

        case MODE_CONTINUOUS:
            performContinuousMeasurement();
            break;
        }
    }

    // Handle async mode polling
    if (currentMode == MODE_ASYNC && asyncMeasurementPending)
    {
        pollAsyncMeasurement();
    }

    // Optional: Check for serial commands
    if (Serial.available())
    {
        handleSerialCommand();
    }
}

// ───────────────────────────────────────────────────────────────────────────────
// MEASUREMENT FUNCTIONS
// ───────────────────────────────────────────────────────────────────────────────

/**
 * Perform a single measurement (most basic mode)
 * Fastest response but more susceptible to noise
 */
void performSingleMeasurement()
{
    Serial.println("┌─ SINGLE MEASUREMENT ─────────────────┐");

    long distance = sensor.measureDistance();

    if (distance > 0)
    {
        Serial.print("Distance: ");
        Serial.print(distance);
        Serial.println(" cm");

        updateStatistics(distance);
        printMeasurementStats();

        stats.validCount++;
    }
    else
    {
        Serial.println("⚠ Invalid measurement (out of range or error)");
        stats.invalidCount++;
    }

    // Additional diagnostics
    unsigned long echoDuration = sensor.getLastEchoDuration();
    Serial.print("Echo Duration: ");
    Serial.print(echoDuration);
    Serial.println(" µs");

    Serial.println("└──────────────────────────────────────┘\n");
}

/**
 * Perform multi-sample averaged measurement
 * Recommended for most applications - good balance of speed and accuracy
 */
void performAveragedMeasurement()
{
    Serial.println("┌─ AVERAGED MEASUREMENT (5 samples) ───┐");

    // Take multiple measurements and average them
    long avgDistance = sensor.measureDistanceAveraged(SAMPLES_PER_BATCH);

    if (avgDistance > 0)
    {
        Serial.print("Average Distance: ");
        Serial.print(avgDistance);
        Serial.println(" cm");

        // Get statistics from the averaging operation
        long minDist, maxDist, calcAvg;
        uint8_t validSamples = sensor.getMeasurementStats(minDist, maxDist, calcAvg);

        Serial.print("Valid Samples: ");
        Serial.print(validSamples);
        Serial.print("/");
        Serial.println(SAMPLES_PER_BATCH);

        Serial.print("Range: ");
        Serial.print(minDist);
        Serial.print(" - ");
        Serial.print(maxDist);
        Serial.println(" cm");

        Serial.print("Signal Quality: ");
        Serial.print(sensor.getSignalQuality());
        Serial.println("%");

        updateStatistics(avgDistance);
        stats.validCount++;
    }
    else
    {
        Serial.println("⚠ Measurement failed");
        stats.invalidCount++;
    }

    Serial.println("└──────────────────────────────────────┘\n");
}

/**
 * Non-blocking asynchronous measurement mode
 * Allows other code to run while sensor is measuring
 * Demonstrates state machine pattern
 */
void performAsyncMeasurement()
{
    if (!asyncMeasurementPending)
    {
        // Start new async measurement
        Serial.println("┌─ ASYNC MEASUREMENT (Starting) ────────┐");

        sensor.startAsyncMeasurement();
        asyncMeasurementPending = true;
        asyncInitTime = millis();

        Serial.println("Measurement initiated (non-blocking)");
        Serial.println("└──────────────────────────────────────┘\n");

        // Note: In real applications, other code would execute here
        // The measurement happens in the background
    }
}

/**
 * Poll for async measurement completion
 */
void pollAsyncMeasurement()
{
    if (sensor.isAsyncMeasurementReady())
    {
        Serial.println("┌─ ASYNC MEASUREMENT (Complete) ────────┐");

        long distance = sensor.readAsyncDistance();
        unsigned long elapsed = millis() - asyncInitTime;

        if (distance > 0)
        {
            Serial.print("Distance: ");
            Serial.print(distance);
            Serial.println(" cm");

            Serial.print("Time to measure: ");
            Serial.print(elapsed);
            Serial.println(" ms");

            updateStatistics(distance);
            stats.validCount++;
        }
        else
        {
            Serial.println("⚠ Invalid measurement");
            stats.invalidCount++;
        }

        Serial.println("└──────────────────────────────────────┘\n");

        asyncMeasurementPending = false;
    }
}

/**
 * Continuous rapid measurements (limited by sensor timing)
 */
void performContinuousMeasurement()
{
    // In continuous mode, we take measurements as fast as possible
    // (limited by HC-SR04 sensor minimum timing: ~60ms between measurements)

    Serial.println("┌─ CONTINUOUS MEASUREMENT ──────────────┐");

    for (int i = 0; i < 3; i++)
    {
        long distance = sensor.measureDistance();

        Serial.print("Measurement ");
        Serial.print(i + 1);
        Serial.print(": ");

        if (distance > 0)
        {
            Serial.print(distance);
            Serial.println(" cm");
            updateStatistics(distance);
            stats.validCount++;
        }
        else
        {
            Serial.println("Invalid");
            stats.invalidCount++;
        }

        delay(100);  // Small delay between rapid measurements
    }

    Serial.println("└──────────────────────────────────────┘\n");
}

// ───────────────────────────────────────────────────────────────────────────────
// HELPER FUNCTIONS
// ───────────────────────────────────────────────────────────────────────────────

/**
 * Configure sensor parameters
 */
void configureMainSensor()
{
    // Set valid measurement range (2 cm to 400 cm - sensor limits)
    sensor.setValidRange(5, 200);  // Practical range for typical use

    // Set noise filter threshold (deviation tolerance)
    sensor.setNoiseThreshold(3);   // Reject measurements deviating > 3 cm from median

    // Enable noise filtering (recommended for most applications)
    sensor.setNoiseFilteringEnabled(true);

    // Set echo timeout (increase for longer-range measurements)
    sensor.setEchoTimeout(30000);  // 30 ms = ~5 m maximum range
}

/**
 * Print sensor configuration information
 */
void printSensorConfiguration()
{
    Serial.println("SENSOR CONFIGURATION:");
    Serial.println("├─ Trigger Pin: 9");
    Serial.println("├─ Echo Pin: 10");
    Serial.println("├─ Valid Range: 5-200 cm");
    Serial.println("├─ Noise Threshold: 3 cm");
    Serial.println("├─ Filtering: ENABLED");
    Serial.println("└─ Measurement Interval: 500 ms\n");
}

/**
 * Update running statistics
 */
void updateStatistics(long distance)
{
    stats.measurementCount++;
    stats.totalDistance += distance;

    if (distance < stats.minDistance)
        stats.minDistance = distance;
    if (distance > stats.maxDistance)
        stats.maxDistance = distance;
}

/**
 * Print current statistics
 */
void printMeasurementStats()
{
    if (stats.measurementCount > 0)
    {
        long avgDist = stats.totalDistance / stats.measurementCount;

        Serial.print("Statistics: Min=");
        Serial.print(stats.minDistance);
        Serial.print("cm, Max=");
        Serial.print(stats.maxDistance);
        Serial.print("cm, Avg=");
        Serial.print(avgDist);
        Serial.print("cm (");
        Serial.print(stats.validCount);
        Serial.print("/");
        Serial.print(stats.measurementCount);
        Serial.println(" valid)");
    }
}

/**
 * Handle serial commands
 */
void handleSerialCommand()
{
    // Read command from serial
    char command = Serial.read();

    Serial.println("\n[COMMAND RECEIVED]");

    switch (command)
    {
    case '1':
        currentMode = MODE_SINGLE;
        Serial.println("Mode: SINGLE MEASUREMENT");
        break;

    case '2':
        currentMode = MODE_AVERAGED;
        Serial.println("Mode: AVERAGED MEASUREMENT");
        break;

    case '3':
        currentMode = MODE_ASYNC;
        Serial.println("Mode: ASYNCHRONOUS MEASUREMENT");
        break;

    case '4':
        currentMode = MODE_CONTINUOUS;
        Serial.println("Mode: CONTINUOUS MEASUREMENT");
        break;

    case 'd':
        Serial.println("\nDEBUG INFORMATION:");
        Serial.println(sensor.getDebugInfo());
        break;

    case 'r':
        // Reset statistics
        stats.minDistance = LONG_MAX;
        stats.maxDistance = LONG_MIN;
        stats.totalDistance = 0;
        stats.measurementCount = 0;
        stats.validCount = 0;
        stats.invalidCount = 0;
        Serial.println("Statistics RESET");
        break;

    case 'h':
        Serial.println("\n╔════════════════════════════════════════╗");
        Serial.println("║  COMMAND REFERENCE                     ║");
        Serial.println("╠════════════════════════════════════════╣");
        Serial.println("║  1: Single Measurement Mode            ║");
        Serial.println("║  2: Averaged Measurement Mode (5x)    ║");
        Serial.println("║  3: Asynchronous Mode                  ║");
        Serial.println("║  4: Continuous Measurement            ║");
        Serial.println("║  d: Debug Information                  ║");
        Serial.println("║  r: Reset Statistics                   ║");
        Serial.println("║  h: Help (this menu)                   ║");
        Serial.println("╚════════════════════════════════════════╝\n");
        break;

    default:
        Serial.println("Unknown command. Type 'h' for help.");
        break;
    }

    Serial.println();
}

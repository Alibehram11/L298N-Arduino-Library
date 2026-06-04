/*
 * ═══════════════════════════════════════════════════════════════════════════════
 * Power Management & Battery Monitoring - Header
 * ═══════════════════════════════════════════════════════════════════════════════
 * Monitor battery voltage, current consumption, and power efficiency
 * 
 * Features:
 *   ✓ Battery voltage monitoring (0-5V ADC)
 *   ✓ Current consumption tracking via ACS712 sensor
 *   ✓ Low battery warnings and shutdown
 *   ✓ Power efficiency metrics
 *   ✓ Runtime estimation
 *
 * Author: Arduino Car Library Contributors
 * License: MIT
 * ═══════════════════════════════════════════════════════════════════════════════
 */

#ifndef POWER_MANAGEMENT_H
#define POWER_MANAGEMENT_H

#include <Arduino.h>

/**
 * PowerManager Class - Battery and power monitoring
 * 
 * Tracks battery health and alerts when power is low.
 */
class PowerManager {
public:
    /**
     * Constructor - Initialize power monitor
     * 
     * @param voltagePin - Analog pin for voltage divider (0-5V → battery voltage)
     * @param currentPin - Analog pin for current sensor (ACS712-5A)
     * @param batteryCapacityMAh - Battery capacity in mAh (typical: 2000-5000)
     */
    PowerManager(int voltagePin, int currentPin = -1, int batteryCapacityMAh = 2000);

    /**
     * Update voltage and current readings
     * Should be called regularly in loop()
     */
    void update();

    /**
     * Get battery voltage
     * 
     * @return Voltage in volts (e.g., 11.5V)
     */
    float getVoltage();

    /**
     * Get estimated battery percentage (0-100%)
     * 
     * @return Battery level percentage
     */
    uint8_t getBatteryPercent();

    /**
     * Get current consumption
     * 
     * @return Current in amperes (requires current sensor)
     */
    float getCurrentAmps();

    /**
     * Check if battery is low
     * 
     * @return true if voltage below warning threshold
     */
    bool isLowBattery();

    /**
     * Set low battery warning threshold
     * 
     * @param voltageThreshold - Warning voltage (e.g., 9.0V for 12V system)
     */
    void setLowBatteryThreshold(float voltageThreshold);

    /**
     * Get estimated runtime remaining
     * 
     * @return Minutes of runtime left (requires current sensor)
     */
    int getEstimatedRuntimeMinutes();

    /**
     * Get total energy consumed since power-on
     * 
     * @return Watt-hours consumed
     */
    float getEnergyConsumedWh();

private:
    int _voltagePin;
    int _currentPin;
    int _batteryCapacityMAh;
    float _lastVoltage;
    float _lastCurrent;
    float _minVoltage;
    float _maxVoltage;
    float _lowBatteryThreshold;
    unsigned long _lastUpdateTime;
    float _totalEnergyWh;
};

#endif // POWER_MANAGEMENT_H

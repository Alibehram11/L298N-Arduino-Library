/*
 * ═════════════════════════════════════════════════════════════════════════════
 * Configuration Manager - Header
 * ═════════════════════════════════════════════════════════════════════════════
 * Persistent storage of robot configuration parameters using EEPROM.
 * Load/save PID gains, motor calibration, sensor thresholds, etc.
 *
 * Features:
 *   ✓ JSON-like configuration format
 *   ✓ EEPROM persistence
 *   ✓ Default configuration recovery
 *   ✓ Runtime parameter tuning
 *   ✓ CRC checksum validation
 *
 * Author: Arduino Car Library Contributors
 * License: MIT
 * ═════════════════════════════════════════════════════════════════════════════
 */

#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <Arduino.h>

/**
 * ConfigManager Class - Persistent Configuration Storage
 *
 * Stores robot configuration in EEPROM with CRC validation.
 *
 * Typical Usage:
 *   ConfigManager config;
 *   
 *   // Load from EEPROM (or use defaults if corrupted)
 *   config.load();
 *   
 *   // Access parameters
 *   float pidKp = config.getFloat("pid_kp", 1.0);  // Default: 1.0
 *   
 *   // Modify and save
 *   config.setFloat("pid_kp", 1.5);
 *   config.save();
 */
class ConfigManager {
public:
    /**
     * Constructor - Initialize config manager
     */
    ConfigManager();

    /**
     * Destructor
     */
    ~ConfigManager();

    /**
     * Load configuration from EEPROM
     *
     * @return true if loaded successfully, false if corrupted (uses defaults)
     *
     * Validates CRC before loading. If invalid, resets to defaults.
     */
    bool load();

    /**
     * Save current configuration to EEPROM
     *
     * @return true if saved successfully, false on error
     *
     * Includes CRC checksum for integrity verification.
     */
    bool save();

    /**
     * Check if the loaded configuration was restored from defaults
     *
     * @return true when load() fell back to defaults because EEPROM was empty,
     *         incompatible, or failed CRC validation.
     */
    bool usedDefaults() const;

    /**
     * Reset all parameters to default values
     *
     * Useful after corrupted config or factory reset.
     */
    void resetToDefaults();

    /**
     * Get integer configuration value
     *
     * @param key - Parameter name (e.g., "motor_max_pwm")
     * @param defaultValue - Value to return if not found
     * @return Configuration value or default
     */
    int32_t getInt(const char* key, int32_t defaultValue = 0);

    /**
     * Get floating-point configuration value
     *
     * @param key - Parameter name
     * @param defaultValue - Default value
     * @return Configuration value or default
     */
    float getFloat(const char* key, float defaultValue = 0.0);

    /**
     * Get string configuration value
     *
     * @param key - Parameter name
     * @param buffer - Output buffer for string
     * @param bufferSize - Size of buffer
     * @param defaultValue - Default value if not found
     * @return Actual string length
     */
    uint8_t getString(const char* key, char* buffer, uint8_t bufferSize, const char* defaultValue = "");

    /**
     * Set integer configuration value
     *
     * @param key - Parameter name
     * @param value - New value
     * @return true if set successfully
     */
    bool setInt(const char* key, int32_t value);

    /**
     * Set floating-point configuration value
     *
     * @param key - Parameter name
     * @param value - New value
     * @return true if set successfully
     */
    bool setFloat(const char* key, float value);

    /**
     * Set string configuration value
     *
     * @param key - Parameter name
     * @param value - String to store
     * @return true if set successfully
     */
    bool setString(const char* key, const char* value);

    /**
     * Check if configuration key exists
     *
     * @param key - Parameter name
     * @return true if found, false otherwise
     */
    bool hasKey(const char* key) const;

    /**
     * Erase specific configuration entry
     *
     * @param key - Parameter name to remove
     * @return true if erased, false if not found
     */
    bool eraseKey(const char* key);

    /**
     * Get EEPROM usage statistics
     *
     * @param outUsedBytes - Output: Bytes used
     * @param outTotalBytes - Output: Total available EEPROM
     * @return true if statistics available
     */
    bool getStorageStats(uint16_t& outUsedBytes, uint16_t& outTotalBytes);

    /**
     * Dump configuration to Serial for debugging
     *
     * Prints all current key-value pairs
     */
    void printToSerial();

private:
    // Configuration storage in RAM (synced with EEPROM)
    struct ConfigEntry {
        char key[16];
        char value[32];
    };

    static const uint8_t MAX_ENTRIES = 16;
    ConfigEntry _entries[MAX_ENTRIES];
    uint8_t _entryCount;
    bool _usedDefaults;

    /**
     * Internal: Calculate CRC checksum
     */
    uint16_t calculateCRC();

    /**
     * Internal: Find entry by key
     */
    int8_t findEntry(const char* key);

    /**
     * Internal: Load defaults
     */
    void loadDefaults();
};

#endif // CONFIG_MANAGER_H

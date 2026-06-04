/*
 * ═══════════════════════════════════════════════════════════════════════════════
 * Diagnostics & Error Logging - Header
 * ═══════════════════════════════════════════════════════════════════════════════
 * Comprehensive system diagnostics, error logging, and self-test capabilities
 * 
 * Features:
 *   ✓ Error logging with timestamps
 *   ✓ System self-test (all modules)
 *   ✓ Performance metrics
 *   ✓ Debug output formatting
 *   ✓ Error history and statistics
 *   ✓ Memory and CPU usage tracking
 *
 * Author: Arduino Car Library Contributors
 * License: MIT
 * ═══════════════════════════════════════════════════════════════════════════════
 */

#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

#include <Arduino.h>

/**
 * Error severity levels
 */
enum ErrorLevel {
    ERROR_INFO = 0,      // Information message
    ERROR_WARNING = 1,   // Warning (non-critical)
    ERROR_ERROR = 2,     // Error (functionality impaired)
    ERROR_CRITICAL = 3   // Critical (system failure)
};

/**
 * Error codes for common issues
 */
enum ErrorCode {
    ERR_OK = 0,
    ERR_MOTOR_NOT_RESPONDING = 10,
    ERR_SENSOR_TIMEOUT = 20,
    ERR_BLUETOOTH_DISCONNECTED = 30,
    ERR_ENCODER_ERROR = 40,
    ERR_LOW_BATTERY = 50,
    ERR_OVERCURRENT = 60,
    ERR_CONFIG_INVALID = 70,
    ERR_UNKNOWN = 255
};

/**
 * DiagnosticsEngine Class - System diagnostics and error management
 * 
 * Monitors system health and maintains error log.
 */
class DiagnosticsEngine {
public:
    /**
     * Constructor - Initialize diagnostics
     * 
     * @param maxLogSize - Maximum number of errors to store (default: 50)
     */
    DiagnosticsEngine(uint8_t maxLogSize = 50);

    /**
     * Log an error or warning
     * 
     * @param level - Severity level
     * @param code - Error code
     * @param message - Description
     */
    void logError(ErrorLevel level, ErrorCode code, const String& message);

    /**
     * Quick error logging
     */
    void logInfo(const String& msg);
    void logWarning(const String& msg);
    void logError(const String& msg);
    void logCritical(const String& msg);

    /**
     * Perform comprehensive system self-test
     * 
     * @return Number of errors found
     */
    uint8_t runSystemTest();

    /**
     * Test individual module
     * 
     * @param moduleName - "motor", "sensor", "bluetooth", "encoder"
     * @return true if module passes test
     */
    bool testModule(const String& moduleName);

    /**
     * Clear error log
     */
    void clearLog();

    /**
     * Get number of errors in log
     */
    uint8_t getErrorCount();

    /**
     * Print error log to Serial
     */
    void printLog();

    /**
     * Print system status summary
     */
    void printSystemStatus();

    /**
     * Get error statistics
     * 
     * @return String with count of each error type
     */
    String getErrorStatistics();

    /**
     * Get memory usage
     * 
     * @return Free RAM in bytes
     */
    int getFreeRAM();

    /**
     * Get uptime
     * 
     * @return System uptime in seconds
     */
    unsigned long getUptime();

private:
    struct LogEntry {
        unsigned long timestamp;
        ErrorLevel level;
        ErrorCode code;
        String message;
    };

    uint8_t _maxLogSize;
    uint8_t _logCount;
    unsigned long _bootTime;
    LogEntry* _log;
};

#endif // DIAGNOSTICS_H

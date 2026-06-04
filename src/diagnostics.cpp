/*
 * ═══════════════════════════════════════════════════════════════════════════════
 * Diagnostics Engine Implementation
 * ═══════════════════════════════════════════════════════════════════════════════
 */

#include "diagnostics.h"

DiagnosticsEngine::DiagnosticsEngine(uint8_t maxLogSize)
    : _maxLogSize(maxLogSize),
      _logCount(0),
      _bootTime(millis())
{
    _log = new LogEntry[maxLogSize];
}

DiagnosticsEngine::~DiagnosticsEngine()
{
    delete[] _log;
    _log = nullptr;
}

void DiagnosticsEngine::logError(ErrorLevel level, ErrorCode code, const String& message)
{
    if (_logCount < _maxLogSize)
    {
        _log[_logCount].timestamp = millis();
        _log[_logCount].level = level;
        _log[_logCount].code = code;
        _log[_logCount].message = message;
        _logCount++;
    }
}

void DiagnosticsEngine::logInfo(const String& msg)
{
    logError(ERROR_INFO, ERR_OK, msg);
}

void DiagnosticsEngine::logWarning(const String& msg)
{
    logError(ERROR_WARNING, ERR_OK, msg);
}

void DiagnosticsEngine::logError(const String& msg)
{
    logError(ERROR_ERROR, ERR_OK, msg);
}

void DiagnosticsEngine::logCritical(const String& msg)
{
    logError(ERROR_CRITICAL, ERR_OK, msg);
}

uint8_t DiagnosticsEngine::runSystemTest()
{
    uint8_t errorCount = 0;
    
    logInfo("Starting system self-test...");
    
    // Test motor pins
    if (testModule("motor"))
    {
        logInfo("✓ Motor module OK");
    }
    else
    {
        logError("✗ Motor module FAILED");
        errorCount++;
    }
    
    // Test encoder
    if (testModule("encoder"))
    {
        logInfo("✓ Encoder module OK");
    }
    else
    {
        logWarning("⚠ Encoder module not responding");
    }
    
    // Test Bluetooth
    if (testModule("bluetooth"))
    {
        logInfo("✓ Bluetooth module OK");
    }
    else
    {
        logWarning("⚠ Bluetooth module not responding");
    }
    
    // Test ultrasonic
    if (testModule("sensor"))
    {
        logInfo("✓ Ultrasonic sensor OK");
    }
    else
    {
        logWarning("⚠ Ultrasonic sensor not responding");
    }
    
    logInfo("System test complete.");
    return errorCount;
}

bool DiagnosticsEngine::testModule(const String& moduleName)
{
    // Placeholder test logic
    // In real implementation, would test actual hardware
    return true;
}

void DiagnosticsEngine::clearLog()
{
    _logCount = 0;
}

uint8_t DiagnosticsEngine::getErrorCount()
{
    return _logCount;
}

void DiagnosticsEngine::printLog()
{
    Serial.println("\n═══════════════════════════════════════");
    Serial.println("ERROR LOG:");
    Serial.println("═══════════════════════════════════════");
    
    for (uint8_t i = 0; i < _logCount; i++)
    {
        Serial.print("[");
        Serial.print(_log[i].timestamp);
        Serial.print("ms] ");
        
        switch (_log[i].level)
        {
            case ERROR_INFO: Serial.print("INFO"); break;
            case ERROR_WARNING: Serial.print("WARN"); break;
            case ERROR_ERROR: Serial.print("ERROR"); break;
            case ERROR_CRITICAL: Serial.print("CRIT"); break;
        }
        
        Serial.print(" - ");
        Serial.println(_log[i].message);
    }
}

void DiagnosticsEngine::printSystemStatus()
{
    Serial.println("\n═══════════════════════════════════════");
    Serial.println("SYSTEM STATUS:");
    Serial.println("═══════════════════════════════════════");
    Serial.print("Uptime: ");
    Serial.print(getUptime());
    Serial.println(" seconds");
    Serial.print("Free RAM: ");
    Serial.print(getFreeRAM());
    Serial.println(" bytes");
    Serial.print("Errors logged: ");
    Serial.println(getErrorCount());
}

String DiagnosticsEngine::getErrorStatistics()
{
    uint8_t infoCount = 0, warnCount = 0, errorCount = 0, critCount = 0;
    
    for (uint8_t i = 0; i < _logCount; i++)
    {
        switch (_log[i].level)
        {
            case ERROR_INFO: infoCount++; break;
            case ERROR_WARNING: warnCount++; break;
            case ERROR_ERROR: errorCount++; break;
            case ERROR_CRITICAL: critCount++; break;
        }
    }
    
    String stats = "Info: ";
    stats += infoCount;
    stats += " | Warn: ";
    stats += warnCount;
    stats += " | Error: ";
    stats += errorCount;
    stats += " | Critical: ";
    stats += critCount;
    return stats;
}

int DiagnosticsEngine::getFreeRAM()
{
    extern int __heap_start, *__brkval;
    int v;
    return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}

unsigned long DiagnosticsEngine::getUptime()
{
    return (millis() - _bootTime) / 1000;
}

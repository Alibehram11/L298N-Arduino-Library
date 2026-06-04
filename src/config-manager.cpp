/*
 * ═════════════════════════════════════════════════════════════════════════════
 * Configuration Manager - Implementation
 * ═════════════════════════════════════════════════════════════════════════════
 */

#include "config-manager.h"
#include <EEPROM.h>
#include <stdlib.h>
#include <string.h>

namespace {
    const uint16_t CONFIG_MAGIC = 0xCAFE;
    const uint8_t CONFIG_VERSION = 1;

    uint16_t crc16Update(uint16_t crc, uint8_t data)
    {
        crc ^= data;
        for (uint8_t i = 0; i < 8; i++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xA001;
            } else {
                crc >>= 1;
            }
        }
        return crc;
    }

    uint16_t crc16Bytes(const uint8_t* data, size_t length)
    {
        uint16_t crc = 0xFFFF;
        for (size_t i = 0; i < length; i++) {
            crc = crc16Update(crc, data[i]);
        }
        return crc;
    }
}

ConfigManager::ConfigManager() : _entryCount(0), _usedDefaults(true)
{
    loadDefaults();
}

ConfigManager::~ConfigManager()
{
}

bool ConfigManager::load()
{
    struct ConfigImage {
        uint16_t magic;
        uint8_t version;
        uint8_t entryCount;
        ConfigEntry entries[MAX_ENTRIES];
        uint16_t crc;
    };

    if (EEPROM.length() < (int)sizeof(ConfigImage)) {
        loadDefaults();
        return false;
    }

    ConfigImage image;
    EEPROM.get(0, image);

    uint16_t expectedCRC = image.crc;
    image.crc = 0;
    uint16_t actualCRC = crc16Bytes((const uint8_t*)&image, sizeof(ConfigImage));

    if (image.magic != CONFIG_MAGIC ||
        image.version != CONFIG_VERSION ||
        image.entryCount > MAX_ENTRIES ||
        expectedCRC != actualCRC) {
        loadDefaults();
        return false;
    }

    _entryCount = image.entryCount;
    for (uint8_t i = 0; i < _entryCount; i++) {
        memcpy(&_entries[i], &image.entries[i], sizeof(ConfigEntry));
        _entries[i].key[sizeof(_entries[i].key) - 1] = '\0';
        _entries[i].value[sizeof(_entries[i].value) - 1] = '\0';
    }

    _usedDefaults = false;
    return true;
}

bool ConfigManager::save()
{
    struct ConfigImage {
        uint16_t magic;
        uint8_t version;
        uint8_t entryCount;
        ConfigEntry entries[MAX_ENTRIES];
        uint16_t crc;
    };

    if (EEPROM.length() < (int)sizeof(ConfigImage)) {
        return false;
    }

    ConfigImage image;
    memset(&image, 0, sizeof(image));
    image.magic = CONFIG_MAGIC;
    image.version = CONFIG_VERSION;
    image.entryCount = _entryCount;

    for (uint8_t i = 0; i < _entryCount; i++) {
        memcpy(&image.entries[i], &_entries[i], sizeof(ConfigEntry));
    }

    image.crc = 0;
    image.crc = crc16Bytes((const uint8_t*)&image, sizeof(ConfigImage));
    EEPROM.put(0, image);
    _usedDefaults = false;
    return true;
}

void ConfigManager::resetToDefaults()
{
    loadDefaults();
}

bool ConfigManager::usedDefaults() const
{
    return _usedDefaults;
}

int32_t ConfigManager::getInt(const char* key, int32_t defaultValue)
{
    int8_t index = findEntry(key);
    if (index >= 0) {
        return atol(_entries[index].value);
    }
    return defaultValue;
}

float ConfigManager::getFloat(const char* key, float defaultValue)
{
    int8_t index = findEntry(key);
    if (index >= 0) {
        return atof(_entries[index].value);
    }
    return defaultValue;
}

uint8_t ConfigManager::getString(const char* key, char* buffer, uint8_t bufferSize, const char* defaultValue)
{
    if (buffer == nullptr || bufferSize == 0) {
        return 0;
    }

    int8_t index = findEntry(key);
    if (index >= 0) {
        strncpy(buffer, _entries[index].value, bufferSize - 1);
        buffer[bufferSize - 1] = '\0';
        return strlen(buffer);
    }
    strncpy(buffer, defaultValue, bufferSize - 1);
    buffer[bufferSize - 1] = '\0';
    return strlen(buffer);
}

bool ConfigManager::setInt(const char* key, int32_t value)
{
    char valueStr[16];
    itoa(value, valueStr, 10);
    return setString(key, valueStr);
}

bool ConfigManager::setFloat(const char* key, float value)
{
    char valueStr[16];
    dtostrf(value, 5, 2, valueStr);
    return setString(key, valueStr);
}

bool ConfigManager::setString(const char* key, const char* value)
{
    if (key == nullptr || value == nullptr || key[0] == '\0') {
        return false;
    }

    int8_t index = findEntry(key);
    if (index >= 0) {
        // Update existing entry
        strncpy(_entries[index].value, value, 31);
        _entries[index].value[31] = '\0';
        return true;
    }

    if (_entryCount < MAX_ENTRIES) {
        // Add new entry
        strncpy(_entries[_entryCount].key, key, 15);
        _entries[_entryCount].key[15] = '\0';
        strncpy(_entries[_entryCount].value, value, 31);
        _entries[_entryCount].value[31] = '\0';
        _entryCount++;
        return true;
    }

    return false;  // Storage full
}

bool ConfigManager::hasKey(const char* key) const
{
    for (uint8_t i = 0; i < _entryCount; i++) {
        if (strcmp(_entries[i].key, key) == 0) {
            return true;
        }
    }
    return false;
}

bool ConfigManager::eraseKey(const char* key)
{
    for (uint8_t i = 0; i < _entryCount; i++) {
        if (strcmp(_entries[i].key, key) == 0) {
            // Shift remaining entries
            for (uint8_t j = i; j < _entryCount - 1; j++) {
                memcpy(&_entries[j], &_entries[j + 1], sizeof(ConfigEntry));
            }
            _entryCount--;
            return true;
        }
    }
    return false;
}

bool ConfigManager::getStorageStats(uint16_t& outUsedBytes, uint16_t& outTotalBytes)
{
    outUsedBytes = _entryCount * sizeof(ConfigEntry);
    outTotalBytes = MAX_ENTRIES * sizeof(ConfigEntry);
    return true;
}

void ConfigManager::printToSerial()
{
    Serial.println("\n=== Configuration ===");
    for (uint8_t i = 0; i < _entryCount; i++) {
        Serial.print(_entries[i].key);
        Serial.print(" = ");
        Serial.println(_entries[i].value);
    }
    Serial.println("====================\n");
}

uint16_t ConfigManager::calculateCRC()
{
    return crc16Bytes((const uint8_t*)_entries, _entryCount * sizeof(ConfigEntry));
}

int8_t ConfigManager::findEntry(const char* key)
{
    for (uint8_t i = 0; i < _entryCount; i++) {
        if (strcmp(_entries[i].key, key) == 0) {
            return i;
        }
    }
    return -1;
}

void ConfigManager::loadDefaults()
{
    _entryCount = 0;
    _usedDefaults = true;

    // Default PID gains
    setFloat("pid_kp", 1.0);
    setFloat("pid_ki", 0.1);
    setFloat("pid_kd", 0.05);

    // Motor calibration
    setInt("motor_max_pwm", 255);
    setInt("motor_min_pwm", 0);

    // Sensor thresholds
    setInt("qtr_threshold", 500);
    setInt("distance_threshold", 20);

    // Speed ramping
    setInt("accel_rate", 100);

    // Robot name
    setString("robot_name", "Arduino-Car");
}

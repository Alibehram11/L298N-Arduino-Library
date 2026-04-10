/*
 * ═════════════════════════════════════════════════════════════════════════════
 * Radio Link (I2C SDA/SCL) Receiver Module - Implementation
 * ═════════════════════════════════════════════════════════════════════════════
 */

#include "radio-link-i2c.h"

/**
 * CONSTRUCTOR: RadioLinkI2C::RadioLinkI2C(uint8_t i2cAddress)
 * ────────────────────────────────────────────────────────────
 * Initialize radio link I2C communication instance
 */
RadioLinkI2C::RadioLinkI2C(uint8_t i2cAddress)
    : _i2cAddress(i2cAddress),
      _moduleActive(false),
      _lastRSSI(-120),
      _lastLinkQuality(0),
      _lastReceivedPacketLength(0),
      _lastPacketTime(0),
      _packetsReceived(0),
      _packetsTransmitted(0),
      _errorCount(0),
      _totalSamples(0)
{
}

/**
 * DESTRUCTOR: RadioLinkI2C::~RadioLinkI2C()
 * ──────────────────────────────────────────
 * Cleanup resources
 */
RadioLinkI2C::~RadioLinkI2C()
{
    end();
}

/*═════════════════════════════════════════════════════════════════════════════
 * INITIALIZATION & CONTROL
 * ═════════════════════════════════════════════════════════════════════════════*/

/**
 * METHOD: uint8_t RadioLinkI2C::readRegister(uint8_t reg)
 * ────────────────────────────────────────────────────────
 * Internal: Read single byte from I2C register
 */
uint8_t RadioLinkI2C::readRegister(uint8_t reg)
{
    Wire.beginTransmission(_i2cAddress);
    Wire.write(reg);
    uint8_t result = Wire.endTransmission(false);  // Repeated START

    if (result != 0)
    {
        return 0xFF;  // I2C error
    }

    Wire.requestFrom(_i2cAddress, (uint8_t)1);

    if (Wire.available())
    {
        return Wire.read();
    }

    return 0xFF;
}

/**
 * METHOD: bool RadioLinkI2C::writeRegister(uint8_t reg, uint8_t value)
 * ────────────────────────────────────────────────────────────────────
 * Internal: Write single byte to I2C register
 */
bool RadioLinkI2C::writeRegister(uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(_i2cAddress);
    Wire.write(reg);
    Wire.write(value);
    uint8_t result = Wire.endTransmission();

    return (result == 0);
}

/**
 * METHOD: uint8_t RadioLinkI2C::readRegisters(uint8_t startReg, uint8_t* buffer, uint8_t count)
 * ───────────────────────────────────────────────────────────────────────────────────────────────
 * Internal: Read multiple consecutive bytes from I2C
 */
uint8_t RadioLinkI2C::readRegisters(uint8_t startReg, uint8_t* buffer, uint8_t count)
{
    Wire.beginTransmission(_i2cAddress);
    Wire.write(startReg);
    uint8_t result = Wire.endTransmission(false);  // Repeated START

    if (result != 0)
    {
        return 0;
    }

    Wire.requestFrom(_i2cAddress, count);

    uint8_t bytesRead = 0;
    while (Wire.available() && bytesRead < count)
    {
        buffer[bytesRead++] = Wire.read();
    }

    return bytesRead;
}

/**
 * METHOD: uint8_t RadioLinkI2C::writeRegisters(uint8_t startReg, const uint8_t* buffer, uint8_t count)
 * ──────────────────────────────────────────────────────────────────────────────────────────────────
 * Internal: Write multiple consecutive bytes to I2C
 */
uint8_t RadioLinkI2C::writeRegisters(uint8_t startReg, const uint8_t* buffer, uint8_t count)
{
    Wire.beginTransmission(_i2cAddress);
    Wire.write(startReg);

    for (uint8_t i = 0; i < count; i++)
    {
        Wire.write(buffer[i]);
    }

    uint8_t result = Wire.endTransmission();

    if (result == 0)
    {
        return count;
    }

    return 0;
}

/**
 * METHOD: bool RadioLinkI2C::begin()
 * ──────────────────────────────────
 * Initialize radio module communication
 */
bool RadioLinkI2C::begin()
{
    // Ensure Wire is initialized
    Wire.begin();

    // Try to read version register to verify communication
    uint8_t version = readRegister(RadioLink_Registers::REG_VERSION);

    if (version == 0xFF)
    {
        _moduleActive = false;
        return false;
    }

    _moduleActive = true;

    // Initialize module with default configuration
    writeRegister(RadioLink_Registers::REG_CONFIG, 0x0F);  // Enable RX mode
    writeRegister(RadioLink_Registers::REG_CHANNEL, 42);   // Default channel
    writeRegister(RadioLink_Registers::REG_POWER, 0x02);   // Default power

    // Clear statistics
    clearStatistics();

    delay(10);  // Wait for module to stabilize

    return true;
}

/**
 * METHOD: void RadioLinkI2C::end()
 * ────────────────────────────────
 * Shutdown radio module
 */
void RadioLinkI2C::end()
{
    if (_moduleActive)
    {
        // Disable RF receiver (power down mode)
        writeRegister(RadioLink_Registers::REG_CONFIG, 0x00);
        _moduleActive = false;
    }
}

/**
 * METHOD: bool RadioLinkI2C::isModuleActive()
 * ─────────────────────────────────────────────
 * Check if module is responding
 */
bool RadioLinkI2C::isModuleActive()
{
    if (!_moduleActive)
    {
        return false;
    }

    // Verify I2C communication by reading a register
    uint8_t status = readRegister(RadioLink_Registers::REG_STATUS);

    return (status != 0xFF);
}

/*═════════════════════════════════════════════════════════════════════════════
 * RECEIVING DATA
 * ═════════════════════════════════════════════════════════════════════════════*/

/**
 * METHOD: bool RadioLinkI2C::isDataAvailable()
 * ──────────────────────────────────────────────
 * Check if data packet is available
 */
bool RadioLinkI2C::isDataAvailable()
{
    if (!_moduleActive)
    {
        return false;
    }

    uint8_t status = readRegister(RadioLink_Registers::REG_STATUS);

    // Check status bit 7 (data available bit, varies by module)
    return ((status & 0x80) != 0);
}

/**
 * METHOD: uint8_t RadioLinkI2C::readDataPacket(uint8_t* buffer, uint8_t bufferSize)
 * ──────────────────────────────────────────────────────────────────────────────────
 * Read received data packet
 */
uint8_t RadioLinkI2C::readDataPacket(uint8_t* buffer, uint8_t bufferSize)
{
    if (!_moduleActive || !buffer)
    {
        return 0;
    }

    // Read packet length from module
    uint8_t packetLength = readRegister(RadioLink_Registers::REG_RX_LENGTH);

    if (packetLength == 0 || packetLength > MAX_PACKET_SIZE)
    {
        _errorCount++;
        return 0;
    }

    // Limit to buffer size
    uint8_t bytesToRead = (packetLength < bufferSize) ? packetLength : bufferSize;

    // Read packet data
    uint8_t bytesRead = readRegisters(RadioLink_Registers::REG_RX_DATA, buffer, bytesToRead);

    if (bytesRead > 0)
    {
        _packetsReceived++;
        _lastReceivedPacketLength = bytesRead;
        _lastPacketTime = millis();

        // Read RSSI for this packet
        _lastRSSI = (int16_t)readRegister(RadioLink_Registers::REG_RSSI);
        _lastRSSI -= 256;  // Convert to signed

        // Calculate link quality
        _lastLinkQuality = rssiToQuality(_lastRSSI);
    }

    return bytesRead;
}

/**
 * METHOD: uint8_t RadioLinkI2C::readDataPacketWithMetadata(...)
 * ───────────────────────────────────────────────────────────────
 * Read packet with signal metadata
 */
uint8_t RadioLinkI2C::readDataPacketWithMetadata(uint8_t* buffer, uint8_t bufferSize, 
                                                  int16_t& outRSSI, uint8_t& outLQI)
{
    uint8_t bytesRead = readDataPacket(buffer, bufferSize);

    outRSSI = _lastRSSI;
    outLQI = _lastLinkQuality;

    return bytesRead;
}

/*═════════════════════════════════════════════════════════════════════════════
 * SIGNAL MONITORING
 * ═════════════════════════════════════════════════════════════════════════════*/

/**
 * METHOD: int16_t RadioLinkI2C::getRSSI()
 * ───────────────────────────────────────
 * Get current signal strength
 */
int16_t RadioLinkI2C::getRSSI()
{
    if (!_moduleActive)
    {
        return -120;
    }

    uint8_t rssiRaw = readRegister(RadioLink_Registers::REG_RSSI);
    int16_t rssi = (int16_t)rssiRaw;

    // Convert to dBm (module specific, adjust if needed)
    if (rssi > 127)
    {
        rssi -= 256;
    }
    rssi = -50 - rssi;  // Typical conversion: raw value offset

    // Apply moving average for stability
    return calculateMovingAverage(rssi, 0.3f);
}

/**
 * METHOD: uint8_t RadioLinkI2C::getLinkQuality()
 * ───────────────────────────────────────────────
 * Get link quality percentage
 */
uint8_t RadioLinkI2C::getLinkQuality()
{
    int16_t rssi = getRSSI();
    return rssiToQuality(rssi);
}

/**
 * METHOD: int16_t RadioLinkI2C::getLastPacketRSSI()
 * ──────────────────────────────────────────────────
 * Get RSSI of last received packet
 */
int16_t RadioLinkI2C::getLastPacketRSSI()
{
    return _lastRSSI;
}

/*═════════════════════════════════════════════════════════════════════════════
 * CONFIGURATION
 * ═════════════════════════════════════════════════════════════════════════════*/

/**
 * METHOD: bool RadioLinkI2C::setChannel(uint8_t channel)
 * ───────────────────────────────────────────────────────
 * Set RF channel/frequency
 */
bool RadioLinkI2C::setChannel(uint8_t channel)
{
    if (!_moduleActive)
    {
        return false;
    }

    return writeRegister(RadioLink_Registers::REG_CHANNEL, channel);
}

/**
 * METHOD: uint8_t RadioLinkI2C::getChannel()
 * ───────────────────────────────────────────
 * Get current RF channel
 */
uint8_t RadioLinkI2C::getChannel()
{
    if (!_moduleActive)
    {
        return 0;
    }

    return readRegister(RadioLink_Registers::REG_CHANNEL);
}

/**
 * METHOD: bool RadioLinkI2C::setTransmitPower(uint8_t powerLevel)
 * ────────────────────────────────────────────────────────────────
 * Set transmit power level
 */
bool RadioLinkI2C::setTransmitPower(uint8_t powerLevel)
{
    if (!_moduleActive || powerLevel > 3)
    {
        return false;
    }

    return writeRegister(RadioLink_Registers::REG_POWER, powerLevel);
}

/**
 * METHOD: uint8_t RadioLinkI2C::getTransmitPower()
 * ────────────────────────────────────────────────
 * Get current transmit power level
 */
uint8_t RadioLinkI2C::getTransmitPower()
{
    if (!_moduleActive)
    {
        return 0;
    }

    return readRegister(RadioLink_Registers::REG_POWER);
}

/**
 * METHOD: void RadioLinkI2C::clearStatistics()
 * ────────────────────────────────────────────
 * Reset all counters
 */
void RadioLinkI2C::clearStatistics()
{
    _packetsReceived = 0;
    _packetsTransmitted = 0;
    _errorCount = 0;
    _totalSamples = 0;
}

/*═════════════════════════════════════════════════════════════════════════════
 * STATISTICS & DIAGNOSTICS
 * ═════════════════════════════════════════════════════════════════════════════*/

/**
 * METHOD: uint16_t RadioLinkI2C::getPacketsReceived()
 * ───────────────────────────────────────────────────
 * Get RX packet counter
 */
uint16_t RadioLinkI2C::getPacketsReceived()
{
    return _packetsReceived;
}

/**
 * METHOD: uint16_t RadioLinkI2C::getPacketsTransmitted()
 * ──────────────────────────────────────────────────────
 * Get TX packet counter
 */
uint16_t RadioLinkI2C::getPacketsTransmitted()
{
    return _packetsTransmitted;
}

/**
 * METHOD: uint16_t RadioLinkI2C::getErrorCount()
 * ───────────────────────────────────────────────
 * Get error/CRC fail counter
 */
uint16_t RadioLinkI2C::getErrorCount()
{
    return _errorCount;
}

/**
 * METHOD: uint8_t RadioLinkI2C::getPacketErrorRate()
 * ───────────────────────────────────────────────────
 * Calculate packet error rate
 */
uint8_t RadioLinkI2C::getPacketErrorRate()
{
    uint16_t total = _packetsReceived + _errorCount;

    if (total == 0)
    {
        return 0;
    }

    return (uint8_t)((_errorCount * 100) / total);
}

/**
 * METHOD: uint8_t RadioLinkI2C::getConnectionHealth()
 * ───────────────────────────────────────────────────
 * Get overall connection health indicator
 */
uint8_t RadioLinkI2C::getConnectionHealth()
{
    if (!_moduleActive)
    {
        return 0;  // Not connected
    }

    int16_t rssi = getRSSI();
    uint8_t errorRate = getPacketErrorRate();

    // Combine signal strength and error rate
    if (rssi > -50 && errorRate == 0)
        return 4;  // Excellent
    if (rssi > -70 && errorRate < 5)
        return 3;  // Good
    if (rssi > -85 && errorRate < 10)
        return 2;  // Fair
    if (rssi > -100)
        return 1;  // Weak

    return 0;  // No connection
}

/**
 * METHOD: uint8_t RadioLinkI2C::getModuleVersion()
 * ─────────────────────────────────────────────────
 * Get firmware version
 */
uint8_t RadioLinkI2C::getModuleVersion()
{
    if (!_moduleActive)
    {
        return 0;
    }

    return readRegister(RadioLink_Registers::REG_VERSION);
}

/*═════════════════════════════════════════════════════════════════════════════
 * DEBUG & DIAGNOSTIC
 * ═════════════════════════════════════════════════════════════════════════════*/

/**
 * METHOD: void RadioLinkI2C::enableDebugMode()
 * ─────────────────────────────────────────────
 * Enable module debug mode
 */
void RadioLinkI2C::enableDebugMode()
{
    if (_moduleActive)
    {
        writeRegister(RadioLink_Registers::REG_DEBUG_MODE, 0x01);
    }
}

/**
 * METHOD: void RadioLinkI2C::disableDebugMode()
 * ──────────────────────────────────────────────
 * Disable module debug mode
 */
void RadioLinkI2C::disableDebugMode()
{
    if (_moduleActive)
    {
        writeRegister(RadioLink_Registers::REG_DEBUG_MODE, 0x00);
    }
}

/**
 * METHOD: int16_t RadioLinkI2C::calculateMovingAverage(int16_t currentValue, float alpha)
 * ────────────────────────────────────────────────────────────────────────────────────────
 * Internal: Exponential moving average for RSSI smoothing
 */
int16_t RadioLinkI2C::calculateMovingAverage(int16_t currentValue, float alpha)
{
    if (_totalSamples == 0)
    {
        _lastRSSI = currentValue;
        _totalSamples = 1;
        return currentValue;
    }

    _lastRSSI = (int16_t)(alpha * currentValue + (1.0f - alpha) * _lastRSSI);
    _totalSamples++;

    return _lastRSSI;
}

/**
 * METHOD: uint8_t RadioLinkI2C::rssiToQuality(int16_t rssi)
 * ──────────────────────────────────────────────────────────
 * Internal: Convert RSSI to quality percentage
 */
uint8_t RadioLinkI2C::rssiToQuality(int16_t rssi)
{
    // Convert RSSI dBm to quality percentage
    // -50 dBm = 100%, -120 dBm = 0%
    if (rssi >= -50)
        return 100;
    if (rssi <= -120)
        return 0;

    int16_t range = -50 - (-120);  // 70 dBm range
    int16_t offset = rssi - (-120);
    return (uint8_t)((offset * 100) / range);
}

/**
 * METHOD: String RadioLinkI2C::getDebugInfo()
 * ────────────────────────────────────────────
 * Get comprehensive debug information
 */
String RadioLinkI2C::getDebugInfo()
{
    String info = "=== Radio Link I2C Debug Info ===\n";
    info += "Module Address: 0x" + String(_i2cAddress, HEX) + "\n";
    info += "Module Active: " + String(_moduleActive ? "YES" : "NO") + "\n";

    if (_moduleActive)
    {
        info += "Firmware Version: " + String(getModuleVersion()) + "\n";
        info += "Channel: " + String(getChannel()) + "\n";
        info += "TX Power: Level " + String(getTransmitPower()) + "\n";
        info += "Current RSSI: " + String(getRSSI()) + " dBm\n";
        info += "Last Packet RSSI: " + String(_lastRSSI) + " dBm\n";
        info += "Link Quality: " + String(getLinkQuality()) + "%\n";
        info += "Connection Health: " + String(getConnectionHealth()) + "/4\n";
        info += "RX Packets: " + String(_packetsReceived) + "\n";
        info += "TX Packets: " + String(_packetsTransmitted) + "\n";
        info += "Errors: " + String(_errorCount) + "\n";
        info += "Error Rate: " + String(getPacketErrorRate()) + "%\n";
        info += "Last Packet Time: " + String(millis() - _lastPacketTime) + " ms ago\n";
    }

    return info;
}

/**
 * METHOD: bool RadioLinkI2C::runSelfTest()
 * ────────────────────────────────────────
 * Run module self-test
 */
bool RadioLinkI2C::runSelfTest()
{
    if (!_moduleActive)
    {
        return false;
    }

    // Test 1: I2C communication
    uint8_t testValue = 0xAA;
    if (!writeRegister(0x3F, testValue))
    {
        return false;
    }

    uint8_t readValue = readRegister(0x3F);
    if (readValue != testValue)
    {
        return false;
    }

    // Test 2: RSSI measurement
    int16_t rssi = getRSSI();
    if (rssi < -120 || rssi > -10)
    {
        return false;  // Invalid RSSI range
    }

    // Test 3: Channel setting
    uint8_t originalChannel = getChannel();
    if (!setChannel(50))
    {
        return false;
    }
    uint8_t testChannel = getChannel();
    setChannel(originalChannel);  // Restore

    if (testChannel != 50)
    {
        return false;
    }

    return true;
}

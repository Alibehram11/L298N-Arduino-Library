/*
 * ═════════════════════════════════════════════════════════════════════════════
 * Radio Link (I2C SDA/SCL) Receiver Module - Header
 * ═════════════════════════════════════════════════════════════════════════════
 * Generic I2C interface for radio receiver modules with data packet reading,
 * signal strength monitoring, and comprehensive debugging capabilities.
 *
 * Supported Modules:
 *   - NRF24L01 (with I2C adapter)
 *   - LoRa modules with I2C interface
 *   - Generic I2C RF receivers
 *
 * Features:
 *   ✓ I2C communication at configurable addresses
 *   ✓ Data packet reading with error detection
 *   ✓ Signal strength (RSSI) monitoring
 *   ✓ Link quality indicator
 *   ✓ Packet statistics (sent/received/errors)
 *   ✓ Debug and diagnostic capabilities
 *   ✓ Configurable transmission power
 *   ✓ Channel frequency selection
 *
 * Author: Arduino Car Library Contributors
 * License: MIT
 * ═════════════════════════════════════════════════════════════════════════════
 */

#ifndef RADIO_LINK_I2C_H
#define RADIO_LINK_I2C_H

#include <Arduino.h>
#include <Wire.h>

/**
 * I2C Register Map for Radio Receiver Module
 * These are typical register definitions for RF modules with I2C interface
 */
namespace RadioLink_Registers {
    // Status and Control Registers
    const uint8_t REG_STATUS = 0x00;           // Status register
    const uint8_t REG_CONFIG = 0x01;           // Configuration register
    const uint8_t REG_POWER = 0x02;            // Power/TX strength level
    const uint8_t REG_CHANNEL = 0x03;          // RF channel frequency
    const uint8_t REG_RSSI = 0x04;             // Received Signal Strength Indicator
    const uint8_t REG_LINK_QUALITY = 0x05;     // Link quality metric
    
    // Data Packet Registers
    const uint8_t REG_RX_LENGTH = 0x10;        // Number of bytes to read
    const uint8_t REG_RX_DATA = 0x11;          // Receive data payload
    const uint8_t REG_TX_DATA = 0x12;          // Transmit data payload
    
    // Statistics Registers
    const uint8_t REG_RX_COUNT = 0x20;         // Received packet count (2 bytes)
    const uint8_t REG_TX_COUNT = 0x22;         // Transmitted packet count (2 bytes)
    const uint8_t REG_ERROR_COUNT = 0x24;      // Error/CRC fail count (2 bytes)
    
    // Diagnostics
    const uint8_t REG_VERSION = 0x30;          // Module firmware version
    const uint8_t REG_DEBUG_MODE = 0x31;       // Enable/disable debug output
}

/**
 * RadioLinkI2C Class - I2C Based Radio Receiver Interface
 *
 * A generic abstraction layer for I2C-connected radio receiver modules.
 * Provides consistent methods for communication regardless of specific RF hardware.
 *
 * Typical Usage:
 *   RadioLinkI2C radio(0x20);  // Module at I2C address 0x20
 *   radio.begin();
 *   if (radio.isDataAvailable()) {
 *       uint8_t data[32];
 *       uint8_t len = radio.readDataPacket(data, sizeof(data));
 *       int16_t rssi = radio.getRSSI();
 *   }
 */
class RadioLinkI2C {
public:
    /**
     * Maximum size of data packet payload
     */
    static constexpr uint8_t MAX_PACKET_SIZE = 32;

    /**
     * Constructor - Initialize I2C radio receiver
     *
     * @param i2cAddress - I2C address of the radio module (typically 0x20-0x7F)
     * @param wiringAddr - Address for wire library (default: 0=Wire, 1=Wire1, etc.)
     *
     * Common I2C addresses:
     *   - NRF24L01: 0x20-0x27 (configurable)
     *   - LoRa: 0x60-0x67 (varies by module)
     *   - Generic RF: 0x40-0x7F (depends on module)
     *
     * Example:
     *   RadioLinkI2C radio(0x20);
     */
    RadioLinkI2C(uint8_t i2cAddress);

    /**
     * Destructor
     */
    ~RadioLinkI2C();

    /*────────────────────────────────────────────────────────────────────────
     * INITIALIZATION & CONTROL
     * ────────────────────────────────────────────────────────────────────────*/

    /**
     * Initialize radio module communication
     *
     * @return true if module responds, false if communication failed
     *
     * Performs:
     *   1. Verifies I2C communication with module
     *   2. Reads module firmware version
     *   3. Initializes default configuration
     */
    bool begin();

    /**
     * Shutdown radio module (low power mode)
     *
     * Disables RF receiver to save power
     */
    void end();

    /**
     * Check if module is initialized and responding
     *
     * @return true if module responds to I2C queries
     */
    bool isModuleActive();

    /*────────────────────────────────────────────────────────────────────────
     * RECEIVING DATA
     * ────────────────────────────────────────────────────────────────────────*/

    /**
     * Check if received data packet is available
     *
     * @return true if data is waiting to be read
     *
     * Polls the module's status register without reading data.
     * Use before calling readDataPacket().
     */
    bool isDataAvailable();

    /**
     * Read received data packet
     *
     * @param buffer - Output buffer to store packet data
     * @param bufferSize - Size of output buffer
     * @return Number of bytes read, or 0 if no data available
     *
     * Reads up to bufferSize bytes from the module's RX FIFO.
     * Each packet includes automatic error checking (CRC).
     *
     * Example:
     *   uint8_t rxBuffer[32];
     *   uint8_t rxLen = radio.readDataPacket(rxBuffer, 32);
     */
    uint8_t readDataPacket(uint8_t* buffer, uint8_t bufferSize);

    /**
     * Read received data packet with metadata
     *
     * @param buffer - Output buffer for packet data
     * @param bufferSize - Size of output buffer
     * @param outRSSI - Output: Received signal strength (-50 to -120 dBm)
     * @param outLQI - Output: Link quality indicator (0-100%)
     * @return Number of bytes read
     *
     * Includes signal strength and quality metrics with the packet.
     */
    uint8_t readDataPacketWithMetadata(uint8_t* buffer, uint8_t bufferSize, 
                                       int16_t& outRSSI, uint8_t& outLQI);

    /*────────────────────────────────────────────────────────────────────────
     * SIGNAL MONITORING
     * ────────────────────────────────────────────────────────────────────────*/

    /**
     * Get Received Signal Strength Indicator (RSSI)
     *
     * @return Signal strength in dBm (typically -50 to -120)
     *
     * RSSI Interpretation:
     *   -30 to -67 dBm  = Excellent (very strong signal)
     *   -67 to -70 dBm  = Good (strong signal)
     *   -70 to -80 dBm  = Fair (moderate signal)
     *   -80 to -90 dBm  = Weak (poor signal)
     *   -90 to -120 dBm = Very Weak (unreliable)
     */
    int16_t getRSSI();

    /**
     * Get link quality indicator as percentage
     *
     * @return Quality metric 0-100% (100% = excellent, 0% = no signal)
     *
     * Combines RSSI and packet success rate into single quality metric.
     * Useful for signal strength visualization.
     */
    uint8_t getLinkQuality();

    /**
     * Get last received packet's signal strength
     *
     * @return RSSI of last received packet in dBm
     *
     * Returns RSSI of the most recently received packet.
     * Useful for analyzing individual packet quality.
     */
    int16_t getLastPacketRSSI();

    /*────────────────────────────────────────────────────────────────────────
     * CONFIGURATION
     * ────────────────────────────────────────────────────────────────────────*/

    /**
     * Set RF channel (frequency)
     *
     * @param channel - Channel number (format depends on module type)
     *                  NRF24L01: 0-125 (2.4 GHz band)
     *                  LoRa: typically 0-multiple bands
     * @return true if channel set successfully
     */
    bool setChannel(uint8_t channel);

    /**
     * Get current RF channel
     *
     * @return Current channel number
     */
    uint8_t getChannel();

    /**
     * Set transmit power level
     *
     * @param powerLevel - Power level 0-3 (module specific)
     *                     0 = -18 dBm (lowest)
     *                     3 = 0 dBm (highest)
     * @return true if power set successfully
     */
    bool setTransmitPower(uint8_t powerLevel);

    /**
     * Get current transmit power level
     *
     * @return Current power level (0-3 or module-specific)
     */
    uint8_t getTransmitPower();

    /**
     * Clear all statistics counters
     *
     * Resets packet count and error tracking
     */
    void clearStatistics();

    /*────────────────────────────────────────────────────────────────────────
     * STATISTICS & DIAGNOSTICS
     * ────────────────────────────────────────────────────────────────────────*/

    /**
     * Get number of successfully received packets
     *
     * @return Total received packet count since last clearStatistics()
     */
    uint16_t getPacketsReceived();

    /**
     * Get number of transmitted packets
     *
     * @return Total transmitted packet count since last clearStatistics()
     */
    uint16_t getPacketsTransmitted();

    /**
     * Get number of reception errors (CRC failures)
     *
     * @return Total error count since last clearStatistics()
     */
    uint16_t getErrorCount();

    /**
     * Get packet error rate
     *
     * @return Error rate as percentage (0-100%)
     *
     * Calculated as: (errors / total_attempts) * 100
     * Returns 0 if no data transmitted yet.
     */
    uint8_t getPacketErrorRate();

    /**
     * Get connection state/health indicator
     *
     * @return 0 = not connected / no signal
     *         1 = weak signal / unreliable
     *         2 = fair signal / mostly reliable
     *         3 = good signal / reliable
     *         4 = excellent signal / very reliable
     */
    uint8_t getConnectionHealth();

    /**
     * Get module firmware version
     *
     * @return Firmware version as packed byte (major.minor format)
     */
    uint8_t getModuleVersion();

    /*────────────────────────────────────────────────────────────────────────
     * DEBUG & DIAGNOSTIC
     * ────────────────────────────────────────────────────────────────────────*/

    /**
     * Enable debug mode on the radio module
     *
     * Module will output verbose diagnostic information
     */
    void enableDebugMode();

    /**
     * Disable debug mode on the radio module
     */
    void disableDebugMode();

    /**
     * Get comprehensive debug information string
     *
     * @return Multi-line string with module status and statistics
     *
     * Example output:
     *   === Radio Link I2C Debug Info ===
     *   Module Address: 0x20
     *   Module Active: YES
     *   Firmware Version: 1.2
     *   Channel: 42
     *   TX Power: Level 2
     *   Last RSSI: -65 dBm
     *   Link Quality: 95%
     *   RX Packets: 1234
     *   TX Packets: 0
     *   Errors: 2
     *   Error Rate: 0.16%
     */
    String getDebugInfo();

    /**
     * Perform module self-test
     *
     * @return true if all tests passed, false if any test failed
     *
     * Tests:
     *   - I2C communication
     *   - Register read/write
     *   - RSSI measurement
     *   - Link detection
     */
    bool runSelfTest();

private:
    uint8_t _i2cAddress;                     // I2C address of radio module
    bool _moduleActive;                      // Module initialization state

    // Last read values (cached)
    int16_t _lastRSSI;                       // Last RSSI value
    uint8_t _lastLinkQuality;                // Last link quality
    uint8_t _lastReceivedPacketLength;       // Size of last received packet
    unsigned long _lastPacketTime;           // Timestamp of last packet

    // Statistics
    uint16_t _packetsReceived;               // Received packet counter
    uint16_t _packetsTransmitted;            // Transmitted packet counter
    uint16_t _errorCount;                    // Error/CRC failure counter
    uint16_t _totalSamples;                  // Total RSSI samples for averaging

    /*────────────────────────────────────────────────────────────────────────
     * PRIVATE I2C HELPER METHODS
     * ────────────────────────────────────────────────────────────────────────*/

    /**
     * Read single byte from I2C register
     *
     * @param reg - Register address to read
     * @return Register value, or 0xFF on error
     */
    uint8_t readRegister(uint8_t reg);

    /**
     * Write single byte to I2C register
     *
     * @param reg - Register address to write
     * @param value - Value to write
     * @return true if write successful
     */
    bool writeRegister(uint8_t reg, uint8_t value);

    /**
     * Read multiple bytes from I2C (sequential register read)
     *
     * @param startReg - Starting register address
     * @param buffer - Output buffer for data
     * @param count - Number of bytes to read
     * @return Number of bytes actually read
     */
    uint8_t readRegisters(uint8_t startReg, uint8_t* buffer, uint8_t count);

    /**
     * Write multiple bytes to I2C (sequential register write)
     *
     * @param startReg - Starting register address
     * @param buffer - Data to write
     * @param count - Number of bytes to write
     * @return Number of bytes actually written
     */
    uint8_t writeRegisters(uint8_t startReg, const uint8_t* buffer, uint8_t count);

    /**
     * Calculate exponential moving average for RSSI
     *
     * @param currentValue - Current RSSI reading
     * @param alpha - Smoothing factor (0.0-1.0, lower = more smoothing)
     * @return Smoothed RSSI value
     */
    int16_t calculateMovingAverage(int16_t currentValue, float alpha = 0.3f);

    /**
     * Convert RSSI to link quality percentage
     *
     * @param rssi - Signal strength in dBm
     * @return Quality 0-100%
     */
    uint8_t rssiToQuality(int16_t rssi);
};

#endif // RADIO_LINK_I2C_H

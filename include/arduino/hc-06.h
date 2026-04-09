/*
 * ============================================================================
 * HC-06 Bluetooth Serial Communication Library
 * ============================================================================
 * A lightweight C++ class for communicating with HC-06 Bluetooth modules
 * on Arduino microcontrollers.
 * 
 * Features:
 *   - UART-based serial communication at configurable baud rates
 *   - Send and receive String data
 *   - Module reset capability
 *   - Automatic cleanup with destructor
 * 
 * Author: Arduino Car Library Contributors
 * License: MIT
 * ============================================================================
 */

#ifndef HC06_H
#define HC06_H

#include <Arduino.h>
#include <SoftwareSerial.h>

/**
 * HC06 Class - Manages Bluetooth module communication
 * 
 * This class abstracts the HC-06 Bluetooth module interface, providing
 * simple methods to send/receive data and manage the connection.
 */
class HC06 {
public:
    /**
     * Constructor - Initialize HC06 with communication pins
     * 
     * @param rxPin    - RX pin (connect to HC-06 TX pin via voltage divider)
     * @param txPin    - TX pin (connect to HC-06 RX pin)
     * @param hc06     - Optional control/enable pin for the HC-06 module
     * 
     * The constructor creates a SoftwareSerial instance and initializes
     * it at 9600 baud (HC-06 default). If hardware UART is available,
     * use pins 0,1 on Arduino Uno (RX0, TX0).
     * 
     * NOTE: For 3.3V modules like HC-06, use a voltage divider on the
     *       RXD line: HC-06 TX -> 1KΩ -> Arduino RX
     *                           -> 2KΩ -> GND
     */
    HC06(int rxPin, int txPin, int hc06);
    HC06(const HC06&) = delete;
    HC06& operator=(const HC06&) = delete;
    
    /**
     * Destructor - Clean up SoftwareSerial resources
     * 
     * Properly ends the serial communication and deallocates the
     * dynamically allocated SoftwareSerial object to prevent memory leaks.
     */
    ~HC06();
    
    /**
     * Initialize communication at specified baud rate
     * 
     * @param baudRate - Communication speed in bits per second
     *                   HC-06 supports: 9600 (default), 19200, 38400, etc.
     * 
     * HC-06 modules are typically configured at 9600 baud by default,
     * but this method allows changing the rate for both Arduino and module.
     * Note: The HC-06 module itself must be configured via AT commands.
     */
    void begin(long baudRate);
    
    /**
     * Send a string message to the HC-06 module
     * 
     * @param data - String data to transmit via Bluetooth
     * 
     * This method appends the string followed by a newline character,
     * which is then transmitted serially to the HC-06 module. The HC-06
     * forwards this data wirelessly to connected smartphones/devices.
     * 
     * Example:
     *   bluetooth.sendData("Motor: Forward");
     */
    void sendData(const String& data);
    
    /**
     * Receive a string message from the HC-06 module
     * 
     * @return String containing received data, or empty string if none available
     * 
     * This method non-blockingly checks if data is available on the serial
     * buffer and reads until a newline character ('\n'). If no data is
     * available, an empty string is returned immediately.
     * 
     * Example:
     *   String message = bluetooth.readData();
     *   if (message.length() > 0) { Serial.println("Received: " + message); }
     */
    String readData();
    
    /**
     * End (stop) the serial communication
     * 
     * Disables the SoftwareSerial interface, freeing up the allocated pins.
     * Call this before using the same pins for other purposes, or when
     * communication with the HC-06 module is no longer needed.
     */
    void end();
    
    /**
     * Reset the HC-06 Bluetooth module
     * 
     * Performs a hardware reset by toggling the RX and TX pins LOW then HIGH.
     * This simulates a power cycle reset of the module, which can resolve
     * connection issues or reset the module to factory settings.
     * 
     * Note: Pin toggling duration is 100ms to ensure proper module detection.
     */
    void reset();
    
    /**
     * Attempt to read available data and print to Serial monitor
     * 
     * This is a debugging/diagnostic method that reads any available data
     * from the HC-06 and prints it to the Serial console with the prefix
     * "Received: ". Useful for testing communication during development.
     * 
     * Example in setup():
     *   Serial.begin(9600);
     *   while(delay(10000)) { bluetooth.attempt(); }
     */
    void attempt();

private:
    // Member variables - encapsulated pin and serial references
    
    int _rxPin;                    // Receive pin number
    int _txPin;                    // Transmit pin number
    int _hc06;                     // HC-06 control/enable pin (optional)
    long _baudRate;                // Last configured baud rate
    SoftwareSerial* _btSerial;     // Dynamic pointer to SoftwareSerial for Bluetooth communication
};

#endif // HC06_H

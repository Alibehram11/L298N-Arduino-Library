/*
 * ============================================================================
 * HC-06 Bluetooth Serial Communication Library - Implementation
 * ============================================================================
 * Complete implementation of HC-06 communication methods
 */

#include "hc-06.h"  // Include the HC06 class definition

/**
 * CONSTRUCTOR: HC06::HC06(int rxPin, int txPin, int hc06)
 * ─────────────────────────────────────────────────────────
 * 
 * Initializes an HC06 object with the specified pins
 * 
 * Parameters:
 *   - rxPin: Arduino pin connected to HC-06 TX (receiver pin)
 *   - txPin: Arduino pin connected to HC-06 RX (transmitter pin)
 *   - hc06:  Optional pin for HC-06 control/enabling
 * 
 * What happens:
 *   1. Store the pin numbers in member variables (_rxPin, _txPin, _hc06)
 *   2. Create a new SoftwareSerial object using the RX and TX pins
 *   3. Start the serial communication at 9600 baud (HC-06 default)
 * 
 * NOTE: Using initializer list ": _rxPin(rxPin), ..." is preferred C++
 *       practice as it initializes members directly without copy assignment
 */
HC06::HC06(int rxPin, int txPin, int hc06) 
    : _rxPin(rxPin),           // Member initialization: store RX pin
      _txPin(txPin),           // Member initialization: store TX pin
      _hc06(hc06),             // Member initialization: store HC-06 control pin
      _baudRate(9600)
{
    // Create a new SoftwareSerial instance on the heap (dynamic memory)
    // SoftwareSerial is a library class that emulates serial communication
    // on any two digital pins (unlike hardware UART which uses 0,1)
    _btSerial = new SoftwareSerial(_rxPin, _txPin);
    
    // Start the serial communication at 9600 baud
    // 9600 baud is the default communication speed for HC-06 modules
    // Baud = bits per second (9600 bps = 9600 data bits/second)
    _btSerial->begin(_baudRate);

    if (_hc06 >= 0)
    {
        pinMode(_hc06, OUTPUT);
        digitalWrite(_hc06, HIGH);
    }
}

/**
 * DESTRUCTOR: HC06::~HC06()
 * ────────────────────────
 * 
 * Cleans up resources when the HC06 object is destroyed
 * 
 * Why destructors matter:
 *   - In the constructor, we used 'new' to allocate memory (_ btSerial = new ...)
 *   - Memory allocated with 'new' must be deallocated with 'delete'
 *   - Without a destructor, memory leaks occur (memory not returned to system)
 *   - Destrcutors automatically run when object goes out of scope
 * 
 * What happens:
 *   1. Check if _btSerial pointer is not null (safety check)
 *   2. If valid, end the serial communication (close the port)
 *   3. Delete the SoftwareSerial object and free memory
 *   4. Set pointer to nullptr to indicate no valid object exists
 */
HC06::~HC06()
{
    // Safety check: ensure _btSerial is not a null pointer
    // This prevents crashes from trying to access invalid memory addresses
    if (_btSerial != nullptr)
    {
        // End the serial communication, cleaning up any buffers
        // This properly closes the port before destroying the object
        _btSerial->end();
        
        // Free the memory that was allocated with 'new'
        // This returns the memory back to the system for other programs to use
        delete _btSerial;
        
        // Set the pointer to nullptr to indicate no valid object exists
        // This is a safety practice; prevents accidental use after deletion
        _btSerial = nullptr;
    }
}

/**
 * METHOD: void HC06::begin(long baudRate)
 * ────────────────────────────────────────
 * 
 * Changes the communication speed of the HC-06 connection
 * 
 * Parameters:
 *   - baudRate: New baud rate in bits per second (e.g., 9600, 19200, 38400)
 * 
 * Use case:
 *   - If you've configured your HC-06 module to use a different baud rate
 *     via AT commands, call this to match the Arduino serial speed
 *   - IMPORTANT: The HC-06 module itself must be configured separately
 *     using AT commands (AT+BAUD command)
 * 
 * What happens:
 *   1. Check if _btSerial pointer is valid (not null)
 *   2. Call the begin() method on the SoftwareSerial object
 *   3. This reconfigures the serial port for the new baud rate
 * 
 * Example:
 *   HC06 bt(3, 4, -1);    // Create at 9600 default
 *   bt.begin(19200);      // Change to 19200 baud
 */
void HC06::begin(long baudRate)
{
    // Safety check: verify the SoftwareSerial object exists
    // Without this check, calling methods on a null pointer causes a crash
    if (_btSerial != nullptr)
    {
        _baudRate = baudRate;
        // Initialize the SoftwareSerial at the specified baud rate
        // This reconfigures the UART parameters for communication
        _btSerial->begin(baudRate);
    }
}

/**
 * METHOD: void HC06::sendData(const String& data)
 * ───────────────────────────────────────────────
 * 
 * Transmits a string message through the Bluetooth module
 * 
 * Parameters:
 *   - data: Reference to a String object containing data to send
 *           Using reference (const String&) avoids copying the string
 *           'const' means we won't modify the original string
 * 
 * What happens:
 *   1. Uses println() which sends the string + newline character ('\n')
 *   2. The data is buffered in SoftwareSerial's output buffer
 *   3. Sent serially byte-by-byte to the HC-06 module
 *   4. HC-06 transmits it wirelessly to connected devices
 * 
 * Example:
 *   bluetooth.sendData("Motor Forward");
 *   // Transmits: "Motor Forward\n" via Bluetooth
 */
void HC06::sendData(const String& data)
{
    if (_btSerial != nullptr)
    {
        // println() sends the string plus a newline character ('\n')
        // This helps the receiving device/app identify message boundaries
        // Alternative: use print() to send without newline
        _btSerial->println(data);
    }
}

/**
 * METHOD: String HC06::readData()
 * ──────────────────────────────
 * 
 * Receives a string message from the Bluetooth module
 * 
 * Returns:
 *   - String containing the received message (without the newline)
 *   - Empty string ("") if no data is available
 * 
 * Behavior:
 *   - This is a non-blocking read (doesn't wait for data)
 *   - Reads one complete line (up to and including '\n')
 *   - Perfect for main loop polling
 * 
 * What happens:
 *   1. Check if data is available in the SoftwareSerial input buffer
 *   2. If available: read bytes until newline ('\n') is found
 *   3. If not available: return immediately with empty string
 * 
 * Example:
 *   String command = bluetooth.readData();
 *   if (command == "FORWARD") { motor.forward(255); }
 */
String HC06::readData()
{
    // Check if there's data waiting in the serial input buffer
    // available() returns the number of bytes ready to read (0 if none)
    if (_btSerial != nullptr && _btSerial->available())
    {
        // readStringUntil('\n') reads all characters up to the newline
        // The '\n' character marks the end of a message
        // Example: "STOP\n" -> returns "STOP" (without the newline)
        String data = _btSerial->readStringUntil('\n');
        data.trim();
        return data;
    }
    
    // Return empty string if no data is available
    // Empty string has length() == 0 (useful for checking)
    return "";
}

/**
 * METHOD: void HC06::end()
 * ────────────────────────
 * 
 * Stops the HC-06 serial communication
 * 
 * Use case:
 *   - When you're done communicating with HC-06
 *   - If you need to use the same pins for something else
 *   - To save power (stops the serial interrupts)
 * 
 * What happens:
 *   1. Calls end() on the SoftwareSerial object
 *   2. Disables the serial port
 *   3. Frees up the Arduino pins for other uses
 *   4. Does NOT delete the object (still exists, just disabled)
 * 
 * Note:
 *   - The HC-06 object still exists, but communication is stopped
 *   - Call begin() again to restart communication
 *   - Not normally needed (the destructor handles cleanup)
 */
void HC06::end()
{
    // Safety check: verify the SoftwareSerial object exists
    // Calling methods on null pointer = crash
    if (_btSerial != nullptr)
    {
        // Disable the serial communication
        // This stops the software UART and frees up pins
        _btSerial->end();
    }
}

/**
 * METHOD: void HC06::reset()
 * ──────────────────────────
 * 
 * Performs a hardware reset of the HC-06 module
 * 
 * How it works:
 *   - HC-06 modules can be reset by toggling their RX/TX lines
 *   - This simulates a power cycle, resetting the module's state
 *   - Useful for recovering from communication errors
 * 
 * What happens:
 *   1. Set RX and TX pins as OUTPUT (not input)
 *   2. Pull both pins LOW (logical 0 / 0V) - simulates power off
 *   3. Wait 100ms for the module to fully power down
 *   4. Pull pins HIGH (logical 1 / 5V) - restarts the module
 * 
 * WARNING:
 *   - After reset(), reassign pins with begin() before communication
 *   - Do not call this while communicating (data loss likely)
 *   - 100ms delay is empirically determined for reliable resets
 * 
 * Technical note:
 *   - pinMode(pin, OUTPUT) enables the Arduino to drive the pin
 *   - digitalWrite LOW = drives pin to 0V
 *   - digitalWrite HIGH = drives pin to 5V
 */
void HC06::reset()
{
    if (_btSerial == nullptr)
    {
        return;
    }

    _btSerial->end();

    if (_hc06 >= 0)
    {
        pinMode(_hc06, OUTPUT);
        digitalWrite(_hc06, LOW);
        delay(100);
        digitalWrite(_hc06, HIGH);
    }

    delay(100);
    _btSerial->begin(_baudRate);
}

/**
 * METHOD: void HC06::attempt()
 * ─────────────────────────────
 * 
 * Diagnostic method: reads and prints incoming data to Serial monitor
 * 
 * Use case:
 *   - Debugging: verify that data is arriving from the HC-06
 *   - Testing: confirm Bluetooth connection is working
 *   - Learning: see what data is being received in real-time
 * 
 * What happens:
 *   1. Check if data is available in the SoftwareSerial buffer
 *   2. If available: read one line of data (up to '\n')
 *   3. Print to Serial monitor with "Received: " prefix
 *   4. If no data: this method does nothing (non-blocking)
 * 
 * To use:
 *   void loop() {
 *       bluetooth.attempt();    // Call repeatedly to check for data
 *       delay(100);
 *   }
 *   // Open Serial monitor to see received messages
 * 
 * Note:
 *   - Serial.begin(9600) must be called in setup()
 *   - Same baud rate (9600) for both Serial and HC-06 is recommended
 */
void HC06::attempt()
{
    // Safety check: verify the SoftwareSerial object exists
    // Also check if data is available (two conditions with &&)
    if (_btSerial != nullptr && _btSerial->available())
    {
        // Read all characters until newline ('\n') is encountered
        // This gets one complete message from the HC-06
        String data = _btSerial->readStringUntil('\n');
        
        // Print "Received: " prefix to Serial monitor
        // This helps identify the data source in the output
        Serial.print("Received: ");
        
        // Print the actual received message data
        // Serial.println adds a newline after printing
        Serial.println(data);
    }
}

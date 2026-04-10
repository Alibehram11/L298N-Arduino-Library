/*
 * ═════════════════════════════════════════════════════════════════════════════
 * EXAMPLE 3: RadioLinkI2CReceiver.ino
 * I2C-Based Radio Link Receiver - Data Packet Reading & Signal Monitoring
 * ═════════════════════════════════════════════════════════════════════════════
 *
 * OVERVIEW:
 *   This example demonstrates a complete I2C radio receiver interface:
 *   - Data packet reception and parsing
 *   - Real-time signal strength (RSSI) monitoring
 *   - Link quality assessment
 *   - Packet statistics and error tracking
 *   - Non-blocking event-driven reception
 *   - Signal visualization and diagnostics
 *
 * SUPPORTED MODULES:
 *   ✓ NRF24L01 (with I2C SPI bridge adapter)
 *   ✓ LoRa modules with I2C interface
 *   ✓ Any RF receiver module with I2C communication
 *
 * HARDWARE SETUP:
 *   Arduino I2C (TWI):
 *   ├─ Arduino SDA (Analog Pin 4 on Uno) ──→ Module SDA
 *   ├─ Arduino SCL (Analog Pin 5 on Uno) ──→ Module SCL
 *   ├─ Arduino GND ──→ Module GND
 *   └─ Arduino 5V ──→ Module VCC (check module voltage requirements)
 *
 *   Arduino Pin Assignments:
 *   ├─ Pin A4 (SDA) – I2C Data Line
 *   ├─ Pin A5 (SCL) – I2C Clock Line
 *   └─ (Wire.h handles I2C automatically)
 *
 *   Alternate I2C on Mega: SDA=Pin 20, SCL=Pin 21
 *
 * MODULE I2C ADDRESS:
 *   Common I2C addresses for RF modules:
 *   - 0x20: NRF24L01 (default, adjustable via module configuration)
 *   - 0x60: LoRa modules (typical)
 *   - 0x40-0x7F: Other RF receivers (check datasheet)
 *
 * USAGE SCENARIO:
 *   This is ideal for:
 *   ✓ Remote controlled robot receiver
 *   ✓ Wireless sensor data collection
 *   ✓ IoT data aggregator node
 *   ✓ Smart car navigation system
 *   ✓ Multi-robot coordinated control
 *
 * SERIAL OUTPUT (9600 baud):
 *   ├─ Real-time packet reception
 *   ├─ Signal strength and quality metrics
 *   ├─ Packet statistics and error rates
 *   ├─ Connection health indicators
 *   ├─ Debug information on command
 *   └─ Automatic loss-of-signal alerts
 *
 * FEATURES DEMONSTRATED:
 *   ✓ Non-blocking packet reception (event-driven)
 *   ✓ RSSI (signal strength) monitoring
 *   ✓ Link quality percentage calculation
 *   ✓ Packet counting and error tracking
 *   ✓ Connection health assessment (0-4 scale)
 *   ✓ Channel/frequency management
 *   ✓ Transmit power levels
 *   ✓ Self-test diagnostics
 *
 * ═════════════════════════════════════════════════════════════════════════════
 */

#include <arduino/arduino.h>

// ───────────────────────────────────────────────────────────────────────────────
// RADIO MODULE CONFIGURATION
// ───────────────────────────────────────────────────────────────────────────────

// Create radio receiver object with I2C address 0x20 (typical NRF24L01 default)
// Change address to match your specific module configuration
RadioLinkI2C radio(0x20);

// Maximum packet size for this receiver module
const uint8_t MAX_RX_BUFFER = 32;

// ───────────────────────────────────────────────────────────────────────────────
// APPLICATION PARAMETERS
// ───────────────────────────────────────────────────────────────────────────────

// Status LED indicators (optional, adjust pins as needed)
const int LED_RX = 13;        // Green LED - Packet received
const int LED_NO_SIGNAL = 12; // Yellow LED - Weak signal
const int LED_ERROR = 11;     // Red LED - Communication error

// Timing parameters
unsigned long lastStatusTime = 0;
const unsigned long STATUS_INTERVAL = 1000;  // Print status every 1 second

unsigned long lastHealthCheckTime = 0;
const unsigned long HEALTH_CHECK_INTERVAL = 5000;  // Health check every 5 seconds

// Data reception counters
struct ReceptionStats {
    unsigned long lastPacketTime;
    int lastRSSI;
    uint8_t lastLQI;
    uint16_t totalPacketsReceived;
    bool signalDetected;
} rxStats;

// Command interpretation buffer
uint8_t commandBuffer[32];
uint8_t commandLength = 0;

// ───────────────────────────────────────────────────────────────────────────────
// SETUP()
// ───────────────────────────────────────────────────────────────────────────────

void setup()
{
    // Initialize serial communication
    Serial.begin(9600);

    // Wait for serial connection
    delay(500);

    // Initialize LED pins
    pinMode(LED_RX, OUTPUT);
    pinMode(LED_NO_SIGNAL, OUTPUT);
    pinMode(LED_ERROR, OUTPUT);

    // Turn off all LEDs initially
    digitalWrite(LED_RX, LOW);
    digitalWrite(LED_NO_SIGNAL, LOW);
    digitalWrite(LED_ERROR, LOW);

    // Print startup banner
    printBanner();

    // Attempt to initialize radio module
    Serial.println("\n[INITIALIZING RADIO MODULE]");
    Serial.println("I2C Address: 0x20");
    Serial.println("Waiting for module response...\n");

    if (radio.begin())
    {
        Serial.println("✓ Radio module initialized successfully!");
        digitalWrite(LED_RX, HIGH);  // Light up to indicate ready
        delay(200);
        digitalWrite(LED_RX, LOW);

        // Configure radio for operation
        configureRadio();

        // Print initial status
        printRadioStatus();

        // Run self-test
        Serial.println("\n[SELF-TEST DIAGNOSTICS]");
        if (radio.runSelfTest())
        {
            Serial.println("✓ All self-tests passed!");
        }
        else
        {
            Serial.println("⚠ Some self-tests failed - check wiring");
        }

        rxStats.totalPacketsReceived = 0;
        rxStats.signalDetected = false;
        rxStats.lastRSSI = -120;
        rxStats.lastLQI = 0;

        Serial.println("\nListening for packets...\n");
    }
    else
    {
        Serial.println("✗ Failed to initialize radio module!");
        Serial.println("Possible causes:");
        Serial.println("  - I2C address mismatch");
        Serial.println("  - Wiring not connected properly");
        Serial.println("  - Module not powered");
        Serial.println("  - Module firmware issue\n");

        // Flash error LED
        for (int i = 0; i < 5; i++)
        {
            digitalWrite(LED_ERROR, HIGH);
            delay(100);
            digitalWrite(LED_ERROR, LOW);
            delay(100);
        }
    }
}

// ───────────────────────────────────────────────────────────────────────────────
// LOOP()
// ───────────────────────────────────────────────────────────────────────────────

void loop()
{
    // Check if radio is still active
    if (!radio.isModuleActive())
    {
        Serial.println("\n✗ Radio module communication lost!");
        digitalWrite(LED_ERROR, HIGH);
        delay(100);
        digitalWrite(LED_ERROR, LOW);
        delay(100);
        return;
    }

    // Check for incoming data packets
    if (radio.isDataAvailable())
    {
        receiveAndProcessPacket();
    }

    // Periodic status reporting
    unsigned long currentTime = millis();

    if (currentTime - lastStatusTime >= STATUS_INTERVAL)
    {
        lastStatusTime = currentTime;
        printPeriodicStatus();
    }

    // Periodic link health check
    if (currentTime - lastHealthCheckTime >= HEALTH_CHECK_INTERVAL)
    {
        lastHealthCheckTime = currentTime;
        performHealthCheck();
    }

    // Handle serial commands
    if (Serial.available())
    {
        handleSerialCommand();
    }

    // Small delay to prevent CPU spinning
    delay(10);
}

// ───────────────────────────────────────────────────────────────────────────────
// PACKET RECEPTION & PROCESSING
// ───────────────────────────────────────────────────────────────────────────────

/**
 * Receive and process incoming data packet
 */
void receiveAndProcessPacket()
{
    uint8_t rxBuffer[MAX_RX_BUFFER];
    int16_t rssi;
    uint8_t lqi;

    // Read packet with metadata
    uint8_t rxLen = radio.readDataPacketWithMetadata(rxBuffer, MAX_RX_BUFFER, rssi, lqi);

    if (rxLen > 0)
    {
        // Update statistics
        rxStats.totalPacketsReceived++;
        rxStats.lastRSSI = rssi;
        rxStats.lastLQI = lqi;
        rxStats.lastPacketTime = millis();
        rxStats.signalDetected = true;

        // Light up RX indicator briefly
        digitalWrite(LED_RX, HIGH);
        delay(50);
        digitalWrite(LED_RX, LOW);

        // Print packet information
        Serial.print("\n[PACKET RECEIVED] #");
        Serial.print(rxStats.totalPacketsReceived);
        Serial.print(" (");
        Serial.print(rxLen);
        Serial.println(" bytes)");

        Serial.print("├─ RSSI: ");
        Serial.print(rssi);
        Serial.println(" dBm");

        Serial.print("├─ Link Quality: ");
        Serial.print(lqi);
        Serial.println("%");

        // Decode packet content
        Serial.print("├─ Data: ");
        for (uint8_t i = 0; i < rxLen; i++)
        {
            Serial.write(rxBuffer[i]);
        }
        Serial.println();

        // Interpret as ASCII command if applicable
        if (isASCIICommand(rxBuffer, rxLen))
        {
            Serial.print("└─ Interpreted as: ");
            interpretCommand(rxBuffer, rxLen);
        }
        else
        {
            Serial.print("└─ Raw hex: ");
            for (uint8_t i = 0; i < rxLen; i++)
            {
                if (rxBuffer[i] < 16)
                    Serial.print("0");
                Serial.print(rxBuffer[i], HEX);
                Serial.print(" ");
            }
            Serial.println();
        }
    }
}

/**
 * Check if received data is ASCII-compatible
 */
bool isASCIICommand(uint8_t* data, uint8_t len)
{
    for (uint8_t i = 0; i < len; i++)
    {
        if (data[i] > 127 || data[i] == 0)
            return false;
    }
    return true;
}

/**
 * Interpret received command
 */
void interpretCommand(uint8_t* data, uint8_t len)
{
    // Treat data as string
    if (len > 1 && data[0] == 'F')  // Forward command example
    {
        Serial.println("FORWARD motion command");
    }
    else if (len > 1 && data[0] == 'B')  // Backward command
    {
        Serial.println("BACKWARD motion command");
    }
    else if (len > 1 && data[0] == 'R')  // Right turn
    {
        Serial.println("RIGHT turn command");
    }
    else if (len > 1 && data[0] == 'L')  // Left turn
    {
        Serial.println("LEFT turn command");
    }
    else if (len > 1 && data[0] == 'S')  // Stop
    {
        Serial.println("STOP command");
    }
    else
    {
        Serial.print("Custom: ");
        Serial.write(data[0]);
        Serial.println();
    }
}

// ───────────────────────────────────────────────────────────────────────────────
// STATUS & DIAGNOSTICS
// ───────────────────────────────────────────────────────────────────────────────

/**
 * Print periodic status information
 */
void printPeriodicStatus()
{
    int16_t currentRSSI = radio.getRSSI();
    uint8_t linkQuality = radio.getLinkQuality();
    uint8_t connectionHealth = radio.getConnectionHealth();

    // Signal strength visualization
    Serial.print("\n[STATUS] RSSI: ");
    Serial.print(currentRSSI);
    Serial.print(" dBm | Quality: ");
    Serial.print(linkQuality);
    Serial.print("% | Health: ");
    Serial.print(connectionHealth);
    Serial.print("/4 | RX: ");
    Serial.println(rxStats.totalPacketsReceived);

    // Visual signal strength bar
    printSignalStrengthBar(linkQuality);

    // Connection status indicator
    if (linkQuality > 75)
    {
        digitalWrite(LED_RX, HIGH);  // Strong signal - light up
    }
    else if (linkQuality > 25)
    {
        // Blink for medium signal
        digitalWrite(LED_NO_SIGNAL, HIGH);
        delay(100);
        digitalWrite(LED_NO_SIGNAL, LOW);
    }
    else if (linkQuality > 0)
    {
        digitalWrite(LED_NO_SIGNAL, HIGH);  // Weak signal - steady
    }
    else
    {
        digitalWrite(LED_RX, LOW);
        digitalWrite(LED_NO_SIGNAL, LOW);
    }
}

/**
 * Print signal strength as ASCII bar
 */
void printSignalStrengthBar(uint8_t quality)
{
    Serial.print("Signal: [");

    uint8_t bars = (quality + 9) / 10;  // Convert 0-100 to 0-10
    for (uint8_t i = 0; i < 10; i++)
    {
        if (i < bars)
            Serial.write(0xDB);  // Full block character
        else
            Serial.write(0xB0);  // Light shade character
    }

    Serial.println("]");
}

/**
 * Perform comprehensive health check
 */
void performHealthCheck()
{
    Serial.println("\n[LINK HEALTH CHECK]");

    uint8_t health = radio.getConnectionHealth();

    Serial.print("Connection Status: ");
    switch (health)
    {
    case 0:
        Serial.println("NO CONNECTION (--) - Check signal");
        digitalWrite(LED_ERROR, HIGH);
        break;

    case 1:
        Serial.println("WEAK (▁) - Move closer to transmitter");
        digitalWrite(LED_NO_SIGNAL, HIGH);
        break;

    case 2:
        Serial.println("FAIR (▂) - Acceptable but unreliable");
        digitalWrite(LED_NO_SIGNAL, HIGH);
        break;

    case 3:
        Serial.println("GOOD (▃) - Reliable connection");
        digitalWrite(LED_RX, HIGH);
        break;

    case 4:
        Serial.println("EXCELLENT (▄) - Very stable link");
        digitalWrite(LED_RX, HIGH);
        break;
    }

    // Print detailed statistics
    uint16_t rxCount = radio.getPacketsReceived();
    uint16_t errorCount = radio.getErrorCount();
    uint8_t errorRate = radio.getPacketErrorRate();

    Serial.print("├─ RX Packets: ");
    Serial.println(rxCount);

    Serial.print("├─ Errors: ");
    Serial.print(errorCount);
    Serial.print(" (");
    Serial.print(errorRate);
    Serial.println("%)");

    Serial.print("└─ Last Packet: ");
    if (rxStats.lastPacketTime > 0)
    {
        unsigned long timeSinceLastPacket = millis() - rxStats.lastPacketTime;
        Serial.print(timeSinceLastPacket);
        Serial.println(" ms ago");
    }
    else
    {
        Serial.println("Never");
    }
}

/**
 * Print initial radio configuration
 */
void printRadioStatus()
{
    Serial.println("\n[RADIO CONFIGURATION]");
    Serial.print("├─ Channel: ");
    Serial.println(radio.getChannel());

    Serial.print("├─ TX Power: ");
    Serial.println(radio.getTransmitPower());

    Serial.print("├─ Firmware: v");
    Serial.println(radio.getModuleVersion());

    Serial.println("└─ Status: READY");
}

/**
 * Print startup banner
 */
void printBanner()
{
    Serial.println("╔═══════════════════════════════════════════════════╗");
    Serial.println("║  I2C Radio Link Receiver Example                 ║");
    Serial.println("║     Data Reception & Signal Monitoring           ║");
    Serial.println("╚═══════════════════════════════════════════════════╝");
}

// ───────────────────────────────────────────────────────────────────────────────
// SERIAL COMMAND INTERFACE
// ───────────────────────────────────────────────────────────────────────────────

/**
 * Handle serial commands from user
 */
void handleSerialCommand()
{
    char cmd = Serial.read();

    Serial.print("\n[COMMAND: ");
    Serial.write(cmd);
    Serial.println("]");

    switch (cmd)
    {
    case 'i':
        Serial.println(radio.getDebugInfo());
        break;

    case 'c':
        performChannelScan();
        break;

    case 's':
        Serial.println("\n[SYSTEM SELF-TEST]");
        if (radio.runSelfTest())
        {
            Serial.println("✓ Self-test: PASSED");
        }
        else
        {
            Serial.println("✗ Self-test: FAILED");
        }
        break;

    case 'r':
        Serial.println("Resetting statistics...");
        radio.clearStatistics();
        rxStats.totalPacketsReceived = 0;
        Serial.println("✓ Statistics cleared");
        break;

    case 'd':
        radio.enableDebugMode();
        Serial.println("Debug mode: ENABLED");
        break;

    case 'q':
        radio.disableDebugMode();
        Serial.println("Debug mode: DISABLED");
        break;

    case 'h':
        printCommandHelp();
        break;

    default:
        Serial.println("Unknown command. Type 'h' for help.");
        break;
    }

    Serial.println();
}

/**
 * Print command help
 */
void printCommandHelp()
{
    Serial.println("\n╔═════════════════════════════════════╗");
    Serial.println("║  COMMAND REFERENCE                  ║");
    Serial.println("╠═════════════════════════════════════╣");
    Serial.println("║  i: Print debug information         ║");
    Serial.println("║  c: Channel scan (find signal)      ║");
    Serial.println("║  s: System self-test                ║");
    Serial.println("║  r: Reset statistics                ║");
    Serial.println("║  d: Enable debug mode               ║");
    Serial.println("║  q: Disable debug mode              ║");
    Serial.println("║  h: Show this help screen           ║");
    Serial.println("╚═════════════════════════════════════╝\n");
}

/**
 * Scan I2C channels to find active signal
 */
void performChannelScan()
{
    Serial.println("\n[CHANNEL SCAN - Searching for signals]");
    Serial.println("Current Channel: " + String(radio.getChannel()));

    uint8_t bestChannel = 0;
    int16_t bestRSSI = -120;

    // Scan from channel 0 to 125 (NRF24L01 typical)
    for (uint8_t ch = 0; ch < 126; ch++)
    {
        radio.setChannel(ch);
        delay(50);  // Let PLL lock

        int16_t rssi = radio.getRSSI();

        if (rssi > bestRSSI)
        {
            bestRSSI = rssi;
            bestChannel = ch;
        }

        // Print progress every 10 channels
        if (ch % 10 == 0)
        {
            Serial.print(".");
        }
    }

    // Restore best channel
    radio.setChannel(bestChannel);
    Serial.print("\n✓ Best signal found on channel ");
    Serial.print(bestChannel);
    Serial.print(" (RSSI: ");
    Serial.print(bestRSSI);
    Serial.println(" dBm)");
}

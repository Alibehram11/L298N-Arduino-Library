/*
 * ═════════════════════════════════════════════════════════════════════════════
 * ARCHITECTURAL DOCUMENTATION
 * Arduino Car Library - Module Design & Implementation Guide
 * ═════════════════════════════════════════════════════════════════════════════
 * 
 * This document describes the design philosophy, architectural patterns,
 * and implementation details of the enhanced Arduino Car Library.
 * 
 * Version: 2.1.0
 * Date: April 2026
 * Target: Arduino Uno/Nano/Mega with C++11 standard
 */

///////////////////////////////////////////////////////////////////////////////
// MODULE ARCHITECTURE OVERVIEW
///////////////////////////////////////////////////////////////////////////////

/*
 * The Arduino Car Library is organized into modular, self-contained components
 * that abstract hardware details while maintaining simplicity and efficiency.
 * 
 * DESIGN PRINCIPLES:
 * 
 * 1. ABSTRACTION LAYERS
 *    ├─ Hardware Layer: Direct pin control, timing, communication
 *    ├─ Logic Layer: Filtering, conversion, state management
 *    └─ Interface Layer: Public API, semantic methods
 * 
 * 2. SEPARATION OF CONCERNS
 *    Each module has single responsibility:
 *    • Motor: Movement control only
 *    • HC-06: Bluetooth communication only
 *    • HCSR04: Distance measurement only
 *    • RadioLinkI2C: RF data reception only
 * 
 * 3. NO DEPENDENCIES BETWEEN MODULES
 *    Modules can be used independently without linking others.
 *    This minimizes code footprint for constrained environments.
 * 
 * 4. CONSISTENT INTERFACE PATTERNS
 *    All modules follow similar method naming and error handling:
 *    • Sensor modules: read/measure/get* methods
 *    • Configuration: set* methods
 *    • Status: is*, get* methods
 *    • Diagnostics: get*Info, run*Test methods
 */

///////////////////////////////////////////////////////////////////////////////
// MODULE 1: HC-SR04 ULTRASONIC SENSOR (ENHANCED)
///////////////////////////////////////////////////////////////////////////////

/*
 * HCSR04 CLASS - Advanced Ultrasonic Distance Measurement
 * Location: include/arduino/hcsr-04.h, src/hcsr-04.cpp
 * 
 * PURPOSE:
 *   Provide accurate, noise-resistant distance measurements from HC-SR04
 *   ultrasonic sensors for obstacle detection and proximity sensing.
 * 
 * KEY ENHANCEMENTS vs. Basic Implementation:
 *   ✓ Noise Filtering: Multi-sample averaging with median absolute deviation
 *   ✓ Async Support: Non-blocking measurement for parallel processing
 *   ✓ Statistics: Track min/max/average and signal quality
 *   ✓ Configuration: Runtime tuning of range, threshold, timeout
 *   ✓ Diagnostics: Comprehensive debug information and validation
 * 
 * ARCHITECTURE:
 * 
 *   Measurement Pipeline:
 *   ┌─ Raw Measurement
 *   │  └─ Send 10µs trigger pulse
 *   │  └─ Measure echo duration (microseconds)
 *   │
 *   ├─ Validation
 *   │  └─ Check timeout and range limits
 *   │
 *   ├─ Conversion
 *   │  └─ Calculate distance: (duration / 2) / 29.1 cm
 *   │
 *   └─ Filtering (Optional)
 *      └─ Multi-sample averaging with outlier rejection
 * 
 * USAGE PATTERNS:
 * 
 *   1. Single Measurement (Fastest, most noise-prone)
 *      long distance = sensor.measureDistance();
 * 
 *   2. Averaged Measurement (Balanced, recommended)
 *      long distance = sensor.measureDistanceAveraged(5);
 * 
 *   3. Asynchronous Measurement (Non-blocking)
 *      sensor.startAsyncMeasurement();
 *      if (sensor.isAsyncMeasurementReady()) {
 *          long distance = sensor.readAsyncDistance();
 *      }
 * 
 * FILTERING ALGORITHM:
 *   Median Absolute Deviation (MAD) Method:
 *   1. Collect N samples
 *   2. Calculate median
 *   3. For each sample: calculate deviation from median
 *   4. Reject samples with deviation > threshold
 *   5. Average remaining samples
 *   
 *   Advantage: Robust against outliers while preserving true changes
 *   Use case: Obstacle detection, level monitoring
 * 
 * PERFORMANCE CHARACTERISTICS:
 *   • Single measurement: ~60 ms (hardware timing limitation)
 *   • Async measurement: <100 µs setup overhead
 *   • Memory overhead: ~50 bytes static variables
 *   • CPU: Minimal (mainly delegated to pulseIn() function)
 * 
 * CONFIGURATION PARAMETERS:
 *   setNoiseThreshold(cm)        - Deviation limit for filtering (default: 5 cm)
 *   setValidRange(min, max)      - Acceptance range (default: 2-400 cm)
 *   setEchoTimeout(micros)       - Max echo wait time (default: 30000 µs)
 *   setNoiseFilteringEnabled()   - Enable/disable filtering (default: ON)
 */

///////////////////////////////////////////////////////////////////////////////
// MODULE 2: RADIO LINK I2C RECEIVER
///////////////////////////////////////////////////////////////////////////////

/*
 * RADIOLINKI2C CLASS - I2C-Based RF Module Interface
 * Location: include/arduino/radio-link-i2c.h, src/radio-link-i2c.cpp
 * 
 * PURPOSE:
 *   Generic abstraction layer for I2C-connected wireless receivers
 *   (NRF24L01, LoRa, etc.) with signal monitoring and diagnostics.
 * 
 * DESIGN APPROACH:
 *   Rather than specify a particular RF module, this class provides
 *   a generic I2C interface that works with multiple hardware options.
 *   Users configure the I2C address and register map for their module.
 * 
 * ARCHITECTURE:
 * 
 *   I2C Communication Stack:
 *   ┌─────────────────────────┐
 *   │  Public API Methods     │  (readDataPacket, getRSSI, etc.)
 *   ├─────────────────────────┤
 *   │  Protocol Layer         │  (Packet framing, error checking)
 *   ├─────────────────────────┤
 *   │  Register Interface     │  (readRegister, writeRegister)
 *   ├─────────────────────────┤
 *   │  I2C Driver (Wire.h)    │  (Hardware I2C layer)
 *   ├─────────────────────────┤
 *   │  RF Module Hardware     │  (NRF24L01, LoRa, etc.)
 *   └─────────────────────────┘
 * 
 * I2C REGISTER MAP (Generic Template):
 *   0x00 - Status register         (RX data available flags)
 *   0x01 - Configuration           (Mode, interrupt enables)
 *   0x02 - TX Power Level          (0-3 scale)
 *   0x03 - RF Channel              (0-125 for NRF24L01)
 *   0x04 - RSSI                    (Signal strength reading)
 *   0x05 - Link Quality Indicator  (0-100%)
 *   0x10 - RX Packet Length        (Bytes in RX buffer)
 *   0x11 - RX Data Payload         (Packet content)
 *   0x20 - RX Count                (Received packet statistics)
 *   0x22 - TX Count                (Transmitted packet statistics)
 *   0x24 - Error Count             (CRC/corruption failures)
 *   0x30 - Module Version          (Firmware version)
 * 
 * PACKET RECEPTION FLOW:
 *   1. Poll isDataAvailable() → checks STATUS register bit
 *   2. Read packet length from REG_RX_LENGTH
 *   3. Read data from REG_RX_DATA (sequential register read)
 *   4. Read metadata (RSSI, LQI) from adjacent registers
 *   5. Process command interpretation (ASCII or binary)
 * 
 * SIGNAL STRENGTH ANALYSIS:
 *   RSSI (Received Signal Strength Indicator):
 *   -30 to -67 dBm  → Excellent (very strong)
 *   -67 to -70 dBm  → Good (strong)
 *   -70 to -80 dBm  → Fair (moderate)
 *   -80 to -90 dBm  → Weak (poor)
 *   -90 to -120 dBm → Very weak (unreliable)
 *   
 *   Link Quality Integration:
 *   • Combines RSSI with packet error rate
 *   • Accounts for signal variations and environmental interference
 *   • Returns 0-100% quality score
 * 
 * CONNECTION HEALTH STATES:
 *   0/4 → NO CONNECTION    (RSSI < -120 dBm, no recent packets)
 *   1/4 → WEAK             (RSSI -100 to -90 dBm)
 *   2/4 → FAIR             (RSSI -85 to -70 dBm, <10% errors)
 *   3/4 → GOOD             (RSSI -70 to -50 dBm, <5% errors)
 *   4/4 → EXCELLENT        (RSSI > -50 dBm, no errors)
 * 
 * SMOOTHING TECHNIQUES:
 *   Exponential Moving Average (EMA):
 *   RSSI_smoothed = α × RSSI_current + (1-α) × RSSI_smoothed
 *   Where α = 0.3 (70% old value, 30% new value)
 *   
 *   Purpose: Reduce short-term fluctuations while tracking trends
 * 
 * PERFORMANCE CHARACTERISTICS:
 *   • I2C bus speed: 100 kHz (standard mode)
 *   • Max latency per read: ~10 ms (packet-size dependent)
 *   • Memory overhead: ~40 bytes static variables
 *   • Self-test time: ~500 ms (channel scan optional)
 * 
 * CONFIGURATION METHODS:
 *   setChannel(0-125)           - Select RF frequency channel
 *   setTransmitPower(0-3)       - Adjust TX output level
 *   enableDebugMode()           - Module verbose output
 *   runSelfTest()               - Verify I2C communication
 */

///////////////////////////////////////////////////////////////////////////////
// CROSS-CUTTING DESIGN PATTERNS
///////////////////////////////////////////////////////////////////////////////

/*
 * 1. ERROR HANDLING STRATEGY
 *    ├─ Silent Failure Mode (Sensor/RF modules)
 *    │  └─ Returns -1 or 0 on error, continues operation
 *    │  └─ Rationale: Embedded systems need reliability
 *    │  └─ Alternative: Check return values, use getSignalQuality()
 *    │
 *    ├─ Status Polling
 *    │  └─ is*() methods check preconditions
 *    │  └─ Example: isDataAvailable() before readDataPacket()
 *    │
 *    └─ Graceful Degradation
 *       └─ System continues at reduced capability on error
 *       └─ LEDs, statistics, and debug info help diagnose issues
 * 
 * 2. MEMORY OPTIMIZATION
 *    ├─ Static Allocation (No new/delete)
 *    │  └─ Reduces heap fragmentation on resource-constrained MCU
 *    │
 *    ├─ Byte-Sized Counters Where Possible
 *    │  └─ uint8_t for quality (0-100%), status flags
 *    │  └─ uint16_t for statistics to avoid overflow
 *    │
 *    └─ Lazy Loading
 *       └─ Statistics updated only when needed
 *       └─ Debug strings generated on-demand
 * 
 * 3. STATE MANAGEMENT
 *    ├─ Async Operations (Finite State Machine)
 *    │  ├─ IDLE → START → IN_PROGRESS → READY → READ
 *    │  └─ Allows polling without blocking
 *    │
 *    ├─ Statistics Accumulation
 *    │  ├─ Running counters in separate variables
 *    │  ├─ Snapshots for batch operations
 *    │  └─ clearStatistics() method for resets
 *    │
 *    └─ Configuration Persistence
 *       └─ Settings maintained across measurements
 *       └─ Allows runtime reconfiguration
 * 
 * 4. DIAGNOSTIC FRAMEWORK
 *    ├─ Metadata Recording
 *    │  ├─ Last measurement quality
 *    │  ├─ Sample counts and validity ratios
 *    │  └─ Timestamps for age tracking
 *    │
 *    ├─ String Generation
 *    │  ├─ Format: String class for easy Serial output
 *    │  ├─ No file I/O (embedded systems)
 *    │  └─ Minimal string allocations
 *    │
 *    └─ Self-Testing
 *       ├─ Register read/write verification
 *       ├─ Range checks on sensor outputs
 *       └─ Pattern verification on configurations
 * 
 * 5. CONFIGURATION VALIDATION
 *    ├─ Type Safety (Strong typing)
 *    │  └─ uint8_t for percentages prevents invalid values
 *    │
 *    ├─ Range Checking
 *    │  ├─ All setters validate input ranges
 *    │  ├─ Example: setTransmitPower(powerLevel) rejects > 3
 *    │  └─ Silent failure: Invalid value rejected, previous retained
 *    │
 *    └─ Consistency Enforcement
 *       └─ setValidRange(min, max) ensures min < max
 */

///////////////////////////////////////////////////////////////////////////////
// INTEGRATION GUIDELINES
///////////////////////////////////////////////////////////////////////////////

/*
 * COMBINING MODULES IN APPLICATIONS:
 * 
 * Example 1: Autonomous Navigation Robot
 *   ├─ Motor: Movement control
 *   ├─ HCSR04: Obstacle detection (averaged mode)
 *   ├─ HC-06: Telemetry uplink (status telemetry)
 *   └─ RadioLinkI2C: Command reception (remote control)
 * 
 * Example 2: Remote Sensor Node
 *   ├─ RadioLinkI2C: Data reception (I2C, non-blocking)
 *   ├─ HCSR04: Environment monitoring (async measurement)
 *   └─ HC-06: Status reporting via Bluetooth
 * 
 * Example 3: Smart Parking System
 *   ├─ HCSR04 (2-4 units): Distance sensors per parking space
 *   ├─ RadioLinkI2C: IoT uplink (availability status)
 *   └─ Motor: Display/barrier mechanism control
 * 
 * RESOURCE SHARING CONSIDERATIONS:
 * 
 *   I2C Bus Contention:
 *   • Multiple RadioLinkI2C modules on same I2C bus (different addresses)
 *   • Use separate Wire instances if needed (Wire vs Wire1 on Mega)
 *   • Keep I2C clock speed ≤ 400 kHz (fast mode) for reliability
 * 
 *   Timer/Interrupt Timing:
 *   • HC-SR04 uses micros() for echo measurement
 *   • Async operations use millis() for state timeouts
 *   • No timer conflicts if each module uses appropriate time scale
 * 
 *   Pin Conflicts:
 *   • Verify no duplicate pin assignments
 *   • PWM-capable pins needed for Motor PWM
 *   • GPIO pins adequate for sensor trigger signals
 * 
 *   Power Supply:
 *   • RF modules draw 10-30 mA at full power
 *   • Multiple sensors require adequate 5V rail
 *   • Use separate power rails for high-current motors
 */

///////////////////////////////////////////////////////////////////////////////
// PERFORMANCE TUNING
///////////////////////////////////////////////////////////////////////////////

/*
 * OPTIMIZATION STRATEGIES:
 * 
 * 1. FOR ACCURACY (Distance Measurement)
 *    ├─ Use measureDistanceAveraged(5-10) with filtering enabled
 *    ├─ Set noiseThreshold to 2-3 cm (tighter filtering)
 *    ├─ Accept longer measurement time (~300 ms for 5 samples)
 *    └─ Result: ±1-2 cm accuracy possible
 * 
 * 2. FOR SPEED (Minimal Latency)
 *    ├─ Use measureDistance() for single measurement
 *    ├─ Or use RadioLinkI2C async mode in loop
 *    ├─ Disable filtering for streaming applications
 *    └─ Result: ~60 ms per measurement (sensor hardware limited)
 * 
 * 3. FOR REAL-TIME RESPONSE (Control Systems)
 *    ├─ Use async measurement mode (startAsyncMeasurement)
 *    ├─ Poll with short loop intervals
 *    ├─ Process data as it becomes available
 *    └─ Result: Main loop remains responsive
 * 
 * 4. FOR SIGNAL STABILITY (Wireless Reception)
 *    ├─ Reduce I2C bus speed if experiencing errors
 *    ├─ Add pull-up resistors if lines are noisy
 *    ├─ Use shielded twisted-pair cabling
 *    ├─ Keep RF module away from power lines
 *    └─ Result: Reliable packet reception in noisy environments
 * 
 * MEMORY FOOTPRINT ANALYSIS:
 * 
 *   HCSR04 Class:
 *   • Static: ~50 bytes (member variables)
 *   • Dynamic: ~100 bytes temporary (sample buffer on stack)
 *   • Total: ~150 bytes per instance (negligible)
 * 
 *   RadioLinkI2C Class:
 *   • Static: ~40 bytes (member variables)
 *   • I2C buffers: ~64 bytes (Wire library internal)
 *   • Total: ~100 bytes per instance
 * 
 *   Example Application: 2x HCSR04 + 1x RadioLinkI2C
 *   Total RAM: ~300 bytes (leaves ~1.4 KB free on Arduino Uno)
 */

///////////////////////////////////////////////////////////////////////////////
// FUTURE ENHANCEMENTS
///////////////////////////////////////////////////////////////////////////////

/*
 * POTENTIAL IMPROVEMENTS:
 *   1. DMA Support for I2C (faster register bulk reads)
 *   2. Interrupt-driven reception (reduce polling latency)
 *   3. Temperature compensation for ultrasonic (improve accuracy)
 *   4. Multi-packet aggregation (reduce I2C overhead)
 *   5. CRC computation for reliability (optional, hardware-dependent)
 *   6. Power management modes (sleep/wake state machine)
 *   7. Circular buffers for packet queuing
 *   8. Configurable data rates for I2C module
 * 
 * EXTENSIBILITY POINTS:
 *   • Virtual base class for sensor interface (multi-module support)
 *   • Template specialization for different RF protocols
 *   • Configuration typedef for compile-time constants
 *   • Callback functions for event notifications
 */

///////////////////////////////////////////////////////////////////////////////
// TESTING & VALIDATION
///////////////////////////////////////////////////////////////////////////////

/*
 * UNIT TEST SCENARIOS:
 * 
 *   HCSR04:
 *   ├─ Valid range measurements (5-400 cm)
 *   ├─ Out-of-range rejection
 *   ├─ Noise filtering effectiveness (inject synthetic noise)
 *   ├─ Averaging convergence (5-point sample set)
 *   ├─ Async state machine transitions
 *   └─ Statistics accumulation and reset
 * 
 *   RadioLinkI2C:
 *   ├─ I2C communication verification (signal on bus)
 *   ├─ Packet reception and CRC
 *   ├─ RSSI measurement range
 *   ├─ Channel switching and persistence
 *   ├─ Link quality percentage calculation
 *   ├─ Connection health state transitions
 *   ├─ Error counting and rate calculation
 *   └─ Self-test execution and reporting
 * 
 *   INTEGRATION:
 *   ├─ Multiple modules on same Arduino
 *   ├─ Timing interference between modules
 *   ├─ Power rail stability under load
 *   └─ RF coherence with motor switching noise
 * 
 * RECOMMENDED TEST EQUIPMENT:
 *   • Oscilloscope (I2C signal verification)
 *   • Logic analyzer (timing analysis)
 *   • Spectrum analyzer (RF interference check)
 *   • Power supply (voltage/current monitoring)
 *   • Arduino Serial Monitor (status/diagnostics)
 */

///////////////////////////////////////////////////////////////////////////////
// CONCLUSION
///////////////////////////////////////////////////////////////////////////////

/*
 * The Arduino Car Library demonstrates professional embedded systems
 * architecture applied to IoT/robotics hardware abstraction:
 * 
 *   ✓ Clean Abstractions   - Hardware details hidden from users
 *   ✓ Robust Design        - Error handling, validation, diagnostics
 *   ✓ Efficient Code       - Minimal memory, optimized algorithms
 *   ✓ Well Documented      - Extensive comments, examples, guides
 *   ✓ Educational Value    - Learn embedded design patterns
 * 
 * Use this library as:
 *   ├─ Working foundation for Arduino robotics projects
 *   ├─ Reference for embedded C++ best practices
 *   ├─ Template for your own hardware abstraction layers
 *   └─ Springboard for advanced applications
 * 
 * For questions, improvements, or contributions:
 *   GitHub: [Project repository URL]
 *   Documentation: See README.md and examples/
 */

cmake_minimum_required(VERSION 3.10)

# Arduino Car Control & Sensor Library
project(ArduinoCar
    VERSION 2.5.0
    DESCRIPTION "Advanced Modular Library: Motor Control + PID + Encoders + Power Management + Diagnostics + Bluetooth + Ultrasonic + I2C Radio"
    LANGUAGES CXX
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define the library target
add_library(arduino-car
    # Motor control
    src/motor.cpp
    
    # Bluetooth communication
    src/hc-06.cpp
    
    # Ultrasonic distance sensor (enhanced with filtering)
    src/hcsr-04.cpp
    
    # I2C radio link receiver
    src/radio-link-i2c.cpp
    
    # ═══════════════════════════════════════════════════════════════════════════
    # NEW ADVANCED FEATURES
    # ═══════════════════════════════════════════════════════════════════════════
    
    # PID motor speed controller
    src/pid-controller.cpp
    
    # QTR line following sensor support
    src/qtr-sensor.cpp
    
    # Motor encoder feedback system
    src/motor-encoder.cpp
    
    # Configuration manager (EEPROM)
    src/config-manager.cpp
    
    # Speed ramping (smooth acceleration/deceleration)
    src/speed-ramper.cpp
    
    # Power management (battery monitoring)
    src/power-management.cpp
    
    # System diagnostics and error logging
    src/diagnostics.cpp
    
    # Motor encoder feedback
    src/encoder.cpp
)

# Public header files (visible to users of the library)
target_include_directories(arduino-car PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Private header files (only used internally)
target_include_directories(arduino-car PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/arduino
)

# Library properties
set_target_properties(arduino-car PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 2
    PUBLIC_HEADER 
        "include/arduino/motor.h;include/arduino/hc-06.h;include/arduino/hcsr-04.h;include/arduino/radio-link-i2c.h;include/arduino/pid-controller.h;include/arduino/qtr-sensor.h;include/arduino/motor-encoder.h;include/arduino/config-manager.h;include/arduino/speed-ramper.h;include/arduino/power-management.h;include/arduino/diagnostics.h;include/arduino/encoder.h;include/arduino/arduino.h"
)

# Installation configuration
install(TARGETS arduino-car
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include/arduino
)

# Optional: Enable testing
enable_testing()

# Optional: Build examples if needed
option(BUILD_EXAMPLES "Build example sketches" OFF)
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Print configuration summary
message(STATUS "")
message(STATUS "╔════════════════════════════════════════╗")
message(STATUS "║  Arduino Car Library Configuration     ║")
message(STATUS "╠════════════════════════════════════════╣")
message(STATUS "║  Version: ${PROJECT_VERSION}")
message(STATUS "║  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "│")
message(STATUS "║  Modules Included:")
message(STATUS "║  ├─ Motor Control (L298N)")
message(STATUS "║  ├─ Bluetooth Module (HC-06)")
message(STATUS "║  ├─ Ultrasonic Sensor (HC-SR04) *Enhanced")
message(STATUS "║  ├─ I2C Radio Link *New")
message(STATUS "║  ├─ PID Motor Speed Controller *New")
message(STATUS "║  ├─ QTR Line Following Sensors *New")
message(STATUS "║  ├─ Motor Encoder Feedback *New")
message(STATUS "║  ├─ Configuration Manager *New")
message(STATUS "║  └─ Speed Ramping *New")
message(STATUS "│")
message(STATUS "║  Examples: " )
message(STATUS "║  ├─ BasicCarControl.ino")
message(STATUS "║  ├─ BluetoothControl.ino")
message(STATUS "║  ├─ AdvancedNavigation.ino")
message(STATUS "║  ├─ AdvancedUltrasonicSensing.ino *New")
message(STATUS "║  └─ RadioLinkI2CReceiver.ino *New")
message(STATUS "║")
message(STATUS "║  Features:")
message(STATUS "║  ├─ Noise filtering & signal averaging")
message(STATUS "║  ├─ Non-blocking async measurement")
message(STATUS "║  ├─ I2C signal monitoring (RSSI/LQI)")
message(STATUS "║  └─ Comprehensive diagnostics")
message(STATUS "╚════════════════════════════════════════╝")
message(STATUS "")


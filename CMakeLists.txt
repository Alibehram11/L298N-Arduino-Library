cmake_minimum_required(VERSION 3.10)

# Arduino Car Control & Sensor Library
project(ArduinoCar
    VERSION 2.1.0
    DESCRIPTION "Modular Library: Motor Control + Bluetooth + Ultrasonic + I2C Radio for Arduino"
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
        "include/arduino/motor.h;include/arduino/hc-06.h;include/arduino/hcsr-04.h;include/arduino/radio-link-i2c.h;include/arduino/arduino.h"
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
message(STATUS "║  └─ I2C Radio Link *New")
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


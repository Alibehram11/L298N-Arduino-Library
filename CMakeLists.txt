cmake_minimum_required(VERSION 3.10)

# Arduino Car Control Library
project(ArduinoCar
    VERSION 2.0.0
    DESCRIPTION "L298N Motor Control + HC-06 Bluetooth Library for Arduino"
    LANGUAGES CXX
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define the library target
add_library(arduino-car
    # Source files
    src/motor.cpp
    src/hc-06.cpp
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
        "include/arduino/motor.h;include/arduino/hc-06.h;include/arduino/arduino.h"
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

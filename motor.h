#ifndef ARDUINO_CAR_H
#define ARDUINO_CAR_H

#include <Arduino.h>

// Arduino Car Motor Control Library
// Motor/Drive control for 2-motor robotics platform with dual motor support
// Built for L298N motor driver module with 2 DC motors
// @author Arduino Car Library Contributors

class Motor {
public:
    // Constructor: Initialize motor with control pins
    // @param in1, in2: Control pins for motor 1 (left motor)
    // @param in3, in4: Control pins for motor 2 (right motor)
    // @param ena: Enable/PWM pin for motor 1
    // @param enb: Enable/PWM pin for motor 2
    Motor(int in1, int in2, int in3, int in4, int enb, int ena);
    
    // Move forward at specified speed (0-255)
    void forward(int speed);
    
    // Move backward at specified speed (0-255)
    void backward(int speed);
    
    // Turn left with independent speed control for each motor
    // @param speedena: Speed for right motor (0-255)
    // @param speedenb: Speed for left motor (0-255)
    void left(int speedena, int speedenb);
    
    // Turn right with independent speed control for each motor
    // @param speedena: Speed for right motor (0-255)
    // @param speedenb: Speed for left motor (0-255)
    void right(int speedena, int speedenb);
    
    // Wide/smooth left turn with speed control
    void WideTurnLeft(int speedena, int speedenb);
    
    // Wide/smooth right turn with speed control
    void WideTurnRight(int speedena, int speedenb);
    
    // Stop motors (coasting mode - motors spin free)
    void stop();
    
    // Brake with full reverse force (emergency stop)
    void brake();

private:
    int _in1, _in2, _enb, _in3, _in4, _ena;
};

#endif
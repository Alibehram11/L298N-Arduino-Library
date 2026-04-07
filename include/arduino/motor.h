/*
 * ============================================================================
 * Arduino Car Motor Control Library - Header
 * ============================================================================
 * Control L298N dual motor driver module with two DC motors
 */

#ifndef ARDUINO_MOTOR_H
#define ARDUINO_MOTOR_H

#include <Arduino.h>

/**
 * Motor Class - L298N Dual DC Motor Driver Control
 * 
 * This class manages the control of two DC motors via an L298N
 * H-bridge motor driver module, commonly used in Arduino robotics.
 */
class Motor {
public:
    /**
     * Constructor: Initialize motor control pins
     * 
     * @param in1   - Control pin for motor 1 direction (left motor forward)
     * @param in2   - Control pin for motor 1 direction (left motor backward)
     * @param in3   - Control pin for motor 2 direction (right motor forward)
     * @param in4   - Control pin for motor 2 direction (right motor backward)
     * @param ena   - PWM pin for motor 1 speed control (0-255)
     * @param enb   - PWM pin for motor 2 speed control (0-255)
     * 
     * All pins are initialized as OUTPUT mode in the constructor.
     */
    Motor(int in1, int in2, int in3, int in4, int ena, int enb);
    
    /**
     * Move both motors forward at the same speed
     * 
     * @param speed - Speed value (0-255): 0=stop, 255=full speed
     */
    void forward(int speed);
    
    /**
     * Move both motors backward at the same speed
     * 
     * @param speed - Speed value (0-255): 0=stop, 255=full speed
     */
    void backward(int speed);
    
    /**
     * Turn left with independent speed control for each motor
     * 
     * @param speedena - Speed for right motor (ENA/motor 1)
     * @param speedenb - Speed for left motor (ENB/motor 2)
     * 
     * Rotates motors in opposite directions for pivot turn.
     */
    void left(int speedena, int speedenb);
    
    /**
     * Turn right with independent speed control for each motor
     * 
     * @param speedena - Speed for left motor (ENA/motor 1)
     * @param speedenb - Speed for right motor (ENB/motor 2)
     * 
     * Rotates motors in opposite directions for pivot turn.
     */
    void right(int speedena, int speedenb);
    
    /**
     * Smooth left turn - both motors forward at different speeds
     * 
     * @param speedena - Speed for motor 1
     * @param speedenb - Speed for motor 2
     * 
     * Both motors move forward but at different speeds, creating
     * a smooth arc left instead of a sharp pivot.
     */
    void WideTurnLeft(int speedena, int speedenb);
    
    /**
     * Smooth right turn - both motors forward at different speeds
     * 
     * @param speedena - Speed for motor 1
     * @param speedenb - Speed for motor 2
     * 
     * Both motors move forward but at different speeds, creating
     * a smooth arc right instead of a sharp pivot.
     */
    void WideTurnRight(int speedena, int speedenb);
    
    /**
     * Stop motors (coasting/free mode)
     * 
     * Motors spin freely without braking force.
     * Vehicle coasts to a stop due to friction.
     */
    void stop();
    
    /**
     * Emergency brake - maximum reverse current
     * 
     * Both motors forced to HIGH, creating maximum braking force.
     * Vehicle stops abruptly (high power consumption).
     */
    void brake();

private:
    // Motor control pins
    int _in1, _in2;  // Motor 1 direction pins (left motor)
    int _in3, _in4;  // Motor 2 direction pins (right motor)
    int _ena;        // Motor 1 PWM/speed pin (ENA on L298N)
    int _enb;        // Motor 2 PWM/speed pin (ENB on L298N)
};

#endif // ARDUINO_MOTOR_H
/*
 * ============================================================================
 * Arduino Car Motor Control Library - Implementation
 * ============================================================================
 * Complete implementation of L298N motor driver control methods
 */

#include "motor.h"

/**
 * CONSTRUCTOR: Motor::Motor(int in1, int in2, int in3, int in4, int ena, int enb)
 * ────────────────────────────────────────────────────────────────────────────────
 * 
 * Initializes the Motor object by setting all control pins as OUTPUT
 * 
 * Parameters:
 *   - in1: Control for motor 1 direction (forward direction line)
 *   - in2: Control for motor 1 direction (backward direction line)
 *   - in3: Control for motor 2 direction (forward direction line)
 *   - in4: Control for motor 2 direction (backward direction line)
 *   - ena: PWM pin for motor 1 speed (uses digitalWrite/analogWrite)
 *   - enb: PWM pin for motor 2 speed (uses digitalWrite/analogWrite)
 * 
 * What happens:
 *   1. Store all 6 pin numbers in member variables using initializer list
 *   2. Set all 6 pins as OUTPUT mode (Arduino can drive them)
 *      - OUTPUT means the Arduino controls the pin voltage
 *      - HIGH = 5V, LOW = 0V
 * 
 * H-Bridge Logic (L298N module):
 *   IN1  | IN2  | Motor 1 Behavior
 *   ─────┼──────┼──────────────────
 *   H    | L    | Forward (high speed)
 *   L    | H    | Backward (high speed)
 *   L    | L    | Stop/Brake (coasting)
 *   H    | H    | Brake (short circuit)
 */
Motor::Motor(int in1, int in2, int in3, int in4, int ena, int enb)
    : _in1(in1),              // Store motor 1 forward pin
      _in2(in2),              // Store motor 1 backward pin
      _in3(in3),              // Store motor 2 forward pin
      _in4(in4),              // Store motor 2 backward pin
      _ena(ena),              // Store motor 1 speed/PWM pin
      _enb(enb)               // Store motor 2 speed/PWM pin
{
    // Configure all pins as OUTPUT - Arduino will drive them
    // This tells Arduino's GPIO controller these are output pins
    
    // Motor 1 (left motor) direction control pins
    pinMode(_in1, OUTPUT);
    pinMode(_in2, OUTPUT);
    
    // Motor 1 speed control pin (PWM capable)
    pinMode(_ena, OUTPUT);
    
    // Motor 2 (right motor) direction control pins
    pinMode(_in3, OUTPUT);
    pinMode(_in4, OUTPUT);
    
    // Motor 2 speed control pin (PWM capable)
    pinMode(_enb, OUTPUT);
}

/**
 * METHOD: void Motor::forward(int speed)
 * ──────────────────────────────────────
 * 
 * Moves both motors forward at the same speed
 * 
 * Parameters:
 *   - speed: PWM value from 0-255
 *            0 = stopped (no power)
 *            128 = ~50% speed
 *            255 = full speed
 * 
 * What happens:
 *   1. Motor 1 (left):  IN1=HIGH, IN2=LOW  → forward direction
 *   2. Motor 2 (right): IN3=HIGH, IN4=LOW  → forward direction
 *   3. Apply PWM speed to both ENA and ENB for smooth acceleration
 * 
 * PWM (Pulse Width Modulation):
 *   - analogWrite() rapidly switches the pin ON/OFF
 *   - ON time vs total period determines power level
 *   - analogWrite(0) = always off (0% power)
 *   - analogWrite(128) = 50% on, 50% off (50% power)
 *   - analogWrite(255) = always on (100% power)
 *   - Frequency: ~490Hz on Arduino (very fast for motors)
 */
void Motor::forward(int speed)
{
    // Set Motor 1 (left motor) to forward direction
    // HIGH on IN1 means forward for this motor
    digitalWrite(_in1, HIGH);
    
    // Set Motor 1 backward control LOW
    // Only one direction line should be HIGH at a time
    digitalWrite(_in2, LOW);
    
    // Set Motor 2 (right motor) to forward direction
    digitalWrite(_in3, HIGH);
    
    // Set Motor 2 backward control LOW
    digitalWrite(_in4, LOW);
    
    // Control speed via PWM on enable pin for motor 1
    // analogWrite uses PWM (0-255 maps to 0%-100% duty cycle)
    analogWrite(_ena, speed);
    
    // Control speed via PWM on enable pin for motor 2
    analogWrite(_enb, speed);
}

/**
 * METHOD: void Motor::backward(int speed)
 * ───────────────────────────────────────
 * 
 * Moves both motors backward at the same speed
 * 
 * Parameters:
 *   - speed: PWM value from 0-255 (same as forward)
 * 
 * What happens:
 *   1. Motor 1 (left):  IN1=LOW, IN2=HIGH  → backward direction
 *   2. Motor 2 (right): IN3=LOW, IN4=HIGH  → backward direction
 *   3. Apply PWM speed to both motors
 */
void Motor::backward(int speed)
{
    // Set Motor 1 (left motor) to backward direction
    // LOW on IN1, HIGH on IN2 = reverse
    digitalWrite(_in1, LOW);
    digitalWrite(_in2, HIGH);
    
    // Set Motor 2 (right motor) to backward direction
    digitalWrite(_in3, LOW);
    digitalWrite(_in4, HIGH);
    
    // Apply speed via PWM (faster = more power)
    analogWrite(_ena, speed);
    analogWrite(_enb, speed);
}

/**
 * METHOD: void Motor::left(int speedena, int speedenb)
 * ──────────────────────────────────────────────────────
 * 
 * Sharp left turn - motors rotate in opposite directions (pivot)
 * 
 * Parameters:
 *   - speedena: Speed for right motor (0-255)
 *   - speedenb: Speed for left motor (0-255)
 * 
 * Typical usage: left(100, 200) or left(200, 0) for sharp turn
 * 
 * What happens:
 *   - Motor 1 (left):  backward
 *   - Motor 2 (right): forward
 *   - Result: vehicle pivots left on center point
 */
void Motor::left(int speedena, int speedenb)
{
    // Motor 1 (left motor): go backward (turn left)
    digitalWrite(_in1, LOW);    // Backward control
    digitalWrite(_in2, HIGH);
    
    // Motor 2 (right motor): go forward (turn left)
    digitalWrite(_in3, HIGH);   // Forward control
    digitalWrite(_in4, LOW);
    
    // Apply different speeds for independent motor control
    analogWrite(_ena, speedena);  // Right motor speed
    analogWrite(_enb, speedenb);  // Left motor speed
}

/**
 * METHOD: void Motor::right(int speedena, int speedenb)
 * ───────────────────────────────────────────────────────
 * 
 * Sharp right turn - motors rotate in opposite directions (pivot)
 * 
 * Parameters:
 *   - speedena: Speed for left motor (0-255)
 *   - speedenb: Speed for right motor (0-255)
 * 
 * What happens:
 *   - Motor 1 (left):  forward
 *   - Motor 2 (right): backward
 *   - Result: vehicle pivots right on center point
 */
void Motor::right(int speedena, int speedenb)
{
    // Motor 1 (left motor): go forward (turn right)
    digitalWrite(_in1, HIGH);    // Forward control
    digitalWrite(_in2, LOW);
    
    // Motor 2 (right motor): go backward (turn right)
    digitalWrite(_in3, LOW);     // Backward control
    digitalWrite(_in4, HIGH);
    
    // Apply different speeds
    analogWrite(_ena, speedena);  // Left motor speed
    analogWrite(_enb, speedenb);  // Right motor speed
}

/**
 * METHOD: void Motor::WideTurnLeft(int speedena, int speedenb)
 * ─────────────────────────────────────────────────────────────
 * 
 * Smooth left turn - both motors forward at different speeds
 * 
 * Parameters:
 *   - speedena: Speed for right motor (higher = sharper turn)
 *   - speedenb: Speed for left motor (lower = sharper turn)
 * 
 * Example: WideTurnLeft(200, 100)
 *   → Right motor: 200 (faster) → Follows outer arc
 *   → Left motor:  100 (slower) → Follows inner arc
 *   → Result: smooth curve to the left
 * 
 * What happens:
 *   - Both motors move forward (same direction)
 *   - But at different speeds - creates curved path
 *   - Smoother than sharp pivot turns
 *   - More gradual, realistic vehicle turning
 */
void Motor::WideTurnLeft(int speedena, int speedenb)
{
    // Both motors go FORWARD (same direction)
    
    // Motor 1 (left motor): forward at lower speed (inner curve)
    digitalWrite(_in1, HIGH);    // Forward
    digitalWrite(_in2, LOW);
    
    // Motor 2 (right motor): forward at higher speed (outer curve)
    digitalWrite(_in3, HIGH);    // Forward
    digitalWrite(_in4, LOW);
    
    // Different speeds create the turning arc
    // Left motor slower, right motor faster = curves left
    analogWrite(_ena, speedena);  // Right motor (outer): faster speed
    analogWrite(_enb, speedenb);  // Left motor (inner): slower speed
}

/**
 * METHOD: void Motor::WideTurnRight(int speedena, int speedenb)
 * ──────────────────────────────────────────────────────────────
 * 
 * Smooth right turn - both motors forward at different speeds
 * 
 * Parameters:
 *   - speedena: Speed for left motor (higher = sharper turn)
 *   - speedenb: Speed for right motor (lower = sharper turn)
 * 
 * What happens:
 *   - Both motors move forward
 *   - Left motor faster (outer arc)
 *   - Right motor slower (inner arc)
 *   - Result: smooth curve to the right
 */
void Motor::WideTurnRight(int speedena, int speedenb)
{
    // Both motors go FORWARD (same direction)
    
    // Motor 1 (left motor): forward at higher speed (outer curve)
    digitalWrite(_in1, HIGH);    // Forward
    digitalWrite(_in2, LOW);
    
    // Motor 2 (right motor): forward at lower speed (inner curve)
    digitalWrite(_in3, HIGH);    // Forward
    digitalWrite(_in4, LOW);
    
    // Left motor faster, right motor slower = curves right
    analogWrite(_ena, speedena);  // Left motor (outer): faster speed
    analogWrite(_enb, speedenb);  // Right motor (inner): slower speed
}

/**
 * METHOD: void Motor::stop()
 * ──────────────────────────
 * 
 * Stop motors (coasting mode)
 * 
 * What happens:
 *   - Both IN pins for each motor set to LOW
 *   - Motors have no power applied
 *   - Motors spin freely due to inertia  
 *   - Vehicle coasts to stop (no active braking)
 *   - Low power consumption
 * 
 * L298N behavior:
 *   IN1=L, IN2=L → Motor 1 floats (no power)
 *   IN3=L, IN4=L → Motor 2 floats (no power)
 *   ENA=0, ENB=0 → No PWM power applied
 */
void Motor::stop()
{
    // Set Motor 1 control pins to LOW
    // Both pins LOW = no drive current, motor coasts
    digitalWrite(_in1, LOW);
    digitalWrite(_in2, LOW);
    
    // Set Motor 2 control pins to LOW
    digitalWrite(_in3, LOW);
    digitalWrite(_in4, LOW);
    
    // Disable PWM to ensure no power to motors
    analogWrite(_ena, 0);  // 0% PWM = no power
    analogWrite(_enb, 0);
}

/**
 * METHOD: void Motor::brake()
 * ───────────────────────────
 * 
 * Emergency brake - maximum braking force
 * 
 * What happens:
 *   - Both control pins for each motor set to HIGH
 *   - This creates a short circuit condition in the H-bridge
 *   - Motor terminals are shorted together
 *   - Motor becomes a generator (opposing current)
 *   - Creates maximum electromagnetic braking force
 *   - Vehicle stops abruptly
 * 
 * L298N behavior:
 *   IN1=H, IN2=H → Motor 1 shorted (braking)
 *   IN3=H, IN4=H → Motor 2 shorted (braking)
 *   ENA=255, ENB=255 → Full PWM power of braking
 * 
 * Warning:
 *   - High current draw (ensure motor power supply can handle it)
 *   - Not for continuous use (motor gets hot)
 *   - Use only for emergency stops or short braking periods
 * 
 * Power Calculation:
 *   Current (I) = Motor Voltage / Motor Resistance
 *   Example: 12V motor with 4Ω resistance = 3A braking current
 */
void Motor::brake()
{
    // Set Motor 1 both control pins to HIGH
    // HIGH on both IN pins shorts the motor terminals
    // Creates maximum back-EMF for braking
    digitalWrite(_in1, HIGH);   // Both HIGH = short
    digitalWrite(_in2, HIGH);   // = maximum braking
    
    // Set Motor 2 both control pins to HIGH
    digitalWrite(_in3, HIGH);
    digitalWrite(_in4, HIGH);
    
    // Apply FULL power to the braking current
    // 255 = maximum PWM = maximum braking force
    analogWrite(_ena, 255);  // 100% PWM = full braking
    analogWrite(_enb, 255);
}
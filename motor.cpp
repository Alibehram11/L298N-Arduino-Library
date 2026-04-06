#include "motor.h"

// Motor Constructor - Initialize all motor control pins as OUTPUT
Motor::Motor(int in1, int in2, int in3, int in4, int enb, int ena) : _in1(in1), _in2(in2), _in3(in3), _in4(in4), _enb(enb), _ena(ena) {
    pinMode(_in1, OUTPUT);
    pinMode(_in2, OUTPUT);
    pinMode(_enb, OUTPUT);
    pinMode(_in3, OUTPUT);
    pinMode(_in4, OUTPUT);
    pinMode(_ena, OUTPUT);
}

// Move both motors forward at the same speed
void Motor::forward(int speed) {
    digitalWrite(_in1, HIGH);
    digitalWrite(_in2, LOW);
    digitalWrite(_in3, HIGH);
    digitalWrite(_in4, LOW);
    analogWrite(_enb, speed);
    analogWrite(_ena, speed);
}

// Move both motors backward at the same speed
void Motor::backward(int speed) {
    digitalWrite(_in1, LOW);
    digitalWrite(_in2, HIGH);
    digitalWrite(_in3, LOW);
    digitalWrite(_in4, HIGH);
    analogWrite(_enb, speed);
    analogWrite(_ena, speed);
}
// Turn left: rotate motors in opposite directions (simple left turn)
void Motor::left(int speed) {
    digitalWrite(_in1, LOW);
    digitalWrite(_in2, HIGH);
    digitalWrite(_in3, HIGH);
    digitalWrite(_in4, LOW);
    analogWrite(_enb, speed);
    analogWrite(_ena, speed);
}

// Turn right: rotate motors in opposite directions (simple right turn)
void Motor::right(int speed) {
    digitalWrite(_in1, HIGH);
    digitalWrite(_in2, LOW);
    digitalWrite(_in3, LOW);
    digitalWrite(_in4, HIGH);
    analogWrite(_enb, speed);
    analogWrite(_ena, speed);
}

// Turn left with independent motor speed control (advanced left turn)
void Motor::left(int speedena, int speedenb) {
    digitalWrite(_in1, LOW);
    digitalWrite(_in2, HIGH);
    digitalWrite(_in3, HIGH);
    digitalWrite(_in4, LOW);
    analogWrite(_enb, speedenb);
    analogWrite(_ena, speedena);
}

// Turn right with independent motor speed control (advanced right turn)
void Motor::right(int speedena, int speedenb) {
    digitalWrite(_in1, HIGH);
    digitalWrite(_in2, LOW);
    digitalWrite(_in3, LOW);
    digitalWrite(_in4, HIGH);
    analogWrite(_enb, speedenb);
    analogWrite(_ena, speedena);
}
// Wide left turn: smooth turning with independent motor speeds
void Motor::WideTurnLeft(int speedena, int speedenb) {
    digitalWrite(_in1, LOW);
    digitalWrite(_in2, HIGH);
    digitalWrite(_in3, HIGH);
    digitalWrite(_in4, LOW);
    analogWrite(_enb, speedenb);
    analogWrite(_ena, speedena);
}
// Wide right turn: smooth turning with independent motor speeds
void Motor::WideTurnRight(int speedena, int speedenb) {
    digitalWrite(_in1, HIGH);
    digitalWrite(_in2, LOW);
    digitalWrite(_in3, LOW);
    digitalWrite(_in4, HIGH);
    analogWrite(_enb, speedenb);
    analogWrite(_ena, speedena);
}
// Stop both motors (coasting mode)
void Motor::stop() {
    digitalWrite(_in1, LOW);
    digitalWrite(_in2, LOW);
    digitalWrite(_in3, LOW);
    digitalWrite(_in4, LOW);
    analogWrite(_enb, 0);
    analogWrite(_ena, 0);
}

// Brake both motors with full reverse force (emergency stop)
void Motor::brake() {
    digitalWrite(_in1, HIGH);
    digitalWrite(_in2, HIGH);
    digitalWrite(_in3, HIGH);
    digitalWrite(_in4, HIGH);
    analogWrite(_enb, 255);
    analogWrite(_ena, 255);
}

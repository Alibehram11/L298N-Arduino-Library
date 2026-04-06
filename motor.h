#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

class Motor {
public:
    Motor(int in1, int in2, int in3, int in4, int enb, int ena);
    void forward(int speed);
    void backward(int speed);
    void left(int speedena, int speedenb);
    void right(int speedena, int speedenb);
    void WideTurnLeft(int speedena, int speedenb);
    void WideTurnRight(int speedena, int speedenb);
    void stop();
    void brake();

private:
    int _in1, _in2, _enb, _in3, _in4, _ena;
};

#endif
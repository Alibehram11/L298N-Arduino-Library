#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

class Motor {
public:
    Motor(int in1, int in2, int en);
    void forward(int speed);
    void backward(int speed);
    void stop();
    void brake();

private:
    int _in1, _in2, _en;
};

#endif

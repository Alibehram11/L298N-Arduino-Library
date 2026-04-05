#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

class Motor {
public:
    Motor(int in1, int in2, int in3, int in4, int enb, int ena) : _in1(in1), _in2(in2), _enb(enb), _in3(in3), _in4(in4), _ena(ena) {
        pinMode(_in1, OUTPUT);
        pinMode(_in2, OUTPUT);
        pinMode(_enb, OUTPUT);
        pinMode(_in3, OUTPUT);
        pinMode(_in4, OUTPUT); 
        pinMode(_ena, OUTPUT);
   
    }
    void forward(int speed);
    void backward(int speed);
    void left(int speed);
    void right(int speed);
    void stop();
    void brake();

private:
    int _in1, _in2, _enb, _in3, _in4, _ena;
};

#endif
#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

class Motor {
public:
    Motor(int in1, int in2, int in3, int in4, int enb, int enA) : _in1(in1), _in2(in2), _en(enA) {
        pinMode(_in1, OUTPUT);
        pinMode(_in2, OUTPUT);
        pinMode(_en, OUTPUT);
        pinMode(in3, OUTPUT);
        pinMode(in4, OUTPUT);   
    }
    void forward(int speed);
    void backward(int speed);
    void stop();
    void brake();

private:
    int _in1, _in2, _en;
};

#endif
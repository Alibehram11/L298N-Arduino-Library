# Arduino Car Motor Control Library

Arduino C++ motor-control library for L298N robot cars with Bluetooth examples.

## Problem

Small robot-car projects often repeat the same L298N wiring, PWM speed, and movement logic in every sketch. That makes the first build slower and makes later changes harder to test.

## Solution

This library groups common robot-car movement commands into reusable Arduino classes:

- `Motor` for forward, backward, left, right, stop, brake, and wide turns.
- Independent PWM speed control for two DC motors.
- Optional `HC06` helper for Bluetooth serial input.
- Example sketches that can be adapted to a real robot-car build.

## Tech Stack

- Arduino
- C++
- L298N dual H-bridge motor driver
- HC-06 Bluetooth module
- DC motors and PWM speed control

## Hardware

| Component | Quantity | Purpose |
| --- | ---: | --- |
| Arduino Uno, Nano, or Mega | 1 | Main controller |
| L298N motor driver | 1 | Drives two DC motors |
| DC motors | 2 | Robot movement |
| External motor power supply | 1 | Motor power |
| HC-06 Bluetooth module | 1 | Optional wireless control |
| Jumper wires | as needed | Connections |

## Quick Start

```cpp
#include <arduino/arduino.h>

Motor car(9, 8, 7, 6, 5, 10);

void setup() {
    Serial.begin(9600);
}

void loop() {
    car.forward(200);
    delay(2000);

    car.left(120, 200);
    delay(1000);

    car.stop();
    delay(500);
}
```

## Pin Example

| Arduino pin | L298N pin | Purpose |
| --- | --- | --- |
| 9 | IN1 | Motor A direction |
| 8 | IN2 | Motor A direction |
| 7 | IN3 | Motor B direction |
| 6 | IN4 | Motor B direction |
| 5 | ENA | Motor A PWM speed |
| 10 | ENB | Motor B PWM speed |
| GND | GND | Shared ground |
| 5V | 5V | Logic power |

Use PWM-capable pins for `ENA` and `ENB`. On Arduino Uno, common PWM pins are 3, 5, 6, 9, 10, and 11.

## Bluetooth Example

```cpp
#include <arduino/arduino.h>

Motor car(9, 8, 7, 6, 5, 10);
HC06 bluetooth(3, 4, -1);

void setup() {
    Serial.begin(9600);
    bluetooth.begin(9600);
}

void loop() {
    String command = bluetooth.readData();

    if (command == "FORWARD") car.forward(200);
    else if (command == "BACK") car.backward(200);
    else if (command == "LEFT") car.left(120, 200);
    else if (command == "RIGHT") car.right(200, 120);
    else if (command == "STOP") car.stop();
}
```

## API Summary

### `Motor(int in1, int in2, int in3, int in4, int ena, int enb)`

Creates a motor controller and configures the L298N pins.

Main methods:

- `forward(int speed)`
- `backward(int speed)`
- `left(int speedena, int speedenb)`
- `right(int speedena, int speedenb)`
- `WideTurnLeft(int speedena, int speedenb)`
- `WideTurnRight(int speedena, int speedenb)`
- `stop()`
- `brake()`

Speed values use Arduino PWM range `0` to `255`.

### `HC06(int rxPin, int txPin, int hc06)`

Helper for reading and writing simple Bluetooth serial data.

Main methods:

- `begin(long baudRate)`
- `sendData(const String& data)`
- `readData()`
- `end()`
- `reset()`

## Installation

Manual Arduino IDE installation:

1. Download this repository as a ZIP.
2. Extract it into your Arduino libraries folder.
3. Restart Arduino IDE.
4. Include the library in your sketch:

```cpp
#include <arduino/arduino.h>
```

Git clone installation:

```bash
cd ~/Arduino/libraries
git clone https://github.com/Alibehram11/Arduino-Car-Library.git ArduinoCar
```

## Verification

- Open the example sketches in Arduino IDE.
- Update pin numbers to match your wiring.
- Compile the sketch before uploading.
- Test motor direction with the wheels lifted before running the robot on the ground.

## Demo / Evidence

- Example sketches are available in `examples/`.
- Hardware and wiring notes are documented in this README.
- The public roadmap is tracked in GitHub Issues.

## Roadmap

- Add one short wiring photo or GIF.
- Add a verified compile note for Arduino Uno.
- Add a small troubleshooting table for motor direction, power, and Bluetooth pairing.

## Status

Portfolio-ready learning library. It is suitable for explaining Arduino robot-car motor control, but real hardware compatibility should be verified per build before competition or long-running use.

## License

MIT License. See [LICENSE](LICENSE).

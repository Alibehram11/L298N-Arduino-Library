/*
 * EXAMPLE 3: BluetoothControl.ino
 *
 * Arduino Car Library - Wireless Bluetooth Control
 *
 * Send single-character commands over an HC-06 Bluetooth module:
 *   F = forward
 *   B = backward
 *   L = left pivot
 *   R = right pivot
 *   S = stop
 *   X = brake
 *   0..9 = set speed preset
 */

#include <arduino/arduino.h>
#include <ctype.h>

Motor car(9, 8, 7, 6, 5, 10);
HC06 bluetooth(2, 3, -1);

uint8_t driveSpeed = 180;

void setup()
{
    Serial.begin(9600);
    bluetooth.begin(9600);

    Serial.println("=== Arduino Car - Bluetooth Control ===");
    Serial.println("Commands: F B L R S X, speed 0..9");
}

void loop()
{
    String command = bluetooth.readData();

    if (command.length() == 0 && Serial.available()) {
        command = Serial.readStringUntil('\n');
        command.trim();
    }

    if (command.length() == 0) {
        return;
    }

    char c = toupper(command.charAt(0));
    handleCommand(c);
}

void handleCommand(char command)
{
    if (command >= '0' && command <= '9') {
        driveSpeed = map(command - '0', 0, 9, 0, 255);
        Serial.print("Speed set to ");
        Serial.println(driveSpeed);
        bluetooth.sendData(String("Speed=") + String(driveSpeed));
        return;
    }

    switch (command) {
    case 'F':
        car.forward(driveSpeed);
        report("Forward");
        break;
    case 'B':
        car.backward(driveSpeed);
        report("Backward");
        break;
    case 'L':
        car.drive(-driveSpeed, driveSpeed);
        report("Left");
        break;
    case 'R':
        car.drive(driveSpeed, -driveSpeed);
        report("Right");
        break;
    case 'S':
        car.stop();
        report("Stop");
        break;
    case 'X':
        car.brake();
        report("Brake");
        break;
    default:
        report("Unknown command");
        break;
    }
}

void report(const String& message)
{
    Serial.println(message);
    bluetooth.sendData(message);
}

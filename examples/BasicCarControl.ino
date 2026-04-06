/*
 * BasicCarControl.ino
 * 
 * Arduino Car Library - Basic Example
 * 
 * This example demonstrates basic car movement control:
 * - Forward/Backward movement at different speeds
 * - Left/Right turning
 * - Smooth stop and emergency brake
 * 
 * Hardware Connections:
 * Arduino Pin 9  --> L298N IN1 (Left Motor Forward)
 * Arduino Pin 8  --> L298N IN2 (Left Motor Backward)
 * Arduino Pin 7  --> L298N IN3 (Right Motor Forward)
 * Arduino Pin 6  --> L298N IN4 (Right Motor Backward)
 * Arduino Pin 5  --> L298N ENA (Left Motor PWM)
 * Arduino Pin 10 --> L298N ENB (Right Motor PWM)
 * Arduino GND    --> L298N GND
 * Arduino 5V     --> L298N 5V (optional, depending on module)
 */

#include "motor.h"

// Create car object: Motor(IN1, IN2, IN3, IN4, ENB, ENA)
Motor car(9, 8, 7, 6, 10, 5);

void setup() {
  Serial.begin(9600);
  Serial.println("=== Arduino Car - Basic Control Demo ===");
  delay(1000);
}

void loop() {
  // 1. Move forward at full speed
  Serial.println(">> Moving FORWARD at full speed (255)");
  car.forward(255);
  delay(3000);

  // 2. Move forward at half speed
  Serial.println(">> Moving FORWARD at half speed (128)");
  car.forward(128);
  delay(2000);

  // 3. Stop smoothly (coasting)
  Serial.println(">> STOP (coasting mode)");
  car.stop();
  delay(2000);

  // 4. Move backward at full speed
  Serial.println(">> Moving BACKWARD at full speed (255)");
  car.backward(255);
  delay(3000);

  // 5. Stop smoothly again
  Serial.println(">> STOP (coasting mode)");
  car.stop();
  delay(2000);

  // 6. Turn left (left motor slow, right motor fast)
  Serial.println(">> Turning LEFT (left slower, right faster)");
  car.left(80, 180);  // Left motor: 80/255, Right motor: 180/255
  delay(2000);

  // 7. Stop
  Serial.println(">> STOP (coasting mode)");
  car.stop();
  delay(1000);

  // 8. Turn right (right motor slow, left motor fast)
  Serial.println(">> Turning RIGHT (right slower, left faster)");
  car.right(180, 80);  // Left motor: 180/255, Right motor: 80/255
  delay(2000);

  // 9. Stop
  Serial.println(">> STOP (coasting mode)");
  car.stop();
  delay(1000);

  // 10. Emergency brake (hard stop)
  Serial.println(">> EMERGENCY BRAKE (hard stop)");
  car.forward(200);
  delay(1000);
  car.brake();  // Motors will lock and stop immediately
  delay(3000);

  Serial.println("\n=== End of Demo - Restarting... ===\n");
}

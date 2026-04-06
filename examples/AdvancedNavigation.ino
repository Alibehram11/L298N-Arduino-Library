/*
 * AdvancedNavigation.ino
 * 
 * Arduino Car Library - Advanced Navigation Example
 * 
 * This example demonstrates advanced car movement patterns:
 * - Square path navigation
 * - Obstacle avoidance simulation
 * - Speed ramping (acceleration/deceleration)
 * - Complex turning patterns
 * 
 * Perfect for robot car competitions and autonomous navigation!
 */

#include "motor.h"

// Create car object: Motor(IN1, IN2, IN3, IN4, ENB, ENA)
Motor car(9, 8, 7, 6, 10, 5);

void setup() {
  Serial.begin(9600);
  Serial.println("=== Arduino Car - Advanced Navigation Demo ===");
  delay(1000);
}

void loop() {
  // Pattern 1: Drive a square
  Serial.println("\n>> PATTERN 1: Driving a SQUARE");
  driveSquare();
  delay(3000);

  // Pattern 2: Zigzag maneuver
  Serial.println("\n>> PATTERN 2: ZIGZAG maneuver");
  zigzagManeuver();
  delay(3000);

  // Pattern 3: Spiral movement
  Serial.println("\n>> PATTERN 3: SPIRAL movement");
  spiralMovement();
  delay(3000);

  // Pattern 4: Speed test (acceleration ramp)
  Serial.println("\n>> PATTERN 4: ACCELERATION TEST");
  accelerationTest();
  delay(2000);

  // Pattern 5: Smooth cornering
  Serial.println("\n>> PATTERN 5: SMOOTH CORNER TEST");
  smoothCornerTest();
  delay(2000);

  Serial.println("\n=== End of Advanced Demo - Restarting... ===\n");
}

// ============== MOVEMENT FUNCTIONS ==============

void driveSquare() {
  /*
   * Drive in a perfect square pattern
   * Each side: 2 seconds forward + 1 second 90-degree turn
   */
  for (int i = 0; i < 4; i++) {
    Serial.print("  Side ");
    Serial.print(i + 1);
    Serial.println(" - Moving forward");
    car.forward(200);
    delay(2000);

    Serial.println("  Turning 90 degrees");
    car.left(50, 150);  // Sharp left turn
    delay(1000);

    car.stop();
    delay(300);
  }
  car.brake();
}

void zigzagManeuver() {
  /*
   * Zigzag left and right while moving forward
   * Useful for obstacle avoidance patterns
   */
  for (int i = 0; i < 3; i++) {
    // Go forward-right
    Serial.println("  Zigzag: Forward-Right");
    car.right(150, 120);  // Slight right turn while moving
    delay(1500);

    // Go forward-left
    Serial.println("  Zigzag: Forward-Left");
    car.left(120, 150);   // Slight left turn while moving
    delay(1500);
  }
  car.stop();
}

void spiralMovement() {
  /*
   * Create a spiral pattern by gradually increasing turn intensity
   */
  for (int speed = 50; speed < 200; speed += 30) {
    Serial.print("  Spiral speed: ");
    Serial.println(speed);
    
    car.left(speed / 2, speed);  // Gradually sharper left turn
    delay(800);
  }
  car.stop();
}

void accelerationTest() {
  /*
   * Test smooth acceleration from 0 to 255
   * Good for tuning motor behavior
   */
  Serial.println("  Accelerating forward...");
  for (int speed = 0; speed <= 255; speed += 20) {
    car.forward(speed);
    Serial.print("  Speed: ");
    Serial.println(speed);
    delay(400);
  }

  Serial.println("  Decelerating...");
  for (int speed = 255; speed >= 0; speed -= 20) {
    car.forward(speed);
    Serial.print("  Speed: ");
    Serial.println(speed);
    delay(400);
  }

  car.stop();
}

void smoothCornerTest() {
  /*
   * Test different turn speeds for smooth navigation
   * Helps find optimal turning speeds for your car
   */
  // Gentle left turn (high-speed corner)
  Serial.println("  Gentle LEFT turn (high-speed)");
  car.left(180, 200);   // Minimal speed difference
  delay(2000);
  car.stop();
  delay(500);

  // Sharp left turn (low-speed corner)
  Serial.println("  Sharp LEFT turn (low-speed)");
  car.left(50, 150);    // Large speed difference
  delay(2000);
  car.stop();
  delay(500);

  // Gentle right turn (high-speed corner)
  Serial.println("  Gentle RIGHT turn (high-speed)");
  car.right(200, 180);  // Minimal speed difference
  delay(2000);
  car.stop();
  delay(500);

  // Sharp right turn (low-speed corner)
  Serial.println("  Sharp RIGHT turn (low-speed)");
  car.right(150, 50);   // Large speed difference
  delay(2000);
  car.stop();
}

/*
 * USEFUL TIPS:
 * 
 * 1. Motor Speed Reference:
 *    - 0-50:     Very slow (calibration, precise movement)
 *    - 50-150:   Medium speed (turning, controlled movement)
 *    - 150-255:  High speed (fast movement, racing)
 * 
 * 2. Turning Tips:
 *    - Gentle turn: Speeds differ by ~20-30
 *    - Sharp turn:  Speeds differ by ~100+
 *    - Spin in place: 0 and 255 (one motor stops)
 * 
 * 3. Calibration:
 *    - If car drifts left/right, adjust motor speeds
 *    - Faster motor on the drift side to balance
 */

/*
 * ═══════════════════════════════════════════════════════════════════════════════
 * EXAMPLE 1: BasicCarControl.ino
 * Arduino Car Library - Basic Movement Control
 * ═══════════════════════════════════════════════════════════════════════════════
 * 
 * OVERVIEW:
 *   This example demonstrates the fundamental movement commands for a two-motor
 *   robot car using the Motor class. Perfect for beginners to understand how
 *   the library controls motor direction and speed.
 * 
 * PREREQUISITE HARDWARE:
 *   ✓ Arduino Uno/Mega/Nano microcontroller
 *   ✓ L298N Dual Motor Driver Module
 *   ✓ 2x DC Motors (6V-12V recommended)
 *   ✓ 1x Power Supply (5V for logic, 6-12V for motors)
 *   ✓ USB cable for programming (Micro-USB for Nano)
 * 
 * PIN CONFIGURATION (REQUIRED):
 *   Arduino Pin 9  ──→ L298N IN1  (Motor 1 / Left Motor forward control)
 *   Arduino Pin 8  ──→ L298N IN2  (Motor 1 / Left Motor back control)
 *   Arduino Pin 7  ──→ L298N IN3  (Motor 2 / Right Motor forward control)
 *   Arduino Pin 6  ──→ L298N IN4  (Motor 2 / Right Motor back control)
 *   Arduino Pin 5  ──→ L298N ENA  (Motor 1 PWM speed - MUST be PWM pin)
 *   Arduino Pin 10 ──→ L298N ENB  (Motor 2 PWM speed - MUST be PWM pin)
 *   Arduino GND ────→ L298N GND
 *   Arduino 5V  ────→ L298N Logic +5V
 * 
 *   ⚠️  CRITICAL: Pins 5 and 10 MUST be PWM-capable on your Arduino.
 *       PWM pins on Arduino Uno: 3, 5, 6, 9, 10, 11
 * 
 * WHAT THIS SKETCH DOES:
 *   The robot will perform a repeating movement sequence:
 *   1. Move forward at full speed for 3 seconds
 *   2. Move forward at half speed for 2 seconds
 *   3. Coast to a stop for 2 seconds
 *   4. Move backward for 2 seconds
 *   5. Stop suddenly (active brake) for 1 second
 *   6. Turn left sharply (pivot) for 1 second
 *   7. Turn right sharply (pivot) for 1 second
 *   8. Repeat the sequence
 * 
 * OUTPUT:
 *   Serial Monitor displays movement status at 9600 baud.
 *   Open Tools > Serial Monitor to see diagnostic messages.
 * 
 * COMMON ISSUES & SOLUTIONS:
 *   Issue: Motors don't spin
 *   → Check all pin connections are secure
 *   → Verify motor power supply is connected and appropriate voltage
 *   → Confirm pins 5 and 10 are truly PWM pins on your board
 *   
 *   Issue: Robot pulls strongly to one side
 *   → Motor speeds are mismatched - adjust speed values in forward/backward calls
 *   → One motor may be weaker - slightly increase its speed
 *   
 *   Issue: Motors spin but wheels don't move
 *   → Check motor shafts are properly coupled to wheels
 *   → Verify wheels rotate freely without obstruction
 * 
 * ═══════════════════════════════════════════════════════════════════════════════
 */

// HEADER INCLUSION
// Includes the Motor class and all necessary supporting libraries
#include <arduino/arduino.h>

// ───────────────────────────────────────────────────────────────────────────────
// MOTOR CONFIGURATION
// ───────────────────────────────────────────────────────────────────────────────

/*
 * Motor Object Constructor
 * Motor car(in1, in2, in3, in4, ena, enb)
 * 
 * The Motor class manages:
 *   - Direction control pins (in1-in4): Determine motor direction
 *   - Speed control pins (ena, enb): PWM signals for speed (0-255)
 * 
 * When this object is created, the constructor automatically:
 *   1. Stores the pin numbers in private member variables
 *   2. Configures all 6 pins as OUTPUT (Arduino can drive them)
 *   3. Sets motor speeds to 0 (motors initially stopped)
 */
Motor car(9, 8, 7, 6, 5, 10);


// ───────────────────────────────────────────────────────────────────────────────
// SETUP() - RUNS ONCE AT ARDUINO STARTUP
// ───────────────────────────────────────────────────────────────────────────────

void setup() {
    /*
     * Serial.begin(9600)
     * 
     * Initializes serial communication at 9600 baud (bits per second).
     * 9600 is the standard speed for Arduino-to-computer communication.
     * This allows us to see diagnostic messages in Arduino IDE's Serial Monitor.
     */
    Serial.begin(9600);
    
    /*
     * Small delay to ensure Serial port is ready
     * (Some Arduino boards need a moment to initialize)
     */
    delay(500);
    
    // Print startup message
    Serial.println("╔════════════════════════════════════════════════════════╗");
    Serial.println("║     ARDUINO CAR LIBRARY - BASIC CONTROL EXAMPLE       ║");
    Serial.println("║                                                        ║");
    Serial.println("║  Motor Configuration:                                  ║");
    Serial.println("║    Left Motor:  IN1(9), IN2(8), ENA(5)                ║");
    Serial.println("║    Right Motor: IN3(7), IN4(6), ENB(10)               ║");
    Serial.println("║                                                        ║");
    Serial.println("║  Speed Scale: 0 (stop) to 255 (full speed)            ║");
    Serial.println("╚════════════════════════════════════════════════════════╝");
    
    // Wait before starting
    delay(2000);
    
    Serial.println("\n>> System ready! Starting movement sequence...\n");
}


// ───────────────────────────────────────────────────────────────────────────────
// LOOP() - RUNS CONTINUOUSLY AFTER SETUP()
// ───────────────────────────────────────────────────────────────────────────────

void loop() {
    
    // ═══════════════════════════════════════════════════════════════════════════
    // MOVEMENT 1: FORWARD AT FULL SPEED
    // ═══════════════════════════════════════════════════════════════════════════
    
    Serial.println("► FORWARD at speed 255 (FULL)");
    
    /*
     * car.forward(255)
     * 
     * This method:
     *   - Sets both motors to "forward" mode (IN1/IN3 HIGH, IN2/IN4 LOW)
     *   - Applies PWM signal of 255 (100% duty cycle) to both ENA and ENB
     *   - The motors accelerate to full rotational speed
     * 
     * PWM (Pulse Width Modulation):
     *   - 255 = motor on 100% of the time (maximum power)
     *   - 128 = motor on 50% of the time (half power)
     *   - 0 = motor off completely
     */
    car.forward(255);
    
    /*
     * delay(3000)
     * 
     * Pauses execution for 3000 milliseconds (3 seconds).
     * During this time, the motors maintain the last commanded speed/direction.
     * The robot travels forward for approximately 3 wheel rotations
     * (actual distance depends on wheel size and gearing).
     */
    delay(3000);
    
    
    // ═══════════════════════════════════════════════════════════════════════════
    // MOVEMENT 2: FORWARD AT HALF SPEED
    // ═══════════════════════════════════════════════════════════════════════════
    
    Serial.println("► FORWARD at speed 128 (HALF)");
    
    /*
     * car.forward(128)
     * 
     * Same backward motion as above, but with speed 128 (~50% power).
     * Reasons to use half speed:
     *   - Prevent tipping on tight turns
     *   - Preserve battery life for longer run times
     *   - Make robot movements more controllable/predictable
     *   - Reduce motor/wheel wear
     */
    car.forward(128);
    delay(2000);
    
    
    // ═══════════════════════════════════════════════════════════════════════════
    // MOVEMENT 3: COASTING STOP (FREE SPINNING MOTORS)
    // ═══════════════════════════════════════════════════════════════════════════
    
    Serial.println("► STOP (coasting - motors disabled)");
    
    /*
     * car.stop()
     * 
     * This method disables motor power completely by:
     *   - Setting all direction pins (IN1, IN2, IN3, IN4) to LOW
     *   - Removing PWM signal from enabled pins (ENA, ENB = 0)
     * 
     * Result: Motors become passive - they spin freely under inertia
     * until friction and air resistance bring them to rest.
     * 
     * Advantages:
     *   - Energy efficient (no power consumed during braking)
     *   - Mechanical friendly (no sudden forces on motors/gears)
     *   - Smooth deceleration (robot coasts smoothly to stop)
     * 
     * Use stop() for:
     *   - Normal operation mode transitions
     *   - Battery-powered robots (saves power)
     *   - Situations where gentle stopping is acceptable
     */
    car.stop();
    delay(2000);
    
    
    // ═══════════════════════════════════════════════════════════════════════════
    // MOVEMENT 4: BACKWARD AT FULL SPEED
    // ═══════════════════════════════════════════════════════════════════════════
    
    Serial.println("► BACKWARD at speed 255 (FULL)");
    
    /*
     * car.backward(255)
     * 
     * This method reverses both motors:
     *   - Sets direction pins:  IN1/IN3 LOW, IN2/IN4 HIGH
     *   - This reverses current flow through motor windings
     *   - Motors spin in opposite direction compared to forward()
     * 
     * The robot now moves backward at full speed for 2 seconds.
     */
    car.backward(255);
    delay(2000);
    
    
    // ═══════════════════════════════════════════════════════════════════════════
    // MOVEMENT 5: EMERGENCY BRAKE (ACTIVE BRAKING)
    // ═══════════════════════════════════════════════════════════════════════════
    
    Serial.println("► BRAKE (emergency stop - maximum force)");
    
    /*
     * car.brake()
     * 
     * This method creates an emergency stop by:
     *   - Setting ALL direction pins to HIGH (IN1, IN2, IN3, IN4 = HIGH)
     *   - Applying maximum PWM to both motors (ENA, ENB = 255)
     *   - Creates a short-circuit condition in the H-bridge
     *   - Motors become generators opposing motion
     * 
     * Result: Robot stops almost instantly with maximum braking force
     * 
     * Physics:
     *   - Moving motor becomes a generator (due to momentum)
     *   - Generated voltage fights the applied short-circuit voltage
     *   - Creates opposing current that resists motion
     *   - Robot decelerates with maximum force
     * 
     * Use brake() ONLY when:
     *   - Emergency stop needed (collision prevention)
     *   - Precision positioning (hold motor against external force)
     *   - Limited duration (don't use continuously)
     * 
     * ⚠️  WARNING: Using brake() continuously:
     *   - Draws very high current (3+ amps)
     *   - May overheat H-bridge and motors
     *   - Can damage power supply if not rated for it
     *   - Consumes battery rapidly
     */
    car.brake();
    delay(1000);
    
    
    // ═══════════════════════════════════════════════════════════════════════════
    // MOVEMENT 6: SHARP LEFT TURN (PIVOT)
    // ═══════════════════════════════════════════════════════════════════════════
    
    Serial.println("► TURN LEFT (sharp pivot)");
    
    /*
     * car.left(speedena, speedenb)
     *        (rightMotor, leftMotor)
     * 
     * This creates a sharp left turn by:
     *   - Left motor:   Rotates BACKWARD
     *   - Right motor:  Rotates FORWARD
     *   - Different speeds create tight pivot
     * 
     * Parameters:
     *   speedena (100) = Right motor speed = 100/255 (~39% power)
     *   speedenb (200) = Left motor speed = 200/255 (~78% power)
     * 
     * The speed difference causes the left side to lag behind the right side,
     * creating a sharp clockwise rotation (when viewed from above).
     * 
     * Turning Modes:
     *   - left(100, 200): Sharp pivot (slow inner wheel, fast outer wheel)
     *   - left(200, 200): Medium pivot (both wheels same speed, opposite direction)
     *   - left(255, 255): Fastest pivot (maximum speed, opposite directions)
     *   - left(150, 100): Very sharp, slow pivot
     * 
     * Recommended turn combinations:
     *   - 90° sharp turn:  ~1000ms at left(100, 200)
     *   - 180° u-turn:     ~2000ms at left(100, 200)
     *   - 45° gentle turn: ~500ms at left(150, 200)
     */
    car.left(100, 200);
    delay(1000);
    
    
    // ═══════════════════════════════════════════════════════════════════════════
    // MOVEMENT 7: SHARP RIGHT TURN (PIVOT)
    // ═══════════════════════════════════════════════════════════════════════════
    
    Serial.println("► TURN RIGHT (sharp pivot)\n");
    
    /*
     * car.right(speedena, speedenb)
     *         (leftMotor, rightMotor)
     * 
     * Mirror image of left() - creates sharp right turn:
     *   - Left motor:   Rotates FORWARD (faster)
     *   - Right motor:  Rotates BACKWARD (slower)
     *   - Result: Pivot right around center point
     * 
     * Same speed rules apply - adjust values to control turn sharpness.
     */
    car.right(200, 100);
    delay(1000);
    
    
    // ═══════════════════════════════════════════════════════════════════════════
    // END OF SEQUENCE
    // ═══════════════════════════════════════════════════════════════════════════
    
    /*
     * At this point, loop() completes and restarts from the top,
     * repeating the entire movement sequence indefinitely.
     * 
     * To stop the robot, either:
     *   1. Press the RESET button on Arduino
     *   2. Cut power to the board
     *   3. Upload a different sketch
     * 
     * Next Steps to Extend This Example:
     *   - Add sensors (distance, light, temperature, etc.)
     *   - Use joystick input to control movement manually
     *   - Add Bluetooth module for wireless control
     *   - Create autonomous navigation with obstacle avoidance
     *   - Implement speed ramping for smooth acceleration
     */
}
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

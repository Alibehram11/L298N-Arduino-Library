# Hardware Setup Guide - Arduino Car Library

Complete hardware wiring guide for Arduino car projects using L298N motor driver.

---

## 📐 Single Motor Setup (BasicCarControl.ino)

![Single Motor Wiring](../images/single-motor-wiring.png)

### Pin Connections

| Arduino Pin | L298N Pin | Purpose |
|-------------|-----------|---------|
| Pin 9 | IN1 | Left Motor Forward |
| Pin 8 | IN2 | Left Motor Backward |
| Pin 10 | ENA | Left Motor Speed (PWM) |
| 5V | 5V | Logic Power |
| GND | GND | Ground |

### Motor Power Supply

- **Motor Supply Voltage:** 6V - 12V (depending on motor)
- **L298N Logic:** 5V from Arduino
- **Current:** 2A per channel max

### Wiring Steps

1. Connect Arduino **Pin 9** → L298N **IN1**
2. Connect Arduino **Pin 8** → L298N **IN2**
3. Connect Arduino **Pin 10** → L298N **ENA** (PWM pin)
4. Connect Arduino **5V** → L298N **+5V**
5. Connect Arduino **GND** → L298N **GND**
6. Connect DC Motor → L298N **OUT1, OUT2**

---

## 🚗 Dual Motor Setup (AdvancedNavigation.ino)

![Dual Motor Wiring](../images/dual-motor-wiring.png)

### Pin Connections

| Arduino Pin | L298N Pin | Purpose |
|-------------|-----------|---------|
| Pin 9 | IN1 | Left Motor Forward |
| Pin 8 | IN2 | Left Motor Backward |
| Pin 7 | IN3 | Right Motor Forward |
| Pin 6 | IN4 | Right Motor Backward |
| Pin 5 | ENA | Left Motor Speed (PWM) |
| Pin 10 | ENB | Right Motor Speed (PWM) |
| 5V | +5V | Logic Power |
| GND | GND | Ground |

### Motor Connections

- **Left Motor** → L298N **OUT1, OUT2**
- **Right Motor** → L298N **OUT3, OUT4**

### Power Supply

- **L298N +12V** → External Power Supply (6V-12V)
- **L298N GND** → External Power Supply GND
- **Arduino GND** → Power Supply GND (common ground)

### Wiring Steps (Detailed)

#### Arduino to L298N
1. Arduino **Pin 9** → L298N **IN1**
2. Arduino **Pin 8** → L298N **IN2**
3. Arduino **Pin 7** → L298N **IN3**
4. Arduino **Pin 6** → L298N **IN4**
5. Arduino **Pin 5** → L298N **ENA**
6. Arduino **Pin 10** → L298N **ENB**
7. Arduino **5V** → L298N **+5V**
8. Arduino **GND** → L298N **GND**

#### Power Supply to L298N
1. Power Supply **+V** (6-12V) → L298N **+12V**
2. Power Supply **GND** → L298N **GND**

#### Motors to L298N
1. Left Motor **Wire 1** → L298N **OUT1**
2. Left Motor **Wire 2** → L298N **OUT2**
3. Right Motor **Wire 1** → L298N **OUT3**
4. Right Motor **Wire 2** → L298N **OUT4**

---

## ⏲️ PWM Pin Requirements

The Enable (EN) pins must be PWM-capable:
- Arduino Uno: **Pins 3, 5, 6, 9, 10, 11**
- Arduino Mega: **Pins 2-13, 44-46**
- Arduino Leonardo: **Pins 3, 5, 6, 9, 10, 13**

In the examples:
- **Pin 5** → PWM (Left motor speed) ✅
- **Pin 10** → PWM (Right motor speed) ✅

---

## 🔋 Power Supply Recommendations

### For Small Motors (3-6V, <200mA)
- 4x AA Battery Pack (6V) per motor
- USB Power Bank (5V, if current < 500mA)

### For Medium Motors (6-9V, 200-500mA)
- LiPo Battery 2S (7.4V)
- 8x AA Battery Pack (12V)
- Regulated Power Supply 9V, 1A+

### For Large Motors (9-12V, 500mA+)
- LiPo Battery 3S (11.1V)
- Car Battery 12V (not recommended, too powerful)
- Regulated Power Supply 12V, 2A+

---

## 🔌 L298N Module Pin Overview

```
           ┌─────────────────────┐
      OUT1 │●        L298N       │ GND
      OUT2 │●   Dual H-Bridge    │ GND
      +12V │●   Motor Driver     │ IN1
       GND │●                    │ IN2
       GND │●                    │ IN3
       ENA │●                    │ IN4
       OUT3│●                    │ ENB
       OUT4│●                    │ +5V
           └─────────────────────┘
```

---

## ⚡ Troubleshooting

| Problem | Solution |
|---------|----------|
| Motors don't move | Check power supply to L298N |
| One motor works, other doesn't | Check EN (PWM) pin connection |
| Both motors work but weak | Increase power supply voltage |
| Car drifts left/right | Adjust motor speeds, calibrate |
| L298N gets hot | Reduce duty cycle or add heatsink |
| Arduino resets | Power supply current too low |

---

## 📚 Component List (BOM)

### Required
- Arduino Uno (or compatible)
- L298N Motor Driver Module (1 needed for 2 motors)
- DC Motors (2x) 3-12V
- Power Supply (6-12V, 1-2A)
- Jumper Wires (20+)
- USB Cable (for Arduino programming)

### Optional but Recommended
- Breadboard (for connections)
- Resistors (10kΩ) - for protection
- Capacitors (100µF) - for power smoothing
- Motor Wheels (2x)
- Robot Car Chassis
- Caster Wheel

---

**Happy Building! 🤖**

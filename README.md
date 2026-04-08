# 🚗 Arduino Car Motor Control Library

<div align="center">

[![Arduino](https://img.shields.io/badge/Arduino-Compatible-00979D?style=for-the-badge&logo=arduino&logoColor=white)](https://www.arduino.cc/)
[![Language](https://img.shields.io/badge/Language-C%2B%2B-blue?style=for-the-badge&logo=cplusplus)](https://isocpp.org/)
[![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)](LICENSE)
[![Version](https://img.shields.io/badge/Version-2.0.0-orange?style=for-the-badge)](https://github.com/Alibehram11/Arduino-Car-Library/releases)

**A professional, well-documented C++ library for controlling L298N dual motor drivers and HC-06 Bluetooth modules on Arduino microcontrollers.**

> Build autonomous robot cars with intuitive commands: `forward()`, `backward()`, `left()`, `right()`, `stop()`, and `brake()`.

[Quick Start](#-quick-start) • [Installation](#-installation) • [API Reference](#-api-reference) • [Examples](#-examples) • [Contributing](#-contributing)

</div>

---

## 📋 Table of Contents

1. [Features](#-features)
2. [Quick Start](#-quick-start)
3. [Hardware Requirements](#-hardware-requirements)
4. [Installation](#-installation)
5. [Pin Configuration](#-pin-configuration)
6. [API Reference](#-api-reference)
7. [Examples](#-examples)
8. [Troubleshooting](#-troubleshooting)
9. [Contributing](#-contributing)
10. [License](#-license)
11. [Language Support](#-language-support)

---

## ✨ Features

- 🎮 **Intuitive Motor Control** — Simple commands for complex robot movements
- 🎯 **Independent Speed Control** — Control each motor separately for precise navigation
- 🎛️ **PWM Speed Adjustment** — Smooth acceleration from 0-255
- ⚡ **Dual Motor Support** — Control two DC motors simultaneously via L298N
- 📱 **Bluetooth Support** — HC-06 wireless control module integration
- 🔒 **Memory Safe** — Proper destructors, null pointer checks, resource cleanup
- 📚 **Extensively Documented** — Line-by-line code documentation with examples
- ⚙️ **Professional Structure** — CMake build system, organized file hierarchy
- 📖 **Beginner-Friendly** — Detailed comments and step-by-step examples
- 🚀 **Production Ready** — Tested, stable, well-maintained codebase

---

## 🚀 Quick Start

### Basic Motor Control (5 minutes)

```cpp
#include <arduino/arduino.h>

Motor car(9, 8, 7, 6, 5, 10);  // Pin configuration (see below)

void setup() {
    Serial.begin(9600);
    Serial.println("Robot car initialized!");
}

void loop() {
    car.forward(255);    // Full speed forward
    delay(2000);
    
    car.backward(150);   // Half speed backward
   delay(2000);
    
    car.left(100, 200);  // Sharp left turn
    delay(1000);
    
    car.stop();          // Smooth stop (coasting)
    delay(500);
    
    car.brake();         // Emergency stop
    delay(500);
}
```

### Bluetooth Control (10 minutes)

```cpp
#include <arduino/arduino.h>

Motor car(9, 8, 7, 6, 5, 10);
HC06 bluetooth(3, 4, -1);  // SoftwareSerial on pins 3, 4

void setup() {
    Serial.begin(9600);
    bluetooth.begin(9600);
}

void loop() {
    String command = bluetooth.readData();
    
    if (command == "FORWARD")  car.forward(200);
    else if (command == "BACK") car.backward(200);
    else if (command == "LEFT") car.left(100, 200);
    else if (command == "RIGHT")car.right(200, 100);
    else if (command == "STOP") car.stop();
}
```

---

## 🔧 Hardware Requirements

### Required Components

| Component | Qty | Purpose | Notes |
|-----------|-----|---------|-------|
| Arduino Uno/Mega/Nano | 1 | Microcontroller | Core of the system |
| L298N Motor Driver | 1 | Dual H-bridge | Controls two motors simultaneously |
| DC Motor 6-12V | 2 | Propulsion | Same voltage/speed rating recommended |
| Power Supply | 1 | Energy | 5V for logic, 6-12V for motors |
| HC-06 Bluetooth | 1 | Wireless control | Optional, for remote operation |
| Connecting Wires | many | Connections | 22 AWG solid core recommended |

### Component Specifications

**L298N Motor Driver:**
- Max Operating Voltage: 46V
- Max Current (per channel): 2A continuous
- Logic Voltage: 5V (Arduino compatible)
- PWM Support: Yes
- Brake Support: Yes

**HC-06 Bluetooth Module:**
- Communication: UART serial at 9600 baud
- Operating Voltage: 3.3V - 5V (5V tolerant)
- Bluetooth Version: 2.1 + EDR
- Range: 10-100 meters (environment dependent)
- Max Current: ~40 mA

---

## 📥 Installation

### Method 1: Arduino IDE Library Manager (Recommended)

1. Open **Arduino IDE**
2. Go to **Sketch** → **Include Library** → **Manage Libraries**
3. Search for "Arduino Car"
4. Click **Install** on the ArduinoCar library
5. Include in your sketch: `#include <arduino/arduino.h>`

### Method 2: Manual Installation

1. Download this repository (GitHub: Code → Download ZIP)
2. Extract the ZIP file
3. Move the `ArduinoCar` folder to your Arduino libraries directory:
   - **Windows**: `Documents\Arduino\libraries\`
   - **Mac**: `~/Documents/Arduino/libraries/`
   - **Linux**: `~/Arduino/libraries/`
4. Restart Arduino IDE
5. Library should now appear under **Sketch** → **Include Library**

### Method 3: Git Clone (For Developers)

```bash
cd ~/Arduino/libraries  # or Documents\Arduino\libraries on Windows
git clone https://github.com/Alibehram11/Arduino-Car-Library.git ArduinoCar
```

---

## 📌 Pin Configuration

### Dual Motor Setup (Standard Robot Car)

```
┌─────────────────────────────────────────────────┐
│             Arduino Uno                          │
├─────────────────────────────────────────────────┤
│ Pin 9  ──────────────────> IN1  ┌────────────┐  │
│ Pin 8  ──────────────────> IN2  │  L298N     │  │
│ Pin 7  ──────────────────> IN3  │  Module    │  │
│ Pin 6  ──────────────────> IN4  │            │  │
│ Pin 5 (PWM) ────────────> ENA   │            │  │
│ Pin 10 (PWM) ──────────> ENB    └────────────┘  │
│ GND ───────────────────> GND                    │
│ 5V ────────────────────> +5V                    │
│                                                  │
│ ⚠️ IMPORTANT: Pins 5 and 10 MUST be PWM pins!  │
│ PWM pins on Arduino Uno: 3, 5, 6, 9, 10, 11    │
└─────────────────────────────────────────────────┘

Motor Connections:
  Left Motor:  L298N OUT1, OUT2 (controlled by IN1, IN2)
  Right Motor: L298N OUT3, OUT4 (controlled by IN3, IN4)
  
Power Connections:
  Motor Power: +12V, GND to L298N +12V, GND
  Logic Power: Arduino 5V, GND to L298N +5V, GND
```

###Bluetooth Module Connection

```
HC-06 Bluetooth ────> Arduino
├─ TX (green)      ──> Pin 3 (with voltage divider!)
├─ RX (blue)       ──> Pin 4
├─ VCC (red)       ──> 5V
└─ GND (black)     ──> GND

⚠️  CRITICAL: HC-06 is 3.3V logic!
Use voltage divider on RX line:
  HC-06 TX ──[1KΩ]──┬──> Arduino Pin 3
                    │
                 [2KΩ]
                    │
                   GND
```

---

## 📖 API Reference

### Motor Class

#### Constructor

```cpp
Motor(int in1, int in2, int in3, int in4, int ena, int enb)
```

Creates a Motor object and configures pins.

| Parameter | Description |
|-----------|-------------|
| `in1` | Motor 1 forward direction pin |
| `in2` | Motor 1 backward direction pin |
| `in3` | Motor 2 forward direction pin |
| `in4` | Motor 2 backward direction pin |
| `ena` | Motor 1 PWM speed pin (must be PWM pin) |
| `enb` | Motor 2 PWM speed pin (must be PWM pin) |

#### Methods

##### void forward(int speed)
Move both motors forward.
- **speed**: 0-255 (0=stop, 255=full)

##### void backward(int speed)
Move both motors backward.

##### void left(int speedena, int speedenb)
Sharp left turn. Motors rotate in opposite directions.
- **speedena**: Right motor speed (0-255)
- **speedenb**: Left motor speed (0-255)

##### void right(int speedena, int speedenb)
Sharp right turn. Motors rotate in opposite directions.

##### void WideTurnLeft(int speedena, int speedenb)
Smooth left curve. Both motors forward at different speeds.

##### void WideTurnRight(int speedena, int speedenb)
Smooth right curve. Both motors forward at different speeds.

##### void stop()
Coasting stop - motors disabled. Smooth deceleration.

##### void brake()
Emergency stop - active braking. Maximum force, instant stop.

⚠️ **Warning**: Brake draws high current. Use sparingly to avoid overheating.

---

### HC06 Bluetooth Class

#### Constructor

```cpp
HC06(int rxPin, int txPin, int hc06)
```

#### Methods

##### void begin(long baudRate)
Initialize communication. HC-06 default: 9600 baud.

##### void sendData(const String& data)
Send string message via Bluetooth.

##### String readData()
Receive message (non-blocking). Returns empty string if no data.

##### void end()
Stop communication. Frees pins for other use.

##### void reset()
Hardware reset - toggles RX/TX pins.

##### void attempt()
Debug method - prints received data to Serial.

---

## 🎓 Examples

Comprehensive examples with line-by-line documentation:

### [BasicCarControl.ino](examples/BasicCarControl.ino)
**Beginner Level**
- Forward/backward movement
- Sharp and smooth turns
- Emergency stop techniques
- Perfect for learning

### [AdvancedNavigation.ino](examples/AdvancedNavigation.ino)
**Intermediate Level**
- Complex movement patterns
- Autonomous navigation
- Speed ramping (acceleration)
- Geometric patterns (square, spiral)

Both examples include:
- ✅ Pin configuration diagrams
- ✅ Step-by-step comments
- ✅ Expected behavior description
- ✅ Tuning suggestions
- ✅ Troubleshooting tips

**To run examples:**
1. Arduino IDE → **File** → **Examples** → **ArduinoCar**
2. Select desired example
3. Update pin numbers to match your wiring
4. Verify and upload to Arduino

---

## 🔍 Troubleshooting

### "Arduino port not recognized"
→ Install USB drivers for your Arduino board  
→ Try different USB cable  
→ Check Device Manager for unknown devices  

### "Motors don't spin"
→ Verify all pin connections are secure  
→ Check motor power supply voltage  
→ Test motor manually (should spin when powered)  
→ Verify pins 5 and 10 are PWM-capable  

### "Robot pulls to one side"
→ Motors have unequal speeds  
→ Adjust speed values slightly (e.g., 200 vs 190)  
→ Check motor mechanical coupling  
→ Verify wheels rotate freely

### "Compile error: 'Motor' not found"
→ Verify library installed in Libraries folder  
→ Restart Arduino IDE completely  
→ Delete ` .arduino-ide` folder (forces cache refresh)  
→ Check #include statement: `#include <arduino/arduino.h>`

### "Compilation succeeds but upload fails"
→ Select correct Board type (Tools → Board)  
→ Select correct COM port (Tools → Port)  
→ Try different USB cable  
→ Hold RESET as upload starts

### "Some features not working"
→ Verify all libraries installed  
→ Check if using SoftwareSerial on correct pins  
→ Monitor Serial output for error messages  
→ Open GitHub Issue with error details

für more help, see [examples/README.md](examples/README.md) or open a  [GitHub Issue](https://github.com/Alibehram11/Arduino-Car-Library/issues).

---

## 🤝 Contributing

Contributions are welcome! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for:

- 🐛 How to report bugs
- 🎯 How to suggest enhancements  
- 🔧 Development setup
- 📝 Code style guidelines
- ✅ Testing requirements

**Quick contribution steps:**

1. Fork the repository
2. Create feature branch: `git checkout -b feature/my-feature`
3. Make changes and comment thoroughly
4. Test on real Arduino hardware
5. Push and create Pull Request
6. Describe changes clearly in PR description

---

## 📄 License

This project is licensed under the **MIT License** - see [LICENSE](LICENSE) file for details.

**You are free to:**
- ✅ Use commercially
- ✅ Modify
- ✅ Distribute
- ✅ Use privately

**You must:**
- ✅ Include license and copyright notice

---

## 📞 Support & Community

- **Documentation**: [docs/](docs/) folder
- **GitHub Issues**: [Report bugs](https://github.com/Alibehram11/Arduino-Car-Library/issues)
- **GitHub Discussions**: [Ask questions](https://github.com/Alibehram11/Arduino-Car-Library/discussions)
- **Examples**: [See working code](examples/)

---

<details>
<summary><h2>🇹🇷 Turkish Language Support (Türkçe Destek)</h2></summary>

## Arduino Araba Kontrol Kütüphanesi

Bu kütüphane, Arduino tabanlı arabalı robotlar inşa ederken L298N çift H-köprü motor sürücü modülünü kullanmayı kolaylaştırır.

### Hızlı Başlangıç

```cpp
#include <arduino/arduino.h>

Motor araba(9, 8, 7, 6, 5, 10);

void setup() { Serial.begin(9600); }

void loop() {
    araba.forward(255);   // İleri
    delay(2000);
    
    araba.backward(150);  // Geri
    delay(2000);
    
    araba.left(100, 200); // Sola dön
    delay(1000);
    
    araba.stop();         // Dur
}
```

### Bluetooth Kontrolü

```cpp
HC06 bluetooth(3, 4, -1);

void loop() {
    String komut = bluetooth.readData();
    if (komut == "İLERİ") araba.forward(200);
    else if (komut == "GERİ") araba.backward(200);
}
```

### API Referansı

- `forward(speed)` — İleri (0-255)
- `backward(speed)` — Geri (0-255)
- `left(speedena, speedenb)` — Sol döş
- `right(speedena, speedenb)` — Sağ döş
- `stop()` — Serbest dur
- `brake()` — Acil fren

### Pin Bağlantıları

```
Arduino Pin 9  → L298N IN1  (Sol motor ileri)
Arduino Pin 8  → L298N IN2  (Sol motor geri)
Arduino Pin 7  → L298N IN3  (Sağ motor ileri)
Arduino Pin 6  → L298N IN4  (Sağ motor geri)
Arduino Pin 5  → L298N ENA  (Sol motor hız - PWM pini)
Arduino Pin 10 → L298N ENB  (Sağ motor hız - PWM pini)
Arduino GND    → L298N GND
Arduino 5V     → L298N 5V
```

Daha fazla bilgi için [docs/HARDWARE_SETUP.md](docs/HARDWARE_SETUP.md) dosyasını inceleyiniz.

</details>

---

<details>
<summary><h2>🇨🇳 Chinese Language Support (中文)</h2></summary>

## Arduino 小车电机控制库

这是一个专业的 C++ 库，用于在 Arduino 微控制器上控制 L298N 双电机驱动器和 HC-06 蓝牙模块。

### 快速开始

```cpp
#include <arduino/arduino.h>

Motor car(9, 8, 7, 6, 5, 10);

void setup() { Serial.begin(9600); }

void loop() {
    car.forward(255);   // 前进
    delay(2000);
    
    car.backward(150);  // 后退
    delay(2000);
    
    car.stop();         // 停止
}
```

### 主要方法

- `forward(speed)` — 前进 (0-255)
- `backward(speed)` — 后退 (0-255)
- `left(speedena, speedenb)` — 左转
- `right(speedena, speedenb)` — 右转
- `stop()` — 停止
- `brake()` — 制动

</details>

---

**Built with ❤️ for the maker community worldwide**

**[GitHub](https://github.com/Alibehram11/Arduino-Car-Library) • [Issues](https://github.com/Alibehram11/Arduino-Car-Library/issues) • [License](LICENSE)**

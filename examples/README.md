# Examples - Arduino Car Library

Complete working examples with extensive documentation covering motor control, wireless communication, and sensor integration.

## 📚 Example List

### 1. **BasicCarControl.ino** - Beginner Level
Learn fundamental motor control with simple movement patterns.

**Topics Covered:**
- ✅ Forward movement (full and half speed)
- ✅ Backward movement
- ✅ Left/Right turns (sharp pivot)
- ✅ Coasting stop (free spin)
- ✅ Emergency brake (active stop)
- ✅ Speed ramping functions
- ✅ Geometric patterns (square, spiral)

**Estimated Runtime:** ~20 seconds per loop cycle

**Pin Configuration:**
```
Arduino Pin 9   → L298N IN1  (Left Motor forward)
Arduino Pin 8   → L298N IN2  (Left Motor backward)
Arduino Pin 7   → L298N IN3  (Right Motor forward)
Arduino Pin 6   → L298N IN4  (Right Motor backward)
Arduino Pin 5   → L298N ENA  (Left Motor PWM)  ← PWM pin required
Arduino Pin 10  → L298N ENB  (Right Motor PWM) ← PWM pin required
GND             → L298N GND
5V              → L298N Logic +5V
```

---

### 2. **AdvancedNavigation.ino** - Intermediate Level
Autonomous movement patterns with complex navigation algorithms.

**Topics Covered:**
- ✅ Square path navigation
- ✅ Zigzag maneuver patterns
- ✅ Spiral movement sequences
- ✅ Speed acceleration ramps
- ✅ Smooth cornering techniques
- ✅ Coordinated two-motor movement
- ✅ Timing-based distance estimation
- ✅ Pose/orientation tracking simulation

**Estimated Runtime:** Varies by pattern (30-60 seconds per loop)

---

### 3. **AdvancedUltrasonicSensing.ino** ⭐ NEW - Intermediate Level
Professional-grade ultrasonic distance measurement with noise filtering and comprehensive diagnostics.

**Topics Covered:**
- ✅ Single measurement mode (basic)
- ✅ Multi-sample averaging (5-point filtered)
- ✅ Non-blocking asynchronous measurements
- ✅ Noise filtering with median deviation
- ✅ Signal quality metrics
- ✅ Measurement statistics (min/max/average)
- ✅ Echo duration diagnostics
- ✅ Real-time configuration tuning
- ✅ Serial command interface

**Key Features:**
- 🎯 **Accuracy:** Noise filtering reduces outliers for stable readings
- ⚡ **Performance:** Non-blocking async mode allows parallel processing
- 📊 **Diagnostics:** Real-time signal quality, valid sample counting
- 🔧 **Configurable:** Adjust range, threshold, and timeout on the fly

**Pin Configuration:**
```
Arduino Pin 9   → HC-SR04 TRIG (trigger signal)
Arduino Pin 10  → HC-SR04 ECHO (echo signal)
GND             → HC-SR04 GND
5V              → HC-SR04 VCC
```

**Serial Commands:**
```
1 - Switch to Single Measurement mode
2 - Switch to Averaged Measurement mode (5 samples)
3 - Switch to Asynchronous mode
4 - Switch to Continuous Measurement
d - Display debug information
r - Reset statistics
h - Help menu
```

**Use Cases:**
- Obstacle detection for autonomous robots
- Parking distance warning systems
- Water/level monitoring
- Motion detection
- Proximity-based triggers

---

### 4. **RadioLinkI2CReceiver.ino** ⭐ NEW - Advanced Level
I2C-based radio receiver interface with complete signal monitoring and packet processing.

**Topics Covered:**
- ✅ I2C communication with RF module
- ✅ Data packet reception and decoding
- ✅ RSSI (Received Signal Strength) monitoring
- ✅ Link quality indicator (LQI) processing
- ✅ Packet statistics and error tracking
- ✅ Command interpretation (ASCII mode)
- ✅ Connection health assessment (0-4 scale)
- ✅ Channel scanning and tuning
- ✅ Self-diagnostic testing
- ✅ LED status indicators

**Key Features:**
- 🛰️ **Flexible Protocol:** Works with NRF24L01, LoRa, and generic I2C RF modules
- 📡 **Signal Analysis:** Real-time RSSI and link quality metrics
- 📤 **Low Latency:** Non-blocking event-driven reception
- 🔍 **Diagnostics:** Comprehensive self-test and debug capabilities
- 🎛️ **Configuration:** Runtime channel and power level adjustment

**Hardware Setup (I2C Bus):**

**Arduino (Master) ← I2C → RF Module (Slave)**
```
Arduino SDA (Pin A4 on Uno) → RF Module SDA
Arduino SCL (Pin A5 on Uno) → RF Module SCL
Arduino GND                  → RF Module GND
Arduino 5V                   → RF Module VCC (check voltage requirements)

Optional Status LEDs:
Arduino Pin 13 → Green LED  (RX indicator)
Arduino Pin 12 → Yellow LED (Weak signal warning)
Arduino Pin 11 → Red LED    (Error indicator)
```

**Typical RF Module I2C Addresses:**
```
0x20 - NRF24L01 (default, addressable)
0x60 - LoRa modules (typical)
0x40-0x7F - Other RF receivers (check datasheet)
```

**Serial Commands:**
```
i - Print complete debug information
c - Scan channels to find strongest signal
s - Run self-diagnostic tests
r - Reset statistics counters
d - Enable module debug mode
q - Disable module debug mode
h - Display command help
```

**Received Packet Format:**
```
Default ASCII Commands (example):
F - Forward command
B - Backward command
L - Left turn command
R - Right turn command
S - Stop command

Custom packets displayed in hexadecimal.
```

**Connection Health Levels:**
```
0/4 ─ NO CONNECTION    (no signal detected)
1/4 ─ WEAK             (unreliable, interference possible)
2/4 ─ FAIR             (acceptable, occasional issues)
3/4 ─ GOOD             (reliable connection, minimal errors)
4/4 ─ EXCELLENT        (very stable, optimal signal)
```

**Use Cases:**
- Remote-controlled robotics receiver
- Wireless sensor data aggregation
- IoT gateway/coordinator
- Multi-robot communication hub
- Distributed control systems

---

## 🚀 How to Use These Examples

### Method 1: Arduino IDE (Recommended)

1. **Open Arduino IDE** on your computer
2. **Sketch** → **Include Library** → **ArduinoCar** (if installed via IDE Library Manager)
3. **File** → **Examples** → **ArduinoCar** → Select example
4. **Verify** code: Sketch → Verify/Compile (shortcut: Ctrl+R)
5. **Connect** Arduino via USB cable
6. **Select Board**: Tools → Board → Arduino Uno (or your board)
7. **Select Port**: Tools → Port → COM3 (or your USB port)
8. **Upload**: Sketch → Upload (shortcut: Ctrl+U)
9. **Monitor**: Tools → Serial Monitor (shortcut: Ctrl+Shift+M)
   - Set baud rate to 9600
   - Watch output and diagnostic messages

### Method 2: Copy-Paste

1. Copy entire example code from `.ino` file
2. Open Arduino IDE and create new sketch
3. Paste the code
4. Update pin numbers if your setup differs
5. Verify and upload

### Method 3: CMake Build System

For advanced users using CMake:

```bash
cd /path/to/ArduinoCar
mkdir build && cd build
cmake -DBUILD_EXAMPLES=ON ..
make
```

---

## 📋 Detailed Setup Guides

### BasicCarControl Setup

1. Connect L298N motor driver to Arduino pins (see pin configuration above)
2. Connect motor power supply (6-12V) to L298N
3. Upload BasicCarControl.ino
4. Open Serial Monitor (9600 baud) to see movement commands
5. Robot will perform sequence: forward → backward → turns → stop

### AdvancedUltrasonicSensing Setup

1. Connect HC-SR04 sensor (Trigger Pin 9, Echo Pin 10)
2. Upload AdvancedUltrasonicSensing.ino
3. Open Serial Monitor (9600 baud)
4. Default mode: Averaged measurement with filtering
5. Send serial commands to switch modes and tune parameters
6. Watch statistics update in real-time

### RadioLinkI2CReceiver Setup

1. Enable I2C on Arduino (Wire library enabled by default)
2. Connect RF module to I2C bus (SDA/SCL)
3. Verify I2C module address (default 0x20)
4. Upload RadioLinkI2CReceiver.ino
5. Open Serial Monitor (9600baud)
6. Module will scan and auto-configure
7. Run 's' command to verify self-tests pass
8. Use 'c' command to find active signal channels
9. Monitor incoming packets in real-time

---

## 🔧 Troubleshooting

### Motor Control Issues
- **Motors don't spin:** Check pin connections, verify 5V+ motor power
- **Robot pulls to one side:** Motor speeds unbalanced, adjust PWM values
- **Weak performance:** Motor power supply may be insufficient

### Ultrasonic Sensor Issues
- **Invalid measurements:** Sensor module not responding, check pins
- **Unstable readings:** Increase averaging samples or filtering threshold
- **Out of range errors:** Object too close (<5cm) or too far (>200cm)

### I2C Radio Issues
- **Module not found:** Wrong I2C address, check datasheet, try channel scan
- **No packets received:** Transmitter not paired/connected, check channel
- **Weak signal:** Move closer to transmitter or reduce distance obstruction
- **Communication errors:** Check SDA/SCL wiring, verify 5V supply

### General Debugging
- Always check Serial Monitor output first
- Enable debug mode (command 'd') for detailed diagnostics
- Run self-tests (command 's') to verify hardware
- Check CMakeLists.txt version matches installed library

---

## 📚 Additional Resources

- **Arduino Documentation:** https://www.arduino.cc/reference/
- **HC-SR04 Datasheet:** Ultrasonic distance measurement specifications
- **L298N Guide:** Motor driver module pinout and specifications
- **I2C Protocol:** https://en.wikipedia.org/wiki/I%C2%B2C
- **NRF24L01 Documentation:** RF module communication protocol
- **Project README:** ../README.md (main project documentation)

---

## 📝 Notes

- All examples run at **9600 baud** for serial communication
- Examples are compatible with Arduino Uno, Nano, and Mega (with pin adjustments)
- Use PWM-capable pins for motor speed control
- I2C examples require SDA/SCL pins (A4/A5 on Uno, 20/21 on Mega)
- Always verify power supply capacity before powering high-current devices

---

**Last Updated:** April 2026  
**Library Version:** 2.1.0

3. Verify → Compile code to check for syntax errors
4. Upload → Transfer code to Arduino board (USB connection required)
5. Watch Arduino IDE status bar for "Upload complete" message

### Step 4: Test & Debug

1. Open Serial Monitor (Tools → Serial Monitor)
2. Verify baud rate is set to 9600
3. You should see diagnostic messages:
   ```
   ╔════════════════════════════════════════════════════════╗
   ║     ARDUINO CAR LIBRARY - BASIC CONTROL EXAMPLE       ║
   ...
   ```
4. Observe robot behavior and compare with expected movements
5. If motors don't move, check Serial output and consult "Troubleshooting" section

---

## ⚙️ Troubleshooting Guide

### Problem: "Arduino port not recognized"
**Solution:**
- Install CH340/USB driver for your Arduino board
- Try different USB cable (some cables are charging-only)
- Check Device Manager for unknown devices
- Restart Arduino IDE

### Problem: "Sketch failed to upload"
**Solution:**
- Verify correct board type selected (Tools → Board)
- Verify correct COM port selected (Tools → Port)  
- Try unplugging and replugging USB cable
- Press RESET button on Arduino before uploading

### Problem: "Motors don't spin at all"
**Solution:**
- Check all pin connections for loose wires
- Verify motor power supply is connected and turned on
- Measure voltage at motor terminals with multimeter
- Try moving motors by hand - check for physical jams
- Test with simple digitalWrite sketch to verify pins work

### Problem: "Robot moves but doesn't turn correctly"
**Solution:**
- Motors have different speeds - adjust speed values
- Check wheel mechanical connection (set screw loose?)
- Verify both motors are same voltage/speed rating
- Try MotorTest: equal left/right speeds should go straight

### Problem: "Compile error: 'Motor' does not name a type"
**Solution:**
- Verify library installed correctly in `/Arduino/libraries/`
- Restart Arduino IDE completely
- Check #include statement points to correct header file
- Delete `.arduino-ide` folder and restart (forces cache refresh)

### Problem: "Motor spins but wheels don't move"
**Solution:**
- Check motor shaft is properly coupled to wheel
- Verify coupling/collar/setscrew is tight
- Check for stripped gears inside motor
- Make sure no wires interfere with wheel rotation

---

## 🔧 Customization Guide

### Adjusting Motor Speeds

To make robot move slower:
```cpp
// Original:
car.forward(200);  // Fast

// Modified:
car.forward(100);  // Slower
```

### Adjusting Turn Duration

To make sharper turns:
```cpp
// Original:
car.left(100, 200);
delay(1000);  // 1 second turn

// Sharper:
car.left(100, 200);
delay(2000);  // 2 second turn (tighter angle)
```

### Adding Acceleration Ramp

```cpp
// Smooth acceleration:
for (int speed = 0; speed <= 200; speed += 10) {
    car.forward(speed);
    delay(50);
}
```

---

## 📊 Performance Metrics

These measurements help you calibrate robot behavior for your setup:

| Parameter | Typical Value | Factors |
|-----------|---------------|---------|
| 90° turn time | ~800ms | Motor speed, friction, weight |
| Straight-line distance | VariesWheelSize | Wheel diameter, motor speed |
| Power consumption | 500-2000mA | Speed, load, battery voltage |
| Top speed | 1-2 m/s | Motor RPM, gear ratio, wheel size |
| Min. turning radius | 5-15cm | Motor speeds, wheel base |

---

## 🎓 Learning Path

**Suggested progression through examples:**

1. Start with **BasicCarControl.ino**
   - Understand basic movement commands
   - Get comfortable with Arduino IDE workflow
   - Test your robot's responsiveness

2. Modify BasicCarControl with different speeds/delays
   - Experiment with tuning
   - Develop intuition for motor control

3. Progress to **AdvancedNavigation.ino**
   - Study complex movement patterns
   - Understand coordinated two-motor control
   - Learn autonomous navigation techniques

4. Build your own sketch base on examples
   - Add Bluetooth control (HC06 class)
   - Add sensor integration (distance, light, temperature)
   - Create your own movement patterns

---

## 📞 Support

- Questions? Open a [GitHub Issue](https://github.com/Alibehram11/Arduino-Car-Library/issues)
- Found a bug? [Submit an Issue with details](https://github.com/Alibehram11/Arduino-Car-Library/issues/new)
- Want to contribute? See [CONTRIBUTING.md](../CONTRIBUTING.md)

---

**Happy robotics! 🤖**

![Single Motor Setup](../images/single-motor-wiring.png)

>Detaylı donanım rehberi için: [HARDWARE_SETUP.md](../docs/HARDWARE_SETUP.md)

**Donanım:**
```
Arduino Pin 9  --> L298N IN1 (Sol Motor İleri)
Arduino Pin 8  --> L298N IN2 (Sol Motor Geri)
Arduino Pin 7  --> L298N IN3 (Sağ Motor İleri)
Arduino Pin 6  --> L298N IN4 (Sağ Motor Geri)
Arduino Pin 5  --> L298N ENA (Sol Motor PWM)
Arduino Pin 10 --> L298N ENB (Sağ Motor PWM)
```

**Nasıl Kullanılır:**
1. Donanımı şemaya göre bağlayın
2. Kodun başındaki pin numaralarını kontrol edin
3. Arduino IDE'de yükleyin
4. Serial Monitor (9600 baud) açarak hareketi izleyin

---

### 2. **AdvancedNavigation.ino** (Proje Geliştirmeye İleri)
Kompleks hareket desenlerini demonstre eder.

**Kapsar:**
- ✅ Kare şeklinde hareket
- ✅ Zigzag manevra (engel kaçınması)
- ✅ Spiral hareketi
- ✅ Hız ramplama (ivme testi)
- ✅ Pürüzsüz viraj testleri

**Donanım Şeması (Çift Motor):**

![Dual Motor Setup](../images/dual-motor-wiring.png)

>Detaylı donanım rehberi: [HARDWARE_SETUP.md](../docs/HARDWARE_SETUP.md)

**Tipler:**

| Hareket | Açıklama |
|---------|----------|
| **Square** | Dört kenarlı kapalı döngü |
| **Zigzag** | Soldan sağa sallantılı hareket |
| **Spiral** | Artan yarıçaplı daireler |
| **Ramp** | Yavaştan hızlıya ivme testi |
| **Cornering** | Farklı dönüş hızları testi |

---

## 🚗 Hız Referansı

```
Motor Hız Aralığı (0-255):

0-50       → ⚠️ Çok Yavaş   (hassas hareket, kalibasyon)
50-150     → 🚕 Orta Hız    (dönerken kontrol edilen hız)
150-255    → 🏎️ Yüksek Hız   (hızlı hareket, yarış)
```

---

## 🎯 Dönüş Önerileri

### Hafif Dönüş (Yüksek Hız Köşe - 150+ km/h simulasyon)
```cpp
car.left(180, 200);   // Hızlar arasında az fark (~20)
car.right(200, 180);  // Dış motor daha hızlı
```

### Keskin Dönüş (Düşük Hız - 30 km/h simulasyon)
```cpp
car.left(50, 150);    // Hızlar arasında çok fark (~100+)
car.right(150, 50);   // Bir motor neredeyse durur
```

### Yerinde Döne (Spin)
```cpp
car.left(0, 180);     // Sol motor durur, sağ hareket eder
car.right(180, 0);    // Sağ motor durur, sol hareket eder
```

---

## 🔧 Araç Kalibrasyonu

Araçınız hareket ederken sola veya sağa saparsa:

1. **Sola sapıyor?** → Sağ motorun hızını arttırın
2. **Sağa sapıyor?** → Sol motorun hızını arttırın

**Örnek:**
```cpp
// Sapıp olmaksızın ileri git
car.forward(170);  // Sol motor
car.forward(180);  // Sağ motor (biraz daha hızlı)
```

---

## 📝 Kendi Örneğinizi Yazma

```cpp
#include "motor.h"

Motor car(9, 8, 7, 6, 10, 5);

void setup() {
  Serial.begin(9600);
}

void loop() {
  car.forward(200);      // İleri git
  delay(2000);

  car.left(100, 200);    // Sola dön
  delay(1000);

  car.stop();            // Dur
  delay(500);

  car.backward(150);     // Geri git
  delay(2000);

  car.brake();           // Acil durdur
  delay(1000);
}
```

---

## 🐛 Sorun Giderme

| Problem | Çözüm |
|---------|-------|
| Motorlar çalışmıyor | Pin bağlantılarını kontrol edin |
| Bir motor çalışıyor diğer hayır | Enable pinini kontrol edin, PWM pini olmalı |
| Araç sapıyor | Motor hızlarını kalibre edin |
| Hareket çok yavaş | Motor hızını 100+ artırın |
| Motoru kontrol edemiyorum | L298N güç kaynağını kontrol edin |

---

## 📚 Daha Fazla Bilgi

- Ana kütüphane: `motor.h` ve `motor.cpp`
- Tam API referansı: Proje README.md dosyası
- Arduino L298N dokümantasyonu: [L298N Datasheet](https://www.st.com/resource/en/datasheet/l298.pdf)

---

**Happy Coding! 🚗💨**

# Examples - Arduino Car Library

Complete working examples with extensive documentation.

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
Arduino Pin 10  → L298N ENB  (Right Motor PWM) ← PWMPin required
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
   - Watch robot movement and diagnostic messages

### Method 2: Copy-Paste

1. Copy entire example code from `.ino` file
2. Open Arduino IDE and create new sketch
3. Paste the code
4. Update pin numbers if your setup differs
5. Verify and upload

---

## 📋 Step-by-Step Guide: Running BasicCarControl

### Step 1: Physical Setup

- [ ] Connect Arduino to computer via USB cable
- [ ] Connect L298N GND to Arduino GND  
- [ ] Connect L298N +5V to Arduino 5V
- [ ] Connect motor power supply (6-12V) to L298N +12V and GND
- [ ] Connect all control pins (IN1-IN4) to specified Arduino pins
- [ ] Connect PWM pins (ENA, ENB) to PWM-capable Arduino pins
- [ ] Double-check all connections before powering on

### Step 2: Software Setup

1. Install Arduino IDE from [arduino.cc](https://www.arduino.cc)
2. Clone/download this library to `/Documents/Arduino/libraries/ArduinoCar`
3. Restart Arduino IDE to recognize new library
4. Verify: Sketch → Include Library → ArduinoCar (should appear in list)

### Step 3: Upload Code

1. Open BasicCarControl.ino in Arduino IDE
2. Verify the pin numbers match your wiring (lines 97-104)
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

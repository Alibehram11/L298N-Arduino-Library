# Arduino Car Library
<div align="center">

# 🚗 Arduino Car Motor Control Library

### Türkçe | [English](#-english-documentation)

[![Arduino](https://img.shields.io/badge/Arduino-Compatible-00979D?style=for-the-badge&logo=arduino&logoColor=white)](https://www.arduino.cc/)
[![Language](https://img.shields.io/badge/Language-C%2B%2B-blue?style=for-the-badge&logo=cplusplus)](https://isocpp.org/)
[![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)](LICENSE)
[![Version](https://img.shields.io/badge/Version-2.0.0-orange?style=for-the-badge)](https://github.com/Alibehram11/Arduino-Car-Library/releases)

> **Arduino Arabalı Robotlar için L298N motor sürücü modülünü kolayca kontrol etmek için tasarlanmış, güçlü bir C++ kütüphanesi. İleri, geri, dön ve fren gibi araç hareketlerini tek bir komutla gerçekleştirin.**

</div>

---

## 🇹🇷 Türkçe Dokümantasyon

### 📋 İçindekiler
- [Genel Bakış](#-genel-bakış)
- [L298N Nedir?](#-l298n-nedir)
- [Özellikler](#-özellikler)
- [Donanım Bağlantısı](#-donanım-bağlantısı)
- [Kurulum](#-kurulum)
- [Kullanım](#-kullanım)
- [API Referansı](#-api-referansı)
- [Örnek Kodlar](#-örnek-kodlar)
- [Donanım Rehberi](docs/HARDWARE_SETUP.md)

---

### 🌟 Genel Bakış

Bu kütüphane, Arduino tabanlı arabalı robotlar (robot arabalar) inşa ederken L298N çift H-köprü motor sürücü modülünü kullanmayı kolaylaştırır. Karmaşık dijital sinyal yönetimini sizin adınıza halleder; siz sadece `forward()`, `backward()`, `left()`, `right()`, `stop()` ve `brake()` gibi anlaşılır komutlarla aracınızı kontrol edersiniz.

**Temel Kullanım:**
- 🚗 **forward(speed)** — Araç ileriye git
- 🚗 **backward(speed)** — Araç geriye git  
- ⬅️ **left(speedLeft, speedRight)** — Sola dön
- ➡️ **right(speedLeft, speedRight)** — Sağa dön
- ⛔ **stop()** — Araç dursun (serbest)
- 🛑 **brake()** — Araç acil fren

---

### 🔩 L298N Nedir?

L298N, DC motorları ve step motorları kontrol etmek için kullanılan çift H-köprü motor sürücü entegresidir. Aynı anda **iki bağımsız DC motoru** veya **bir step motoru** sürebilir.

| Özellik | Değer |
|---|---|
| Maksimum Çalışma Gerilimi | 46V |
| Maksimum Akım (kanal başına) | 2A |
| Mantık Gerilimi | 5V (Arduino uyumlu) |
| PWM Desteği | ✅ Evet |
| Fren (Brake) Desteği | ✅ Evet |

---

### ✨ Özellikler

- � **Araç Kontrol Fonksiyonları** — İleri, geri, sol döş, sağ dön, duro commands
- 🎯 **Bağımsız Motor Kontrolü** — Her motorun hızını ayrı ayarlayın (sol/sağ)
- 🎛️ **PWM Hız Kontrolü** — `analogWrite` ile 0–255 arası hassas hız ayarı
- ⛔ **İki Farklı Durdurma Modu** — Serbest duruş (`stop`) ve aktif fren/ani durdurma (`brake`)
- 🔌 **Hafif ve Verimli** — Sadece `<Arduino.h>` bağımlılığı
- 📦 **Kolay Entegreasyon** — Plug & play, minimum setup gerekli

---

### 🔌 Donanım Bağlantısı

#### Çift Motor Kurulumu (Araç Kontrolü)

![Arduino Car Wiring - Dual Motor L298N](images/dual-motor-wiring.png)

```
Arduino          L298N Modülü
-------          ------------
Pin 9    ──────► IN1 (Sol Motor İleri)
Pin 8    ──────► IN2 (Sol Motor Geri)
Pin 7    ──────► IN3 (Sağ Motor İleri)
Pin 6    ──────► IN4 (Sağ Motor Geri)
Pin 5    ──────► ENA (Sol Motor PWM)
Pin 10   ──────► ENB (Sağ Motor PWM)
5V       ──────► +5V (Logic Power)
GND      ──────► GND
```

**Motor Bağlantıları:**
- Sol Motor → L298N OUT1, OUT2
- Sağ Motor → L298N OUT3, OUT4

> **⚠️ Dikkat:** The EN ports must connect to PWM-capable pins (Arduino Uno: 3, 5, 6, 9, 10, 11).

**Detaylı Donanım Rehberi:** [docs/HARDWARE_SETUP.md](docs/HARDWARE_SETUP.md)

---

### 📥 Kurulum

**Manuel kurulum:**

1. Bu repoyu ZIP olarak indirin:
   ```
   https://github.com/Alibehram11/L298N-Arduino-Library/archive/refs/heads/main.zip
   ```
2. `motor.h` ve `motor.cpp` dosyalarını Arduino projenizin klasörüne kopyalayın.
3. Sketçinizin başına ekleyin:
   ```cpp
   #include "motor.h"
   ```

**Arduino IDE ile:**

Arduino IDE → Sketch → Include Library → Add .ZIP Library... yolunu izleyip indirdiğiniz ZIP dosyasını seçin.

---

### 🚀 Kullanım

#### Temel Kullanım (Araç Kontrolü)

```cpp
#include "motor.h"

// Motor(IN1, IN2, IN3, IN4, ENB, ENA)
// IN1, IN2 = Sol Motor kontrol pinleri
// IN3, IN4 = Sağ Motor kontrol pinleri
// ENA = Sol motor PWM (hız) pini
// ENB = Sağ motor PWM (hız) pini
Motor car(9, 8, 7, 6, 5, 10);

void setup() {
  // Kurulum gerekmez, constructor pinleri ayarlar
}

void loop() {
  // Araç ileriye git
  car.forward(200);
  delay(2000);

  // Araç geriye git
  car.backward(150);
  delay(2000);

  // Araç sola dön (bağımsız hızlar)
  car.left(100, 200);  // Sol motor yavaş, sağ motor hızlı
  delay(1000);

  // Araç sağa dön
  car.right(200, 100); // Sağ motor yavaş, sol motor hızlı
  delay(1000);

  // Araç dur (serbest)
  car.stop();
  delay(1000);

  // Acil durdurma
  car.brake();
  delay(1000);
}
```

---

### 📖 API Referansı

#### `Motor(int in1, int in2, int in3, int in4, int enb, int ena)`
Kurucu fonksiyon. Motor (araç) nesnesini oluşturur ve tüm pinleri `OUTPUT` olarak ayarlar.

| Parametre | Açıklama |
|---|---|
| `in1` | L298N IN1 - Sol Motor ileri pini |
| `in2` | L298N IN2 - Sol Motor geri pini |
| `in3` | L298N IN3 - Sağ Motor ileri pini |
| `in4` | L298N IN4 - Sağ Motor geri pini |
| `ena` | L298N ENA - Sol Motor PWM (hız) pini |
| `enb` | L298N ENB - Sağ Motor PWM (hız) pini |

---

#### `void forward(int speed)`
Araç **ileriye** hareket eder.

| Parametre | Açıklama |
|---|---|
| `speed` | Hız değeri (0 = dur, 255 = tam hız) |

```cpp
car.forward(255);  // Tam hızda ileri
car.forward(128);  // Yarım hızda ileri (~%50)
```

---

#### `void backward(int speed)`
Araç **geriye** hareket eder.

| Parametre | Açıklama |
|---|---|
| `speed` | Hız değeri (0 = dur, 255 = tam hız) |

```cpp
car.backward(200);  // Geri git
```

---

#### `void left(int speedena, int speedenb)`
Araç **sola** döner. Motor hızlarını bağımsız olarak kontrol edebilirsiniz.

| Parametre | Açıklama |
|---|---|
| `speedena` | Sol motor hızı (0-255) |
| `speedenb` | Sağ motor hızı (0-255) |

```cpp
car.left(100, 200);  // Sola dön - sağ motor daha hızlı
```

---

#### `void right(int speedena, int speedenb)`
Araç **sağa** döner. Motor hızlarını bağımsız olarak kontrol edebilirsiniz.

| Parametre | Açıklama |
|---|---|
| `speedena` | Sol motor hızı (0-255) |
| `speedenb` | Sağ motor hızı (0-255) |

```cpp
car.right(200, 100);  // Sağa dön - sol motor daha hızlı
```

---

#### `void stop()`
Araç **serbest durur**. Motor kendi ataletinden dolayı yavaşça durur.

```cpp
car.stop();  // Yumuşak duruş
```

---

#### `void brake()`
Araç **acilen durdurulur**. Motor kilitlenir ve ani durur.

```cpp
car.brake();  // Ani ve sert duruş
```

---

### 💡 Örnek Kodlar

Tam çalışan örnekler `examples/` klasöründe bulunmaktadır:

- **[BasicCarControl.ino](examples/BasicCarControl.ino)** — Başlangıçlar için temel hareket komutları
- **[AdvancedNavigation.ino](examples/AdvancedNavigation.ino)** — İleri seviye hareket desenleri ve navigasyon

Tüm örnekler hakkında: [examples/README.md](examples/README.md)

---

#### İki Motor ile Robot Araba

```cpp
#include "motor.h"

Motor leftMotor(9, 8, 10);    // Sol motor
Motor rightMotor(6, 7, 5);    // Sağ motor

void setup() {}

void loop() {
  // İleri git
  leftMotor.forward(200);
  rightMotor.forward(200);
  delay(2000);

  // Sağa dön (sol motor hızlı, sağ motor yavaş)
  leftMotor.forward(200);
  rightMotor.forward(80);
  delay(800);

  // Dur
  leftMotor.brake();
  rightMotor.brake();
  delay(1000);
}
```

#### Hız Rampalama (Yumuşak Başlangıç)

```cpp
#include "motor.h"

Motor motor(9, 8, 10);

void setup() {}

void loop() {
  // Yavaşça hızlan
  for (int speed = 0; speed <= 255; speed += 5) {
    motor.forward(speed);
    delay(30);
  }

  delay(1000);

  // Yavaşça dur
  for (int speed = 255; speed >= 0; speed -= 5) {
    motor.forward(speed);
    delay(30);
  }

  motor.stop();
  delay(1000);
}
```

---

## 🇬🇧 English Documentation

### 📋 Table of Contents
- [Overview](#-overview)
- [What is L298N?](#-what-is-l298n)
- [Features](#-features)
- [Hardware Connection](#-hardware-connection)
- [Installation](#-installation)
- [Usage](#-usage-1)
- [API Reference](#-api-reference)
- [Examples](#-examples)

---

### 🌟 Overview

This library makes it incredibly simple to control DC motors using the L298N dual H-bridge motor driver module with Arduino. It abstracts all the low-level pin toggling, so you can focus on your project logic using clean, readable commands like `forward()`, `backward()`, `stop()`, and `brake()`.

---

### 🔩 What is L298N?

The L298N is a dual full-bridge motor driver IC capable of driving **two independent DC motors** or **one stepper motor** simultaneously.

| Specification | Value |
|---|---|
| Max Operating Voltage | 46V |
| Max Current (per channel) | 2A |
| Logic Voltage | 5V (Arduino compatible) |
| PWM Speed Control | ✅ Yes |
| Active Braking | ✅ Yes |

---

### ✨ Features

- 🚀 **Simple API** — Full motor control with just 4 intuitive methods
- 🎛️ **PWM Speed Control** — Precise speed adjustment from 0–255 via `analogWrite`
- ⛔ **Two Stop Modes** — Coast (`stop`) and active lock (`brake`)
- 🔌 **Multi-Motor Support** — Instantiate multiple `Motor` objects for independent control
- 📦 **Lightweight** — No unnecessary dependencies, only requires `<Arduino.h>`

---

### 🔌 Hardware Connection

```
Arduino          L298N Module
-------          ------------
Pin 9    ──────► IN1
Pin 8    ──────► IN2
Pin 10   ──────► EN (Enable / PWM)
GND      ──────► GND
```

> **⚠️ Important:** The Enable (EN) pin must be connected to a PWM-capable pin. On Arduino Uno, PWM pins are: 3, 5, 6, 9, 10, 11.

**Typical L298N Module Pinout:**

```
[ Motor A ]──► OUT1, OUT2  ──► IN1, IN2, ENA  ◄── Arduino
[ Motor B ]──► OUT3, OUT4  ──► IN3, IN4, ENB  ◄── Arduino
               GND, 5V, VCC (power supply)
```

---

### 📥 Installation

**Manual installation:**

1. Download this repository as ZIP:
   ```
   https://github.com/Alibehram11/L298N-Arduino-Library/archive/refs/heads/main.zip
   ```
2. Copy `motor.h` and `motor.cpp` into your Arduino project folder.
3. Include the library at the top of your sketch:
   ```cpp
   #include "motor.h"
   ```

**Via Arduino IDE:**

Arduino IDE → Sketch → Include Library → Add .ZIP Library... → Select the downloaded ZIP file.

---

### 🚀 Usage

#### Basic Usage

```cpp
#include "motor.h"

// Motor(IN1 pin, IN2 pin, EN pin)
Motor motor(9, 8, 10);

void setup() {
  // No setup needed — the constructor configures the pins
}

void loop() {
  motor.forward(200);   // Move forward at speed 200/255
  delay(2000);

  motor.backward(150);  // Move backward at speed 150/255
  delay(2000);

  motor.stop();         // Freewheel (coast to stop)
  delay(1000);

  motor.brake();        // Active brake (instant stop)
  delay(500);
}
```

---

### 📖 API Reference

#### `Motor(int in1, int in2, int en)`
Constructor. Creates the motor object and configures all three pins as `OUTPUT`.

| Parameter | Description |
|---|---|
| `in1` | Arduino pin connected to L298N IN1 |
| `in2` | Arduino pin connected to L298N IN2 |
| `en` | PWM-capable Arduino pin connected to L298N Enable |

---

#### `void forward(int speed)`
Spins the motor in the **forward** direction.

| Parameter | Description |
|---|---|
| `speed` | Speed value (0 = stop, 255 = full speed) |

```cpp
motor.forward(255);  // Full speed forward
motor.forward(128);  // Half speed forward (~50%)
```

> IN1=HIGH, IN2=LOW, EN=speed

---

#### `void backward(int speed)`
Spins the motor in the **backward** direction.

| Parameter | Description |
|---|---|
| `speed` | Speed value (0 = stop, 255 = full speed) |

```cpp
motor.backward(200);  // Move backward
```

> IN1=LOW, IN2=HIGH, EN=speed

---

#### `void stop()`
**Coasts** the motor to a stop. The motor freewheels until friction naturally decelerates it.

```cpp
motor.stop();  // Soft stop
```

> IN1=LOW, IN2=LOW, EN=0

---

#### `void brake()`
**Actively brakes** the motor, locking the shaft almost instantly by driving both inputs HIGH.

```cpp
motor.brake();  // Hard, instant stop
```

> IN1=HIGH, IN2=HIGH, EN=255

---

### 💡 Examples

#### Two-Motor Robot Car

```cpp
#include "motor.h"

Motor leftMotor(9, 8, 10);    // Left motor
Motor rightMotor(6, 7, 5);    // Right motor

void setup() {}

void loop() {
  // Drive forward
  leftMotor.forward(200);
  rightMotor.forward(200);
  delay(2000);

  // Turn right (left faster, right slower)
  leftMotor.forward(200);
  rightMotor.forward(80);
  delay(800);

  // Full stop
  leftMotor.brake();
  rightMotor.brake();
  delay(1000);
}
```

#### Speed Ramping (Smooth Start)

```cpp
#include "motor.h"

Motor motor(9, 8, 10);

void setup() {}

void loop() {
  // Gradually accelerate
  for (int speed = 0; speed <= 255; speed += 5) {
    motor.forward(speed);
    delay(30);
  }

  delay(1000);

  // Gradually decelerate
  for (int speed = 255; speed >= 0; speed -= 5) {
    motor.forward(speed);
    delay(30);
  }

  motor.stop();
  delay(1000);
}
```

---

## 📁 Proje Yapısı / Project Structure

```
L298N-Arduino-Library/
├── motor.h        # Sınıf tanımı / Class declaration
├── motor.cpp      # Fonksiyon implementasyonları / Function implementations
├── LICENSE        # MIT Lisansı / MIT License
└── README.md      # Bu dosya / This file
```

---

## 🪪 Lisans / License

Bu proje [MIT Lisansı](LICENSE) ile lisanslanmıştır. Özgürce kullanabilir, değiştirebilir ve dağıtabilirsiniz.

This project is licensed under the [MIT License](LICENSE). You are free to use, modify, and distribute it.

---

<div align="center">

**Geliştirici / Developer:** [Alibehram11](https://github.com/Alibehram11)

⭐ Beğendiyseniz yıldız vermeyi unutmayın! / If you found it useful, don't forget to star!

</div>

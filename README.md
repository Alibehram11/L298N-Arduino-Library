# L298N-Arduino-Library
<div align="center">

# ⚙️ L298N Arduino Motor Library

### Türkçe | [English](#-english-documentation)

[![Arduino](https://img.shields.io/badge/Arduino-Compatible-00979D?style=for-the-badge&logo=arduino&logoColor=white)](https://www.arduino.cc/)
[![Language](https://img.shields.io/badge/Language-C%2B%2B-blue?style=for-the-badge&logo=cplusplus)](https://isocpp.org/)
[![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)](LICENSE)
[![Version](https://img.shields.io/badge/Version-1.0.0-orange?style=for-the-badge)](https://github.com/Alibehram11/L298N-Arduino-Library/releases)

> **L298N motor sürücü modülünü Arduino ile kolayca kullanmak için tasarlanmış, sade ve güçlü bir C++ kütüphanesi.**

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

---

### 🌟 Genel Bakış

Bu kütüphane, Arduino projelerinizde L298N çift H-köprü motor sürücü modülünü kullanmayı son derece basitleştirir. Karmaşık dijital sinyal yönetimini sizin adınıza halleder; siz sadece `forward()`, `backward()`, `stop()` ve `brake()` gibi anlaşılır komutlarla motorunuzu kontrol edersiniz.

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

- 🚀 **Basit API** — 4 temel fonksiyon ile tam motor kontrolü
- 🎛️ **PWM Hız Kontrolü** — `analogWrite` ile 0–255 arası hassas hız ayarı
- ⛔ **İki Farklı Durdurma Modu** — Serbest duruş (`stop`) ve aktif fren (`brake`)
- 🔌 **Çoklu Motor Desteği** — Her motor için ayrı nesne oluşturarak birden fazla motoru bağımsız yönetin
- 📦 **Hafif ve Sade** — Gereksiz bağımlılık yok, sadece `<Arduino.h>`

---

### 🔌 Donanım Bağlantısı

```
Arduino          L298N Modülü
-------          ------------
Pin 9    ──────► IN1
Pin 8    ──────► IN2
Pin 10   ──────► EN (Enable / PWM)
GND      ──────► GND
```

> **⚠️ Dikkat:** Enable (EN) pini mutlaka PWM destekleyen bir pine bağlanmalıdır (Arduino Uno'da: 3, 5, 6, 9, 10, 11).

**Tipik L298N Modül Pinleri:**

```
[ Motor A ]──► OUT1, OUT2  ──► IN1, IN2, ENA  ◄── Arduino
[ Motor B ]──► OUT3, OUT4  ──► IN3, IN4, ENB  ◄── Arduino
               GND, 5V, VCC (güç kaynağı)
```

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

#### Temel Kullanım

```cpp
#include "motor.h"

// Motor(IN1 pini, IN2 pini, EN pini)
Motor motor(9, 8, 10);

void setup() {
  // Kurulum gerekmez, constructor pinleri ayarlar
}

void loop() {
  motor.forward(200);   // İleri git, hız: 200/255
  delay(2000);

  motor.backward(150);  // Geri git, hız: 150/255
  delay(2000);

  motor.stop();         // Serbest dur (kılavuz bırak)
  delay(1000);

  motor.brake();        // Aktif fren (ani dur)
  delay(500);
}
```

---

### 📖 API Referansı

#### `Motor(int in1, int in2, int en)`
Kurucu fonksiyon. Motor nesnesini oluşturur ve pinleri `OUTPUT` olarak ayarlar.

| Parametre | Açıklama |
|---|---|
| `in1` | L298N IN1 pinine bağlanan Arduino pini |
| `in2` | L298N IN2 pinine bağlanan Arduino pini |
| `en` | L298N Enable pinine bağlanan PWM pini |

---

#### `void forward(int speed)`
Motoru **ileri** yönde döndürür.

| Parametre | Açıklama |
|---|---|
| `speed` | Hız değeri (0 = dur, 255 = tam hız) |

```cpp
motor.forward(255);  // Tam hızda ileri
motor.forward(128);  // Yarım hızda ileri (~%50)
```

---

#### `void backward(int speed)`
Motoru **geri** yönde döndürür.

| Parametre | Açıklama |
|---|---|
| `speed` | Hız değeri (0 = dur, 255 = tam hız) |

```cpp
motor.backward(200);  // Geri git
```

---

#### `void stop()`
Motoru **serbest bırakarak** durdurur. Motor kendi ataletinden dolayı yavaşça durur.

```cpp
motor.stop();  // Yumuşak duruş
```

> IN1=LOW, IN2=LOW, EN=0

---

#### `void brake()`
Motoru **aktif olarak frenleyerek** anında durdurur. Her iki giriş HIGH yapılır ve motor kilitlenir.

```cpp
motor.brake();  // Ani ve sert duruş
```

> IN1=HIGH, IN2=HIGH, EN=255

---

### 💡 Örnek Kodlar

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

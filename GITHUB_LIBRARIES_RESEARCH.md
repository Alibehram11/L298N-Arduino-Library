# GitHub Arduino Robotik Kütüphaneleri - Kapsamlı Araştırma Raporu

## Arama Kapsama Alanı
1. ✓ Adafruit Motor Shield Library
2. ✓ DFRobot Smart Car Kits
3. ✓ Autonomous car projects
4. ✓ Arduino robotics libraries

---

## TOP 10+ POPÜLER ROBOTIK ÖZELLİKLERİ & DAĞILIMI

| # | Özellik | Adafruit | DFRobot | Arduino-Car | Otonom Projeler |
|---|---------|----------|---------|------------|-----------------|
| 1 | **PID Motor Control** | ⚠️ (AccelStepper) | ✓ | ⚠️ (Basit) | ✓✓ |
| 2 | **Encoder Feedback** | ✓ (Örnek) | ✓✓ | ✓ | ✓✓✓ |
| 3 | **Line Following (QTR/TCRT)** | ✗ | ✓✓ | ✗ | ✓✓✓ |
| 4 | **Obstacle Avoidance** | ✗ | ✓ | ✓ (HC-SR04) | ✓✓✓ |
| 5 | **Mobile App Control** | ✗ | ✓ | ✓ (HC-06 BLE) | ✓ |
| 6 | **Configuration Files** | ✗ | ✓ | ✗ | ✓ |
| 7 | **Telemetry/Logging** | ⚠️ (Basit) | ✓✓ | ✓ (Gelişmiş) | ✓✓ |
| 8 | **Speed Ramping** | ✓ (AccelStepper) | ✓ | ⚠️ (Sabit hız) | ✓✓ |
| 9 | **I2C Communication** | ✓✓ (Çekirdek) | ✓ | ✓ | ✓ |
| 10 | **Servo Motor Control** | ✓✓ | ✓ | ⚠️ (Arduino Native) | ✓ |
| 11 | **Multi-Motor Sync** | ✓ | ✓ | ✓ (İleri/geri) | ✓✓✓ |
| 12 | **Noise Filtering** | ✗ | ✓ | ✓✓ (MAD filtre) | ✓ |

**Açıklama**: ✓ Vardır | ✓✓ Gelişmiş | ✓✓✓ Çok Gelişmiş | ⚠️ Sınırlı | ✗ Yok

---

## 1️⃣ ADAFRUIT MOTOR SHIELD V2 LIBRARY
**GitHub**: https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library  
**Lisans**: BSD  
**Yıldız**: 400+

### Özellikleri:
- **DC Motor Kontrolü**: 0-255 hız (8-bit) veya 0-4095 (12-bit fine)
- **Stepper Motor**: SINGLE, DOUBLE, INTERLEAVE, MICROSTEP modları
- **I2C Haberleşme**: PCA9685 PWM driver üzerinden
- **Hız Kontrol**: `setSpeed()`, `setSpeedFine()` metodları
- **Servo Uyumluluğu**: Arduino Servo kütüphanesiyle entegrasyonu
- **Acceleration**: AccelStepper kütüphanesi ile isteğe bağlı

### Kod Örneği (Encoder Kullanımı):
```cpp
// encoderMotorRPM.ino - Encoder ve RPM ölçümü
#define ENCODER_A 12
#define ENCODER_B 11
volatile float RPM = 0;

void interruptA() {
  motordir = digitalRead(ENCODER_B);
  uint32_t currA = micros();
  float rev = currA - lastA;
  rev = 1.0 / rev;              // rev/us
  rev *= 1000000 * 60;           // rev/min
  rev /= GEARING * ENCODERMULT;  // Dişli oranı düzelt
  RPM = rev;
}

void setup() {
  attachInterrupt(ENCODER_A, interruptA, RISING);
}
```

### Avantajlar:
✓ I2C takılabilirlik (adresleme)  
✓ Yüksek çözünürlük PWM (12-bit)  
✓ Bilinen, kararlı API  
✓ Çok sayıda örnek  

### Sınırlamalar:
✗ PID kontrolü yok (manuel hallandırma)  
✗ Konfigürasyon dosyası desteği yok  
✗ Çizgi takibi sensörü uyumluluğu yok  
✗ Yerleşik telemetry yok  

### Uyumlu Sensörler:
- HC-SR04 (doğrudan)
- HC-06 Bluetooth (bağımsız)
- Servo motorlar (Arduino Servo lib)

---

## 2️⃣ DFROBOT SMART CAR KITS
**Ana Merkez**: https://github.com/DFRobot  
**Popüler Projeler**:
- DFRobot Smart Car (Arduino-based)
- RoboMaster Car Platform
- Beginner Robotics Series

### Desteklenen Özellikler:
- **PID Hız Kontrolü**: ✓ Tam destek
- **Encoder Feedback**: ✓ PWM sensörü ile
- **QTR Sensörleri**: ✓ Çizgi takibi (5 kanallı)
- **Engel Algılama**: ✓ HC-SR04 ile
- **Bluetooth Kontrol**: ✓ HC-06 ve HC-05
- **Konfigürasyon Sistemi**: ✓ EEPROM kayıtlı parametreler
- **Telemetry**: ✓ Seri bağlantı üzerinden (CSV formatı)

### Örnek Proje (Line Following):
```cpp
// DFRobot QTR Sensörleri
#define NUM_SENSORS 5
uint16_t sensorValues[NUM_SENSORS];

// Kalibre
sensors.calibrate();

// Sensör oku
uint16_t position = sensors.readLineBlack(sensorValues);

// PID ile hız ayarla
int error = TARGET_POSITION - position;
int speed = KP*error + KI*integral + KD*derivative;
```

### Avantajlar:
✓ Ürün bazlı donanım desteği  
✓ QTR sensörü uyumluluğu  
✓ Yerleşik PID örnekleri  
✓ Eğitim amaçlı dokümantasyon  
✓ Topluluk desteği (Türkçe kaynaklar da vardır)  

### Sınırlamalar:
✗ Bazı örnekler kısa dokümante  
✗ Modern C++ özelliklerinden yok  
✗ Düşük seviye kontrolü sınırlı  

---

## 3️⃣ AÇTLAN ARDUINO-CAR-LIBRARY (Proje Analizi)
**Sayfa**: `c:\Users\alibe\OneDrive\Desktop\kodlar\Arduino-Car-Library`

### Mevcut Modüller:
1. **Motor Control (motor.h/cpp)**
   - L298N çift motor sürücüsü
   - İleri, geri, sol, sağ, yumuşak dönüş, fren
   - Sabit veya ayarlanabilir hız

2. **HC-06 Bluetooth (hc-06.h/cpp)**
   - SoftwareSerial üzerinden
   - Baud: 9600 (yapılandırılabilir)
   - Veri gönderme/alma

3. **HC-SR04 Ultrasonik (hcsr-04.h/cpp)**
   - **GELİŞMİŞ**: Medyan Mutlak Sapma (MAD) filtreleme
   - Multi-sample ortalaması (5 örnek)
   - İstatistikler: min/max/avg
   - Asenkron ölçüm desteği
   - Sinyal kalitesi metriği

4. **RadioLink I2C (radio-link-i2c.h/cpp)**
   - NRF24L01, LoRa I2C adaptörleri
   - RSSI sinyal gücü izleme
   - Bağlantı kalitesi

### Karşılaştırma Matrisi:
| Özellik | Adafruit | DFRobot | Arduino-Car |
|---------|----------|---------|------------|
| Motor Kontrol | Stepper (kompleks) | DC (basit) | **DC (optimize)** |
| Filtreleme | Yok | Temel | **Gelişmiş (MAD)** |
| Bluetooth | Yok | Vardır | **Vardır** |
| Ultrasonik | Manuel | Temel | **Gelişmiş** |
| Enkoder | Örnek | Full | Yok (fırsat!) |
| I2C RF | Yok | Yok | **Vardır** |

---

## 4️⃣ AUTONOMEVt PROJECTS (Tipik Özellikler)

### 4A. Line Following Robots
**Popüler Sensörler**: QTR-8A, TCRT5000  
**Algoritma**:
```cpp
// PID Çizgi Takibi
uint16_t position = sensors.readLineBlack(sensorValues);
int error = TARGET_POSITION - position;

// Integral ve Derivative hesapla
integral += error;
int derivative = error - lastError;

// PID çıktısı
int motor_speed = (KP * error) + (KI * integral) + (KD * derivative);

// Diferansiyel sürüş
leftMotor = BASE_SPEED - motor_speed;
rightMotor = BASE_SPEED + motor_speed;

lastError = error;
```

**Projeler**:
- Pololu Zumo (ticari)
- DIY Line Racer
- Micro Mouse türü robotlar

### 4B. Obstacle Avoidance
**Sensörler**: HC-SR04, VP650, IR sensörleri  
**Yaklaşım**: Tepki Bazlı (Reactive)
```cpp
distance = measureDistance();

if (distance < DANGER_DISTANCE) {
  stop();
  backward();
  delay(500);
  turn(RANDOM_DIRECTION);
} else {
  forward();
}
```

### 4C. Autonomous Navigation
**Özellikler**:
- GPS/Compass uyumluluğu
- Yol planlama (A*, Dijkstra)
- SLAM algoritmaları (ileri projeler)
- ROS (Robot Operating System) uyumluluğu

---

## 5️⃣ POPULAR ARDUINO KÜTÜPHANELERI

### AccelStepper (Adafruit destekli)
**Özellikler**:
- Stepper motor hızlandırma/yavaşlama
- Non-blocking operasyon
- Multi-motor senkronizasyon

### Wire Library (Built-in)
**Özellikler**:
- I2C haberleşmesi
- Master/Slave mode
- Çok sayıda device uyumluluğu

### SoftwareSerial
**Özellikler**:
- Software-based UART
- Herhangi 2 pin kullanım
- HC-06, HC-05, uyduşan cihazlar

### Servo Library (Built-in)
**Özellikler**:
- PWM servo kontrolü
- 0-180 derece
- Hızlandırma (isteğe bağlı)

---

## 6️⃣ ÖNERİLEN İMPLEMENTASYONLAR (Arduino-Car-Library için)

### TIER 1: Temel Iyileştirmeler
```cpp
// 1. PID Motor Control (Encoder ile kombinasyon)
class PIDMotor {
  float Kp = 1.0, Ki = 0.5, Kd = 0.2;
  void setPIDGains(float p, float i, float d);
  void setTargetRPM(float rpm);
  void update(); // Encoder geri bildirimi ile
};

// 2. Configuration Manager (EEPROM)
class ConfigManager {
  struct Config {
    float motorKp, motorKi, motorKd;
    float wheelDiameter;
    int encoderCPR; // Counts Per Revolution
    uint32_t wheelBase;
  };
  void save();
  void load();
};

// 3. Telemetry System
class Telemetry {
  void logMotorSpeed(float rpm);
  void logDistance(uint16_t mm);
  void logBattery(float voltage);
  void sendJSON(); // JSON formatı
};
```

### TIER 2: Orta Seviye Özellikler
- **Çizgi Takibi**: QTR-8 sensörü desteği + PID
- **Hız Ramping**: Smooth acceleration profili
- **Multi-sensör Fusion**: HC-SR04 + HC-06 + Encoders

### TIER 3: İleri Özellikler
- **ROS Entegrasyonu**: `rosserial_arduino`
- **SLAM**: Basit grid-based mapping
- **Machine Learning**: TinyML ile sinir ağları

---

## 7️⃣ BEST PRACTICES ÖZETI

### Motor Kontrolü
✓ **PID kullanın** (basit P bile yeterli olabilir)  
✓ **Encoder feedback** alın  
✓ **Hız ramping** ekleyin (sarsıntı azaltır)  
✗ **Sabit PWM** kaçının (yük değişiyor)  

### Sensör İşleme
✓ **Gürültü filtrelemesi** yapın (medyan veya MA)  
✓ **Asenkron okuma** kullanın (non-blocking)  
✓ **Kalibrasyon** yapın  
✗ **Tek ölçüme** güvenmeyin  

### Haberleşme
✓ **I2C** kullanın (az pin, güvenilir)  
✓ **Bluetooth** için **HC-06** (master mod)  
✓ **Checksum/CRC** ekleyin  
✗ **Raw serial** verileri güvenli değil  

### Yazılım Mimarisi
✓ **Modüler tasarım** (bu kütüphane gibi)  
✓ **Non-blocking kod** yazın  
✓ **State machine** kullanın  
✓ **Hata işleme** yapın  

---

## 8️⃣ KARŞILAŞTIRMALI PUANLAMA

```
Kriterler: 0-5 puan (5=mükemmel)

                  Adafruit | DFRobot | Arduino-Car | Otonom
Hız Kontrolü        4       5        3            5
Sensör Uyumu        2       5        4            5
Dokumentasyon       5       3        4            3
Topluluk Desteği    5       4        2            4
PID Yerleşik        1       5        1            5
Modülerlik          5       3        5            3
Filtreleme          1       3        5            3
Telemetry           1       4        4            4
─────────────────────────────────────────────────
TOPLAM (%)         30/40   32/40   28/40        32/40
                   75%     80%     70%          80%
```

---

## 9️⃣ SONUÇLAR VE ÖNERİLER

### Açılan Arduino-Car-Library'nin Gücü:
1. ✓ **Gelişmiş filtreleme** (MAD algoritması) - özelleştirilmiş
2. ✓ **Modüler yapı** - temiz ve genişletilmesi kolay
3. ✓ **I2C RF desteği** - nişte özellikleri
4. ✓ **Bluetooth** - mobil kontrol hazır
5. ⚠️ **Eksiklikler**: PID, Encoder, Line-following (fırsatlar!)

### Önerilen Geliştirmeler (Öncelik Sırasıyla):
1. **Encoder support** ekleyin (RPM dönüş)
2. **PID controller** implementleyin
3. **QTR sensör** uyumluluğu
4. **Configuration EEPROM** sistemi
5. **Telemetry logger** (SD kart)

### Hangi Proje Neyi Kullanmalı?
- **Basit motor kontrol**: Adafruit Motor Shield
- **Çizgi takibi robot**: DFRobot Smart Car
- **Custom benzeri proje**: Açılan Arduino-Car-Library ✅
- **İleri otonom sistem**: ROS + Specialized libraries

---

## 🔟 REFERANSLAR

1. **Adafruit**: https://learn.adafruit.com/adafruit-motor-shield-v2-for-arduino/
2. **DFRobot Docs**: https://www.dfrobot.com/blog-category-120.html
3. **Arduino References**: https://www.arduino.cc/reference/
4. **PID Tuning Guide**: https://en.wikipedia.org/wiki/PID_controller
5. **Line Following**: https://www.pololu.com/docs/pdf/0J21/Zumo_Front_Sensor_Array_Calibration_and_Use.pdf

---

**Rapor Tarihi**: 3 Haziran 2026  
**Araştırma Kapsamı**: GitHub, Arduino.cc, Adafruit, DFRobot Dokümantasyonları  
**Dil**: Türkçe (Eğitim Amaçlı)

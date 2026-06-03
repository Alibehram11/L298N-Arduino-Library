# Arduino Robotik Kütüphaneleri - Özellik Karşılaştırma Tablosu

## TIER 1: En Önemli 10+ Özellik

### 1. PID Motor Control (Hız Dengelemesi)
| Kütüphane | Destek | Seviye | Notlar |
|-----------|--------|--------|--------|
| **Adafruit** | ❌ | - | AccelStepper ile hızlandırma var, PID yok |
| **DFRobot** | ✅ | **5/5** | Tam PID loop, hazır kalibrasyon |
| **Arduino-Car** | ⚠️ | 1/5 | Sadece sabit PWM, PID'ye açık |
| **Autonomous** | ✅ | **5/5** | Kompleks kontrol algoritmaları |

**EN İYİ**: DFRobot, Autonomous Projects

---

### 2. Encoder Feedback (Motor Durumu)
| Kütüphane | Destek | Seviye | Notlar |
|-----------|--------|--------|--------|
| **Adafruit** | ⚠️ | 2/5 | `encoderMotorRPM.ino` örneği var |
| **DFRobot** | ✅ | **5/5** | PWM sensörü, RPM ölçümü, hız senkro |
| **Arduino-Car** | ❌ | - | Yok (GELİŞTİRİLEBİLİR!) |
| **Autonomous** | ✅ | **5/5** | IMU + Odometri fusionları |

**EN İYİ**: DFRobot, Autonomous Projects

---

### 3. Line Following Sensors (QTR/TCRT)
| Kütüphane | Destek | Seviye | Notlar |
|-----------|--------|--------|--------|
| **Adafruit** | ❌ | - | Hiç destek yok |
| **DFRobot** | ✅ | **5/5** | 5-8 kanal sensör, otomatik kalibre |
| **Arduino-Car** | ❌ | - | HC-SR04 var, QTR yok |
| **Autonomous** | ✅ | **5/5** | Gelişmiş vision algorithms |

**EN İYİ**: DFRobot, Autonomous Projects

---

### 4. Obstacle Avoidance Algorithms
| Kütüphane | Destek | Seviye | Notlar |
|-----------|--------|--------|--------|
| **Adafruit** | ❌ | - | Motor kontrolü sadece |
| **DFRobot** | ✅ | 3/5 | HC-SR04 + basit reactive logic |
| **Arduino-Car** | ✅ | **4/5** | HC-SR04 + MAD filtreleme (kaliteli) |
| **Autonomous** | ✅ | **5/5** | SLAM, A* path planning |

**EN İYİ**: Autonomous Projects, Arduino-Car (başarısız)

---

### 5. Mobile App Control Protocols
| Kütüphane | Destek | Seviye | Notlar |
|-----------|--------|--------|--------|
| **Adafruit** | ❌ | - | Hiç Bluetooth desteği |
| **DFRobot** | ✅ | **4/5** | HC-06, HC-05, Android App |
| **Arduino-Car** | ✅ | **4/5** | HC-06 SoftwareSerial |
| **Autonomous** | ✅ | **3/5** | ROS middleware, Web interface |

**EN İYİ**: DFRobot, Arduino-Car

---

### 6. Configuration File Systems
| Kütüphane | Destek | Seviye | Notlar |
|-----------|--------|--------|--------|
| **Adafruit** | ❌ | - | Hard-coded constants |
| **DFRobot** | ✅ | **4/5** | EEPROM parametreleri |
| **Arduino-Car** | ❌ | - | Hard-coded (fırsat!) |
| **Autonomous** | ✅ | **5/5** | YAML, JSON dosyaları |

**EN İYİ**: Autonomous Projects, DFRobot

---

### 7. Telemetry & Logging
| Kütüphane | Destek | Seviye | Notlar |
|-----------|--------|--------|--------|
| **Adafruit** | ❌ | - | Hiç logging |
| **DFRobot** | ✅ | **4/5** | CSV over serial, SD kart opsiyonel |
| **Arduino-Car** | ✅ | **4/5** | HC-06 üzerinden dump mümkün |
| **Autonomous** | ✅ | **5/5** | ROS bag files, RViz visualization |

**EN İYİ**: Autonomous Projects, DFRobot

---

### 8. Speed Ramping (Yumuşak Hızlandırma)
| Kütüphane | Destek | Seviye | Notlar |
|-----------|--------|--------|--------|
| **Adafruit** | ✅ | **5/5** | AccelStepper + DCMotor.step() |
| **DFRobot** | ✅ | **4/5** | Doğrusal interpolasyon |
| **Arduino-Car** | ⚠️ | 1/5 | Sabit hız (ramp'a açık!) |
| **Autonomous** | ✅ | **5/5** | Spline-based S-curves |

**EN İYİ**: Adafruit (Stepper), Autonomous (DC)

---

### 9. I2C Communication & Expansion
| Kütüphane | Destek | Seviye | Notlar |
|-----------|--------|--------|--------|
| **Adafruit** | ✅ | **5/5** | PCA9685 core, cascading support |
| **DFRobot** | ✅ | **4/5** | I2C sensörler desteklenmiş |
| **Arduino-Car** | ✅ | **5/5** | RadioLink I2C (diğerlerde YOK!) |
| **Autonomous** | ✅ | **4/5** | Distributed I2C nodes |

**EN İYİ**: Adafruit, Arduino-Car (unique)

---

### 10. Servo Motor Control
| Kütüphane | Destek | Seviye | Notlar |
|-----------|--------|--------|--------|
| **Adafruit** | ✅ | **5/5** | Direkt PWM pin, MotorParty örneği |
| **DFRobot** | ✅ | **4/5** | Servo uyumu, hız kontrol |
| **Arduino-Car** | ⚠️ | 2/5 | Arduino Servo lib ile, entegrasyon yok |
| **Autonomous** | ✅ | **4/5** | Robot manipulator destekleri |

**EN İYİ**: Adafruit, DFRobot

---

### 11. Multi-Motor Synchronization
| Kütüphane | Destek | Seviye | Notlar |
|-----------|--------|--------|--------|
| **Adafruit** | ✅ | **5/5** | 4 DC + 2 Stepper, AccelStepper multi |
| **DFRobot** | ✅ | **5/5** | Encoder-based synchrization |
| **Arduino-Car** | ✅ | **4/5** | left(ena,enb), right(ena,enb) |
| **Autonomous** | ✅ | **5/5** | Coordinated path tracking |

**EN İYİ**: Adafruit, DFRobot, Autonomous

---

### 12. Noise Filtering & Signal Processing
| Kütüphane | Destek | Seviye | Notlar |
|-----------|--------|--------|--------|
| **Adafruit** | ❌ | - | Raw ADC values |
| **DFRobot** | ✅ | **3/5** | Basit moving average |
| **Arduino-Car** | ✅ | **5/5** | **MAD (Median Abs Dev) - BEST** |
| **Autonomous** | ✅ | **4/5** | Kalman filters, Bayesian |

**EN İYİ**: Arduino-Car (MAD algoritması!), Autonomous

---

## Ek Özellikler

### Battery Management
| Kütüphane | Destek | Notlar |
|-----------|--------|--------|
| **Adafruit** | ❌ | - |
| **DFRobot** | ✅ | Voltaj ölçümü, düşük pil uyarısı |
| **Arduino-Car** | ⚠️ | Potansiyel: Analog pin okuma |
| **Autonomous** | ✅ | Enerji optimizasyonu |

### Wireless Debug Console
| Kütüphane | Destek | Notlar |
|-----------|--------|--------|
| **Adafruit** | ❌ | - |
| **DFRobot** | ✅ | Serial üzerinden terminalr |
| **Arduino-Car** | ✅ | HC-06 ile mümkün |
| **Autonomous** | ✅ | ROS logging |

### Over-The-Air Updates
| Kütüphane | Destek | Notlar |
|-----------|--------|--------|
| **Adafruit** | ❌ | - |
| **DFRobot** | ❌ | - |
| **Arduino-Car** | ❌ | - |
| **Autonomous** | ⚠️ | Bazı platformlarda |

---

## ÖZET PUANLAMA

```
Kriterler        | Adafruit | DFRobot | Arduino-Car | Autonomous
─────────────────┼──────────┼─────────┼─────────────┼───────────
PID Control      | 1/5      | 5/5     | 1/5         | 5/5
Encoder Support  | 2/5      | 5/5     | 0/5 ⚠️      | 5/5
Line Following   | 0/5      | 5/5     | 0/5         | 5/5
Obstacle Avoid   | 0/5      | 3/5     | 4/5         | 5/5
Mobile Control   | 0/5      | 4/5     | 4/5         | 3/5
Config System    | 0/5      | 4/5     | 0/5 ⚠️      | 5/5
Telemetry        | 0/5      | 4/5     | 4/5         | 5/5
Speed Ramping    | 5/5      | 4/5     | 1/5 ⚠️      | 5/5
I2C Support      | 5/5      | 4/5     | 5/5 🔥      | 4/5
Servo Control    | 5/5      | 4/5     | 2/5         | 4/5
Multi-Motor      | 5/5      | 5/5     | 4/5         | 5/5
Noise Filter     | 0/5      | 3/5     | 5/5 🔥      | 4/5
─────────────────┼──────────┼─────────┼─────────────┼───────────
TOPLAM           | 28/60    | 51/60   | 30/60 ⚠️    | 55/60
YÜZDE            | 47%      | 85%     | 50% + FIX   | 92%
```

### Efsane
- 🔥 **Benzersiz/En İyi**: Arduino-Car'da MAD filtresi, RadioLink I2C
- ⚠️ **Geliştirme Fırsatı**: Arduino-Car'da PID, Encoder, Config, Speed Ramp
- ✅ **Güçlü**: DFRobot (endüstri standardı), Autonomous (araştırma)

---

## TAVSIYELIR PROJELERİ

### Basit Projeler (Başlangıç)
→ **Arduino-Car-Library** + Adafruit Motor Shield  
✓ Temiz kod  
✓ Çizgi takibi için QTR sensör ekleyin  

### Orta Düzey Projeler
→ **DFRobot Smart Car Kit**  
✓ Hazır PID  
✓ Ticari destek  
✓ Öğrenme kaynakları  

### İleri Projeler
→ **ROS + Custom C++ Stack**  
✓ Otonom navigasyon  
✓ Multi-robot coordination  
✓ Araştırma-grade kalite  

---

**Hazırlayan**: GitHub Libraries Research Team  
**Versiyon**: 1.0 (3 Haziran 2026)  
**Format**: Türkçe Eğitim Rehberi

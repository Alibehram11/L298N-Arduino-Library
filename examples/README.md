# Arduino Car Library - Examples

Tüm Arduino Car kütüphanesi örneklerini bu klasörde bulabilirsiniz.

---

## 📂 Örnek Listesi

### 1. **BasicCarControl.ino** (Başlangıçlar için)
Temel araç hareketi komutlarını öğrenmek için ideal.

**Kapsar:**
- ✅ İleri hareket (tam hız ve yarı hız)
- ✅ Geri hareket
- ✅ Sola dön / Sağa dön
- ✅ Serbest durdurma (`stop`)
- ✅ Acil durdurma (`brake`)

**Donanım Şeması:**

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

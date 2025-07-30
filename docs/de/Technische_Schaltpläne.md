# Technische Dokumentation: Hardware-Schaltpläne

## GNSS-Sensor Schaltplan

```
                            +------------+
                            |            |
+------------+              | ESP32      |
|            |  TX (GPIO16) |            |
|  GNSS      |------------->|            |         +------------+
|  Modul     |              |            |         |            |
|            |  RX (GPIO17) |            | MOSI    |            |
|            |<-------------|            |-------->|            |
+------------+              |            | MISO    |            |
                            |            |<--------|  SD-Karten |
                            |            | SCK     |  Modul     |
                            |            |-------->|            |
                            |            | CS      |            |
                            |            |-------->|            |
                            |            |         |            |
+------------+              |            |         +------------+
|            |  RED_LED     |            |
|  Status    |<-------------|            |
|  LEDs      |  GREEN_LED   |            |
|            |<-------------|            |
+------------+              +------------+
```

### Verbindungstabelle GNSS-Sensor

| ESP32 Pin | Verbunden mit | Funktion |
|-----------|---------------|----------|
| GPIO 16   | GNSS TX       | UART Datenempfang |
| GPIO 17   | GNSS RX       | UART Datenübertragung |
| GPIO 23   | SD MOSI       | SPI Master Out Slave In |
| GPIO 19   | SD MISO       | SPI Master In Slave Out |
| GPIO 18   | SD SCK        | SPI Clock |
| GPIO 5    | SD CS         | SD Chip Select |
| RED_LED   | Rote LED      | Fehleranzeige |
| GREEN_LED | Grüne LED     | Betriebsanzeige |
| 5V        | Stromversorgung | Betriebsspannung |
| GND       | Masse         | Gemeinsamer Massepunkt |

### Spezifikation der Komponenten

1. **ESP32-WROOM-32**
   - Betriebsspannung: 3,3V (5V über USB)
   - CPU: Dual-Core Xtensa LX6, bis zu 240 MHz
   - Flash-Speicher: 4 MB
   - RAM: 520 KB

2. **GNSS-Modul**
   - Betriebsspannung: 3,3V
   - Stromaufnahme: ca. 30-50 mA
   - Empfänger: GPS, GLONASS, BeiDou, Galileo
   - Update-Rate: 1 Hz (Standard)

3. **SD-Kartenmodul**
   - Betriebsspannung: 3,3V
   - Unterstützt SD- und SDHC-Karten
   - Kommunikation: SPI

4. **Status-LEDs**
   - Betriebsspannung: 3,3V
   - Vorwiderstand: 220 Ohm pro LED
   - Farben: Rot, Grün, Gelb

---

## RPM-Sensor Schaltplan

```
                 +------------+
                 |            |         +------------+
                 | ESP32      |  SDA    |            |
+------------+   |            |-------->|            |
|            |   |            |  SCL    |  OLED      |
|  Hall-     |   |            |-------->|  Display   |
|  Sensor    |   |            |         |            |
|            |---|GPIO 15     |         |            |
+------------+   |            |         +------------+
                 |            |
                 |            |         +------------+
+------------+   |            |  SDA    |            |
|            |   |            |-------->|            |
|  Tasten    |---|GPIO 25/26/27         |  DS3231    |
|  (3x)      |   |            |  SCL    |  RTC       |
|            |   |            |-------->|            |
+------------+   |            |         |            |
                 |            |         +------------+
                 |            |
                 |            |         +------------+
+------------+   |            |  CS     |            |
|            |   |            |-------->|            |
|  Status    |   |            |  MOSI   |  SD-Karten |
|  LED       |---|GPIO 12     |-------->|  Modul     |
|            |   |            |  MISO   |            |
+------------+   |            |<--------|            |
                 |            |  SCK    |            |
                 |            |-------->|            |
                 |            |         +------------+
                 |            |
                 |            |         +------------+
                 |            |  SDA2   |            |
                 |            |-------->|            |
                 |            |  SCL2   |  EEPROM    |
                 |            |-------->|  24AA512   |
                 |            |         |            |
                 +------------+         +------------+
```

### Verbindungstabelle RPM-Sensor

| ESP32 Pin | Verbunden mit | Funktion |
|-----------|---------------|----------|
| GPIO 15   | Hall-Sensor   | Impulssignal |
| GPIO 21   | SDA (I2C-1)   | I2C-Datenleitung für OLED und RTC |
| GPIO 22   | SCL (I2C-1)   | I2C-Taktleitung für OLED und RTC |
| GPIO 16   | SDA (I2C-2)   | I2C-Datenleitung für EEPROM |
| GPIO 17   | SCL (I2C-2)   | I2C-Taktleitung für EEPROM |
| GPIO 25   | PLUS-Taste    | Werterhöhung (blau) |
| GPIO 26   | MINUS-Taste   | Wertverringerung (weiß) |
| GPIO 27   | SET-Taste     | Bestätigung/Einstellung (gelb) |
| GPIO 12   | Status-LED    | Betriebsanzeige |
| GPIO 5    | SD CS         | SD Chip Select |
| MOSI      | SD MOSI       | SPI Master Out Slave In |
| MISO      | SD MISO       | SPI Master In Slave Out |
| SCK       | SD SCK        | SPI Clock |
| 5V        | Stromversorgung | Betriebsspannung |
| GND       | Masse         | Gemeinsamer Massepunkt |

### Spezifikation der Komponenten

1. **ESP32-WROOM-32**
   - Betriebsspannung: 3,3V (5V über USB)
   - CPU: Dual-Core Xtensa LX6, bis zu 240 MHz
   - Flash-Speicher: 4 MB
   - RAM: 520 KB

2. **Hall-Sensor**
   - Betriebsspannung: 3,3V-5V
   - Ausgang: Digital (High/Low)
   - Empfindlichkeit: Optimiert für 1-3 mm Abstand zum Magneten

3. **OLED-Display**
   - Typ: 128x64 Pixel
   - Schnittstelle: I2C
   - Betriebsspannung: 3,3V

4. **DS3231 RTC**
   - Hochpräzise Echtzeituhr
   - I2C-Schnittstelle
   - Backup-Batterie: CR2032

5. **SD-Kartenmodul**
   - Betriebsspannung: 3,3V
   - Unterstützt SD- und SDHC-Karten bis 16 GB
   - Kommunikation: SPI

6. **24AA512-MIC EEPROM**
   - Kapazität: 64 KB
   - I2C-Schnittstelle
   - Datenhaltung: >100 Jahre
   - Schreib-/Lesezyklenzahl: >1 Million

7. **Tasten und LED**
   - 3 Tasten mit Pull-up-Widerständen (10 kOhm)
   - LED mit Vorwiderstand (220 Ohm)

### Montage der Magnete

Für die korrekte Funktion des RPM-Sensors ist die richtige Montage der Magnete entscheidend:

Generelle Montage eines Hall Sensors:
Die Anzahl der Magnete muss mit dem verwendeten Progamm des ESP32 abgestimmt sein. Im Fall dieses Projektes werden 4 Magnete pro Umdrehung gemessen.

- Verwenden Sie 4 Magnete, gleichmäßig verteilt um den Umfang des rotierenden Teils
- Abstand zwischen Magneten und Hall-Sensor: max 18-20 mm da in der in diesem Projekt verwendeten Magnetmanschette jeweils 4 starke Neodym-Magnete pro Position zum Einsatz kommen. (sonst Abstand 1-3 mm)
- Die Magnete sollten alle mit der gleichen Polarität zum Sensor ausgerichtet sein
- Befestigen Sie die Magnete sicher, um ein Lösen während des Betriebs zu vermeiden

### Schematische Darstellung der Magnete

```
      [Magnet 1]
     /           \
[Magnet 2]     [Magnet 4]
     \           /
      [Magnet 3]

       Rotierendes
          Teil
```

---

## Anhang: Bildmaterial

### GNSS-Sensor

![GNSS-Sensor Grundplatine](./bilder/Anhang/GNSS-Sensor%20Grundplatine.jpg)
*GNSS-Sensor Grundplatine mit Komponenten*

### RPM-Sensor

![RPM-Box Platine1 ohne OLED](./bilder/Anhang/RPM-Box%20Platine1%20ohne%20OLED.jpg)
*RPM-Box Hauptplatine (Ansicht 1)*

![RPM-Box Platine2 ohne SD und RTC](./bilder/Anhang/RPM-Box%20Platine2%20ohne%20SD%20und%20RTC.jpg)
*RPM-Box Hauptplatine (Ansicht 2)*

![RPM-Box Platine3 mit EEPROM](./bilder/Anhang/RPM-Box%20Platine3%20mit%20EEPROM.jpg)
*RPM-Box Hauptplatine mit EEPROM*

![Hall-Sensor Montage](./bilder/Anhang/Hall-Sensor%20Montage.jpg)
*Montage des Hall-Sensors*

---

## GitHub-Repositories

Der vollständige Quellcode, Schaltpläne und 3D-Druckvorlagen für beide Projekte sind in den folgenden GitHub-Repositories verfügbar:

### GNSS-Sensor
**Repository:** [github.com/hansratzinger/GnssSensor](https://github.com/hansratzinger/GnssSensor)

### RPM-Sensor
**Repository:** [github.com/hansratzinger/RpmSensor](https://github.com/hansratzinger/RpmSensor)

Im Ordner `/3d` beider Repositories finden Sie .3mf Dateien für den 3D-Druck aller benötigten Gehäuseteile, Halterungen und anderer Komponenten.

---

*Diese technische Dokumentation ist Teil der Dokumentation des RNLI-Projekts.*
*Version 1.0, Juli 2025*

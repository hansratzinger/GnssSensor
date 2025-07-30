# Installationsanleitung für RNLI-Sensoren

Diese Anleitung führt Sie durch den Prozess der Installation und Einrichtung der GNSS- und RPM-Sensoren für das RNLI-Projekt.

## Inhaltsverzeichnis

1. Voraussetzungen
2. GNSS-Sensor Installation
3. RPM-Sensor Installation
4. Softwareinstallation und -kompilierung
5. Testen und Kalibrieren
6. Fehlerbehebung

## 1. Voraussetzungen

Bevor Sie mit der Installation beginnen, stellen Sie sicher, dass Sie über folgende Komponenten verfügen:

### Hardware
- ESP32 Entwicklungsboards (2×)
- GNSS/GPS-Modul mit UART-Schnittstelle
- SD-Kartenmodule (2×)
- OLED-Display(s)
- Hall-Sensor
- DS3231 RTC-Modul
- 24AA512-MIC EEPROM
- Status-LEDs
- Bedientasten (3×)
- Verbindungskabel und Steckbretter für Prototyping
- Gehäuse für die fertigen Sensoren
- Stromversorgung (5V)

### Software und Werkzeuge
- Visual Studio Code mit PlatformIO-Erweiterung
- USB-Kabel zur Programmierung der ESP32-Boards
- Lötkolben und Zubehör
- Multimeter zur Fehlersuche
- Schraubendreher-Set für die Montage

### SD-Karten
- Bevorzugt 8 GB SD-Karten, formatiert in FAT32
- Vermeiden Sie SD-Karten größer als 16 GB, da diese zu Problemen führen können

## 2. GNSS-Sensor Installation

### 2.1 Hardware-Zusammenbau

1. **Verbinden Sie das GNSS-Modul mit dem ESP32:**
   - GNSS TX → ESP32 GPIO 16
   - GNSS RX → ESP32 GPIO 17
   - GNSS VCC → ESP32 3.3V
   - GNSS GND → ESP32 GND

2. **Verbinden Sie das SD-Kartenmodul mit dem ESP32:**
   - SD MOSI → ESP32 GPIO 23
   - SD MISO → ESP32 GPIO 19
   - SD SCK → ESP32 GPIO 18
   - SD CS → ESP32 GPIO 5
   - SD VCC → ESP32 3.3V
   - SD GND → ESP32 GND

3. **Verbinden Sie die Status-LEDs mit dem ESP32:**
   - Rote LED → Entsprechender GPIO-Pin (mit 220 Ohm Vorwiderstand)
   - Grüne LED → Entsprechender GPIO-Pin (mit 220 Ohm Vorwiderstand)
   - Gelbe LED → Entsprechender GPIO-Pin (mit 220 Ohm Vorwiderstand)

4. **Schließen Sie die Stromversorgung an:**
   - 5V-Stromversorgung → ESP32 5V
   - GND → ESP32 GND

### 2.2 Gehäusemontage

1. Bohren Sie Löcher für die LEDs und die Kabeleinführung in das Gehäuse.
2. Montieren Sie das ESP32-Board und alle anderen Komponenten im Gehäuse.
3. Stellen Sie sicher, dass das GNSS-Modul eine freie Sicht zum Himmel hat oder verwenden Sie eine externe Antenne.
4. Dichten Sie das Gehäuse ab, um es vor Wasser und Staub zu schützen (IP65).
5. Befestigen Sie das Gehäuse an einer geeigneten Stelle mit freier Sicht zum Himmel.

## 3. RPM-Sensor Installation

### 3.1 Hardware-Zusammenbau

1. **Verbinden Sie den Hall-Sensor mit dem ESP32:**
   - Hall-Sensor Signal → ESP32 GPIO 15
   - Hall-Sensor VCC → ESP32 3.3V
   - Hall-Sensor GND → ESP32 GND

2. **Verbinden Sie das OLED-Display und das RTC-Modul (I2C-1):**
   - Display/RTC SDA → ESP32 GPIO 21
   - Display/RTC SCL → ESP32 GPIO 22
   - Display/RTC VCC → ESP32 3.3V
   - Display/RTC GND → ESP32 GND

3. **Verbinden Sie das EEPROM (I2C-2):**
   - EEPROM SDA → ESP32 GPIO 16
   - EEPROM SCL → ESP32 GPIO 17
   - EEPROM VCC → ESP32 3.3V
   - EEPROM GND → ESP32 GND

4. **Verbinden Sie das SD-Kartenmodul:**
   - SD MOSI → ESP32 GPIO 23
   - SD MISO → ESP32 GPIO 19
   - SD SCK → ESP32 GPIO 18
   - SD CS → ESP32 GPIO 5
   - SD VCC → ESP32 3.3V
   - SD GND → ESP32 GND

5. **Verbinden Sie die Tasten:**
   - PLUS-Taste → ESP32 GPIO 25 (mit 10k Pull-up-Widerstand)
   - MINUS-Taste → ESP32 GPIO 26 (mit 10k Pull-up-Widerstand)
   - SET-Taste → ESP32 GPIO 27 (mit 10k Pull-up-Widerstand)
   - Tasten GND → ESP32 GND

6. **Verbinden Sie die Status-LED:**
   - LED → ESP32 GPIO 12 (mit 220 Ohm Vorwiderstand)
   - LED GND → ESP32 GND

7. **Schließen Sie die Stromversorgung an:**
   - 5V-Stromversorgung → ESP32 5V
   - GND → ESP32 GND

### 3.2 Magnete montieren

1. Identifizieren Sie ein geeignetes rotierendes Teil, an dem die Drehzahl gemessen werden soll.
2. Befestigen Sie 4 Magnete gleichmäßig verteilt um den Umfang (90° Abstand).
3. Verwenden Sie starken Klebstoff oder mechanische Befestigungen, die den Betriebsbedingungen standhalten.
4. Achten Sie darauf, dass alle Magnete mit der gleichen Polarität nach außen zeigen.

### 3.3 Hall-Sensor positionieren

1. Montieren Sie den Hall-Sensor in einem Abstand von 1-3 mm zu den vorbeifahrenden Magneten.
2. Befestigen Sie den Sensor sicher, so dass er nicht durch Vibrationen verschoben werden kann.
3. Schützen Sie das Sensorkabel vor mechanischer Belastung.

### 3.4 Gehäusemontage

1. Bohren Sie Löcher für das Display, die Tasten, die LED und die Kabeleinführung in das Gehäuse.
2. Montieren Sie alle Komponenten im Gehäuse.
3. Dichten Sie das Gehäuse ab, um es vor Wasser und Staub zu schützen (IP65).
4. Befestigen Sie das Gehäuse an einer gut zugänglichen Stelle.

## 4. Softwareinstallation und -kompilierung

### 4.1 Entwicklungsumgebung einrichten

1. Installieren Sie Visual Studio Code von der offiziellen Website.
2. Installieren Sie die PlatformIO-Erweiterung in VS Code.
3. Starten Sie VS Code neu, damit die Erweiterung aktiviert wird.

### 4.2 GNSS-Sensor-Projekt kompilieren

1. Öffnen Sie den Projektordner `GnssSensor` in VS Code.
2. Überprüfen Sie die `platformio.ini`-Datei und passen Sie die Upload-Port-Einstellung an Ihren COM-Port an.
3. Installieren Sie alle erforderlichen Bibliotheken:
   - TinyGPSPlus

4. Kompilieren Sie das Projekt mit PlatformIO (Build-Taste).
5. Schließen Sie das ESP32-Board per USB an und laden Sie das kompilierte Programm hoch (Upload-Taste).

### 4.3 RPM-Sensor-Projekt kompilieren

1. Öffnen Sie den Projektordner `RpmSensor` in VS Code.
2. Überprüfen Sie die `platformio.ini`-Datei und passen Sie die Upload-Port-Einstellung an Ihren COM-Port an.
3. Installieren Sie alle erforderlichen Bibliotheken:
   - RTClib (Version 2.1.4 oder höher)
   - Adafruit BusIO (Version 1.17.0 oder höher)
   - U8g2 (Version 2.34.17 oder höher)
   - TFT_eSPI (in lib/ enthalten)

4. Kompilieren Sie das Projekt mit PlatformIO (Build-Taste).
5. Schließen Sie das ESP32-Board per USB an und laden Sie das kompilierte Programm hoch (Upload-Taste).

## 5. Testen und Kalibrieren

### 5.1 GNSS-Sensor testen

1. Öffnen Sie den seriellen Monitor in PlatformIO mit einer Baudrate von 115200.
2. Beobachten Sie die Debug-Ausgaben, während der Sensor startet und nach Satelliten sucht.
3. Überprüfen Sie, ob die LEDs korrekt funktionieren:
   - Rotes Licht (hell) während des Setups
   - Grünes Licht beim Suchen nach Satelliten
   - Blinkendes grünes Licht, wenn Positionen auf die SD-Karte geschrieben werden
4. Entnehmen Sie die SD-Karte und prüfen Sie, ob die CSV-Dateien korrekt erstellt wurden.

### 5.2 RPM-Sensor testen und kalibrieren

1. Stellen Sie nach dem ersten Start Datum und Uhrzeit ein:
   - SET-Taste gedrückt halten, um in den Einstellungsmodus zu gelangen
   - Mit PLUS- und MINUS-Tasten die Werte anpassen
   - Mit SET-Taste zwischen den Werten wechseln
2. Drehen Sie das Teil mit den Magneten langsam und überprüfen Sie, ob der Sensor die Impulse korrekt erkennt (LED sollte bei jedem Impuls blinken).
3. Erhöhen Sie die Drehgeschwindigkeit und überprüfen Sie, ob die RPM-Anzeige plausibel ist.
4. Überprüfen Sie die Datenspeicherung:
   - Entnehmen Sie die SD-Karte und prüfen Sie, ob die CSV-Dateien korrekt erstellt wurden
   - Die Daten sollten das Format `YYYY-MM-DD,HH:MM:SS,RPM,Temperatur` haben

### 5.3 Langzeittest

1. Lassen Sie beide Sensoren für mindestens 24 Stunden in einer realistischen Umgebung laufen.
2. Überprüfen Sie regelmäßig die gespeicherten Daten auf Konsistenz und Vollständigkeit.
3. Achten Sie auf unerwartete Verhaltensweisen oder Fehler.

## 6. Fehlerbehebung

### 6.1 GNSS-Sensor Probleme

| Problem | Mögliche Ursache | Lösung |
|---------|------------------|--------|
| Keine GNSS-Daten | Keine Sicht zum Himmel | Repositionieren oder externe Antenne verwenden |
| Rote LED bleibt an | SD-Karte nicht erkannt | SD-Karte überprüfen oder ersetzen |
| Keine LEDs leuchten | Stromversorgungsproblem | Verkabelung und Spannungsversorgung prüfen |
| Keine CSV-Dateien | Schreibrechte-Problem | SD-Karte formatieren (FAT32) |

### 6.2 RPM-Sensor Probleme

| Problem | Mögliche Ursache | Lösung |
|---------|------------------|--------|
| Keine RPM-Anzeige | Hall-Sensor falsch positioniert | Abstand und Position anpassen |
| Falsche RPM-Werte | Magnetzahl falsch konfiguriert | Code auf 4 Magnete überprüfen |
| Display zeigt nichts an | I2C-Verbindungsproblem | Verkabelung prüfen, I2C-Scanner verwenden |
| RTC zeigt falsche Zeit | Batterie leer | RTC-Batterie ersetzen |
| SD-Karte Schreibfehler | Inkompatible SD-Karte | 8 GB SD-Karte verwenden |

### 6.3 Allgemeine Hinweise

- Verwenden Sie zur Fehlersuche immer den seriellen Monitor mit 115200 Baud.
- Bei I2C-Problemen können Sie einen I2C-Scanner-Sketch verwenden, um die korrekten Adressen zu überprüfen.
- Bei Stabilitätsproblemen kann ein zusätzlicher 100 µF Kondensator zwischen VCC und GND helfen.
- Stellen Sie sicher, dass alle Verbindungen stabil und gegen Vibration gesichert sind.

---

## Anhang: Installationsbeispiele

### Montage des GNSS-Sensors

![GNSS-Sensor Grundplatine](./bilder/Anhang/GNSS-Sensor%20Grundplatine.jpg)
*GNSS-Sensor Grundplatine vor der Installation im Gehäuse*

### Montage des RPM-Sensors

![RPM-Box und HALL-Sensor-Box](./bilder/Anhang/RPM-Box%20und%20HALL-Sensor-Box.jpg)
*RPM-Box mit angeschlossener HALL-Sensor-Box*

![Montage RPM Sensor und Box](./bilder/Anhang/Montage%20RPM%20Sensor%20und%20Box.jpg)
*Beispiel für die Montage des RPM-Sensors*

![Hall-Sensor Montage](./bilder/Anhang/Hall-Sensor%20Montage.jpg)
*Korrekte Montage des Hall-Sensors*

![RPM-Box und Haltevorrichtung](./bilder/Anhang/RPM-Box%20und%20Haltevorrichtung.jpg)
*RPM-Box mit Haltevorrichtung zur sicheren Befestigung*

---

## Quellcode und 3D-Druckdateien

Für die Installation benötigen Sie möglicherweise die 3D-Druckvorlagen für Gehäuseteile, Halterungen und andere Komponenten. Diese sind in den GitHub-Repositories der Projekte verfügbar:

- **GNSS-Sensor:** [github.com/hansratzinger/GnssSensor](https://github.com/hansratzinger/GnssSensor)
- **RPM-Sensor:** [github.com/hansratzinger/RpmSensor](https://github.com/hansratzinger/RpmSensor)

Die .3mf Dateien für den 3D-Druck finden Sie im Ordner `/3d` der jeweiligen Repositories. Diese können mit gängigen 3D-Druck-Programmen geöffnet, bei Bedarf angepasst und für den Druck vorbereitet werden.

---

*Diese Installationsanleitung ist Teil der Dokumentation des RNLI-Projekts.*
*Version 1.0, Juli 2025*

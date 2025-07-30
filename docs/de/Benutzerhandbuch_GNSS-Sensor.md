# Benutzerhandbuch GNSS-Sensor

## Einführung

Dieses Handbuch beschreibt die Bedienung und Wartung des GNSS-Sensors, der für das RNLI-Projekt entwickelt wurde. Der GNSS-Sensor ermöglicht die präzise Erfassung von Positionsdaten und deren Speicherung auf einer SD-Karte.

## Inhaltsverzeichnis

1. Überblick
2. Erste Schritte
3. Tägliche Nutzung
4. LED-Statusanzeigen
5. Datenabruf und -verwaltung
6. Fehlersuche und -behebung
7. Wartung und Pflege
8. Technische Spezifikationen

## 1. Überblick

Der GNSS-Sensor ist ein kompaktes Gerät zur kontinuierlichen Erfassung von Positionsdaten mittels GNSS (Global Navigation Satellite System). Er wurde speziell für den Einsatz auf Seenotrettungsfahrzeugen entwickelt und ist robust gegen maritime Bedingungen.

Das System speichert automatisch folgende Daten:
- Geographische Koordinaten (Längen- und Breitengrad)
- Datum und Uhrzeit
- Anzahl der verfügbaren Satelliten

## 2. Erste Schritte

### 2.1 Lieferumfang

- GNSS-Sensor-Einheit im wetterfesten Gehäuse
- Stromkabel (5V)
- Montagematerial
- SD-Karte (vorformatiert)
- Diese Dokumentation

### 2.2 Installation

1. **Montage**: Befestigen Sie das Gerät an einer Position mit freier Sicht zum Himmel. Idealerweise an einem erhöhten Punkt ohne Abdeckung.

2. **Stromversorgung anschließen**: Verbinden Sie das Stromkabel mit einer 5V-Stromquelle. Das Gerät schaltet sich automatisch ein.

3. **Erstinbetriebnahme**: Nach dem Einschalten durchläuft das Gerät automatisch folgende Schritte:
   - Selbsttest (rote LED leuchtet)
   - Initialisierung des GNSS-Moduls
   - Suche nach Satelliten (grüne LED leuchtet)
   - Start der Datenaufzeichnung (grüne LED blinkt)

## 3. Tägliche Nutzung

Der GNSS-Sensor arbeitet vollautomatisch und benötigt im täglichen Betrieb keine Benutzerinteraktion. Nach dem Einschalten beginnt das Gerät automatisch mit der Erfassung und Speicherung von GNSS-Daten.

### 3.1 Ein- und Ausschalten

- **Einschalten**: Verbinden Sie das Gerät mit der Stromversorgung.
- **Ausschalten**: Trennen Sie das Gerät von der Stromversorgung.

Es wird empfohlen, das Gerät während der gesamten Einsatzzeit eingeschaltet zu lassen, um eine kontinuierliche Datenerfassung zu gewährleisten.

### 3.2 Normale Betriebszustände

Im normalen Betrieb durchläuft das Gerät folgende Zustände:

1. **Initialisierung**: Beim Einschalten führt das Gerät einen Selbsttest durch.
2. **Satellitensuche**: Das Gerät sucht nach Satellitensignalen (kann je nach Umgebungsbedingungen einige Minuten dauern).
3. **Datenerfassung**: Sobald ausreichend Satelliten gefunden wurden, beginnt die Datenerfassung.
4. **Datenspeicherung**: Die erfassten Daten werden regelmäßig auf die SD-Karte geschrieben.

## 4. LED-Statusanzeigen

Die LEDs an der Gehäusefront zeigen den aktuellen Betriebszustand an:

| LED-Status | Bedeutung |
|------------|-----------|
| Rotes Licht (hell) | Setup nach dem Einschalten |
| Grün ein | Suche nach Position |
| Grün blinkend | Position auf SD-Karte geschrieben |
| Grün blinkend und Rot ein | Position berechnet, aber Schreibfehler auf SD-Karte |
| Rot ein (gedimmt) | Keine SD-Karte gefunden |

## 5. Datenabruf und -verwaltung

### 5.1 Zugriff auf die gespeicherten Daten

Um auf die aufgezeichneten Daten zuzugreifen:

1. Schalten Sie das Gerät aus (Stromversorgung trennen).
2. Öffnen Sie vorsichtig das Gehäuse (achten Sie auf die Wasserdichtigkeit).
3. Entnehmen Sie die SD-Karte, dazu sind vorher die Schrauben des Platinenhalters zu lösen.
4. Lesen Sie die SD-Karte mit einem Computer aus.
5. Setzen Sie die SD-Karte wieder ein und verschließen Sie das Gehäuse sorgfältig.

### 5.2 Datenformat

Die Daten werden auf der SD-Karte in CSV-Dateien gespeichert. Der Dateiname folgt dem Format `backup_YYYYMMDD.csv`, wobei YYYYMMDD das aktuelle Datum ist. Datum und Zeit sind in UTC.

Jede Zeile in der CSV-Datei enthält folgenden Eintrag:
```
YYYYMMDD,HH:MM:SS,Breitengrad,Längengrad,Satellitenanzahl
```

Beispiel:
```
20250730,14:35:22,50.123456,7.654321,8
```

### 5.3 Empfohlene Datensicherung

Es wird empfohlen, die Daten regelmäßig (z.B. wöchentlich) von der SD-Karte zu sichern. Dies kann durch Kopieren der CSV-Dateien auf einen Computer oder ein anderes Speichermedium erfolgen.

## 6. Fehlersuche und -behebung

### 6.1 LED-Fehlercodes

| LED-Status | Problem | Lösung |
|------------|---------|--------|
| Rot ein (gedimmt) | Keine SD-Karte gefunden | SD-Karte prüfen, ggf. ersetzen |
| Grün blinkend und Rot ein | Schreibfehler auf SD-Karte | SD-Karte prüfen, formatieren oder ersetzen |
| Keine LED leuchtet | Keine Stromversorgung | Stromanschluss und Kabel prüfen |

### 6.2 Häufige Probleme

1. **Keine Positionsbestimmung möglich**:
   - Stellen Sie sicher, dass das Gerät freie Sicht zum Himmel hat.
   - Warten Sie mindestens 5 Minuten für den ersten Fix.
   - Überprüfen Sie, ob das Gerät in einem Bereich mit guter GNSS-Abdeckung ist.

2. **SD-Karten-Fehler**:
   - Formatieren Sie die SD-Karte im FAT32-Format.
   - Verwenden Sie eine neue SD-Karte.
   - Stellen Sie sicher, dass die SD-Karte korrekt eingesetzt ist.

3. **Gerät reagiert nicht**:
   - Trennen Sie die Stromversorgung und schließen Sie sie nach 30 Sekunden wieder an.
   - Überprüfen Sie alle Verbindungen.

## 7. Wartung und Pflege

### 7.1 Regelmäßige Überprüfung

- Überprüfen Sie regelmäßig die Dichtigkeit des Gehäuses.
- Kontrollieren Sie die Befestigung des Geräts.
- Reinigen Sie das Gerät bei Bedarf mit einem feuchten Tuch (keine aggressiven Reinigungsmittel).

### 7.2 SD-Karte

Die SD-Karte hat eine begrenzte Lebensdauer. Es wird empfohlen:
- Die SD-Karte alle 6-12 Monate zu ersetzen.
- Nur hochwertige SD-Karten zu verwenden.
- Regelmäßig die SD-Karte auf Fehler zu überprüfen.

## 8. Technische Spezifikationen

- **Stromversorgung**: 
  - Eingang: 5V DC
  - Intern: Buck Step Down Converter auf 3,3V für SD-Karte und GNSS-Modul
  - ESP32: Direkte 5V-Versorgung über USB-Eingang
- **Stromaufnahme**: max. 200mA
- **Betriebstemperatur**: -10°C bis +50°C
- **Schutzart**: IP65
- **GNSS-Genauigkeit**: Typ. ±2,5m
- **Datenaufzeichnungsintervall**: Ca. alle 1-5 Sekunden (abhängig von der Konfiguration)
- **SD-Karte**: FAT32-Format, empfohlene Größe 8-16GB
- **Abmessungen**: Gemäß Gehäuse
- **Gewicht**: Gemäß Konfiguration

---

## Anhang: Bilder und Diagramme

![GNSS-Sensor Grundplatine](./bilder/Anhang/GNSS-Sensor%20Grundplatine.jpg)
*GNSS-Sensor Grundplatine mit ESP32 und Buck Step Down Converter*

![GNSS-Sensor Grundplatine](./bilder/Anhang/GNSS-Sensor%20Platine%20oben%20SD%20und%20GNSS%20gelb.jpg)
*GNSS-Sensor Platine oben mit SD-Card und GNSS-Modul (gelbes Isolierband)*

---

## Quellcode und 3D-Druckvorlagen

Der vollständige Quellcode und alle zugehörigen Dateien für dieses Projekt sind im GitHub-Repository verfügbar:

**GitHub Repository:** [github.com/hansratzinger/GnssSensor](https://github.com/hansratzinger/GnssSensor)

Das Repository enthält:
- Den vollständigen Quellcode des Projekts
- Konfigurationsdateien
- 3D-Druckvorlagen (.3mf Dateien) im Ordner `/3d` für alle benötigten Gehäuseteile und Halterungen

---

*Dieses Handbuch ist Teil der Dokumentation des RNLI-Projekts.*
*Version 1.0, Juli 2025*

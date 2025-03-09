#include <Arduino.h>
#include <TinyGPSPlus.h>

// Serielle Schnittstelle für den Monitor
#define SERIALMONITOR_BAUD 115200

// Serielle Schnittstelle für das GPS-Modul
#define GPS_BAUD 115200
#define GPS_RX 16 // RX-Pin des ESP32 (mit TX des GPS-Moduls verbinden)
#define GPS_TX 17 // TX-Pin des ESP32 (mit RX des GPS-Moduls verbinden)

// TinyGPSPlus-Objekt
TinyGPSPlus gps;

// HardwareSerial-Objekt für die GPS-Kommunikation
HardwareSerial gpsSerial(1);

void setup() {
    Serial.begin(SERIALMONITOR_BAUD);
    delay(1000);
    Serial.println("Minimalbeispiel für TinyGPSPlus");

    gpsSerial.begin(GPS_BAUD, SERIAL_8N1, GPS_RX, GPS_TX);
    delay(1000);
    Serial.println("GPS Serial initialisiert");
}

void loop() {
    while (gpsSerial.available() > 0) {
        char c = gpsSerial.read();
        if (gps.encode(c)) {
            Serial.println("Vollständiger NMEA-Satz empfangen!");
            if (gps.location.isValid()) {
                Serial.print("Latitude: ");
                Serial.println(gps.location.lat(), 6);
                Serial.print("Longitude: ");
                Serial.println(gps.location.lng(), 6);
            } else {
                Serial.println("Keine gültige Position gefunden");
            }
        }
    }
}
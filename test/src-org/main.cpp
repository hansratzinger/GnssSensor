// G N S S - S E N S O R
//
// This code is part of the GNSS sensor project.
// -  is used to send GNSS data to the FDRS gateway.
// -  is based on the ESP32 WROOM-32 microcontroller.
// -  uses the LC76 GNSS module to receive GNSS data.
// -  uses the TinyGPS++ library to parse the GNSS data.
// -  uses the FDRS library to send the GNSS data to the FDRS gateway.
// -  uses the pins.h file to define the pin mappings.
// -  uses the fdrs_node_config.h file to define the FDRS node configuration.
// -  uses the fdrs_node.h file to define the FDRS node functions.
// -  uses the DEV_Config.h file to define the SPI configuration.
// -  uses the L76X.h file to define the LC76 GNSS module functions.
// -  uses the GNSS_module.h file to define the GNSS module functions.
// -  uses the SPI.h file to define the SPI functions.
// -  uses the Arduino.h file to define the Arduino functions.

// Release 1.1.1
// HR 2025-03-07 NK Initial version

#include <Arduino.h>
#include <pins.h>
#include "GNSS_module.h"
#include <TinyGPSPlus.h>
#include "DEV_Config.h"
#include "L76X.h"
#include "fdrs_node_config.h"
#include <fdrs_node.h>


#define SERIALMONITOR_BAUD 115200
#define GPS_BAUD 115200

int GPSbaud = 115200; // Baudrate des GPS-Moduls

// Globale Statusvariable
float statusSensor = 99; // FDRS data 99 = Initialisierung läuft, 0 = OK, andere Werte für Fehler

// const String BRANCH = "main"; // Branch name
// const String RELEASE = "1.1.1"; // Branch name

// Deklaration von Variablen
const bool TEST = false; // Definition der Konstante TEST
unsigned long lastPositionTime = 0;
unsigned long currentTime = 0;
int switchTime = 5000; // Wartezeit von mindestens 5 Sekunden, wird durch calculateInterval() später geschwindigkeitsabhängig verändert!
int initSwitchTime = 500; // Wartezeit beim GPNSS Init von mindestens 500 milliSekunden

double lastLat = 0.0;
double lastLon = 0.0;   

// The TinyGPS++ object
TinyGPSPlus gps;

// Create an instance of the HardwareSerial class for Serial 2
HardwareSerial gpsSerial(1); // Initialisierung von gpsSerial

// LED Funktion vereinfacht
void setLed(bool state, uint8_t pin) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, state);
}

// bool initGPS() {
//     Serial.println("Initializing GPS Serial...");
//     int baudRates[] = {115200}; // Liste der zu testenden Baudraten
//     int numBaudRates = sizeof(baudRates) / sizeof(baudRates[0]);
//     int currentBaudRateIndex = 0;
//     int attempts = 0;
//     const int maxAttemptsPerBaudRate = 10;

//     while (true) { // Endlosschleife
//         int currentBaudRate = baudRates[currentBaudRateIndex];
//         Serial.print("Trying baud rate: ");
//         Serial.println(currentBaudRate);

//         gpsSerial.begin(currentBaudRate, SERIAL_8N1, GPS_RX, GPS_TX);
//         setLed(true, RED_LED_PIN);
//         delay(100); // Kurze Verzögerung nach dem Starten der seriellen Schnittstelle

//         if (gpsSerial) {
//             // Puffer leeren
//             while (gpsSerial.available()) {
//                 gpsSerial.read();
//             }

//             // Warte auf erste gültige NMEA-Daten
//             unsigned long startTime = millis();
//             while (millis() - startTime < initSwitchTime) {  // 2 Sekunden Timeout
//                 if (gpsSerial.available()) {
//                     char c = gpsSerial.read();
//                     Serial.print(c); // Gib die empfangenen Daten aus
//                     if (gps.encode(c) && gps.location.isValid()) {
//                         Serial.println("GPS data stream verified");
//                         statusSensor = 0; // GPS Initialisierung erfolgreich
//                         setLed(false, RED_LED_PIN);
//                         return true; // Erfolgreiche Initialisierung
//                     }
//                 }
//                 delay(10);
//             }
//         }

//         Serial.println("Waiting for GPS Serial, retrying...");
//         attempts++;

//         if (attempts >= maxAttemptsPerBaudRate) {
//             attempts = 0; // Zurücksetzen des Zählers
//             currentBaudRateIndex++; // Nächste Baudrate

//             if (currentBaudRateIndex >= numBaudRates) {
//                 currentBaudRateIndex = 0; // Starte von vorne mit den Baudraten
//                 Serial.println("All baud rates tried, restarting from the beginning.");
//             }
//         }
//         delay(1000);
//     }

//     return false; // Wird eigentlich nie erreicht, da die Schleife endlos ist
// }

bool initGPS() {
    Serial.println("Initializing GPS Serial...");
    gpsSerial.begin(GPS_BAUD, SERIAL_8N1, GPS_RX, GPS_TX);

    // Erweiterte Initialisierungsversuche
    int gpsRetries = 25;
    while (gpsRetries > 0) {
        if (gpsSerial) {
            // Puffer leeren
            while (gpsSerial.available()) {
                gpsSerial.read();
            }

            // Warte auf erste gültige NMEA-Daten
            unsigned long startTime = millis();
            while (millis() - startTime < 2000) {  // 2 Sekunden Timeout
                if (gpsSerial.available()) {
                    char c = gpsSerial.read();
                    if (gps.encode(c) && gps.location.isValid()) {
                        Serial.println("GPS data stream verified");
                        return true;
                    }
                }
                delay(10);
            }
        }

        Serial.printf("Waiting for GPS Serial, attempt %d of 5...\n", 6 - gpsRetries);
        delay(1000);
        gpsRetries--;
    }

    return false;
}

void serialTestOutput() {
    Serial.print("Latitude: ");
    Serial.print(gps.location.lat(), 6);
    Serial.print(" Longitude: ");
    Serial.print(gps.location.lng(), 6);
    Serial.print(" Altitude: ");
    Serial.print(gps.altitude.meters());
    Serial.print(" Speed: ");
    Serial.print(gps.speed.kmph());
    Serial.print(" Heading: ");
    Serial.print(gps.course.deg());
    Serial.print(" Date: ");
    Serial.print(gps.date.day());
    Serial.print("/");
    Serial.print(gps.date.month());
    Serial.print("/");
    Serial.print(gps.date.year());
    Serial.print(" Time: ");
    Serial.print(gps.time.hour());
    Serial.print(":");
    Serial.print(gps.time.minute());
    Serial.print(":");
    Serial.print(gps.time.second());
    Serial.println();
}

void sendStatus(int status) {
    loadFDRS(status, STATUS_T); // Status 0 = OK, 99 = waiting for GNSS data
    sendFDRS();
}

int calculateInterval(float speed) {
    float interval = 1 / (speed * 1000 / 3600); // intevall 1 meter in relation to speed   
    int intInterval = (round)(interval * 1000); // convert to milliseconds
    return intInterval;
}

void sendGnss() {   // Sendet die RPM-Werte an den FDRS-Gateway   
    // FDRS data types  

    float fdrsALT = (float)gps.altitude.meters(); // GPS Altitude
    float fdrsHDOP = (float)gps.hdop.hdop(); // GPS HDOP
    float fdrsSATS = (float)gps.satellites.value(); // satellites   
    float fdrsLAT = (float)gps.location.lat();
    float fdrsLON = (float)gps.location.lng();
    float fdrsLATDIR = (fdrsLAT >= 0) ? 1 : 0; // 1 für Nord, 0 für Süd
    float fdrsLONDIR = (fdrsLON >= 0) ? 1 : 0; // 1 für Ost, 0 für West
    float fdrsHEADING = (float)gps.course.deg(); // heading
    float fdrsSPEED = (float)gps.speed.kmph(); // SPEED KMH
    float fdrsPOSITION_DIFF  = (float)calculateDistance(gps.location.lat(), gps.location.lng(), lastLat, lastLon); // positionDifference
    float fdrsYEAR = (float)gps.date.year(); // year
    float fdrsMONTH = (float)gps.date.month(); // month
    float fdrsDAY = (float)gps.date.day(); // day
    float fdrsHOUR = (float)gps.time.hour(); // hour
    float fdrsMINUTE = (float)gps.time.minute(); // minute
    float fdrsSECOND = (float)gps.time.second(); // second 

    loadFDRS(fdrsSPEED, SPEED);
    loadFDRS(fdrsLAT, LATITUDE_T);
    loadFDRS(fdrsLON, LONGITUDE_T);
    loadFDRS(fdrsALT, ALTITUDE_T);
    loadFDRS(fdrsHDOP, HDOP_T);
    loadFDRS(fdrsSATS, SATELLITES);
    loadFDRS(fdrsLATDIR, DIRECTION_LAT);
    loadFDRS(fdrsLONDIR, DIRECTION_LON);
    loadFDRS(fdrsHEADING, HEADING);
    loadFDRS(fdrsPOSITION_DIFF , POSITION_DIFF);
    loadFDRS(fdrsYEAR, YEAR);
    loadFDRS(fdrsMONTH, MONTH);
    loadFDRS(fdrsDAY, DAY);
    loadFDRS(fdrsHOUR, HOUR);
    loadFDRS(fdrsMINUTE, MINUTE);
    loadFDRS(fdrsSECOND, SECOND);

    // Speichern der aktuellen Position
    lastLat = gps.location.lat();
    lastLon = gps.location.lng();

    // calculate interval
    switchTime = calculateInterval(fdrsSPEED);
      
    // DBG(sendFDRS()); // Debugging 
    if (sendFDRS()) {
        DBG("Big Success!");
    } else {
        DBG("Nope, not so much.");
    }
}

void setup() {
    Serial.begin(SERIALMONITOR_BAUD);
    delay(1000);
    Serial.println("=====================================");
    Serial.println("======= G N S S - S E N S O R =======");
    Serial.println("=====================================");

    initGPS(); // Die Funktion blockiert, bis die Initialisierung erfolgreich ist
    
    Serial.println("GPS erfolgreich initialisiert");
    delay(100);
    beginFDRS(); // Initialisiert FDRS 
    Serial.println("Setup finished!");
    delay(1000);
}

void loop() {
    currentTime = millis();
    static unsigned long lastPositionTime = 0;
    static String nmeaSentence = ""; // String zum Speichern des NMEA-Satzes

    while (gpsSerial.available() > 0) {
        char c = gpsSerial.read();
        nmeaSentence += c; // Füge das Zeichen zum NMEA-Satz hinzu

        if (c == '\n' || c == '\r') { // Prüfe auf Zeilenende (Ende des Satzes)
            Serial.print("Empfangener NMEA-Satz: ");
            Serial.println(nmeaSentence); // Gib den vollständigen Satz aus

            bool encoded = true; // Flag, um den Erfolg der Verarbeitung zu verfolgen
            for (int i = 0; i < nmeaSentence.length(); i++) {
                if (!gps.encode(nmeaSentence.charAt(i))) { // Verarbeite jedes Zeichen einzeln
                    encoded = false; // Setze das Flag auf false, wenn ein Fehler auftritt
                    break; // Beende die Schleife, wenn ein Fehler auftritt
                }
            }

            if (encoded) { // Wenn alle Zeichen erfolgreich verarbeitet wurden
                Serial.println("GPS data stream verified");

                if (gps.location.isValid() && gps.hdop.hdop() < 3.0 && gps.date.year() != 2000 && gps.date.month() != 0 && gps.date.day() != 0 && gps.time.hour() != 0 && gps.time.minute() != 0 && gps.time.second() != 0) {
                    statusSensor = 0;
                    sendGnss(); // Sendet die GNSS-Daten an den FDRS-Gateway
                    setLed(false, RED_LED_PIN);
                    setLed(true, GREEN_LED_PIN);
                    delay(150);
                    setLed(false, GREEN_LED_PIN);
                    if (TEST) {
                        serialTestOutput();
                    }
                } else {
                    Serial.println("Ungültige GPS-Daten!");
                }
            } else {
                Serial.println("Fehler beim Verarbeiten des NMEA-Satzes!");
            }

            nmeaSentence = ""; // Setze den String für den nächsten Satz zurück
        }

        if (!gps.location.isValid()) {
            statusSensor = 99; // no valid GPS data
            sendStatus(statusSensor); // Sendet einen Fehler an den FDRS-Gateway
            setLed(true, RED_LED_PIN);
        }
    }
}
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

// Release 1.0.0
// HR 2025-03-07 NK Initial version

#include <Arduino.h>
#include <pins.h>
#include "GNSS_module.h"
#include <TinyGPSPlus.h>
#include "DEV_Config.h"
#include "L76X.h"
#include "fdrs_node_config.h"
#include <fdrs_node.h>

#define GPS_BAUD 115200
#define SERIALMONITOR_BAUD 115200

// Globale Statusvariable
float statusSensor = 99; // FDRS data 99 = Initialisierung läuft, 0 = OK, andere Werte für Fehler

// const String BRANCH = "main"; // Branch name
// const String RELEASE = "1.1.0"; // Branch name

// Deklaration von Variablen
const bool TEST = false; // Definition der Konstante TEST
unsigned long lastPositionTime = 0;
unsigned long currentTime = 0;
float switchTime = 5000; // Wartezeit von mindestens 5 Sekunden, wird durch calculateInterval() später geschwindigkeitsabhängig verändert!

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

bool initGPS() {
    Serial.println("Initializing GPS Serial...");
    gpsSerial.begin(GPS_BAUD, SERIAL_8N1, GPS_RX, GPS_TX);
    setLed(true, RED_LED_PIN);

    while (true) { // Endlosschleife
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
                        statusSensor = 0; // GPS Initialisierung erfolgreich
                        delay(50);
                        setLed(false, RED_LED_PIN);
                        return true; // Erfolgreiche Initialisierung
                    }
                }
                delay(10);
            }
        }

        Serial.println("Waiting for GPS Serial, retrying...");
        delay(1000);
    }

    return false; // Wird eigentlich nie erreicht, da die Schleife endlos ist
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

    // GPS initialisieren
    Serial.println("Initializing GPS...");
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
    
    while (gpsSerial.available() > 0) {
        gps.encode(gpsSerial.read());
    }

    if ((TEST) && (gps.location.isValid())) {
        serialTestOutput();
        statusSensor = 0;
    } else if (TEST) {
        statusSensor = 99;
        Serial.print(statusSensor);
        Serial.println(" Keine gültigen GPS-Daten");
    }

    if ((currentTime - lastPositionTime >= switchTime) && (statusSensor == 0)) { // Wartezeit von mindestens SWITCHTIME
        lastPositionTime = currentTime;
        // Überprüfung ob die Position aktualisiert wurde und der HDOP-Wert unter dem Schwellenwert liegt
        if ((gps.location.isValid()) && (gps.hdop.hdop() < 3.0) && (gps.date.year()) != 2000 && (gps.date.month()) != 0 && (gps.date.day()) != 0 && (gps.time.hour()) != 0 && (gps.time.minute()) != 0 && (gps.time.second()) != 0) {
            sendGnss(); // Sendet die GNSS-Daten an den FDRS-Gateway 
            setLed(true, GREEN_LED_PIN);
            delay(150);
            setLed(false, GREEN_LED_PIN);
        }
    } else if (statusSensor != 0) {
        sendStatus(statusSensor); // Sendet einen Fehler an den FDRS-Gateway
        setLed(true, RED_LED_PIN);
        delay(150);
        setLed(false, RED_LED_PIN);
    }
}
// G N S S - S E N S O R
//
// This code is part of the GNSS sensor project.
// -  is used to send GNSS data to the FDRS gateway.
// -  is based on the ESP32 WROOM-32 microcontroller.
// Release 1.2.0
// -  uses the TinyGPS++ library to parse the GNSS data.
// -  uses the FDRS library to send the GNSS data to the FDRS gateway.
// -  uses the pins.h file to define the pin mappings.
// -  uses the fdrs_node_config.h file to define the FDRS node configuration.
// -  uses the fdrs_node.h file to define the FDRS node functions.
// -  uses the GNSS_module.h file to define the GNSS module functions.
// -  uses the SPI.h file to define the SPI functions.
// -  uses the Arduino.h file to define the Arduino functions.
// Release 1.2.1 HR 2025-03-09 NK 
// -  LED funktions added
// -  during Setup RED LED is on
// -  GNSS init complete, waiting on positions
// -  blinking GREEN LED when new position arrived
// -  sendGnss when FDRS signal is not confirmed by gateway RED LED is on
// -  the DATA TYPES have been altered compared to the original code
// -  the code has been simplified

#include <Arduino.h>
#include <pins.h>
#include "GNSS_module.h"
#include <TinyGPSPlus.h>
#include "fdrs_node_config.h"
#include <fdrs_node.h>

#define SERIALMONITOR_BAUD 115200
#define GPS_BAUD 115200

#define YEAR        25
#define MONTH       26
#define DAY         27      
#define HOUR        28
#define MINUTE      29
#define SECOND      30
#define BOARDTIME   31

// const String BRANCH = "main"; // Branch name
// const String RELEASE = "1.2.0"; // Branch name

// Deklaration von Variablen
const bool TEST = true; // Definition der Konstante TEST
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

void sendGnss() {   // Sendet die RPM-Werte an den FDRS-Gateway   
    // FDRS data types  
    // float fdrsALT = (float)gps.altitude.meters(); // GPS Altitude
    // float fdrsHDOP = (float)gps.hdop.hdop(); // GPS HDOP
    // float fdrsSATS = (float)gps.satellites.value(); // satellites   
    float fdrsLAT = (float)gps.location.lat();
    float fdrsLON = (float)gps.location.lng(); // GPS Longitude
    // float fdrsLATDIR = (fdrsLAT >= 0) ? 1 : 0; // 1 für Nord, 0 für Süd
    // float fdrsLONDIR = (fdrsLON >= 0) ? 1 : 0; // 1 für Ost, 0 für West
    // float fdrsHEADING = (float)gps.course.deg(); // heading
    // float fdrsSPEED = (float)gps.speed.kmph(); // SPEED KMH
    float fdrsPOSITION_DIFF  = (float)calculateDistance(gps.location.lat(), gps.location.lng(), lastLat, lastLon); // positionDifference
    float fdrsYEAR = (float)gps.date.year(); // year
    float fdrsMONTH = (float)gps.date.month(); // month
    float fdrsDAY = (float)gps.date.day(); // day
    float fdrsHOUR = (float)gps.time.hour(); // hour
    float fdrsMINUTE = (float)gps.time.minute(); // minute
    float fdrsSECOND = (float)gps.time.second(); // second 
    float fdrsBOARD_TIME = (float)millis(); // second uptime for testing



    // Load FDRS data
    // loadFDRS(fdrsSPEED, SPEED);
    loadFDRS(fdrsLAT, LATITUDE_T);
    loadFDRS(fdrsLON, LONGITUDE_T);
    // loadFDRS(fdrsALT, ALTITUDE_T);
    // loadFDRS(fdrsHDOP, HDOP_T);
    // loadFDRS(fdrsSATS, SATELLITES);
    // loadFDRS(fdrsLATDIR, DIRECTION_LAT);
    // loadFDRS(fdrsLONDIR, DIRECTION_LON);
    // loadFDRS(fdrsHEADING, HEADING);
    loadFDRS(fdrsPOSITION_DIFF , POSITION_DIFF);
    loadFDRS(fdrsYEAR, YEAR);
    loadFDRS(fdrsMONTH, MONTH);
    loadFDRS(fdrsDAY, DAY);
    loadFDRS(fdrsHOUR, HOUR);
    loadFDRS(fdrsMINUTE, MINUTE);
    loadFDRS(fdrsSECOND, SECOND);
    loadFDRS(fdrsBOARD_TIME, BOARDTIME);

    // Speichern der aktuellen Position
    lastLat = gps.location.lat();
    lastLon = gps.location.lng();

    if (TEST) {
        // Serial.print(String(fdrsYEAR) + String(fdrsMONTH) + String(fdrsDAY) + String(fdrsHOUR) + String(fdrsMINUTE) + String(fdrsSECOND));
        // Serial.println(" " + String(fdrsLAT) + " " + String(fdrsLON) + " " + String(fdrsALT) + " " + String(fdrsHDOP) + " " + String(fdrsSATS) + " " + String(fdrsLATDIR) + " " + String(fdrsLONDIR) + " " + String(fdrsHEADING) + " " + String(fdrsSPEED) + " " + String(fdrsPOSITION_DIFF));
    }
      
    // DBG(sendFDRS()); // Debugging 
    if (sendFDRS()) {
        DBG("Big Success!");
        setLed(false, RED_LED);
    } else {
        DBG("Nope, not so much.");
        setLed(true, RED_LED);
    }
}

void setup() {
    setLed(true, RED_LED);
    Serial.begin(SERIALMONITOR_BAUD);
    delay(1000);

    Serial.println("=====================================");
    Serial.println("======= G N S S - S E N S O R =======");
    Serial.println("=====================================");

        
    gpsSerial.begin(GPS_BAUD, SERIAL_8N1, GPS_RX, GPS_TX);
    delay(1000);

    Serial.println("GPS erfolgreich initialisiert");
    delay(100);

    // FDRS Setup 
    beginFDRS(); // Initialisiert FDRS 
    Serial.println("Setup finished!");
    delay(1000);

    Serial.print("GPS_RX: ");
Serial.println(GPS_RX);
Serial.print("GPS_TX: ");
Serial.println(GPS_TX);

setLed(false, RED_LED);
setLed(true, GREEN_LED);
}


void loop() {
    currentTime = millis();
    static unsigned long lastPositionTime = 0;
    
    while (gpsSerial.available() > 0) {
        char c = gpsSerial.read();
        if (gps.encode(c)) {
            if (gps.location.isValid() && (currentTime - lastPositionTime >= 1000)) { // Prüfe, ob eine Sekunde vergangen ist
                setLed(true, GREEN_LED);
                Serial.print("Time: ");
                Serial.println(millis());                Serial.print("Latitude: ");
                Serial.println(gps.location.lat(), 6);
                Serial.print("Longitude: ");
                Serial.println(gps.location.lng(), 6);
                lastPositionTime = currentTime; // Aktualisiere die letzte Ausgabezeit
                sendGnss(); // Sendet die GNSS-Daten an den FDRS-Gateway (jetzt innerhalb der Zeitschleife)
                delay(200); // Wartezeit von 3 Sekunden
                setLed(false, GREEN_LED);
            }   
        }
    }
    delay(100);
}
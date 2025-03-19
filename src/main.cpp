// -----------------------------------------------------------------------------
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
// Release 1.2.2 HR 2025-03-18 NK
// -  added the function createTimestamp() to create a timestamp using gps.time
// -  added the function createDatestamp() to create a datestamp using gps.date
// -  changed the timing of the GNSS data transmission related to the timestamp
// Release 1.2.3 HR 2025-03-19 NK
// -  added the function backupCsv() to backup the GNSS data to the SD card
// -  added the function sendGnss() to send the GNSS data to the FDRS gateway
// -----------------------------------------------------------------------------
// Include the necessary libraries
// -----------------------------------------------------------------------------
#include <Arduino.h>
#include <pins.h>
#include "GNSS_module.h"
#include <TinyGPSPlus.h>
#include "fdrs_node_config.h"
#include <fdrs_node.h>
#include <SPI.h>
#include <SD.h>
#include "SD_module.h"

// Deklaration von Variablen
const int meterBetweenTwoPoints = 0; // Distanz zwischen zwei Punkten
const bool TEST = true; // Definition der Konstante TEST
int lastPositionTime = 0;

int dateStampFdrs = 0;
int dateStampCsv = 0;
int timeStamp = 0;
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

int createTimestamp() {
    int hours = (float)gps.time.hour();
    int minutes = (float)gps.time.minute();
    int seconds = (float)gps.time.second();
    return hours*10000 + minutes*100 + seconds;
}

int createDatestampFdrs() {
    int year = (float)gps.date.year() - 2000;  //  year - 2000
    int month = (float)gps.date.month();
    int day = (float)gps.date.day();
    int date = day*10000 + month*100 + year ;
    // Serial.println(year);
    // Serial.println(month);
    // Serial.println(day);
    return date;
}

int createDatestampCsv() {
    int year = (float)gps.date.year();  
    int month = (float)gps.date.month();
    int day = (float)gps.date.day();
    int date = year * 10000 + month * 100 + day;
    // Serial.println(date);
    return date;
}

void backupCsv() {
    // Backup-Daten vorbereiten
    String backupData = String(dateStampCsv) + "," + String(timeStamp) + "," + String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6) + "," + String(gps.course.deg()) + "," + String(gps.speed.kmph()) + "," + String(gps.altitude.meters()) + "," + String(gps.hdop.hdop()) + "," + String(gps.satellites.value());
    // Backup der Daten auf die SD-Karte
    if (!backupDataToSD(dateStampCsv, backupData, SD_CS)) {
        Serial.println("Failed to backup data to SD card.");
    }
}

void sendGnss() {   // Sendet die RPM-Werte an den FDRS-Gateway   
    if (!gps.location.isValid() || !gps.date.isValid() || !gps.time.isValid()) {
        Serial.println("Invalid GPS data");
        return;
    }
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
    float fdrsDATE = (float)dateStampFdrs; // date
    float fdrsTIME = (float)timeStamp; // time
    // float fdrsBOARD_TIME = (float)millis(); // second uptime for testing

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
    loadFDRS(fdrsDATE, DATE);
    loadFDRS(fdrsTIME, TIME);
    // loadFDRS(fdrsBOARD_TIME, BOARDTIME);

    // Speichern der aktuellen Position
    lastLat = gps.location.lat();
    lastLon = gps.location.lng();
      
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
    delay(500);

    // Initialisierung der SD-Karte
    pinMode(SD_MISO, INPUT_PULLUP);
    SPI.begin(SD_SCLK, SD_MISO, SD_MOSI);
    if (!SD.begin(SD_CS)) {
        Serial.println("> It looks like you haven't inserted the SD card..");
    } else {
        Serial.println("SD card initialized successfully.");
        uint32_t cardSize = SD.cardSize() / (1024 * 1024);
        String str = "> SDCard Size: " + String(cardSize) + "MB";
        Serial.println(str);
    }

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
    while (gpsSerial.available() > 0) {
        char c = gpsSerial.read();
        if (gps.encode(c)) {
            if (gps.location.isValid()) {
                timeStamp = createTimestamp();
                if (lastPositionTime != timeStamp) { // Prüfe, ob eine Sekunde vergangen ist
                    setLed(true, GREEN_LED);
                    dateStampFdrs = createDatestampFdrs();
                    dateStampCsv = createDatestampCsv();
                    if (TEST) { serialTestOutput(); } // Testausgabe                 }
                    lastPositionTime = timeStamp; // Aktualisiere die letzte Ausgabezeit            
                    backupCsv(); // Backup der Daten auf die SD-Karte    
                    sendGnss(); // Sendet die GNSS-Daten an den FDRS-Gateway (jetzt innerhalb der Zeitschleife)
                    delay(100); // Wartezeit von 3 Sekunden
                    setLed(false, GREEN_LED);
                }   
            }   
        }
    }
    delay(10);
}
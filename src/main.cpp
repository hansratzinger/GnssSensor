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
// Release 1.2.4 HR 2025-03-20 NK
// -  added the function createTimestampCsv() to create a timestamp in CSV format
// -  added the function createDatestampCsv() to create a datestamp in CSV format
// -  added the function backupDataToSD() to backup the GNSS data to the SD card
// -  added the function createFilename() to create a filename based on the datestamp
// -  added the function initializeSD() to initialize the SD card
// Release 1.2.5 HR 2025-07-27 NK
// -  removed the function sendGnss() as it is not used anymore
// -----------------------------------------------------------------------------
// Include the necessary libraries
// -----------------------------------------------------------------------------
#include <Arduino.h>
#include <pins.h>
#include "GNSS_module.h"
#include <TinyGPSPlus.h>
#include <SPI.h>
#include <SD.h>
#include "SD_module.h"

// Deklaration von Variablen
const int meterBetweenTwoPoints = 0; // Distanz zwischen zwei Punkten
const bool TEST = true; // Definition der Konstante TEST
int lastPositionTime = 0; // Zeitstempel der letzten Position
int timeStampFdrs = 0;
int dateStampFdrs = 0;
String dateStampCsv = "";
String timeStampCsv = "";

double lastLat = 0.0;
double lastLon = 0.0;   

const int BUFFER_SIZE = 15;  // Anzahl der zu speichernden Datenpunkte
String dataBuffer[BUFFER_SIZE];  // Puffer für die Daten
int bufferIndex = 0;  // Aktueller Index im Puffer
unsigned long lastWriteTime = 0;  // Zeitpunkt des letzten Schreibvorgangs
const unsigned long WRITE_INTERVAL = 15000;  // Schreibintervall in Millisekunden (15 Sekunden)

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

int createTimestampFdrs() {
    int hours = gps.time.hour();
    int minutes = gps.time.minute();
    int seconds = gps.time.second();
    int timestamp = hours*10000 + minutes*100 + seconds;
    return timestamp;
}

String createTimestampCsv() {
    String timestamp ="";
    if (gps.time.hour() < 10) {
        timestamp = "0";
    }
    timestamp += String(gps.time.hour()) + ":";
    if (gps.time.minute() < 10) {
        timestamp += "0";
    }
    timestamp += String(gps.time.minute()) + ":";
    if (gps.time.second() < 10) {
        timestamp += "0";
    }
    timestamp += String(gps.time.second());
    return timestamp;
}

int createDatestampFdrs() {
    int year = gps.date.year() - 2000;  //  year - 2000
    int month = gps.date.month();
    int day = gps.date.day();
    int date = day*10000 + month*100 + year ;
    // Serial.println(year);
    // Serial.println(month);
    // Serial.println(day);
    return date;
}

String createDatestampCsv() {
    String datestamp = String(gps.date.year());
    if (gps.date.month() < 10) {
        datestamp += "0";
    }
    datestamp += String(gps.date.month());
    if (gps.date.day() < 10) {
        datestamp += "0";
    }
    datestamp += String(gps.date.day());
    return datestamp;
}


// Neue Funktion zum effizienteren Schreiben mehrerer Datenpunkte
bool backupMultipleDataToSD(const String &dateStamp, const String dataArray[], int count, int csPin) {
    String filename = "/backup_" + dateStamp + ".csv";
    
    // Öffne die Datei einmal für mehrere Schreibvorgänge
    File file = SD.open(filename, FILE_APPEND);
    if (!file) {
        Serial.println("Failed to open file for writing!");
        return false;
    }

    // Schreibe alle Datenpunkte
    for (int i = 0; i < count; i++) {
        file.println(dataArray[i]);
    }
    
    // Schließe die Datei erst nach dem Schreiben aller Daten
    file.close();
    return true;
}

void writeBufferToSD() {
    if (bufferIndex == 0) return;  // Keine Daten im Puffer
    
     
    if (backupMultipleDataToSD(dateStampCsv, dataBuffer, bufferIndex, SD_CS)) {
        Serial.println("Successfully wrote " + String(bufferIndex) + " data points to SD card.");
        setLed(false, RED_LED);
    } else {
        Serial.println("Failed to backup data to SD card.");
        setLed(true, RED_LED);
        
        // Versuche die SD-Karte neu zu initialisieren
        Serial.println("Attempting to reinitialize SD card...");
        SD.end();
        delay(100);
        if (SD.begin(SD_CS)) {
            Serial.println("SD card reinitialized successfully.");
        } else {
            Serial.println("SD card reinitialization failed.");
        }
    }
    
    // Puffer zurücksetzen
    bufferIndex = 0;
    lastWriteTime = millis();
    
}

void backupCsv() {
    // Daten für den Puffer vorbereiten
    String dateStampCsv = createDatestampCsv();
    String timeStampCsv = createTimestampCsv();
    String dataPoint = String(dateStampCsv) + "," + String(timeStampCsv) + "," + 
                       String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6) + "," + 
                       String(gps.satellites.value());
    
    // Datenpunkt dem Puffer hinzufügen
    dataBuffer[bufferIndex] = dataPoint;
    bufferIndex++;
    
    // Überprüfen, ob der Puffer voll ist oder ob das Schreibintervall erreicht wurde
    if (bufferIndex >= BUFFER_SIZE || millis() - lastWriteTime >= WRITE_INTERVAL) {
        writeBufferToSD();
    }
}
 
void setup() {
    setLed(true, RED_LED);
    Serial.begin(SERIALMONITOR_BAUD);
    delay(500);

    Serial.println("=====================================");
    Serial.println("======= G N S S - S E N S O R =======");
    Serial.println("=====================================");
        
    gpsSerial.begin(GPS_BAUD, SERIAL_8N1, GPS_RX, GPS_TX);
    delay(1000);

    Serial.println("GPS erfolgreich initialisiert");
    setLed(true, GREEN_LED);
    delay(100);

    Serial.print("GPS_RX: ");
    Serial.println(GPS_RX);
    Serial.print("GPS_TX: ");
    Serial.println(GPS_TX);

    Serial.println("=====================================");
    Serial.println("======= S D - K A R T E ============");
    Serial.println("=====================================");

    // Verbesserte Initialisierung der SD-Karte
    pinMode(SD_MISO, INPUT_PULLUP);
    pinMode(SD_MOSI, OUTPUT);  // MOSI sollte OUTPUT sein
    pinMode(SD_SCLK, OUTPUT);  // SCLK sollte OUTPUT sein
    pinMode(SD_CS, OUTPUT);    // CS sollte OUTPUT sein
    digitalWrite(SD_CS, HIGH); // Deaktiviere die SD-Karte zunächst
    
    delay(100);  // Warte, bis sich die Signale stabilisiert haben
       
    SPI.setFrequency(4000000); // Reduziere auf 4 MHz für stabilere Kommunikation
    SPI.begin(SD_SCLK, SD_MISO, SD_MOSI);
    
    int retry = 0;
    bool sdInitialized = false;
    
    // Versuche die SD-Karte bis zu 3 Mal zu initialisieren
    while (!sdInitialized && retry < 3) {
        if (SD.begin(SD_CS)) {
            sdInitialized = true;
            Serial.println("SD card initialized successfully.");
            uint32_t cardSize = SD.cardSize() / (1024 * 1024);
            String str = "> SDCard Size: " + String(cardSize) + "MB";
            Serial.println(str);
        } else {
            retry++;
            Serial.println("SD card initialization failed. Retry: " + String(retry));
            delay(500);
        }
    }
    
    if (!sdInitialized) {
        Serial.println("> It looks like you haven't inserted the SD card or the card is write-protected.");
    } else {
        setLed(false, RED_LED);        
    }
     
    Serial.println("Setup finished!");
    delay(300);
}
        


void loop() {
    while (gpsSerial.available() > 0) {
        char c = gpsSerial.read();
        if (gps.encode(c)) {
            if (gps.location.isValid() && gps.date.isValid() && gps.time.isValid()) {
                timeStampFdrs = createTimestampFdrs();
                if (lastPositionTime != timeStampFdrs) { // Prüfe, ob eine Sekunde vergangen ist
                    setLed(true, GREEN_LED);
                    dateStampFdrs = createDatestampFdrs();
                    dateStampCsv = createDatestampCsv();
                    if (TEST) { serialTestOutput(); } // Testausgabe
                    lastPositionTime = timeStampFdrs; // Aktualisiere die letzte Ausgabezeit            
                    backupCsv(); // Fügt Daten zum Puffer hinzu
                    delay(100); // Kürzere Wartezeit
                    setLed(false, GREEN_LED);
                }   
            }   
        }
    }
    
    // Überprüfe, ob das Schreibintervall erreicht wurde, auch wenn der Puffer nicht voll ist
    if (millis() - lastWriteTime >= WRITE_INTERVAL && bufferIndex > 0) {
        writeBufferToSD();
    }
    
    delay(10);
}
#include <Arduino.h>
#include <pins.h>
#include "GNSS_module.h"
#include <SPI.h>
#include <FS.h>
#include <TinyGPSPlus.h>
#include "DEV_Config.h"
#include "L76X.h"
#include "fdrs_node_config.h"
#include <fdrs_node.h>

#define GPS_BAUD 115200
#define SERIALMONITOR_BAUD 115200
#define LOGGING_BUFFER_SIZE 256 // Nochmals erhöhte Puffergröße
#define ESPNOW_SEND_TIMEOUT 200 // Timeout in Millisekunden
#define DEBUG_SD 1  // Debug-Ausgaben aktivieren
#define SWITCHTIME 1000 // Wartezeit von mindestens 1 Sekunde!
// Globale Statusvariable
float statusSensor = 99; // FDRS data 99 = Initialisierung läuft, 0 = OK, andere Werte für Fehler

// const String BRANCH = "dev"; // Branch name
// const String RELEASE = "2.4.0"; // Branch name

// Deklaration von Variablen
unsigned long timeDifference = 0;
// double positionDifference = 0.0;
// char gpstime[10] = "", date[11] = "", lat[15] = "", directionLat[2] = "", lon[15] = "", directionLng[2] = "", speed[10] = "", altitude[10] = "", hdop[10] = "", satellites[10] = "";
// char gpstimeLast[10] = "", dateLast[11] = "", latLast[15] = "", lonLast[15] = "", speedLast[10] = "", altitudeLast[10] = "", hdopLast[10] = "", satellitesLast[10] = "", loggingLast[100] = "", firstlineLast[100] = "";
// char logging[LOGGING_BUFFER_SIZE] = "";
// char gpsData[130] = "";
// double distanceLast = 0.0, latDifference = 0.0, lonDifference = 0.0; 

const bool TEST = false; // Definition der Konstante TEST
unsigned long lastPositionTime = 0;
unsigned long currentTime = 0;
static const unsigned long switchTime = 250; // Wartezeit von mindestens 0,25 Sekunde
// // Globale Variable für die SPI-Einstellungen
// SPISettings spiSettings(10000000, MSBFIRST, SPI_MODE0);

// static const size_t SMALL_BUFFER_SIZE = 15;
// static const size_t TINY_BUFFER_SIZE = 10;

// char getDirectionOfLat(double lat);
// char getDirectionOfLng(double lng);
// bool initSDCard();

// GPS Status Struktur
// struct GPSState {
//     char gpstime[TINY_BUFFER_SIZE];
//     char date[TINY_BUFFER_SIZE];
//     char lat[SMALL_BUFFER_SIZE];
//     char lon[SMALL_BUFFER_SIZE];
//     char speed[TINY_BUFFER_SIZE];
//     char altitude[TINY_BUFFER_SIZE];
//     char hdop[TINY_BUFFER_SIZE];
//     char satellites[TINY_BUFFER_SIZE];

double lastLat = 0.0;
double lastLon = 0.0;   
// double distance = 0.0;
// } gpsState = {0};

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
// // Funktion zum sicheren Konvertieren von String nach Double
// double safeStrtod(const char* str, double defaultValue) {
//     if (str == nullptr || *str == '\0') {
//         Serial.println("Konvertierungsfehler: Eingabestring ist leer oder Null");
//         return defaultValue;
//     }
//     char* endPtr;
//     double result = strtod(str, &endPtr);
//     if (*endPtr != '\0' && *endPtr != '.' && *endPtr != ',') {
//         Serial.print("Konvertierungsfehler: Ungültige Eingabe: ");
//         Serial.println(str);
//         return defaultValue;
//     }
//     return result;
// }

// void processPosition() {
//     // Statische Puffer statt dynamischer Speicherzuweisung
//     static char lat[16];
//     static char lon[16];
//     static char directionLat[3];
//     static char directionLng[3];
//     static char gpstime[11];
//     static char date[12];
//     static char hdop[11];
//     static char satellites[11];
//     static char speed[11];
//     static char altitude[11];
//     static char logging[LOGGING_BUFFER_SIZE]; // Nochmals erhöhte Puffergröße

//     // GPS-Daten formatieren
//     snprintf(lat, sizeof(lat), "%.6f", gps.location.lat());
//     snprintf(lon, sizeof(lon), "%.6f", gps.location.lng());

//     // Himmelsrichtung bestimmen
//     char directionLatChar = getDirectionOfLat(gps.location.lat());
//     char directionLngChar = getDirectionOfLng(gps.location.lng());
//     snprintf(directionLat, sizeof(directionLat), "%c", directionLatChar);
//     snprintf(directionLng, sizeof(directionLng), "%c", directionLngChar);

//     // Zeit und Datum
//     snprintf(gpstime, sizeof(gpstime), "%02d:%02d:%02d",
//         gps.time.hour(), gps.time.minute(), gps.time.second());
//     snprintf(date, sizeof(date), "%04d-%02d-%02d",
//         gps.date.year(), gps.date.month(), gps.date.day());

//     // GPS-Daten
//     snprintf(hdop, sizeof(hdop), "%.1f", gps.hdop.hdop());
//     snprintf(satellites, sizeof(satellites), "%d", gps.satellites.value());
//     snprintf(speed, sizeof(speed), "%.1f", gps.speed.knots());
//     snprintf(altitude, sizeof(altitude), "%.1f", gps.altitude.meters());

//     // Logging-String zusammenbauen
//     int snprintfResult = snprintf(logging, sizeof(logging), "%s;%s;%s;%s;%s;%s;%s;%s;%s;%s",
//         date, gpstime, lat, directionLat, lon, directionLng,
//         speed, altitude, hdop, satellites);

//     if (snprintfResult >= sizeof(logging)) {
//         Serial.println("FEHLER: Logging-Puffer ist zu klein!");
//         return;
//     }

//     // Distanzberechnungen
//     Serial.println("processPosition: Vor safeStrtod()"); // Debug-Ausgabe
//     double currentLat = safeStrtod(lat, 0.0);
//     double currentLon = safeStrtod(lon, 0.0);
//     double lastLat = safeStrtod(latLast, 0.0);
//     double lastLon = safeStrtod(lonLast, 0.0);
//     Serial.println("processPosition: Nach safeStrtod()"); // Debug-Ausgabe

//     latDifference = calculateDifference(currentLat, lastLat);
//     lonDifference = calculateDifference(currentLon, lastLon);
//     positionDifference = calculateDistance(currentLat, currentLon, lastLat, lastLon);

    
//     // Differenzen anhängen
//     snprintfResult = snprintf(logging + strlen(logging), sizeof(logging) - strlen(logging),
//         ";%.6f;%.6f;%.6f", latDifference, lonDifference, positionDifference);

//     if (snprintfResult >= sizeof(logging) - strlen(logging)) {
//         Serial.println("FEHLER: Logging-Puffer ist zu klein (Differenzen)!");
//         return;
//     }
    
//     // Debug-Ausgaben hinzufügen
//     Serial.printf("gpstimeLast: %s\n", gpstimeLast);
//     Serial.printf("dateLast: %s\n", dateLast);
//     Serial.printf("latLast: %s\n", latLast);
//     Serial.printf("lonLast: %s\n", lonLast);
//     Serial.printf("speedLast: %s\n", speedLast);
//     Serial.printf("altitudeLast: %s\n", altitudeLast);
//     Serial.printf("hdopLast: %s\n", hdopLast);
//     Serial.printf("satellitesLast: %s\n", satellitesLast);

//     // Aktuelle Position als letzte Position speichern
//     snprintf(gpstimeLast, sizeof(gpstimeLast), "%s", gpstime);
//     snprintf(dateLast, sizeof(dateLast), "%s", date);
//     snprintf(latLast, sizeof(latLast), "%s", lat);
//     snprintf(lonLast, sizeof(lonLast), "%s", lon);
//     snprintf(speedLast, sizeof(speedLast), "%s", speed);
//     snprintf(altitudeLast, sizeof(altitudeLast), "%s", altitude);
//     snprintf(hdopLast, sizeof(hdopLast), "%s", hdop);
//     snprintf(satellitesLast, sizeof(satellitesLast), "%s", satellites);

//     // Serial.println("processPosition() finished");
// }

void sendStatus(int status) {
    loadFDRS(status, STATUS_T); // Status 0 = OK, 99 = waiting for GNSS data
    sendFDRS();
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
    float fdrsDISTANCE_TO_LAST_POSITION = (float)calculateDistance(gps.location.lat(), gps.location.lng(), lastLat, lastLon); // positionDifference
    float fdrsYEAR = (float)gps.date.year(); // year
    float fdrsMONTH = (float)gps.date.month(); // month
    float fdrsDAY = (float)gps.date.day(); // day
    float fdrsHOUR = (float)gps.time.hour(); // hour
    float fdrsMINUTE = (float)gps.time.minute(); // minute
    float fdrsSECOND = (float)gps.time.second(); // second 

    loadFDRS(fdrsLAT, LATITUDE_T);
    loadFDRS(fdrsLON, LONGITUDE_T);
    loadFDRS(fdrsALT, ALTITUDE_T);
    loadFDRS(fdrsHDOP, HDOP_T);
    loadFDRS(fdrsSATS, LEVEL_T);
    loadFDRS(fdrsLATDIR, CURRENT_T);
    loadFDRS(fdrsLONDIR, UV_T);
    loadFDRS(fdrsHEADING, PM1_T);
    loadFDRS(fdrsDISTANCE_TO_LAST_POSITION, PM2_5_T);
    loadFDRS(fdrsYEAR, PM10_T);
    loadFDRS(fdrsMONTH, POWER_T);
    loadFDRS(fdrsDAY, POWER2_T);
    loadFDRS(fdrsHOUR, ENERGY_T);
    loadFDRS(fdrsMINUTE, ENERGY2_T);
    loadFDRS(fdrsSECOND, WEIGHT_T);

    // Speichern der aktuellen Position
    lastLat = gps.location.lat();
    lastLon = gps.location.lng();
      
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
    } else if (TEST) {
        Serial.print(statusSensor);
        Serial.println(" Keine gültigen GPS-Daten");
    }

    if ((currentTime - lastPositionTime >= SWITCHTIME) && (statusSensor == 0)) { // Wartezeit von mindestens 0,25 Sekunde
        lastPositionTime = currentTime;
        if ((gps.location.isValid()) && (gps.hdop.hdop() < 3.0) && (gps.date.year()) != 2000 && (gps.date.month()) != 0 && (gps.date.day()) != 0 && (gps.time.hour()) != 0 && (gps.time.minute()) != 0 && (gps.time.second()) != 0) {
            // Überprüfung ob die Position aktualisiert wurde und der HDOP-Wert unter dem Schwellenwert liegt
            // Aufrufen der Funktion zur Verarbeitung und Speicherung der Positionsdaten
            // processPosition();
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
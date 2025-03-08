#ifndef PINS_H
#define PINS_H

/**
 * @file pins.h
 * @brief Pin-Definitionen für ESP32 WROOM-32 mit SD-Karte und LC76 GNSS Modul
 */
 
// LC76 GNSS Modul UART Schnittstelle
#define GPS_RX      16    // ESP32 RX (Input) <- LC76 TX (Output)
#define GPS_TX      17    // ESP32 TX (Output) -> LC76 RX (Input)

// Status LED 
#define GREEN_LED_PIN  25    // Grüne LED an GPIO25 (output-fähig)
#define RED_LED_PIN    26    // Rote LED an GPIO26 (output-fähig)

#define SD_CHECK(x) do { \
    int __err_rc = (x); \
    if (__err_rc != ESP_OK) { \
        Serial.printf("SD error: %d at %s:%d\n", __err_rc, __FILE__, __LINE__); \
        return false; \
    } \
} while(0)

#endif /* PINS_H */

// FDRS Data Types

#define STATUS_T         0  // Status 0 = OK, 1 = Error
#define SPEED           17  // Status 0 = OK, 1 = Error
#define RPM             20 // RPM
#define LATITUDE_T      21 // GPS Latitude
#define LONGITUDE_T     22 // GPS Longitude
#define ALTITUDE_T      23 // GPS Altitude
#define HDOP_T          24 // GPS HDOP
#define SATELLITES      25 // satellites
#define DIRECTION_LAT   18 // direction lat 1/N 0/S 
#define DIRECTION_LON   19 // direction lat 1/N 0/S 
#define SPEED_KMH       26 // SPEED KMH
#define HEADING         27 // heading
#define POSITION_DIFF   28 // positionDifference
#define YEAR            29 // year
#define MONTH           30 // month
#define DAY             31 // day
#define HOUR            32 // hour
#define MINUTE          33 // minute
#define SECOND          34 // second

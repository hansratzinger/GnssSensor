#ifndef PINS_H
#define PINS_H

/**
 * @file pins.h
 * @brief Pin-Definitionen für ESP32 WROOM-32 mit SD-Karte und LC76 GNSS Modul
 */
 
// LC76 GNSS Modul UART Schnittstelle
#define GPS_RX      16    // ESP32 RX (Input) <- LC76 TX (Output)
#define GPS_TX      17    // ESP32 TX (Output) -> LC76 RX (Input)

// SD-Karte SPI Schnittstelle
#define SD_MISO 19
#define SD_MOSI 23
#define SD_SCLK 18
#define SD_CS    5

// Status LED 
#define GREEN_LED  25    // Grüne LED an GPIO25 (output-fähig)
#define RED_LED    26    // Rote LED an GPIO26 (output-fähig)

// FDRS Data Types
#define STATUS_T           0  // Status 0 = OK, 1 = Error
#define DATE              01
#define UTC               02
#define BOARDTIME_MILLSEC 07 // millis() from start of the board
#define BOARDTIME_SEC     16  // secounds,millsec
#define DIRECTION_LAT     18 // direction lat 1/N 0/S 
#define DIRECTION_LON     19 // direction lat 1/N 0/S 
#define RPM               20 // RPM
#define LATITUDE          21 // GPS Latitude
#define LONGITUDE         22 // GPS Longitude
#define ALTITUDE_T        23 // GPS Altitude
#define HDOP_T            24 // GPS HDOP
#define SATELLITES        25 // satellites
#define SPEED_KMH         26 // SPEED KMH
#define HEADING           27 // heading
#define POSITION_DISTANCE 28 // positionDifference

#define SERIALMONITOR_BAUD 115200
#define GPS_BAUD 115200

#endif /* PINS_H */
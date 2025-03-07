#ifndef PINS_H
#define PINS_H

/**
 * @file pins.h
 * @brief Pin-Definitionen für ESP32 WROOM-32 mit SD-Karte und LC76 GNSS Modul
 */
 
// SD-Karten Schnittstelle
#define SD_MISO     19    // SD Card: Master In Slave Out
#define SD_MOSI     23    // SD Card: Master Out Slave In  
#define SD_SCLK     18    // SD Card: Serial Clock
#define SD_CS       5     // SD Card: Chip Select
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

#ifndef FDRS_DATA_TYPES
#define FDRS_DATA_TYPES

#define STATUS_T        0  // Status 0 = OK, 1 = Error
#define CURRENT_T       18 // Current
#define IT_T            20 // RPM
#define LATITUDE_T      21 // GPS Latitude
#define LONGITUDE_T     22 // GPS Longitude
#define ALTITUDE_T      23 // GPS Altitude
#define HDOP_T          24 // GPS HDOP
#define LEVEL_T         25 // satellites
#define UV_T            26 // direction lat 1/N 0/S
#define UV_T            26 // direction lon 1/E 0/W 
#define PM1_T           27 // heading
#define PM2_5_T         28 // positionDifference
#define PM10_T          29 // year
#define POWER_T         30 // month
#define POWER2_T        31 // day
#define ENERGY_T        32 // hour
#define ENERGY2_T       33 // minute
#define WEIGHT_T        34 // second


#endif //FDRS_DATA_TYPES

// FDRS Data Types

//  FARM DATA RELAY SYSTEM
//
//  ESP-NOW Sensor Example
//
//  Developed by Timm Bogner (timmbogner@gmail.com) in Urbana, Illinois, USA.
//  An example of how to send data via ESP-NOW using FDRS.
//  HR 2025-03-07 10:12 NK   

#include <Arduino.h>
#include "fdrs_node_config.h"
#include <fdrs_node.h>

// FDRS Data Types

#ifndef FDRS_DATA_TYPES
#define FDRS_DATA_TYPES

#define STATUS_T        0  // Status 0 = OK, 1 = Error
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


void sendGnss(float lat, float lon, float alt, float hdop) {   // Sendet die RPM-Werte an den FDRS-Gateway 
  loadFDRS(lat, LATITUDE_T);
  loadFDRS(lon, LONGITUDE_T);
  loadFDRS(alt, ALTITUDE_T);
  loadFDRS(hdop, HDOP_T);
  
  // DBG(sendFDRS()); // Debugging 
  if (sendFDRS()) {
    DBG("Big Success!");
  } else {
    DBG("Nope, not so much.");
  }
}

void setupFDRS() {
  beginFDRS();
}

void sendStatus(int status) {
  loadFDRS(status, STATUS_T); // Status 0 = OK, 99 = waiting for GNSS data
  sendFDRS();
}






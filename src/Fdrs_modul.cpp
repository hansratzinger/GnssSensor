//  FARM DATA RELAY SYSTEM
//
//  ESP-NOW Sensor Example
//
//  Developed by Timm Bogner (timmbogner@gmail.com) in Urbana, Illinois, USA.
//  An example of how to send data via ESP-NOW using FDRS.
//  HR   
#include <Arduino.h>
#include "fdrs_node_config.h"
#include <fdrs_node.h>
// FDRS Data Types
#define IT_T            20 // RPM
#define LATITUDE_T      21 // GPS Latitude
#define LONGITUDE_T     22 // GPS Longitude
#define ALTITUDE_T      23 // GPS Altitude
#define HDOP_T          24 // GPS HDOP
#define LEVEL_T         25 // satellites
#define UV_T            26 // speed
#define PM1_T           27 // date
#define PM2_5_T         28 // positionDifference
#define PM10_T          29 
#define POWER_T         30 // directionLat
#define POWER2_T        31 // directionLng
#define ENERGY_T        32 
#define ENERGY2_T       33 

void GnssFDRS(float lat, float lon, float alt, float hdop) {   // Sendet die RPM-Werte an den FDRS-Gateway 
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






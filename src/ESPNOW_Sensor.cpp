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

#define LATITUDE_T      21 // GPS Latitude
#define LONGITUDE_T     22 // GPS Longitude
#define ALTITUDE_T      23 // GPS Altitude
#define HDOP_T          24 // GPS HDOP

float data1;
float data2;

void RpmSensorFDRS(float data1) {   // Sendet die RPM-Werte an den FDRS-Gateway 
  loadFDRS(data1, IT_T);
  // DBG(sendFDRS()); // Debugging 
  if (sendFDRS()) {
    DBG("Big Success!");
  } else {
    DBG("Nope, not so much.");
  }
}

void setup1() {
  beginFDRS();
}






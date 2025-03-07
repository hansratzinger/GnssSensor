//  FARM DATA RELAY SYSTEM
//
//  ESP-NOW Sensor Example
//
//  Developed by Timm Bogner (timmbogner@gmail.com) in Urbana, Illinois, USA.
//  An example of how to send data via ESP-NOW using FDRS.
//  HR   
#ifndef FDRS_MODUL_H
#define FDRS_MODUL_H
// #include <Arduino.h>
// #include "fdrs_node_config.h"
// #include <fdrs_node.h>

#define IR_SENSOR_PIN 15    // GPIO15: Infrarotsensor Pin
#define LED_PIN 2           // GPIO2: Kontroll-LED Pin

// FDRS Data Types
#define LATITUDE_T      21 // GPS Latitude
#define LONGITUDE_T     22 // GPS Longitude
#define ALTITUDE_T      23 // GPS Altitude
#define HDOP_T          24 // GPS HDOP

float data1;
float data2;

void GnssFDRS(float lat, float lon, float alt, float hdop);  // Sendet die RPM-Werte an den FDRS-Gateway

void setupFDRS(); // Initialisiert FDRS

void sendStatus(int status); // Sendet einen Fehler an den FDRS-Gateway

#endif// FDRS_MODUL_H

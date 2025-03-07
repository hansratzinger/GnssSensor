#ifndef GNSS_MODULE_H
#define GNSS_MODULE_H

#include <HardwareSerial.h>
#include <TinyGPS++.h>

// GPS Serial Interface
extern HardwareSerial gpsSerial;

/**
 * @brief Konvertiert Dezimalgrad in DMM Format
 * @param decimalDegrees Grad in Dezimalschreibweise
 * @return String im DMM Format
 */
String convertToDMM(double decimalDegrees);

/**
 * @brief Berechnet Distanz zwischen zwei Koordinaten
 * @return Distanz in Metern
 */
double calculateDistance(double lat1, double lon1, double lat2, double lon2);

/**
 * @brief Berechnet Differenz zwischen zwei Werten
 * @return Absolute Differenz
 */
double calculateDifference(double firstData, double secondData);

void sendGnnsData(); // Sendet die GNSS-Daten an den FDRS-Gateway

#endif // GNSS_MODULE_H


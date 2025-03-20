#ifndef SD_MODULE_H
#define SD_MODULE_H

#include <Arduino.h>

// Deklaration der Funktionen
bool initializeSD(int csPin);
String createFilename(String dateStamp);
bool backupDataToSD(String dateStamp, const String &data, int csPin);

#endif // SD_MODULE_H
#ifndef SD_MODULE_H
#define SD_MODULE_H

#include <Arduino.h>

// Deklaration der Funktionen
bool initializeSD(int csPin);
String createFilename(int dateStamp);
bool backupDataToSD(int dateStamp, const String &data, int csPin);

#endif // SD_MODULE_H
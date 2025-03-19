#include <Arduino.h>
#include <SD.h>

// Funktion zum Erstellen des Dateinamens basierend auf dem Datum
String createFilename(int dateStamp) {
    return "/backup_" + String(dateStamp) + ".csv";
}

// Funktion zum Schreiben von Daten in die CSV-Datei
bool backupDataToSD(int dateStamp, const String &data, int csPin) {
    String filename = createFilename(dateStamp);

    // Öffne die Datei im Anhängemodus
    File file = SD.open(filename, FILE_APPEND);
    if (!file) {
        Serial.println("Failed to open file for writing!");
        return false;
    }

    // Schreibe die Daten in die Datei
    file.println(data);
    file.close();
    Serial.println("Data successfully written to " + filename);
    return true;
}
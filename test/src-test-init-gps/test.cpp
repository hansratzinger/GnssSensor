#include <Arduino.h>
#include <HardwareSerial.h>
#include <pins.h> // Stelle sicher, dass diese Datei existiert und die Pins definiert

HardwareSerial gpsSerial(1); // Initialisierung von gpsSerial

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting...");

  gpsSerial.begin(115200, SERIAL_8N1, GPS_RX, GPS_TX);
  delay(100);

  if (gpsSerial) {
    Serial.println("GPS Serial initialized successfully!");
    gpsSerial.println("Hello from GPS Serial!"); // Sende einen Teststring
  } else {
    Serial.println("GPS Serial initialization failed!");
  }
}

void loop() {
  // Nichts zu tun in der Loop
}
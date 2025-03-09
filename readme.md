
# ÄNDERUNG DER FDRS DATENTYPEN

Da die einzelnen Datentypen bei FDRS fix belegt sind, wird die Zuordnung intern vorgenommen:

## FDRS Data Types  

// FDRS Data Types

#define STATUS_T         0  // Status 0 = OK, 1 = Error
#define LATITUDE        16  // RPM Packet Number
#define SPEED           17  // Status 0 = OK, 1 = Error
#define RPM             20 // RPM
#define LATITUDE_T      21 // GPS Latitude
#define LONGITUDE_T     22 // GPS Longitude
#define ALTITUDE_T      23 // GPS Altitude
#define HDOP_T          24 // GPS HDOP
#define SATELLITES      25 // satellites
#define DIRECTION_LAT   18 // direction lat 1/N 0/S 
#define DIRECTION_LON   19 // direction lat 1/N 0/S 
#define SPEED_KMH       26 // SPEED KMH
#define HEADING         27 // heading
#define POSITION_DIFF   28 // positionDifference
#define YEAR            29 // year
#define MONTH           30 // month
#define DAY             31 // day
#define HOUR            32 // hour
#define MINUTE          33 // minute
#define SECOND          34 // second

# GNSSLogger - ACHTUNG nicht AKTUELL!!! HR 2025-03-07 NK

- -  Verbindungsknoten im RNLI-E-class Sensoren-Network

- GNSSLogger ermittelt Lat/Lon Daten, speichert sie auf SD-card und broadcastet sie per ESPnow.
- RPMSensor misst per Infrarot Sensor die Drehzahl (RPM) der Motorwelle und broadcastet per ESPnow.
- EspNowHub empfängt die Daten von GNSSLogger und RPMSensor, erstellt einen Datenstring aus beiden Informationen und sendet sie an GPRSsender.

- -  GPRSSender  

- speichert die von EspNowHub empfangenen Datensätze, speichert sie auf MicroSD-card und lädt sie per GPRS NB-IoT auf eine Internet Datenbank hoch
- broadcastet jede Minute die aktuelle Zeit  

- - -  Ablaufbeschreibung

1. Es werden mit dem GNSS-Modul Positionen laufend an das Programm über die serielle Schnittstelle (UART) geschickt.

2. Wenn die übermittelte Position gültige Positionswerte, ein aktuelles Datum und HDOP kleiner 3 aufweist wird sie verarbeitet.

HR 2025-02-28 14:40 NK

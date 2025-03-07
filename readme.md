
# ÄNDERUNG DER FDRS DATENTYPEN

Da die einzelnen Datentypen bei FDRS fix belegt sind, wird die Zuordnung intern vorgenommen:

## FDRS Data Types  

- define IT_T            20 // RPM  
- define LATITUDE_T      21 // GPS Latitude
- define LONGITUDE_T     22 // GPS Longitude
- define ALTITUDE_T      23 // GPS Altitude
- define HDOP_T          24 // GPS HDOP
- define LEVEL_T         25 // satellites
- define UV_T            26 // speed
- define PM1_T           27 // date
- define PM2_5_T         28 // positionDifference
- define PM10_T          29  
- define POWER_T         30 // directionLat
- define POWER2_T        31 // directionLng
- define ENERGY_T        32  
- define ENERGY2_T       33  

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

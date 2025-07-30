# RNLI Project Documentation

This documentation describes the development and implementation of the sensors for the RNLI project.

## Project Overview

The RNLI project consists of two main sensor systems:

1. **GNSS Sensor**: A GPS-based system for recording position data with SD card storage.
2. **RPM Sensor**: A system for measuring rotational speed using Hall sensors, with data storage on SD card and EEPROM.

Both systems are based on the ESP32 microcontroller platform and are designed for use in maritime environments.

## System Architecture

The project implements two independent sensor systems that can be used separately or in combination:

```
                 +-------------+
                 |             |
                 | GNSS Sensor |
                 |             |
                 +------+------+
                        |
                        | SD Card Data
                        |
                        v
                 +-------------+
                 |             |
                 |  Analysis   |
                 |  Software   |
                 |             |
                 +-------------+
                        ^
                        |
                        | SD Card Data
                        |
                 +------+------+
                 |             |
                 | RPM Sensor  |
                 |             |
                 +-------------+
```

## Technical Specifications

### GNSS Sensor

- **Microcontroller**: ESP32-WROOM-32
- **GPS Module**: GNSS module with GPS, GLONASS, BeiDou, and Galileo support
- **Data Storage**: microSD card (FAT32 format, up to 16 GB)
- **Power Supply**: 5V via USB or external supply with buck converter to 3.3V
- **Status Indication**: Three LEDs (red, green, yellow)
- **Interfaces**: Serial interface for programming/debugging
- **Housing**: 3D printed waterproof housing (IP54)

### RPM Sensor

- **Microcontroller**: ESP32-WROOM-32
- **Sensor**: Hall effect sensor (waterproof)
- **Display**: 1.8" OLED, 128x64 pixels
- **Data Storage**:
  - Primary: microSD card (FAT32 format, up to 16 GB)
  - Backup: 24AA512-MIC EEPROM (512 Kbit)
- **Real-time Clock**: DS3231 with battery backup
- **User Interface**: Three buttons (plus, minus, set)
- **Power Supply**: 5V via USB or external supply with buck converter to 3.3V
- **Status Indication**: LED
- **Housing**: 3D printed waterproof housing (IP54)

## Software Design

### GNSS Sensor Software

The GNSS sensor software is based on the TinyGPSPlus library and provides the following functions:

- GPS/GNSS data acquisition
- Position calculation
- Data logging to SD card in CSV format
- Status indication via LEDs
- Low power operation (sleep mode when not active)

The main processing loop reads data from the GPS module, processes it, and writes it to the SD card at a configurable interval (default: every 5 seconds).

### RPM Sensor Software

The RPM sensor software provides the following functions:

- Hall sensor signal processing
- RPM calculation based on signal pulses
- Temperature measurement
- Real-time data display on OLED screen
- Data logging to SD card in CSV format
- Backup data storage in EEPROM
- Date and time management via RTC
- User interface for settings and calibration

The main processing loop reads the Hall sensor input, calculates the RPM based on the time between pulses, and updates the display and storage media.

## Data Format

### GNSS Sensor Data Format

The GNSS sensor stores data in CSV format with the following fields:

```
Date,Time,Latitude,Longitude,Altitude,Speed,Satellites,HDOP
```

Example:
```
2025-07-10,15:30:45,47.123456,9.234567,450.5,12.3,8,1.2
```

### RPM Sensor Data Format

The RPM sensor stores data in CSV format with the following fields:

```
Date,Time,RPM,Temperature
```

Example:
```
2025-07-10,15:30:45,1250,24.5
```

## Installation and Deployment

For detailed installation instructions, see the [Installation Guide](Installation_Guide.md).

### Quick Setup Guide

1. Mount the sensors in their designated locations
2. Insert formatted SD cards into both sensors
3. Connect power supply (5V)
4. For the RPM sensor, set the date and time via the user interface
5. For the RPM sensor, mount the TPU sleeve with the 16 neodymium magnets (4 sets of 4 magnets) on the rotating part
6. Verify proper operation by checking the status indicators and test data

## Use Cases

### Maritime Engine Monitoring

The RPM sensor can be used to monitor the RPM of a boat engine, providing:

- Real-time RPM display for the operator
- RPM history for maintenance and operational analysis
- Temperature monitoring

### Position Tracking

The GNSS sensor can be used to:

- Record the path of a vessel
- Analyze movement patterns
- Document positions for various research purposes

## Data Analysis

The data collected by both sensors can be analyzed using standard spreadsheet software or specialized analysis tools:

1. **Data Import**: The CSV files can be directly imported into Excel, LibreOffice Calc, or similar software
2. **Visualization**: Data can be plotted to create graphs showing RPM trends or position tracks
3. **Statistical Analysis**: Calculate average RPM, maximum speed, etc.
4. **Position Mapping**: GPS data can be imported into mapping software like Google Earth

## Maintenance

### Preventive Maintenance

1. **Regular Inspections**:
   - Check housing seals for water ingress
   - Verify secure mounting of all components
   - Check for cable damage

2. **SD Card Maintenance**:
   - Periodically backup data
   - Reformat the SD card every few months
   - Use high-quality SD cards to minimize data corruption risks

3. **Battery Maintenance**:
   - Replace the RTC backup battery (CR2032) every 2-3 years

### Troubleshooting

For detailed troubleshooting guidance, refer to the [Installation Guide](Installation_Guide.md).

## Developers' Documentation

### Dependencies

**GNSS Sensor**:
- TinyGPSPlus library
- SD library

**RPM Sensor**:
- SD library
- SPI library
- Wire library (I2C)
- RTClib
- U8g2 (for OLED display)

### Building the Software

1. Install PlatformIO (recommended) or Arduino IDE
2. Clone the respective GitHub repository
3. Open the project in PlatformIO/Arduino IDE
4. Install the required libraries
5. Select the ESP32 board
6. Compile and upload the firmware

## Future Enhancements

Potential future enhancements for the project include:

1. **Wireless Connectivity**:
   - Add WiFi or Bluetooth capabilities for real-time data transmission
   - Implement remote configuration options

2. **Advanced Analytics**:
   - Develop dedicated software for analyzing the collected data
   - Implement predictive maintenance algorithms

3. **Integration**:
   - Combine the GNSS and RPM sensors into a single unit
   - Add additional sensors (e.g., temperature, humidity, pressure)

4. **User Interface Improvements**:
   - Develop a mobile app for configuration and data viewing
   - Add a larger graphical display option

## GitHub Repositories

Complete documentation, source code, and 3D printing files (.3mf) for the housings and components are available in the following GitHub repositories:

### GNSS Sensor
**Repository:** [github.com/hansratzinger/GnssSensor](https://github.com/hansratzinger/GnssSensor)

### RPM Sensor
**Repository:** [github.com/hansratzinger/RpmSensor](https://github.com/hansratzinger/RpmSensor)

In the `/3d` folder of both repositories, you will find .3mf files for 3D printing all required housing parts, mounts, and other components.

## Appendix: Images

### GNSS Sensor

![GNSS Sensor Base Board](images/GNSS-Sensor%20Grundplatine.jpg)
*GNSS Sensor Base Board with Components*

### RPM Sensor

![RPM Box Board1 without OLED](images/RPM-Box%20Platine1%20ohne%20OLED.jpg)
*RPM Box Main Board (View 1)*

![RPM Box Board2 without SD and RTC](images/RPM-Box%20Platine2%20ohne%20SD%20und%20RTC.jpg)
*RPM Box Main Board (View 2)*

![RPM Box Board3 with EEPROM](images/RPM-Box%20Platine3%20mit%20EEPROM.jpg)
*RPM Box Main Board with EEPROM*

![Hall Sensor Mounting](images/Hall-Sensor%20Montage.jpg)
*Mounting of the Hall Sensor*

---

*This project documentation is part of the documentation of the RNLI project.*
*Version 1.0, July 2025*

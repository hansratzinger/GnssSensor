# GNSS Sensor User Manual

## Introduction

This manual describes the operation and maintenance of the GNSS sensor developed for the RNLI project. The GNSS sensor enables precise acquisition of position data and their storage on an SD card.

## Table of Contents

1. Overview
2. Getting Started
3. Daily Use
4. LED Status Indicators
5. Data Retrieval and Management
6. Troubleshooting
7. Maintenance and Care
8. Technical Specifications

## 1. Overview

The GNSS sensor is a compact device for continuous acquisition of position data using GNSS (Global Navigation Satellite System). It was specifically developed for use on sea rescue vessels and is robust against maritime conditions.

The system automatically stores the following data:
- Geographical coordinates (longitude and latitude)
- Date and time
- Number of available satellites

## 2. Getting Started

### 2.1 Package Contents

- GNSS sensor unit in weatherproof housing
- Power cable (5V)
- Mounting materials
- SD card (pre-formatted)
- This documentation

### 2.2 Installation

1. **Mounting**: Attach the device at a position with clear view of the sky. Ideally at an elevated point without cover.

2. **Connect power supply**: Connect the power cable to a 5V power source. The device switches on automatically.

3. **Initial setup**: After switching on, the device automatically goes through the following steps:
   - Self-test (red LED lights up)
   - Initialization of the GNSS module
   - Search for satellites (green LED lights up)
   - Start of data recording (green LED flashes)

## 3. Daily Use

The GNSS sensor works fully automatically and does not require user interaction during daily operation. After switching on, the device automatically begins capturing and storing GNSS data.

### 3.1 Switching On and Off

- **Switching on**: Connect the device to the power supply.
- **Switching off**: Disconnect the device from the power supply.

It is recommended to keep the device switched on during the entire operation time to ensure continuous data acquisition.

### 3.2 Normal Operating States

In normal operation, the device goes through the following states:

1. **Initialization**: When switched on, the device performs a self-test.
2. **Satellite search**: The device searches for satellite signals (may take a few minutes depending on environmental conditions).
3. **Data acquisition**: Once sufficient satellites have been found, data acquisition begins.
4. **Data storage**: The captured data is regularly written to the SD card.

## 4. LED Status Indicators

The LEDs on the front of the housing indicate the current operating status:

| LED Status | Meaning |
|------------|---------|
| Red light (bright) | Setup after power-on |
| Green on | Searching for position |
| Green flashing | Position written to SD card |
| Green flashing and Red on | Position calculated, but write error on SD card |
| Red on (dim) | No SD card found |

## 5. Data Retrieval and Management

### 5.1 Accessing Stored Data

To access the recorded data:

1. Switch off the device (disconnect power supply).
2. Carefully open the housing (pay attention to waterproofing).
3. Remove the SD card; for this, the screws of the board holder must be loosened first.
4. Read the SD card with a computer.
5. Re-insert the SD card and carefully close the housing.

### 5.2 Data Format

The data is stored on the SD card in CSV files. The file name follows the format `backup_YYYYMMDD.csv`, where YYYYMMDD is the current date. Date and time are in UTC.

Each line in the CSV file contains the following entry:
```csv
YYYYMMDD,HH:MM:SS,Latitude,Longitude,NumberOfSatellites
```

Example:
```csv
20250730,14:35:22,50.123456,7.654321,8
```

### 5.3 Recommended Data Backup

It is recommended to back up the data regularly (e.g., weekly) from the SD card. This can be done by copying the CSV files to a computer or another storage medium.

## 6. Troubleshooting

### 6.1 LED Error Codes

| LED Status | Problem | Solution |
|------------|---------|---------|
| Red on (dim) | No SD card found | Check SD card, replace if necessary |
| Green flashing and Red on | Write error on SD card | Check SD card, format or replace |
| No LED lights up | No power supply | Check power connection and cable |

### 6.2 Common Problems

1. **Position determination not possible**:
   - Make sure the device has a clear view of the sky.
   - Wait at least 5 minutes for the first fix.
   - Check if the device is in an area with good GNSS coverage.

2. **SD card errors**:
   - Format the SD card in FAT32 format.
   - Use a new SD card.
   - Make sure the SD card is correctly inserted.

3. **Device does not respond**:
   - Disconnect the power supply and reconnect after 30 seconds.
   - Check all connections.

## 7. Maintenance and Care

### 7.1 Regular Inspection

- Regularly check the water tightness of the housing.
- Check the mounting of the device.
- Clean the device with a damp cloth if necessary (no aggressive cleaning agents).

### 7.2 SD Card

The SD card has a limited lifespan. It is recommended to:
- Replace the SD card every 6-12 months.
- Use only high-quality SD cards.
- Regularly check the SD card for errors.

## 8. Technical Specifications

- **Power supply**: 
  - Input: 5V DC
  - Internal: Buck Step Down Converter to 3.3V for SD card and GNSS module
  - ESP32: Direct 5V supply via USB input
- **Power consumption**: max. 200mA
- **Operating temperature**: -10°C to +50°C
- **Protection class**: IP65
- **GNSS accuracy**: Typ. ±2.5m
- **Data recording interval**: Approx. every 1-5 seconds (depending on configuration)
- **SD card**: FAT32 format, recommended size 8-16GB
- **Dimensions**: According to housing
- **Weight**: According to configuration

---

## Appendix: Images and Diagrams

![GNSS Sensor Base Board](images/GNSS-Sensor%20Grundplatine.jpg)
*GNSS Sensor Base Board with ESP32 and Buck Step Down Converter*

![GNSS Sensor Board Top](images/GNSS-Sensor%20Platine%20oben%20SD%20und%20GNSS%20gelb.jpg)
*GNSS Sensor Board Top with SD Card and GNSS Module (yellow insulation tape)*

---

## Source Code and 3D Print Templates

The complete source code and all associated files for this project are available in the GitHub repository:

**GitHub Repository:** [github.com/hansratzinger/GnssSensor](https://github.com/hansratzinger/GnssSensor)

The repository contains:

- The complete source code of the project
- Configuration files
- 3D printing templates (.3mf files) in the `/3d` folder for all required housing parts and mounts

---

*This manual is part of the documentation of the RNLI project.*
*Version 1.0, July 2025*

# GNSS Sensor Documentation

## Overview
This application is designed for ESP32 microcontrollers to collect, process, and store GNSS (Global Navigation Satellite System) data. The system reads positioning information from a GNSS module, processes it, and backs up the data to an SD card. The data includes coordinates, time, date, and satellite information.

## Hardware Requirements
- ESP32 development board
- GNSS/GPS module (connected via UART)
- SD card module (connected via SPI)
- LEDs for status indication
- Power supply (5V)

## Pin Configuration
- **GNSS Module**:
  - RX: GPIO 16
  - TX: GPIO 17
- **SD Card Module**:
  - MISO: As defined in pins.h
  - MOSI: As defined in pins.h
  - SCLK: As defined in pins.h
  - CS: As defined in pins.h
- **Status LEDs**:
  - RED_LED: Error indicator
  - GREEN_LED: Operation indicator
  - YELLOW_LED: Optional, for SD card write operations

## Software Components

### Main Functionality
The software continuously reads GNSS data, processes it when valid information is received, and periodically writes this data to an SD card in CSV format.

### Key Features
1. **GNSS Data Processing**: Reads raw GNSS data and extracts position, time, date, and satellite information
2. **Buffered Writing**: Implements a buffer system to reduce frequent SD card writes
3. **Error Handling**: Handles communication errors with both GNSS module and SD card
4. **Status Indication**: Uses LEDs to indicate operational status

### Data Flow
1. Read data from GNSS module via UART
2. Process data and extract relevant information
3. Add processed data to buffer
4. Write buffer to SD card when full or at specified intervals
5. Handle any errors encountered during processing

## Software Architecture

### Core Functions

#### GNSS Data Processing
- `serialTestOutput()`: Outputs GNSS data to serial monitor for debugging
- `createTimestampFdrs()`: Creates a timestamp in FDRS format
- `createTimestampCsv()`: Creates a timestamp in CSV format (HH:MM:SS)
- `createDatestampFdrs()`: Creates a datestamp in FDRS format
- `createDatestampCsv()`: Creates a datestamp in CSV format (YYYYMMDD)

#### SD Card Operations
- `backupMultipleDataToSD()`: Writes multiple data entries to the SD card in one operation
- `writeBufferToSD()`: Manages the buffer and writes it to the SD card when appropriate
- `backupCsv()`: Prepares GNSS data for buffer storage and triggers writes when needed

#### System Management
- `setup()`: Initializes hardware components and systems
- `loop()`: Main program loop that reads GNSS data and processes it
- `setLed()`: Controls LED indicators based on system status

### Data Buffering
The system implements a data buffer to optimize SD card write operations:
- Buffer Size: Defined by `BUFFER_SIZE` (default: 15 entries)
- Write Interval: Defined by `WRITE_INTERVAL` (default: 15000ms)
- The buffer is written to the SD card when either:
  - The buffer is full
  - The write interval has elapsed since the last write

## SD Card Data Format
Data is stored in CSV files named with the pattern `backup_YYYYMMDD.csv`. Each line contains:
- Date (YYYYMMDD)
- Time (HH:MM:SS)
- Latitude (decimal degrees, 6 decimal places)
- Longitude (decimal degrees, 6 decimal places)
- Number of satellites in view

## Error Handling
- The system attempts to reinitialize the SD card if write operations fail
- LED indicators show operational status:
  - RED_LED on: Error condition
  - GREEN_LED on: Normal operation
  - GREEN_LED blinking: Data processing in progress

## Performance Optimizations
- SPI frequency reduced to 4MHz for more reliable SD card communication
- Proper pin configuration with pullup resistors where appropriate
- Multiple retry attempts for SD card initialization
- Buffered writes to reduce wear on the SD card

## Known Limitations
- SD card write errors may still occur due to hardware issues
- Performance may degrade when writing large amounts of data
- System assumes GNSS module provides valid data when available

## Future Improvements
- Implementation of power management features
- Remote data transmission capabilities
- More comprehensive error logging
- Enhanced data filtering and validation

## Troubleshooting
If the system fails to write to the SD card:
1. Check that the SD card is properly inserted and not write-protected
2. Ensure adequate power supply to both ESP32 and SD card module
3. Try formatting the SD card to FAT32 format
4. Check physical connections between ESP32 and SD card module
5. Try a different SD card

## LED´s
red light (bright) - setup after power on
green on - searching for postion
green flashing - position written to SD-card
green flashing and red on - position calculated but write error occurred on SD-card
red on (dim) - no card found

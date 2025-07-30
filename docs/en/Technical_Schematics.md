# Technical Documentation: Hardware Schematics

## GNSS Sensor Schematic

```
                            +------------+
                            |            |
+------------+              | ESP32      |
|            |  TX (GPIO16) |            |
|  GNSS      |------------->|            |         +------------+
|  Module    |              |            |         |            |
|            |  RX (GPIO17) |            | MOSI    |            |
|            |<-------------|            |-------->|            |
+------------+              |            | MISO    |            |
                            |            |<--------|  SD Card   |
                            |            | SCK     |  Module    |
                            |            |-------->|            |
                            |            | CS      |            |
                            |            |-------->|            |
                            |            |         |            |
+------------+              |            |         +------------+
|            |  RED_LED     |            |
|  Status    |<-------------|            |
|  LEDs      |  GREEN_LED   |            |
|            |<-------------|            |
|            |  YELLOW_LED  |            |
|            |<-------------|            |
+------------+              +------------+
```

### Connection Table GNSS Sensor

| ESP32 Pin | Connected to | Function |
|-----------|-------------|----------|
| GPIO 16   | GNSS TX     | UART Data Reception |
| GPIO 17   | GNSS RX     | UART Data Transmission |
| GPIO 23   | SD MOSI     | SPI Master Out Slave In |
| GPIO 19   | SD MISO     | SPI Master In Slave Out |
| GPIO 18   | SD SCK      | SPI Clock |
| GPIO 5    | SD CS       | SD Chip Select |
| RED_LED   | Red LED     | Error Indication |
| GREEN_LED | Green LED   | Operation Indication |
| YELLOW_LED| Yellow LED  | SD Card Access |
| 5V        | Power Supply | Operating Voltage |
| GND       | Ground      | Common Ground Point |

### Component Specifications

1. **ESP32-WROOM-32**
   - Operating voltage: 3.3V (5V via USB)
   - CPU: Dual-Core Xtensa LX6, up to 240 MHz
   - Flash memory: 4 MB
   - RAM: 520 KB

2. **GNSS Module**
   - Operating voltage: 3.3V
   - Current consumption: approx. 30-50 mA
   - Receiver: GPS, GLONASS, BeiDou, Galileo
   - Update rate: 1 Hz (standard)

3. **SD Card Module**
   - Operating voltage: 3.3V
   - Supports SD and SDHC cards
   - Communication: SPI

4. **Status LEDs**
   - Operating voltage: 3.3V
   - Series resistor: 220 Ohm per LED
   - Colors: Red, Green, Yellow

---

## RPM Sensor Schematic

```
                 +------------+
                 |            |         +------------+
                 | ESP32      |  SDA    |            |
+------------+   |            |-------->|            |
|            |   |            |  SCL    |  OLED      |
|  Hall      |   |            |-------->|  Display   |
|  Sensor    |   |            |         |            |
|            |---|GPIO 15     |         |            |
+------------+   |            |         +------------+
                 |            |
                 |            |         +------------+
+------------+   |            |  SDA    |            |
|            |   |            |-------->|            |
|  Buttons   |---|GPIO 25/26/27         |  DS3231    |
|  (3x)      |   |            |  SCL    |  RTC       |
|            |   |            |-------->|            |
+------------+   |            |         |            |
                 |            |         +------------+
                 |            |
                 |            |         +------------+
+------------+   |            |  CS     |            |
|            |   |            |-------->|            |
|  Status    |   |            |  MOSI   |  SD Card   |
|  LED       |---|GPIO 12     |-------->|  Module    |
|            |   |            |  MISO   |            |
+------------+   |            |<--------|            |
                 |            |  SCK    |            |
                 |            |-------->|            |
                 |            |         +------------+
                 |            |
                 |            |         +------------+
                 |            |  SDA2   |            |
                 |            |-------->|            |
                 |            |  SCL2   |  EEPROM    |
                 |            |-------->|  24AA512   |
                 |            |         |            |
                 +------------+         +------------+
```

### Connection Table RPM Sensor

| ESP32 Pin | Connected to | Function |
|-----------|-------------|----------|
| GPIO 15   | Hall Sensor  | Pulse Signal |
| GPIO 21   | SDA (I2C-1) | I2C Data Line for OLED and RTC |
| GPIO 22   | SCL (I2C-1) | I2C Clock Line for OLED and RTC |
| GPIO 16   | SDA (I2C-2) | I2C Data Line for EEPROM |
| GPIO 17   | SCL (I2C-2) | I2C Clock Line for EEPROM |
| GPIO 25   | PLUS Button | Value Increase (blue) |
| GPIO 26   | MINUS Button | Value Decrease (white) |
| GPIO 27   | SET Button  | Confirmation/Setting (yellow) |
| GPIO 12   | Status LED  | Operation Indication |
| GPIO 5    | SD CS       | SD Chip Select |
| MOSI      | SD MOSI     | SPI Master Out Slave In |
| MISO      | SD MISO     | SPI Master In Slave Out |
| SCK       | SD SCK      | SPI Clock |
| 5V        | Power Supply | Operating Voltage |
| GND       | Ground      | Common Ground Point |

### Component Specifications

1. **ESP32-WROOM-32**
   - Operating voltage: 3.3V (5V via USB)
   - CPU: Dual-Core Xtensa LX6, up to 240 MHz
   - Flash memory: 4 MB
   - RAM: 520 KB

2. **Hall Sensor**
   - Operating voltage: 3.3V-5V
   - Output: Digital (High/Low)
   - Sensitivity: Optimized for 1-3 mm distance to the magnet

3. **OLED Display**
   - Type: 128x64 pixels
   - Interface: I2C
   - Operating voltage: 3.3V

4. **DS3231 RTC**
   - High-precision real-time clock
   - I2C interface
   - Backup battery: CR2032

5. **SD Card Module**
   - Operating voltage: 3.3V
   - Supports SD and SDHC cards up to 16 GB
   - Communication: SPI

6. **24AA512-MIC EEPROM**
   - Capacity: 64 KB
   - I2C interface
   - Data retention: >100 years
   - Write/read cycle count: >1 million

7. **Buttons and LED**
   - 3 buttons with pull-up resistors (10 kOhm)
   - LED with series resistor (220 Ohm)

### Mounting the Magnets

For the correct function of the RPM sensor, the proper mounting of the magnets is crucial:

General mounting of a Hall sensor:
The number of magnets must be coordinated with the ESP32 program being used. In the case of this project, 4 magnets per revolution are measured.

- Use 4 magnets, evenly distributed around the circumference of the rotating part
- Distance between magnets and Hall sensor: max 18-20 mm since in this project's magnet sleeve, 4 strong neodymium magnets per position are used. (otherwise distance 1-3 mm)
- All magnets should be oriented with the same polarity towards the sensor
- Secure the magnets firmly to prevent them from coming loose during operation

### Schematic Representation of the Magnets

```
      [Magnet 1]
     /           \
[Magnet 2]     [Magnet 4]
     \           /
      [Magnet 3]

       Rotating
          Part
```

---

## GitHub Repositories

The complete source code, schematics, and 3D printing templates for both projects are available in the following GitHub repositories:

### GNSS Sensor
**Repository:** [github.com/hansratzinger/GnssSensor](https://github.com/hansratzinger/GnssSensor)

### RPM Sensor
**Repository:** [github.com/hansratzinger/RpmSensor](https://github.com/hansratzinger/RpmSensor)

In the `/3d` folder of both repositories, you will find .3mf files for 3D printing all required housing parts, mounts, and other components.

---

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

*This technical documentation is part of the documentation of the RNLI project.*
*Version 1.0, July 2025*

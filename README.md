# EVCS Sensor Research

>Author: John Phillips (jcp3ats@virginia.edu) <br>
Faculty Supervisor: Stephan De Wekker (sfd3d@virginia.edu)
<br>
These code projects were written in the Spring 2023 semester for EVSC: 4995 (Supervised Research) at the University of Virginia.  The purpose of this codebase is to create an embedded logging peripheral for an FT-7 Series anemometer.


## Projects
* [BLE_Server_ESP32](#ble_server_esp32)
* [BLE_Test_ESP32](#ble_test_esp32)
* [GPS_Clock](#gps_clock)
* [NonBlockingWrite](#nonblockingwrite)
* [RS485_example](#rs485_example)
* [RTCZero_SetTime](#rtczero_settime)
* [RTCZero_SetTime_Optimized](#rtczero_settime_optimized)
* [SensorLog](#sensorlog)
* [SensorLog_OLD](#sensorlog_old)
* [SensorPoll](#sensorpoll)
* [SensorPoll_ESP32](#sensorpoll_esp32)
* [SensorSerial](#sensorserial)

### BLE_Server_ESP32
> Polls the FT702LT anemometer for wind speed and direction and send data via BLE GATT Characteristics

***Code Created for RocketScream Red Comet ESP32***

Serial Pins (anemometer)
- RX Pin -> 4
- TX Pin -> 5

### BLE_Test_ESP32
> Simple BLE sketch for ESP32, shows writing to GATT characteristics and callback structure

***Code Created for RocketScream Red Comet ESP32***

### GPS_Clock
> Set the Arduino Zero on-board RTC through GPS data

### NonBlockingWrite
> Modified sketch of "Non-blocking Write" example in SD.h

### RS485_example
> Basic usage of RS-485 (half-duplex) between 2 Arduino boards
- RX functions -> Reciever (Connected to SerialUSB)
- TX functions -> Transmitter (Connected to power)

### RTCZero_SetTime
> Sets Arduino Zero on-board RTC to the complile time of the program <br>
***Code from a [Github Issue](https://github.com/arduino-libraries/RTCZero/issues/53)***

### RTCZero_SetTime_Optimized
> Optimized version of the [RTCZero_SetTime](#rtczero_settime)

Optimizes compile time parsing and uses a user-defined date to reduce the compile time delay to 7.4s

### SensorLog
> Polls the FT702LT anemometer for wind speed and direction and saves the data to attached SD card <br> ***Uses Sdfat library***

This was implimented using an FSM to write sensor data to a user defined 512-byte global buffer. The sensor readings would be formatted into 32-byte packets in the following format: $MM/DD/YY,HH:MM:SS.SSS,VVV.V,DDD; V=wind speed in m/s, D=wind direction in degrees. Once 16 of these packets (512-bytes) populated the user defined buffer, this buffer would be writen to the SD card.  THIS IS NOT NEEDED.  The SD Library will automatically buffer writes, making this user defined buffer extraneous.
- ***TODO*** : Test checksum response validation latency

### SensorLog_OLD
> Polls the FT702LT anemometer for wind speed and direction and saves the data to attached SD card <br> ***Uses SD library (wrapper for Sdfat)***

### SensorPoll
> Polls the FT702LT anemometer for wind speed and direction (prints to SerialUSB)

### SensorPoll_ESP32
> Polls the FT702LT anemometer for wind speed and direction (prints to SerialUSB)

***Code Created for RocketScream Red Comet ESP32***

### SensorSerial
> Enables the FT-7 anemometer to send continuous updates without periphal polling commands

This was an early project that was abandonded in favor of polling the sensor.  Untested, but the command for enabling the continuous updates (CU) is correct.

# EVCS Sensor Research

>Author: John Phillips (jcp3ats@virginia.edu) <br>
Faculty Supervisor: Stephan De Wekker (sfd3d@virginia.edu)
<br>
These code projects were written in the Spring 2023 semester for EVSC: 4995 (Supervised Research) at the University of Virginia.  The purpose of this codebase is to create an embedded logging peripheral for an FT-7 Serier anemometer.


## Projects
* [GPS_Clock](#gps_clock)
* [RS485_example](#rs485_example)
* [RTCZero_SetTime](#rtczero_settime)
* [RTCZero_SetTime_Optimized](#rtczero_settime_optimized)
* [SensorLog](#sensorlog)
* [SensorPoll](#sensorpoll)
* [SensorSerial](#sensorserial)

### GPS_Clock

> Set the Arduino Zero on-board RTC through GPS data

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
> ***TODO*** Continuation of [SensorPoll](#sensorpoll)

### SensorPoll
> Polls the FT702LT anemometer for wind speed and direction (prints to SerialUSB)

### SensorSerial
> ***TODO*** Enables the FT-7 anemometer to send continuous updates without periphal polling commands

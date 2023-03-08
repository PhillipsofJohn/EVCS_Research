#include <Arduino.h>
#include <TimeLib.h>
#include <RTCZero.h>
#include <stdio.h>

#define Serial SerialUSB

void print2digits(int number);

/* Create an rtc object */
RTCZero rtc;

void setup() 
{
    int hour, minute, second;

    rtc.begin(); // initialize RTC

    // set system time = compile time
    sscanf(__TIME__, "%2d %*c %2d %*c %2d", &hour, &minute, &second);
    rtc.setTime(hour, minute, second);
    rtc.setDate(31, 1, 2023);

    while(!SerialUSB);
    Serial.begin(9600);

    // Serial.println("Set Compile time to RTC");
    Serial.println(String("__TIME__ = ") + __TIME__);

    pinMode(LED_BUILTIN, OUTPUT);
}

/* Function call makes set operation slower */
// void set_time(char const *time)
// {
//   uint8_t hour, minute, second;
//   sscanf(time, "%2d %*c %2d %*c %2d", &hour, &minute, &second);
//   rtc.setTime(hour, minute, second);
// }


void loop() {
  printDate();
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  printDate();
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}

void printDate() {
  // Print date...
  print2digits(rtc.getDay());
  Serial.print("/");
  print2digits(rtc.getMonth());
  Serial.print("/");
  print2digits(rtc.getYear());
  Serial.print(" ");

  // ...and time
  print2digits(rtc.getHours());
  Serial.print(":");
  print2digits(rtc.getMinutes());
  Serial.print(":");
  print2digits(rtc.getSeconds());

  Serial.println();

  delay(1000);
}

void print2digits(int number) {
  if (number < 10) {
    Serial.print("0"); // print a 0 before if the number is < than 10
  }
  Serial.print(number);
}

#include <RTCZero.h>
#include <TimeLib.h>

#define TIME_MSG_LEN 11    // Length of serial time sync message
#define TIME_HEADER  'T'   // Header tag for serial time sync message

/* Change these values to set the current initial date */
#define day   30
#define month 1
#define year  23

#define Serial SerialUSB

/* Create an rtc object */
RTCZero rtc;

void setup() {
  while(!SerialUSB);
  Serial.begin(9600);

  rtc.begin(); // initialize RTC
  
  // Set the date
  rtc.setDate(day, month, year);
  rtc.setTime(1, 1, 1);
  Serial.print("T");
  // getPCtime();

}

void loop() {
  // printDate();
  getPCtime();
  delay(1000);
}

void getPCtime() {
  while (Serial.available() > 0) { // time message consists of header & 10 ASCII digits
    Serial.print(Serial.read());
    // char c = Serial.read() ;
    // Serial.print(c);
    // if ( c == TIME_HEADER ) {
      
    // }
  }
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

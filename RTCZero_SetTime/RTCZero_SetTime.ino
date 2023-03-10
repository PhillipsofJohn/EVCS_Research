#include <Arduino.h>
#include <TimeLib.h>
#include <RTCZero.h>
#include <stdio.h>

#define Serial SerialUSB

time_t cvt_date(char const *date, char const *time);
void print2digits(int number);

/* Create an rtc object */
RTCZero rtc;

void setup() 
{
    while(!SerialUSB);
    Serial.begin(9600);
    delay(5000);

    Serial.println("Set Compile time to RTC");
    
    // Show raw system strings
    Serial.println(String("__DATE__ = ") + __DATE__);
    Serial.println(String("__TIME__ = ") + __TIME__);

    rtc.begin(); // initialize RTC
    
    // set system time = compile time
    rtc.setEpoch(cvt_date(__DATE__, __TIME__));
    Serial.print("Unix Time: ");
    Serial.println(rtc.getEpoch());
}


time_t cvt_date(char const *date, char const *time)
{
    char s_month[5];
    int year, day;
    int hour, minute, second;
    tmElements_t t;
    static const char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
    sscanf(date, "%s %d %d", s_month, &day, &year);
    //sscanf(time, "%2hhd %*c %2hhd %*c %2hhd", &t.Hour, &t.Minute, &t.Second);
    sscanf(time, "%2d %*c %2d %*c %2d", &hour, &minute, &second);
    // Find where is s_month in month_names. Deduce month value.
    t.Month = (strstr(month_names, s_month) - month_names) / 3 + 1;
    t.Day = day;
    // year can be given as '2010' or '10'. It is converted to years since 1970
    if (year > 99) t.Year = year - 1970;
    else t.Year = year + 50;
    t.Hour = hour;
    t.Minute = minute;
    t.Second = second;
    return makeTime(t);
}


void loop() {
  printDate();
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

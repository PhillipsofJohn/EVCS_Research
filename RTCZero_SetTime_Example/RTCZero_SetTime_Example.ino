#include <Arduino.h>
#include <TimeLib.h>
#include <RTCZero.h>
#include <stdio.h>

#define Serial SerialUSB

// void set_time(char const *time);
// time_t cvt_date(char const *date, char const *time);
// void printDate();
void print2digits(int number);

/* Create an rtc object */
RTCZero rtc;

void setup() 
{
    while(!SerialUSB);
    Serial.begin(9600);

    // Serial.println("Set Compile time to RTC");
    
    // Show raw system strings
    // Serial.println(String("__DATE__ = ") + __DATE__);
    Serial.println(String("__TIME__ = ") + __TIME__);

    int hour, minute, second;

    rtc.begin(); // initialize RTC
    // set_time(__TIME__);
    sscanf(__TIME__, "%2d %*c %2d %*c %2d", &hour, &minute, &second);
    rtc.setTime(  hour, minute, second);
    rtc.setDate(31, 1, 2023);
    
    // set system time = compile time
    // rtc.setEpoch(cvt_date(__DATE__, __TIME__));
    // Serial.print("Unix Time: ");
    // Serial.println(rtc.getEpoch());

    pinMode(LED_BUILTIN, OUTPUT);
}

// void set_time(char const *time)
// {
//   uint8_t hour, minute, second;
//   sscanf(time, "%2d %*c %2d %*c %2d", &hour, &minute, &second);
//   rtc.setTime(hour, minute, second);
// }


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

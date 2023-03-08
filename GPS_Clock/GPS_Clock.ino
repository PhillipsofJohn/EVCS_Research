/**
  EVCS Research
  Name: GPS_Clock
  Purpose: Set the built in RTC using GPS

  @author John Phillps
  @version 1.0 2/20/23

*/

#include <Arduino.h>
#include <TimeLib.h>
#include <RTCZero.h>

#define SerialOut SerialUSB
#define SerialGPS Serial1

char qTM[] = {0xB5, 0x62, 0x06, 0x3D, 0x00, 0x00, 0x00};
int qTM_len = 7;


// void set_time(char const *time);
// time_t cvt_date(char const *date, char const *time);
// void printDate();
void print2digits(int number);

/* Create an rtc object */
RTCZero rtc;

#define ppsPin  10


void setup() {
  while(!SerialOut);
  SerialOut.begin(9600);

  pinMode(ppsPin, INPUT);
  digitalWrite(ppsPin, HIGH);
}

void loop() {
  if(digitalRead(ppsPin) == HIGH) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    SerialOut.println("beep");
  }
}

void setup2() {
  while(!SerialOut);
  SerialOut.begin(9600);

  SerialGPS.begin(9600);
}

void loop2() {
  while (SerialGPS.available() > 0){
    // get the byte data from the GPS
    char gpsData = SerialGPS.read();
    SerialOut.write(gpsData);
  }
}

void setup1() 
{
    while(!SerialOut);
    SerialOut.begin(9600);

    // SerialOut.println("Set Compile time to RTC");
    
    // Show raw system strings
    // SerialOut.println(String("__DATE__ = ") + __DATE__);
    SerialOut.println(String("__TIME__ = ") + __TIME__);

    int hour, minute, second;

    rtc.begin(); // initialize RTC
    // set_time(__TIME__);
    sscanf(__TIME__, "%2d %*c %2d %*c %2d", &hour, &minute, &second);
    rtc.setTime(  hour, minute, second);
    rtc.setDate(31, 1, 2023);
    
    // set system time = compile time
    // rtc.setEpoch(cvt_date(__DATE__, __TIME__));
    // SerialOut.print("Unix Time: ");
    // SerialOut.println(rtc.getEpoch());

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


void loop1() {
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
  SerialOut.print("/");
  print2digits(rtc.getMonth());
  SerialOut.print("/");
  print2digits(rtc.getYear());
  SerialOut.print(" ");

  // ...and time
  print2digits(rtc.getHours());
  SerialOut.print(":");
  print2digits(rtc.getMinutes());
  SerialOut.print(":");
  print2digits(rtc.getSeconds());

  SerialOut.println();

  delay(1000);
}

void print2digits(int number) {
  if (number < 10) {
    SerialOut.print("0"); // print a 0 before if the number is < than 10
  }
  SerialOut.print(number);
}

void addCK(char msg[], int len) {
  char ckA = 0, ckB = 0;
  for(int i=2; i<(len-2); i++) {
    ckA += msg[i];
    ckB += ckA;
  }
  msg[len-2] = ckA;
  msg[len-1] = ckB; 
}

char* addCK_1(char msg[]) {
  int n = (int) (sizeof(msg)/sizeof(msg[0]));
  char pkt[n+2];
  char ckA = 0, ckB = 0;
  for(int i=2; i<n; i++) {
    ckA += msg[i];
    ckB += ckA;
  }
  for(int i=0; i<n; i++) {
    pkt[i] = msg[i];
  }
  pkt[n+1] = ckA;
  pkt[n+2] = ckB;
  return msg;
}

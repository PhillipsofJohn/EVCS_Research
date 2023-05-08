/**
  EVCS Research
  Name: SensorLog_OLD
  Purpose:  Poll the FT702LT Wind Sesnor for wind speed and direction
            and save the data to attached SD card

  Note: Uses <SD.h> library not "SdFat.h"
  @author John Phillps
  @version 1.0 2/21/23
*/

#include <Arduino.h>
#include <RTCZero.h>
#include <SPI.h>
#include <SD.h>

#define SerialOut         SerialUSB
#define SerialSensor      Serial
#define USB_BAUDRATE      9600
#define SENSOR_BAUDRATE   9600
#define TX_EN_PIN         10

#define QWV_CMD           "$//,WV?*//\r\n"

#define USB_CONNECTED     1

 

/* FSM States */
enum {BOOT, SENSOR_POLL, SD_SAVE} FSM;

/* Create an rtc object */
RTCZero rtc;

/* SD Card File */
File sdFile;
const int ss = 4;
const int PKT_LEN = 32;
char sdFileName[13]; // SD FILE NAME LIMIT = 13
const char* tempFileName = "test.txt";

/* Function Prototypes */
void queryWV();
void queryCMD(char * cmd);
void sensorInit(int status);
byte verifyCkSum(char data[]);
void printWV(char data[]);
void setFileName();
uint32_t rtc_millis();

/* Globals */
int gCount = 0;
const int saveNum = 16; // needs to be 16 so that gBuf==512
char gBuf[saveNum * PKT_LEN];
int gYear, gMonth, gDay;

/* === STATE MACHINE === */
void RunFSM() {
  switch(FSM) {
    case BOOT:
      if(USB_CONNECTED) {
        // Start SerialUSB
        while(!SerialOut);
        SerialOut.begin(USB_BAUDRATE);
      }
      // Init Sensor  
      sensorInit(HIGH);
      // Init LED_BUILTIN
      pinMode(LED_BUILTIN, OUTPUT);
      digitalWrite(LED_BUILTIN, LOW);

      // Init SD Card
      if(!SD.begin(4)) {
        SerialOut.println("\r\n--ERROR-- SD CARD INIT FAILED\r\n");
        while(1) {
          digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
          delay(100);
        }
      }
      setFileName();
      // sdFile = SD.open(tempFileName, FILE_WRITE);
      // if(sdFile) {
      //   const char* tempBuf = "$-------------------------------$Sensor:FT702LT,Serial#:8107-011$-------------------------------";
      //   sdFile.write(tempBuf, (saveNum * PKT_LEN));
      //   sdFile.close();
      // } else {
      //   SerialOut.println("SD Header Write failed");
      //   while(1);
      // }


      // Set global date
      gYear  = rtc.getYear();
      gMonth = rtc.getMonth();
      gDay   = rtc.getDay();
      
      SerialOut.println("BOOT");
      FSM = SENSOR_POLL;    // >>>> State Change! <<<<//
      delay(1000);
      break;

    case SENSOR_POLL:
      char data[24];
      int rlen;

      queryWV(); // Poll Sensor for Windspeed
      
      // SerialOut.println("Bytes in serial buffer: " + String(SerialSensor.available()));
      if(SerialSensor.available()) {
        rlen = SerialSensor.readBytes(data, 24);
      }
      if (rlen == 24) {
        digitalWrite(LED_BUILTIN, HIGH);
        // SerialOut.println(data);  // FIX: causes weird overflow in serial buffer for next print
        // SerialOut.flush();      // not fixed by flush()

        if(data[18] == '0') { // data[18] = status bit
          // SerialOut.print("Count: ");
          // SerialOut.println(gCount);
          // SerialOut.print("Saving to index: ");
          // SerialOut.println(gCount*PKT_LEN);
          // char buf[11];
          // sprintf(buf, "ERROR: %c\r\n", status);
          // SerialOut.print(buf);
          
          // printWV(data); // !!! NEED TO TEST

          int sec   = rtc.getSeconds();
          int min   = rtc.getMinutes();
          int hour  = rtc.getHours();
          // uint32_t millis = rtc_millis();
          // SerialOut.print("Millis: ");
          // SerialOut.println(millis);
          // char buf[28];
          // sprintf(buf, "$%02d/%02d/%02d,%02d:%02d:%02d,%c%c%c.%c,%c%c%c", gMonth, gDay, gYear, hour, min, sec, data[8], data[9], data[10], data[12], data[14], data[15], data[16]);
          // memcpy(&gBuf[gCount*PKT_LEN], buf, PKT_LEN);
          sprintf(&gBuf[gCount*PKT_LEN], "$%02d/%02d/%02d,%02d:%02d:%02d.%03d,%c%c%c.%c,%c%c%c", gMonth, gDay, gYear, hour, min, sec, (millis() % 1000), data[8], data[9], data[10], data[12], data[14], data[15], data[16]);
          // SerialOut.println(buf);
          // SerialOut.println("---------");
          // SerialOut.println(gBuf);
          // SerialOut.println("---------");
          // SerialOut.println(rtc.getY2kEpoch());

          // Inc gCount only if packet is valid
          gCount++;          
        }
      }      
      if (gCount == saveNum) {
        gCount = 0;
        FSM = SD_SAVE;    // >>>> State Change! <<<<//
      }
      // delay(1000);
      digitalWrite(LED_BUILTIN, LOW);
      break;

    case SD_SAVE:
      // save data to SD card
      // SerialOut.println("SD_SAVE");
      sdFile = SD.open("test99.txt", FILE_WRITE);
      if(sdFile) {
        // SerialOut.print("Available to write: ");
        // SerialOut.println(sdFile.availableForWrite());
        sdFile.write(gBuf, (saveNum * PKT_LEN));
        sdFile.close();
      } else {
        SerialOut.println("\r\n--ERROR-- SD CARD WRITE FAILED");
        SerialOut.println(sdFile.getWriteError());
        SerialOut.println();
        delay(2000);
      }
      FSM = SENSOR_POLL;    // >>>> State Change! <<<<//
      break;

  } // end of switch(FSM)
}   // end of RunFSM

/**
  Queries sensor for wind speed and direction
  @param void
*/
void  queryWV() {
  queryCMD(QWV_CMD);
}

/**
  Sends command to sensor via RS485 (timing set for Query Commands)
  @param cmd Sensor Command
*/
void queryCMD(char * cmd) {
  digitalWrite(TX_EN_PIN, HIGH);     // Transmit Enabled
  delay(10);
  SerialSensor.print(cmd);           // Write WV Query CMD to sensor
  SerialSensor.flush();              // Waits for the transmission of outgoing serial data to complete
  delay(50);
  digitalWrite(TX_EN_PIN, LOW);     // Recieve Enabled
  delay(150);
}

/**
  Inits sensor serial interface and sets the control pin of RS-485 (TX_EN)
  @param status Sets TX_EN (High=TX || LOW=RX)
*/
void sensorInit(int status) {
  SerialSensor.begin(SENSOR_BAUDRATE);
  pinMode(TX_EN_PIN, OUTPUT);       // DE/RE Controling pin of RS-485 (TX_EN)
  delay(20);
  digitalWrite(TX_EN_PIN, status);     // DE/RE==LOW Receive Enabled
  if(USB_CONNECTED) { 
    SerialOut.println("Sensor initialized...");
  }
}

/**
  Verifies the checksum of the WV query response (Section 5.3.3 in FT702LT Manual)
  @param data 24 byte WV response
  @return (byte) 0 -> valid checksum
*/
byte verifyCkSum(char *data) {
  // Calculate packet checksum (XOR everything from '$' to '*')
  byte ckSum = data[1];
  for(int i=2; i<19; i++) {
    ckSum ^= data[i];
  }
  // Assemble checksum from data[20 - 21]
  byte recvCkSum = ((data[20]<<4) & data[21]);
  if(ckSum == recvCkSum) {
    return 0;
  }
  return 1;
}

void printWV(char data[]) {
  float wSpeed = (data[9]-48) * 10 + (data[10]-48) + (data[12]-48) * 0.1;
  float wDir   = (data[14]-48) * 100 + (data[15]-48) * 10 + (data[16]-48);
  SerialOut.print("Speed :");
  SerialOut.println(wSpeed, 1);
  SerialOut.print("Direction :");
  SerialOut.println(wDir);
  SerialOut.println("-------------");
}

void setFileName() {
  char* months[] = {"JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};
  int year = rtc.getYear();
  int month = rtc.getMonth();
  int day = rtc.getDay();
  char* month_str = months[month-1];
  sprintf(sdFileName, "%02d%s%02d.txt", day, month_str, year);
  SerialOut.print("sdFileName: ");
  SerialOut.println(sdFileName);   
}

uint32_t rtc_millis() {
  // uint8_t sec = rtc.getSeconds();
  // uint32_t millis = millis();
  char buf[7];
  // SerialOut.print("Millis Raw: ");
  // SerialOut.println(millis()%1000);
  sprintf(buf, "%02d.%03d", rtc.getSeconds(), (millis() % 1000));
  SerialOut.println(buf);
  return (rtc.getSeconds() * 1000) + (millis() % 1000);
}




void setup() {
  // Init RTC from compile time (__TIME__)
  int hour, minute, second;
  rtc.begin();
  sscanf(__TIME__, "%2d %*c %2d %*c %2d", &hour, &minute, &second);
  rtc.setTime(hour, minute, (second+8));
  rtc.setDate(15, 3, 23);

  FSM = BOOT; // Initial State
}

void loop() {
  RunFSM();
}
/**
  EVCS Research
  Name: SerialSensorPoll
  Purpose: Poll the FT702LT Wind Sesnor for wind speed and direction

  @author John Phillps
  @version 1.0 2/17/23

*/

#include <Arduino.h>

#define SerialOut       SerialUSB
#define SerialIn        Serial
#define USB_BAUDRATE    9600
#define SENSOR_BAUDRATE 9600
#define TX_EN_PIN       10

#define QWV_CMD         "$//,WV?*//\r\n"
#define WVQ_RSP_P       "$WI,WVP=020.0,045,0*//\r\n" // Polar -> len = 24


char data[8];

void setup() {
  while(!SerialOut);
  SerialOut.begin(USB_BAUDRATE);

  // SerialIn.begin(SENSOR_BAUDRATE);
  // pinMode(TX_EN_PIN, OUTPUT);       // DE/RE Controling pin of RS-485 (TX_EN)
  // delay(20);
  // digitalWrite(TX_EN_PIN, HIGH);     // DE/RE==LOW Receive Enabled
  // SerialOut.println("Sensor initialized...");
  sensorInit(HIGH);
  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
}

void loop() {
  float wSpeed;
  uint16_t wDir;
  char status;

  char data[24];
  int rlen;

  char buf[11];

  queryWV();
  
  // SerialOut.println("Bytes in serial buffer: " + String(SerialIn.available()));
  if(SerialIn.available()) {
    rlen = SerialIn.readBytes(data, 24);
  }
  if (rlen == 24) {
    // SerialOut.print(data);  // FIX: causes weird overflow in serial buffer for next print
    // SerialOut.flush();      // not fixed by flush()
    status = data[18];
    if(status == '0') {
      // sprintf(buf, "ERROR: %c\r\n", status);
      // SerialOut.print(buf);
         
      wSpeed = (data[9]-48) * 10 + (data[10]-48) + (data[12]-48) * 0.1;
      wDir   = (data[14]-48) * 100 + (data[15]-48) * 10 + (data[16]-48);
      SerialOut.print("Speed :");
      SerialOut.println(wSpeed, 1);
      SerialOut.print("Direction :");
      SerialOut.println(wDir);
      SerialOut.println("-------------");
    }
    // char buf[20];

    // // mem



    // sprintf(buf, "error bit: %c", data[18]);
    // // sprintf(buf, "error bit: %c, wind speed %03.1f m/s, wind direction: %03d", data[18])
    // SerialOut.println(buf);
    // SerialOut.flush();
  }
  delay(2000);
}

// Queries sensor for wind speed and direction
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
  SerialIn.print(cmd);   // Write WV Query CMD to sensor
  SerialIn.flush();                  // Waits for the transmission of outgoing serial data to complete
  delay(50);
  digitalWrite(TX_EN_PIN, LOW);     // Recieve Enabled
  delay(150);
}

/**
  Inits sensor serial interface and sets the control pin of RS-485 (TX_EN)

  @param status Sets TX_EN (High=TX || LOW=RX)
*/
void sensorInit(int status) {
  SerialIn.begin(SENSOR_BAUDRATE);
  pinMode(TX_EN_PIN, OUTPUT);       // DE/RE Controling pin of RS-485 (TX_EN)
  delay(20);
  digitalWrite(TX_EN_PIN, status);     // DE/RE==LOW Receive Enabled
  SerialOut.println("Sensor initialized...");
}

// unsigned long t_prev, t_now = 0;
// void get_windspeed() {
//   char data[26];
//   char ch;
//   int i = 0, index = 0;
//   float get_speed = 0.0;
//   float direction_datum = 0.0;
//   t_now = millis();
//   windSerial.write("$//,WV?*//\r\n");
//   //  delay(500);
// //  if ( t_now - t_prev < 500) {
//     while  ( windSerial.available())
//     {
//       ch = windSerial.read();
//       data[i++] = ch - 48; //convert ascii to numeric value
//     }
//     if ( data[18] == 0)   // Check error bit if 0 then no error
//     {
//       // extract speed from sensor data packet
//       get_speed = data[9] * 10 + data[10] + data[12] * 0.1;
//       direction_datum = data[14] * 100 + data[15] * 10 + data[16];
//       Serial.print("Speed :");
//       Serial.println(get_speed, 1);
//       Serial.print("Direction :");
//       Serial.println(direction_datum);
//             uint8_t packet[2 + sizeof(float) + sizeof(float)];
//             // uint8_t packet[2 + sizeof(long)];
//             int k = 0;
//             packet[0] = 1;
//             packet[1] = sizeof(float) + sizeof(float);
//             memcpy(&packet[2], &get_speed, sizeof(float));
//             memcpy(&packet[6], &direction_datum, sizeof(float));
// //            for (int  k = 0 ; k < 10 ; k ++) {
// //              mySerial.write(packet[k]);
// //              Serial.print(packet[k]);
// //            }
//     }
//     t_prev = t_now;
//   }

/**
  toggles LED_BUILTIN

  @param delay delay after LED toggle
*/
void toggleLED(int d) {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  delay(d);
}

/**
  clears SerialUSB   
*/
void serialOutFlush(){
  while(SerialOut.available() > 0) {
    char t = SerialOut.read();
  }
}


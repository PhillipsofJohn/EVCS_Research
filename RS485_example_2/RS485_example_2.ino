#include <Arduino.h>

#define SerialOut SerialUSB
#define SerialIn Serial
#define SENSOR_BAUDRATE 9600
#define USB_BAUDRATE    9600
#define TX_EN_PIN       7

// #define CUE_CMD         "$//,CUE00050*//\r\n"
int count;
char buffer[]

void setup() {
  setupTX();
}

void loop() {
  loopTX();
}

void setupTX() {
  while(!SerialOut);
  SerialOut.begin(USB_BAUDRATE);

  Serial.begin(SENSOR_BAUDRATE);
  pinMode(TX_EN_PIN, OUTPUT);       // DE/RE Controling pin of RS-485 (TX_EN)
  delay(10);
  digitalWrite(TX_EN_PIN, HIGH);     // DE/RE==LOW Receive Enabled

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  count = 0;
  SerialIn.println()
}

void loopTX() {

}

// void setupECHO() {
//   Serial.begin(SENSOR_BAUDRATE);
//   pinMode(TX_EN_PIN, OUTPUT);       // DE/RE Controling pin of RS-485 (TX_EN)
//   delay(10);
//   digitalWrite(TX_EN_PIN, LOW);     // DE/RE==LOW Receive Enabled

//   pinMode(LED_BUILTIN, OUTPUT);
//   digitalWrite(LED_BUILTIN, HIGH);
//   while(!SerialIn.available());
// }

// void loopECHO() {

// }
#include <Arduino.h>

#define SerialOut SerialUSB
#define SerialIn Serial
#define SENSOR_BAUDRATE 9600
#define TX_EN_PIN       7

int count;

void setup() {
  // setupRX();
  setupTX();
}

void loop() {
  // loopRX();
  loopTX();
}

// void setupRX() {
//   while(!SerialOut);
//   SerialOut.begin(9600);

//   SerialIn.begin(SENSOR_BAUDRATE);
//   pinMode(TX_EN_PIN, OUTPUT);       // DE/RE Controling pin of RS-485 (TX_EN)
//   delay(20);
//   digitalWrite(TX_EN_PIN, LOW);     // DE/RE==LOW Receive Enabled
//   SerialOut.println("Starting Serial...");

//   pinMode(LED_BUILTIN, OUTPUT);
//   digitalWrite(LED_BUILTIN, HIGH);
//   delay(1000);
// }

// void loopRX() {
//     char data[17];
//     int rlen = SerialIn.readBytes(data, 17); // read response
//     SerialOut.print("Data recv: ");
//     for(int i=0; i<rlen; i++) {
//       SerialOut.print(char(data[i]));
//     }
//     // SerialOut.print("\r\n");
//   // while(SerialIn.available()) {
//     // String s = SerialIn.readString();
//     // // int data = SerialIn.read();
//     // SerialOut.println(s);
//     digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
//     delay(100);
//   // }
//   // digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
//   // delay(1000);
// }

void setupTX() {
  SerialIn.begin(SENSOR_BAUDRATE);
  pinMode(TX_EN_PIN, OUTPUT);       // DE/RE Controling pin of RS-485 (TX_EN)
  delay(10);
  digitalWrite(TX_EN_PIN, HIGH);     // DE/RE==LOW Receive Enabled

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  count = 0;
  delay(1000);
}

void loopTX() {
  char buffer[20];
  if (count == 100) {
    count = 0;
  }
  sprintf(buffer, "$%02d,CUE00050*//\r\n", count);
  SerialIn.print(buffer);
  count++;
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  delay(200);
}

#include <Arduino.h>

#define SerialOut SerialUSB
#define SerialIn Serial
#define SENSOR_BAUDRATE 9600
#define TX_EN_PIN       7

char data[8];

void setup() {
  while(!SerialOut);
  SerialOut.begin(9600);

  pinMode(TX_EN_PIN, OUTPUT);       // DE/RE Controling pin of RS-485 (TX_EN)
  // digitalWrite(TX_EN_PIN, LOW);     // DE/RE==LOW Receive Enabled
  SerialIn.begin(SENSOR_BAUDRATE);
  SerialOut.println("Starting Serial...");

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  enableCU();
  // queryCU();
}

void loop() {
  int rlen = 0;
  if(SerialIn.available()) {
    rlen = SerialIn.readBytes(data, 8);
    digitalWrite(LED_BUILTIN, HIGH);
  }
  if(rlen > 0) {
    SerialOut.print("Data recv: ");
    for(int i=0; i<rlen; i++) {
      SerialOut.print(String(data[i]));
    }
    SerialOut.print("\r\n");
  }  
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
}

#define CUE_CMD         "$//,CUE00050*//\r\n"

// Sends SET CMD to sensor to enable Continuous updates on 100ms intervals
void enableCU() {
  digitalWrite(TX_EN_PIN, HIGH);     // DE/RE==HIGH Transmit Enabled
  delay(20);
  SerialIn.print(CUE_CMD);
  SerialIn.flush();                  // Waits for the transmission of outgoing serial data to complete
  delay(50);
  digitalWrite(TX_EN_PIN, LOW);
}

#define QCU_CMD         "$01,CU?*//\r\n"

void queryCU() {
  digitalWrite(TX_EN_PIN, HIGH);     // Transmit Enabled
  delay(50);
  SerialIn.write(QCU_CMD);          // Write Continuous Update Query CMD to sensor
  delay(500);
  digitalWrite(TX_EN_PIN, LOW);     // Recieve Enabled
  delay(50);
  if(!SerialIn.available()) {
    int rlen = SerialIn.readBytes(data, 8); // read response
    SerialOut.print("Data recv: ");
    for(int i=0; i<rlen; i++) {
      SerialOut.print(String(data[i]));
    }
    SerialOut.print("\r\n");
  } else {
    SerialOut.println("No query response");
  }
}

void toggleLED() {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  delay(200);
}


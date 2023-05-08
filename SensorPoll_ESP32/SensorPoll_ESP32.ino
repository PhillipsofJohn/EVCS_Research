/**
  EVCS Research
  Name: SensorPoll_ESP32
  Purpose: Poll the FT702LT Wind Sesnor for wind speed and direction
  Device: RocketScream Red Comet ESP32

  @author John Phillps
  @version 1.0 2/17/23

*/

#include <Arduino.h>

#define SerialOut       Serial
#define SerialIn        Serial1 
#define USB_BAUDRATE    9600
#define USB_BAUDRATE    115200
#define SENSOR_BAUDRATE 9600
#define TX_EN_PIN       3
#define RX_PIN          4         // GPIO 4 => RX for Serial1
#define TX_PIN          5         // GPIO 5 => TX for Serial1
#define LED_BUILTIN     2         // Yellow LED attached to DIO2

#define QWV_CMD         "$//,WV?*//\r\n"
#define WVQ_RSP_P       "$WI,WVP=020.0,045,0*//\r\n" // Polar -> len = 24


/**
 This is a sample callback function that will be activated on UART RX events
*/
void onReceiveFunction(void) {
  size_t available = SerialIn.available();
  SerialOut.printf("onReceive Callback:: There are %d bytes available: ", available);
  while (available --) {
    SerialOut.print((char)SerialIn.read());
  }
  SerialOut.println();
}

void sensorParse() {
  size_t available = SerialIn.available();
  while(available--) {
    if(((char)SerialIn.read() == '$') && (available > 13)) {
        char serialData[23];
        SerialIn.readBytes(serialData, 23);
        // for(int i=0; i<23; i++) {
        //   SerialOut.print(serialData[i]);
        // }
        if(serialData[17] == '0') {
          float wSpeed    = (serialData[8]-48) * 10 + (serialData[9]-48) + (serialData[11]-48) * 0.1;
          uint16_t wDir   = (serialData[13]-48) * 100 + (serialData[14]-48) * 10 + (serialData[15]-48);
          SerialOut.print("Speed :");
          SerialOut.println(wSpeed, 1);
          SerialOut.print("Direction :");
          SerialOut.println(wDir);
          SerialOut.println("-------------");
        }
      }
    }
  }

void setup() {
  Serial.begin(USB_BAUDRATE);
  sensorInit(HIGH);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
}

void loop() {
  queryWV();
  toggleLED(20);
}

/**
  Inits sensor serial interface and sets the control pin of RS-485 (TX_EN)

  @param status Sets TX_EN (High=TX || LOW=RX)
*/
void sensorInit(int status) {
  SerialIn.begin(SENSOR_BAUDRATE, SERIAL_8N1, RX_PIN, TX_PIN);
  SerialIn.setRxFIFOFull(120);
  SerialIn.onReceive(sensorParse);     // Set Serial1 Callback func
  pinMode(TX_EN_PIN, OUTPUT);          // DE/RE Controling pin of RS-485 (TX_EN)
  delay(20);
  digitalWrite(TX_EN_PIN, status);     // DE/RE==LOW Receive Enabled
  SerialOut.println("Sensor initialized...");
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
  SerialIn.print(cmd);               // Write CMD to sensor
  SerialIn.flush();                  // Waits for the transmission of outgoing serial data to complete
  delay(50);
  digitalWrite(TX_EN_PIN, LOW);     // Recieve Enabled
  delay(150);
}

/**
  toggles LED_BUILTIN

  @param delay delay after LED toggle
*/
void toggleLED(int d) {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  delay(d);
}
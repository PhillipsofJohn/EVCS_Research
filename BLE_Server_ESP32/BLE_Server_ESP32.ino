/**
  EVCS Research
  Name: BLE_Server_ESP32
  Purpose: Poll the FT702LT Wind Sesnor for wind speed and direction and send data via BLE
  Device: RocketScream Red Comet ESP32

  @author John Phillps
  @version 1.0 4/26/23

*/

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SerialOut       Serial
#define SerialIn        Serial1 
// #define USB_BAUDRATE    9600
#define USB_BAUDRATE    115200
#define SENSOR_BAUDRATE 9600
#define TX_EN_PIN       3
#define RX_PIN          4         // GPIO 4 => RX for Serial1
#define TX_PIN          5         // GPIO 5 => TX for Serial1
#define LED_BUILTIN     2         // Yellow LED attached to DIO2

#define QWV_CMD         "$//,WV?*//\r\n"
#define WVQ_RSP_P       "$WI,WVP=020.0,045,0*//\r\n" // Polar -> len = 24

#define S_EVCS_UUID       "181A"    // Environmental Sensing Service UUID
#define C_TEMP_UUID       "2A6E"    // Temperature Characteristic UUID
#define C_TEMP_UUID_L     "00002a6e-0000-1000-8000-00805f9b34fb"
#define C_WDIR_UUID       "2A71"    // True Wind Direction Characteristic UUID
#define C_WDIR_UUID_L     "00002a71-0000-1000-8000-00805f9b34fb"
#define C_WSPD_UUID       "2A70"    // True Wind Speed Characteristic UUID
#define C_WSPD_UUID_L     "00002a70-0000-1000-8000-00805f9b34fb"

BLECharacteristic *cTemp;
BLECharacteristic *cWdir;
BLECharacteristic *cWspd;
bool deviceConnected = false;


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
          uint16_t wSpeed_val = (serialData[8]-48) * 1000 + (serialData[9]-48) * 100 + (serialData[11]-48) * 10;
          uint16_t wDir_val   = (serialData[13]-48) * 10000 + (serialData[14]-48) * 1000 + (serialData[15]-48)*100;

          cWdir->setValue(wDir_val);
          cWspd->setValue(wSpeed_val);
          if(deviceConnected) {
            cWdir->notify();
            cWspd->notify();
          }

          // float wSpeed    = (serialData[8]-48) * 10 + (serialData[9]-48) + (serialData[11]-48) * 0.1;
          // uint16_t wDir   = (serialData[13]-48) * 100 + (serialData[14]-48) * 10 + (serialData[15]-48);
          // SerialOut.print("Speed :");
          // SerialOut.println(wSpeed, 1);
          // SerialOut.print("Direction :");
          // SerialOut.println(wDir);
          // SerialOut.println("-------------");
      }
    }
  }
}

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      delay(2000);
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onRead(BLECharacteristic *pCharacteristic) {
      BLEUUID UUID = pCharacteristic->getUUID();
      Serial.print("[READ] UUID:");
      // Serial.println(UUID.toString().c_str());
      if(UUID.toString() == C_TEMP_UUID_L) {
        Serial.print(C_TEMP_UUID);
        Serial.println(" (Temperature)");
      } else if(UUID.toString() == C_WDIR_UUID_L) {
        Serial.print(C_WDIR_UUID);
        Serial.println(" (Wind Direction)");
      } else if(UUID.toString() == C_WSPD_UUID_L) {
        Serial.print(C_WSPD_UUID);
        Serial.println(" (Wind Speed)");
      } else {
        Serial.print(UUID.toString().c_str());
        Serial.println(" (Unknown)");
      }
    }

    void onNotify(BLECharacteristic *pCharacteristic) {
      BLEUUID UUID = pCharacteristic->getUUID();
      Serial.print("[Notify] UUID:");
      // Serial.println(UUID.toString().c_str());
      if(UUID.toString() == C_TEMP_UUID_L) {
        Serial.print(C_TEMP_UUID);
        Serial.println(" (Temperature)");
      } else if(UUID.toString() == C_WDIR_UUID_L) {
        Serial.print(C_WDIR_UUID);
        Serial.println(" (Wind Direction)");
      } else if(UUID.toString() == C_WSPD_UUID_L) {
        Serial.print(C_WSPD_UUID);
        Serial.println(" (Wind Speed)");
      } else {
        Serial.print(UUID.toString().c_str());
        Serial.println(" (Unknown)");
      }
    }
};

void setup() {
  Serial.begin(USB_BAUDRATE);
  sensorInit(HIGH);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  BLEDevice::init("Katherine");
  BLEServer *bleServer = BLEDevice::createServer();
  bleServer->setCallbacks(new MyServerCallbacks());

  BLEService *sECVS = bleServer->createService(S_EVCS_UUID);

  cTemp = sECVS->createCharacteristic(
                                         C_TEMP_UUID,
                                         BLECharacteristic::PROPERTY_READ | 
                                         BLECharacteristic::PROPERTY_NOTIFY |
                                         BLECharacteristic::PROPERTY_BROADCAST
                                       );

  cTemp->setCallbacks(new MyCallbacks());          
  cTemp->setValue("0");
                           
  cWdir = sECVS->createCharacteristic(
                                         C_WDIR_UUID,
                                         BLECharacteristic::PROPERTY_READ | 
                                         BLECharacteristic::PROPERTY_NOTIFY |
                                         BLECharacteristic::PROPERTY_BROADCAST
                                       );
  cWdir->setCallbacks(new MyCallbacks());          
  cWdir->setValue("0");

  cWspd = sECVS->createCharacteristic(
                                         C_WSPD_UUID,
                                         BLECharacteristic::PROPERTY_READ | 
                                         BLECharacteristic::PROPERTY_NOTIFY |
                                         BLECharacteristic::PROPERTY_BROADCAST
                                       );
  cWspd->setCallbacks(new MyCallbacks());          
  cWspd->setValue("0");                                                                                                                   

  sECVS->start();

  BLEAdvertising *bleAdvertise = bleServer->getAdvertising();
  bleAdvertise->start();
  Serial.println("Waiting a client connection to notify...");

}

void loop() {
  queryWV();
  toggleLED(100);
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

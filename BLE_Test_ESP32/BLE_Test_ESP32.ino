/**
  EVCS Research
  Name: BLE_Test_ESP32
  Purpose: Send data via BLE GATT
  Device: RocketScream Red Comet ESP32

  @author John Phillps
  @version 1.0 4/26/23

*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define USB_BAUDRATE      115200
#define LED_BUILTIN       2           // Yellow LED attached to DIO2

#define S_EVCS_UUID       "181A"    // Environmental Sensing Service UUID
#define C_TEMP_UUID       "2A6E"    // Temperature Characteristic UUID
#define C_WDIR_UUID       "2A71"    // True Wind Direction Characteristic UUID
#define C_WSPD_UUID       "2A70"    // True Wind Speed Characteristic UUID

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"


// class MyCallbacks: public BLECharacteristicCallbacks {
//     void onWrite(BLECharacteristic *pCharacteristic) {
//       std::string value = pCharacteristic->getValue();

//       if (value.length() > 0) {
//         Serial.println("*********");
//         Serial.print("New value: ");
//         for (int i = 0; i < value.length(); i++)
//           Serial.print(value[i]);

//         Serial.println();
//         Serial.println("*********");
//       }
//     }

//     void onRead(BLECharacteristic *pCharacteristic) {
//       BLEUUID UUID = pCharacteristic->getUUID();
//       Serial.println("*********");
//       Serial.println("UUID:");
//       Serial.println("*********");
//     }
// };

void setup() {
  Serial.begin(115200);

  Serial.println("1- Download and install an BLE scanner app in your phone");
  Serial.println("2- Scan for BLE devices in the app");
  Serial.println("3- Connect to MyESP32");
  Serial.println("4- Go to CUSTOM CHARACTERISTIC in CUSTOM SERVICE and write something");
  Serial.println("5- See the magic =)");

  BLEDevice::init("MyESP32");
  Serial.println("after init");
  BLEServer *pServer = BLEDevice::createServer();
  Serial.println("after createServer()");

  BLEService *pService = pServer->createService(S_EVCS_UUID);
  Serial.println("aftercreateService");

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         C_WSPD_UUID,
                                         BLECharacteristic::PROPERTY_READ
                                       );
  Serial.println("after char");

  // pCharacteristic->setCallbacks(new MyCallbacks());

  pCharacteristic->setValue("Hello World");
  Serial.println("after setValue");

  pService->start();
  Serial.println("after servieStart");


  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
  Serial.println("after advertising start");
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
}
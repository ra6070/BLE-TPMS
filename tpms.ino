// TPMS BLE ESP32
// 2020 RA6070
// v0.2 06/08/20
//
// TPMS BLE "manufacturer data" format
// "000180eaca108a78e36d0000e60a00005b00"
//  0001                                    Manufacturer (0001: TomTom)
//      80                                  Sensor Number (80:1, 81:2, 82:3, 83:4, ..)
//      80eaca108a78                        Sensor Address
//                  e36d0000                Pressure
//                          e60a0000        Temperature
//                                  5b      Battery percentage
//                                    00    Alarm Flag (00: OK, 01: No Pressure Alarm)
//
// How calculate Sensor Address:            (Sensor number):EA:CA:(Code binding reported in the leaflet) - i.e. 80:EA:CA:10:8A:78


// BLE Service
#include "BLEDevice.h"
BLEScan* pBLEScan;
BLEClient*  pClient;

// Variables
static BLEAddress *pServerAddress;
// TPMS BLE SENSORS known addresses
String knownAddresses[] = { "80:ea:ca:10:8a:78" , "81:ea:ca:20:89:e5"};   

static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
    //Serial.print("Notify callback for characteristic ");
    //Serial.print(pBLERemoteCharacteristic->getUUID().toString().c_str());
    //Serial.print(" of data length ");
    //Serial.println(length);
  }
  class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice Device){
      //Serial.print("BLE Advertised Device found: ");
      //Serial.println(Device.toString().c_str());
      pServerAddress = new BLEAddress(Device.getAddress());
      bool known = false;
      bool Master = false;
      String ManufData = Device.toString().c_str();
      for (int i = 0; i < (sizeof(knownAddresses) / sizeof(knownAddresses[0])); i++) {
        if (strcmp(pServerAddress->toString().c_str(), knownAddresses[i].c_str()) == 0)
          known = true;
      }
      if (known) {
        String instring=retmanData(ManufData, 0); 
        Serial.println(instring);
        Serial.print("Device found: ");
        Serial.println(Device.getRSSI());
        // Tire Temperature in C°
        Serial.print("Temperature: ");
        Serial.print(returnData(instring,12)/100.0);
        Serial.println("C°");
        // Tire pressure in Kpa           
        Serial.print("Pressure:    ");
        Serial.print(returnData(instring,8)/1000.0);
        Serial.println("Kpa");
        // Tire pressure in Bar           
        Serial.print("Pressure:    ");
        Serial.print(returnData(instring,8)/100000.0);
        Serial.println("bar");
        // Battery percentage             
        Serial.print("Battery:     ");
        Serial.print(returnBatt(instring));
        Serial.println("%");
        if (returnAlarm(instring)) {
          Serial.println("ALARM!");
        }
        Serial.println("");
        Device.getScan()->stop();
        delay(100);
      }
    }
};

void setup() {
  // Opening serial port
  Serial.begin(115200);
  delay(100);
  // BLE Init
  Serial.print("Init BLE. ");
  BLEDevice::init("");
  pClient  = BLEDevice::createClient();
  pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);
  Serial.println("Done");
}

void loop() {
  BLEScanResults scanResults = pBLEScan->start(5);
}

// FUNCTIONS 

String retmanData(String txt, int shift) {
  // Return only manufacturer data string
  int start=txt.indexOf("data: ")+6+shift;
  return txt.substring(start,start+(36-shift));  
}

byte retByte(String Data,int start) {
  // Return a single byte from string
  int sp=(start)*2;
  char *ptr;
  return strtoul(Data.substring(sp,sp+2).c_str(),&ptr, 16);
}

long returnData(String Data,int start) {
  // Return a long value with little endian conversion
  return retByte(Data,start)|retByte(Data,start+1)<<8|retByte(Data,start+2)<<16|retByte(Data,start+3)<<24;
}

int returnBatt(String Data) {
  // Return battery percentage
  return retByte(Data,16);
}

int returnAlarm(String Data) {
  // Return battery percentage
  return retByte(Data,17);
}

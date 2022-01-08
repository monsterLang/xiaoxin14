///*
//  func1 scan example
//*/
//
///*
//   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
//   Ported to Arduino ESP32 by Evandro Copercini
//*/
//
//#include <BLEDevice.h>
//#include <BLEUtils.h>
//#include <BLEScan.h>
//#include <BLEAdvertisedDevice.h>
//
//int scanTime = 5; //In seconds
//BLEScan* pBLEScan;
//
//class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
//    void onResult(BLEAdvertisedDevice advertisedDevice) {
//      Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
//    }
//};
//
//void setup() {
//  Serial.begin(115200);
//  Serial.println("Scanning...");
//
//  BLEDevice::init("");
//  pBLEScan = BLEDevice::getScan(); //create new scan
//  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
//  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
//  pBLEScan->setInterval(100);
//  pBLEScan->setWindow(99);  // less or equal setInterval value
//}
//
//void loop() {
//  // put your main code here, to run repeatedly:
//  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
//  Serial.print("Devices found: ");
//  Serial.println(foundDevices.getCount());
//  Serial.println("Scan done!");
//  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
//  delay(2000);
//}

///*
//  func2 scan example + keyB to start scan
//*/
//
///*
//   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
//   Ported to Arduino ESP32 by Evandro Copercini
//*/
//
//#include <BLEDevice.h>
//#include <BLEUtils.h>
//#include <BLEScan.h>
//#include <BLEAdvertisedDevice.h>
//#include <Arduino.h>
//#include <Adafruit_NeoPixel.h>
//
//const int buttonPin = 2;     // the number of the pushbutton pin
//bool keyB_status = false;
//int buttonState = 0;  
//Adafruit_NeoPixel pixels(3,17, NEO_GRB + NEO_KHZ800);
//
//int scanTime = 5; //In seconds
//BLEScan* pBLEScan;
//
//class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
//    void onResult(BLEAdvertisedDevice advertisedDevice) {
//      Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
//    }
//};
//
//void led_on(){
//  pixels.setPixelColor(0, pixels.Color(10, 0, 0));
//  pixels.show();
//  Serial.println("led_on");
//}
//
//void led_off(){
//  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
//  pixels.show();
//  Serial.println("led_off");
//}
//
//void start_scan(){
//  //开始检索附件的蓝牙设备
//  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
//  Serial.print("Devices found: ");
//  //找到蓝牙设备数目
//  Serial.println(foundDevices.getCount());
//  Serial.println("Scan done!");
//  //清空存储的蓝牙设备信息
//  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
//}
//
//void setup() {
//  Serial.begin(115200);
//  Serial.println("Scanning...");
//
//  pinMode(buttonPin, INPUT);
//
//  BLEDevice::init("esp32-zhangkongban");
//  pBLEScan = BLEDevice::getScan(); //create new scan
//  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
//  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
//  pBLEScan->setInterval(100);
//  pBLEScan->setWindow(99);  // less or equal setInterval value
//  Serial.println("end setup");
//}
//
//void loop() {
//  Serial.println("in loop");
//  // put your main code here, to run repeatedly:
//  buttonState = digitalRead(buttonPin);
//  Serial.println(buttonState);
//  
//  if (buttonState == LOW) {
//    delay(500);
//    if(keyB_status == false){
//      led_on();
//      start_scan();
//      keyB_status = true;
//    }
//    else
//    {
//      led_off();
//      keyB_status = false;
//    }
//  }
//
//  delay(1000);
//  Serial.println("out loop");
//}

/*
  func3 scan example + keyB to start scan + display ble device which can be connected
*/

/*
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
   Ported to Arduino ESP32 by Evandro Copercini
*/
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

//add display
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SH1106Wire.h"        // legacy: #include "SSD1306.h"
SH1106Wire display(0x3c, 23, 22); 

const int buttonPin = 2;     // the number of the pushbutton pin
bool keyB_status = false;
int buttonState = 0;  
Adafruit_NeoPixel pixels(3,17, NEO_GRB + NEO_KHZ800);

int scanTime = 5; //In seconds
BLEScan* pBLEScan;

String str1 = "ble_device:";

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
      //Serial.printf("Advertised Device name: %s \n",advertisedDevice.getName().c_str());
      str1.concat(advertisedDevice.getName().c_str());
      
    }
};

void led_on(){
  pixels.setPixelColor(0, pixels.Color(10, 0, 0));
  pixels.show();

  display.clear();
  display.drawString(0, 0, "light on");
  display.display();
  
  Serial.println("led_on");
}

void led_off(){
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.show();

  display.clear();
  display.drawString(0, 0, "light off");
  display.display();
  
  Serial.println("led_off");
}

void start_scan(){
  //开始检索附件的蓝牙设备
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.print("Devices found: ");
  //找到蓝牙设备数目
  Serial.println(foundDevices.getCount());
  Serial.println("Scan done!");
  //清空存储的蓝牙设备信息
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
}

void setup() {
  Serial.begin(115200);
  Serial.println("Scanning...");

  //init display
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

  pinMode(buttonPin, INPUT);

  BLEDevice::init("esp32-zhangkongban");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value
  Serial.println("end setup");
}

void loop() {
  Serial.println("in loop");
  // put your main code here, to run repeatedly:
  buttonState = digitalRead(buttonPin);
  Serial.println(buttonState);
  
  if (buttonState == LOW) {
    delay(500);
    if(keyB_status == false){
      led_on();
      start_scan();
      keyB_status = true;


      //display.clear();
      display.drawString(0, 16, str1);
      display.display();
      delay(500);
    }
    else
    {
      led_off();
      keyB_status = false;
    }
  }

  delay(1000);
  Serial.println("out loop");
}

//
///*
//  func4 bt_classic_device_discovery.ino
//  fail
//*/
//
///*
//   https://github.com/espressif/arduino-esp32/blob/master/libraries/BluetoothSerial/examples/bt_classic_device_discovery/bt_classic_device_discovery.ino
//*/
//
//#include <BluetoothSerial.h>
//
//#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
//#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
//#endif
//
//BluetoothSerial SerialBT;
//
//
//#define BT_DISCOVER_TIME  10000
//
//
//static bool btScanAsync = true;
//static bool btScanSync = true;
//
//
//void btAdvertisedDeviceFound(BTAdvertisedDevice* pDevice) {
//  Serial.printf("Found a device asynchronously: %s\n", pDevice->toString().c_str());
//}
//
//void setup() {
//  Serial.begin(115200);
//  SerialBT.begin("ESP32test"); //Bluetooth device name
//  Serial.println("The device started, now you can pair it with bluetooth!");
//
//
//  if (btScanAsync) {
//    Serial.print("Starting discoverAsync...");
//    if (SerialBT.discoverAsync(btAdvertisedDeviceFound)) {
//      Serial.println("Findings will be reported in \"btAdvertisedDeviceFound\"");
//      delay(10000);
//      Serial.print("Stopping discoverAsync... ");
//      SerialBT.discoverAsyncStop();
//      Serial.println("stopped");
//    } else {
//      Serial.println("Error on discoverAsync f.e. not workin after a \"connect\"");
//    }
//  }
//  
//  if (btScanSync) {
//    Serial.println("Starting discover...");
//    BTScanResults *pResults = SerialBT.discover(BT_DISCOVER_TIME);
//    if (pResults)
//      pResults->dump(&Serial);
//    else
//      Serial.println("Error on BT Scan, no result!");
//  }
//}
//
//void loop() {
//  delay(100);
//}

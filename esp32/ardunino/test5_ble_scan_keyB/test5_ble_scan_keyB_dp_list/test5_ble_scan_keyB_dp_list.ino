/*
  func5.3 display scrolling list
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <string.h>

#define MAX_LIST_NUM 70
#define MAX_DISPLAY_NUM 2   // can display 3 line

//add display
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SH1106Wire.h"        // legacy: #include "SSD1306.h"
SH1106Wire display(0x3c, 23, 22); 

String device_list = "A|B|C|D|E|F|G";
String device_list_array[MAX_LIST_NUM] = {"A","B","C","D","E","F"};
String device_list_array_ble[MAX_LIST_NUM] = {};
String temp_string = "";

int status_dp_list_num = 0;

int status_array_num_current = 0;
int status_array_num_max = 6;

bool status_scan_result = false;
String status_scan_result_dp = "";
int status_scan_device_num = 0;

const int buttonPin = 2;     // the number of the pushbutton pin
bool keyB_status = false;
int buttonState = 0;  
Adafruit_NeoPixel pixels(3,17, NEO_GRB + NEO_KHZ800);

int scanTime = 5; //In seconds
BLEScan* pBLEScan;

//String str1 = "ble_device:";

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
      //Serial.printf("Advertised Device name: %s \n",advertisedDevice.getName().c_str());
//      str1.concat(advertisedDevice.getName().c_str());

      if (status_scan_device_num < MAX_LIST_NUM){
          device_list_array_ble[status_scan_device_num] = advertisedDevice.getName().c_str();
          status_scan_device_num ++;
      }

    }
};

void led_on(){
  pixels.setPixelColor(0, pixels.Color(10, 0, 0));
  pixels.show();

  status_scan_result_dp = ""; 

//  Serial.println(status_scan_result_dp);
  status_scan_result_dp.concat("scaning_");
  display.drawString(0, 0, status_scan_result_dp);
  display.display();
//  Serial.println(status_scan_result_dp);

//  status_scan_result_dp = "";
//  status_scan_result_dp.concat("light on _");
//  display.clear();
//  display.drawString(0, 0, status_scan_result_dp);
//  display.display();  

//  display.drawString(0, 0, "light on");
//  display.display();
  
  Serial.println("led_on");
}

void led_off(){
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.show();

//  Serial.println(status_scan_result_dp);
  status_scan_result_dp.replace("ing","end");
  status_scan_result_dp.concat(status_array_num_max);
  display.drawString(0, 0, status_scan_result_dp);
  display.display();
//  Serial.println(status_scan_result_dp);

//  status_scan_result_dp.concat("light off_");
//  Serial.println(status_scan_result_dp);
//  status_scan_result_dp.setCharAt(7,'f');
//  status_scan_result_dp.replace("on ","off");
//  Serial.println(status_scan_result_dp);

//  display.clear();
//  display.drawString(0, 0, "light off");
//  display.display();

//  display.clear();
//  display.drawString(0, 0, status_scan_result_dp);
//  display.display();
  
  Serial.println("led_off");
}

void start_scan(){
  Serial.println("in scan");
  status_scan_device_num = 0;

  // clear devices name list 

  //开始检索附件的蓝牙设备
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  Serial.print("Devices found: ");
  //找到蓝牙设备数目
  Serial.println(foundDevices.getCount());
  Serial.println("Scan done!");
  //清空存储的蓝牙设备信息
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory

  // save scan device num and set scan status true
  status_array_num_max = foundDevices.getCount();
  status_scan_result = true;

//  Serial.println(status_array_num_max);
//  Serial.print("status_scan_result ");
//  Serial.println(status_scan_result);
  Serial.println("out scan");
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
//  Serial.println("in loop");

  //1. get keyB for scan ble device
  buttonState = digitalRead(buttonPin);
//  Serial.println(buttonState);
  
  if (buttonState == LOW) {
    delay(500);
    if(keyB_status == false){
      
      led_on();
      keyB_status = true;
      
      start_scan();
      
      led_off();
      keyB_status = false;
      
    }
    else
    {
      led_off();
      keyB_status = false;
    }
  }

  //2. judge scan status for display ble devices which can be connected
  if (status_scan_result == true){

    Serial.println("display ble list");
    
    display.clear();
    display.drawString(0, 0, status_scan_result_dp);
    display.display();
  
    Serial.print("current: ");
    Serial.println(status_array_num_current);
    
    for (int temp=0; MAX_DISPLAY_NUM+1 > temp ; temp++){
      //to_string(status_array_num_current)+
      temp_string = String(status_array_num_current+temp)+" "+device_list_array_ble[status_array_num_current + temp];
      Serial.println(temp_string);
      display.drawString(0, 16+16*temp, temp_string);
      display.display();
    }
    
    delay(1000);
  
    status_array_num_current = status_array_num_current+1;
    if (status_array_num_current >= status_array_num_max-MAX_DISPLAY_NUM)
      status_array_num_current = 0;
    
  }

//  delay(1000);
//  Serial.println("out loop");
}

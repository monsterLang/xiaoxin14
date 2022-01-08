/*
  func3.2 display scrolling list
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define MAX_LIST_NUM 15
#define MAX_DISPLAY_NUM 2   // can display 3 line

//add display
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SH1106Wire.h"        // legacy: #include "SSD1306.h"
SH1106Wire display(0x3c, 23, 22); 

String device_list = "A|B|C|D|E|F|G";
String device_list_array[MAX_LIST_NUM] = {"A","B","C","D","E","F"};
String temp_string = "";

int status_dp_list_num = 0;

int status_array_num_current = 0;
int status_array_num_max = 6;

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


//  pinMode(buttonPin, INPUT);
//
//  BLEDevice::init("esp32-zhangkongban");
//  pBLEScan = BLEDevice::getScan(); //create new scan
//  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
//  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
//  pBLEScan->setInterval(100);
//  pBLEScan->setWindow(99);  // less or equal setInterval value
//  Serial.println("end setup");
}

void loop() {
  Serial.println("in loop");

  display.clear();
  display.drawString(0, 0, "light on");
  display.display();

  Serial.print("current: ");
  Serial.println(status_array_num_current);
  
  for (int temp=0; MAX_DISPLAY_NUM+1 > temp ; temp++){
    //to_string(status_array_num_current)+
    temp_string = String(status_array_num_current+temp)+" "+device_list_array[status_array_num_current + temp];
    Serial.println(temp_string);
    display.drawString(0, 16+16*temp, temp_string);
    display.display();
  }
  
  
  delay(1000);

  status_array_num_current = status_array_num_current+1;
  if (status_array_num_current >= status_array_num_max-MAX_DISPLAY_NUM)
    status_array_num_current = 0;
  
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
//
//
//      //display.clear();
//      display.drawString(0, 16, str1);
//      display.display();
//      delay(500);
//    }
//    else
//    {
//      led_off();
//      keyB_status = false;
//    }
//  }

//  delay(1000);
  Serial.println("out loop");
}

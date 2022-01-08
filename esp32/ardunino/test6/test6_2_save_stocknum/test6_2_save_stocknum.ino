#include <Arduino.h>
#include<string>

// temp
String stock_num = "";
String temp_char = "0";
int temp_int = 0;
int temp_num = 0;

// keyA
const int buttonPin = 0;     // key A ---- the number of the pushbutton pin
bool keyA_status = false;  // in input status
int buttonState = 0; 


// display
#include "SH1106Wire.h"
SH1106Wire display(0x3c, 23, 22);   
int16_t line = 1;


#define MAX_LIST_NUM 70
#define MAX_DISPLAY_NUM 2   // can display 2 line

int status_array_num_current = 0; // now show which stock num
int status_array_num_max = 0;     // how many of stock num
bool status_display_next = false; // can display next 2 stock num

#define STOCK_LONG 2    // length of stock num , if stock num has 6 numbers, set 5

String device_list_array[MAX_LIST_NUM] = {};
//int num_current_save_stock = 0;

int delaytime =0;
#define DELAY_TIME_STAY 15  // roll delay

String temp_string = "";

// gsensor
#include <Adafruit_MSA301.h>
Adafruit_MSA301 msa;

struct msa300_value
{
  int16_t x;
  int16_t y;
  int16_t z;
};
msa300_value p_stabe;  // use to save calibrated msa300 value
msa300_value p_old;
msa300_value p_new;

void msa300_init() {
  Serial.println("Adafruit MSA301 test!");
  // Try to initialize!
  //38,msa301_wire
  if (! msa.begin()) {
    Serial.println("Failed to find MSA301 chip");
    while (1) { delay(10); }
  }
  Serial.println("MSA301 Found!");
}

void print_msa300_value(msa300_value p)
{
  Serial.print("X:  "); Serial.print(p.x); 
  Serial.print("  \tY:  "); Serial.print(p.y); 
  Serial.print("  \tZ:  "); Serial.print(p.z);
  Serial.println();

}

void get_msa300(){
  //get msa xyz value
  msa.read();      // get X Y and Z data at once
  
//  // Then print out the raw data
//  Serial.print("X:  "); Serial.print(msa.x); 
//  Serial.print("  \tY:  "); Serial.print(msa.y); 
//  Serial.print("  \tZ:  "); Serial.print(msa.z); 
//
//  Serial.println();

  p_old.x = p_new.x;
  p_old.y = p_new.y;
  p_old.z = p_new.z;

  p_new.x = msa.x;
  p_new.y = msa.y;
  p_new.z = msa.z;

//  Serial.print("new value: ");
//  print_msa300_value(p_new);
//  Serial.print("old value: ");
//  print_msa300_value(p_old);

  sensors_event_t event; 
  msa.getEvent(&event);
  
//  /* Display the results (acceleration is measured in m/s^2) */
//  Serial.print("\t\tX: "); Serial.print(event.acceleration.x);
//  Serial.print(" \tY: "); Serial.print(event.acceleration.y); 
//  Serial.print(" \tZ: "); Serial.print(event.acceleration.z); 
//  Serial.println(" m/s^2 ");

//  Serial.print("Z: "); Serial.print(event.acceleration.z); 
//  Serial.println();

  // display now char
  temp_int = int(abs(event.acceleration.z));
  if (temp_int > 9)
  {
    Serial.print("temp_int: ");
    Serial.println(temp_int);
    temp_int = 0;
  }
    
  temp_char = String(temp_int);
//  Serial.println(temp_char);
  
  
  display.clear_line(2);
  display.drawString(0, 0, "touch light to choose word");
  display.drawString(0, 16, stock_num);
  display.drawString(temp_num*6+6, 16, "<");
  display.drawString(temp_num*6, 16, temp_char);
  display.display();


//  x_old = x_new;
//  y_old = y_new;
//  z_old = z_new;
//  x_new = msa.x;
//  y_new = msa.y;
//  z_new = msa.z;
//  Serial.print(x_old);Serial.print("  \t");
//  Serial.print(x_new);Serial.print(";  \t");
//  Serial.print(y_old);Serial.print("  \t");
//  Serial.print(y_new);Serial.print(";  \t");
//  Serial.print(z_old);Serial.print("  \t");
//  Serial.print(z_new);Serial.print(";  \t");
//  Serial.println();
}

// light
# define A_LIGHT 39
int sensorValue;
bool status_light = false;

void get_light_setstring()
{
  sensorValue = analogRead(A_LIGHT);
//  Serial.print("sensorValue: ");
//  Serial.println(sensorValue);
  if (sensorValue == 0)
  {
    Serial.print("choose char: ");
    Serial.println(temp_char);
    
    if (temp_num > STOCK_LONG -1)
    {
      exit_input_status();
    }
    else if(temp_num == STOCK_LONG -1)
    {
      stock_num.concat(temp_char);
      exit_input_status();
    }
    else
      stock_num.concat(temp_char);
      
    temp_num = temp_num+1;

    // choose next word
    delay(1500);
  }
}

void display_stock_num()
{
  display.clear_line(line);
  display.drawString(0, 0, "Press keyA to input num");
  display.clear_line_num(1);
  display.display();
}

void enter_input_status()
{
  Serial.println("enter_input_status");
  keyA_status = true;
  status_light = true;
  temp_num = 0;
}

void exit_input_status()
{
  Serial.println("exit_input_status");
  keyA_status = false;
  status_light = false;
  temp_num = 0;
  display_stock_num();

  // save stock num
  if (status_array_num_max < MAX_LIST_NUM){
      device_list_array[status_array_num_max] = stock_num;
      status_array_num_max ++;
  }
}


void display_2line()
{
  if(status_array_num_max == 0)
    return;

  Serial.print("status_array_num_current\t");
  Serial.println(status_array_num_current);

//  if ((status_array_num_current == status_array_num_max-1)&&(status_array_num_max > 2)  )
//  {
//    // clear 3/4 line
//    display.clear_line_num(2);
//    display.clear_line_num(3);
//    temp_string = String(status_array_num_max-1)+" "+ device_list_array[status_array_num_max-1];
//    display.drawString(0, 16*2, temp_string);
//    temp_string = String(0)+" "+ device_list_array[0];
//    display.drawString(0, 16*3, temp_string);
//    display.display();
//
//    delay(1500);
//
//    status_array_num_current = -1;
//  }
//  else
//  {


    // show now display 2 lines stock num in 3/4 line
    for (int temp=0; MAX_DISPLAY_NUM > temp ; temp++){
      //to_string(status_array_num_current)+

      int a = status_array_num_current + temp;
      if((a == status_array_num_max-1)&&(status_array_num_max <2))
        continue;
//        a = status_array_num_max-1;
      if(a == status_array_num_max)
        a =0;
      temp_string = String(a)+" "+ device_list_array[a];

      
//      temp_string = String(status_array_num_current + temp)+" "+ device_list_array[status_array_num_current + temp];
  //    Serial.print("temp_string:");
  //    Serial.println(temp_string);
      display.clear_line_num(temp+2);
      display.drawString(0, 32+16*temp, temp_string);
      display.display();

    }

    
//  }
  


  // judge delay time and set
  status_display_next = (delaytime / DELAY_TIME_STAY) > 0;
//  Serial.print("delaytime\t");
//  Serial.print(delaytime);
//  Serial.print("\tstatus_display_next");
//  Serial.println(status_display_next);
  if(status_display_next == true)
  {
//    Serial.println("enter status_display_next");
    status_array_num_current = status_array_num_current+1;
    status_display_next = false;
    delaytime =0;
  }
//  Serial.print("status_array_num_current\t");
//  Serial.println(status_array_num_current);
//  if (status_array_num_current >= status_array_num_max-MAX_DISPLAY_NUM+1)
//    status_array_num_current = 0;

  if(status_array_num_current == status_array_num_max )
    status_array_num_current = 0;

}


void setup() {
  Serial.begin(115200);
  Serial.println("enter setup");

  // init msa300
  msa300_init();
  
  // init display
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

  //display "Press keyA"
  display.clear();
  display.drawString(0, 0, "Press keyA to input num");
  display.display();

//  display "Press keyB"
//  display.clear();
//  display.drawString(0, 16, "Press keyB");
//  display.display();
//  display.clear_line(line);
//  display.display();

  Serial.println("exit setup");
}

void loop() {
  //1. read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  //check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == LOW) {
    delay(500);  //延迟除抖
    if(keyA_status == false){

      // enter input status
      Serial.println("keyA down, enter input status");
      stock_num = "";
      
      display.clear_line(line);
      display.drawString(0, 0, "overturn as AB axle");
//      display.drawString(6, 16, "<");
      display.display();

      delay(1500);
      
      // can get gsensor and choose char
      enter_input_status();
    }
    else
    {
      // exit input status
      Serial.println("keyA down, exit input status");
      exit_input_status();
    }
  }

  if(keyA_status == true)
    get_msa300();
  if(status_light == true)
    get_light_setstring();

  display_2line();

  
  delay(100);
  delaytime = delaytime + 1;
}

///*
//  func3.2 display scrolling list
//*/
//
//#include <BLEDevice.h>
//#include <BLEUtils.h>
//#include <BLEScan.h>
//#include <BLEAdvertisedDevice.h>
//#include <Arduino.h>
//#include <Adafruit_NeoPixel.h>
//
//#define MAX_LIST_NUM 15
//#define MAX_DISPLAY_NUM 2   // can display 3 line
//
////add display
//#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
//#include "SH1106Wire.h"        // legacy: #include "SSD1306.h"
//SH1106Wire display(0x3c, 23, 22); 
//
//String device_list = "A|B|C|D|E|F|G";
//String device_list_array[MAX_LIST_NUM] = {"A","B","C","D","E","F"};
//String temp_string = "";
//
//int status_dp_list_num = 0;
//
//int status_array_num_current = 0;
//int status_array_num_max = 6;
//
//void setup() {
//  Serial.begin(115200);
//  Serial.println("Scanning...");
//
//  //init display
//  display.init();
//  display.flipScreenVertically();
//  display.setFont(ArialMT_Plain_10);
//}
//
//void loop() {
//  Serial.println("in loop");
//
//  display.clear();
//  display.drawString(0, 0, "light on");
//  display.display();
// 
//  Serial.print("current: ");
//  Serial.println(status_array_num_current);
//
//    for (int temp=0; MAX_DISPLAY_NUM > temp ; temp++){
//      //to_string(status_array_num_current)+
//      temp_string = String(status_array_num_current + temp)+" "+ device_list_array[status_array_num_current + temp];
//      
//      Serial.println(temp_string);
//      display.drawString(0, 32+16*temp, temp_string);
//      display.display();
//    }
//  
//  delay(1000);
//
//  status_array_num_current = status_array_num_current+1;
//  if (status_array_num_current >= status_array_num_max-MAX_DISPLAY_NUM-1)
//    status_array_num_current = 0;
//  
//  Serial.println("out loop");
//}

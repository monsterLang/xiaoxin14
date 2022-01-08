#include <Arduino.h>
#include<string>

// led light
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel pixels(3,17, NEO_GRB + NEO_KHZ800);
const int ledPin =  13;      // the number of the LED pin


// temp
String stock_num = "";
String temp_char = "0";
int temp_int = 0;
int temp_num = 0;

// keyA
const int buttonPin = 0;     // key A ---- the number of the pushbutton pin
bool keyA_status = false;  // in input status
int buttonState = 0; 

//keyB
# define KEY_B 2 
bool keyB_status = false;  // in input status
int buttonState_B = 0; 

// display
#include "SH1106Wire.h"
SH1106Wire display(0x3c, 23, 22);   
int16_t line = 1;


#define MAX_LIST_NUM 70
#define MAX_DISPLAY_NUM 2   // can display 2 line

int status_array_num_current = 0; // now show which stock num
int status_array_num_max = 0;     // how many of stock num
bool status_display_next = false; // can display next 2 stock num

#define STOCK_LONG 9    // length of stock num , if stock num has 6 numbers, set 5

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

int16_t temp_compare = 0;
#define COMPARE_UPDOWN_LEVEL0 1
#define COMPARE_UPDOWN_LEVEL1 5
#define COMPARE_UPDOWN_LEVEL2 10
//#define COMPARE_UPDOWN_LEVEL3 9

#define JUDGE_UPDOWN_LEVEL0 150
#define JUDGE_UPDOWN_LEVEL1 600
#define JUDGE_UPDOWN_LEVEL2 1200
#define JUDGE_UPDOWN_LEVEL3 2000

#define DEVIATION_ANGLE1 700
#define DEVIATION_ANGLE2 1600

#define NUM_WORD 10+26*2+7
String array_word[] = {"0","1","2","3","4","5","6","7","8","9",
"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z",
"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z",
" ","_",",",".","?","!","*"};
int middle_num = 0;
int array_middle[] = {5,24,50};

void get_msa300(){
  //get msa xyz value
  msa.read();      // get X Y and Z data at once
  
//  // Then print out the raw data
//  Serial.print("X:  "); Serial.print(msa.x); 
//  Serial.print("  \tY:  "); Serial.print(msa.y); 
//  Serial.print("  \tZ:  "); Serial.print(msa.z); 
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


  //------------------------
//  for (int a = 0; a<10;a++)
//  {
//    Serial.print('"');
//    Serial.print(a);
//    Serial.print('"');
//    Serial.print(',');
//  }
//  for (char a = 'A'; a<='Z';a++)
//  {
//    Serial.print('"');
//    Serial.print(a);
//    Serial.print('"');
//    Serial.print(',');
//  }
  //------------------------

  //----------------------------------------------------{
  // func3: judge y minus , word plus

  Serial.print("y = :  \t");
  Serial.println(p_new.y);
    // | 0-防误触-500 | 500-加1-1500 | 1500-加5-2000 |
  int16_t symbol = 1;
  int16_t level_num = 0;
  if (p_new.y > 0)
    symbol = -1;  // left down
  else
    symbol = 1;   // right up
  level_num = abs(p_new.y);

  if (level_num < DEVIATION_ANGLE1)
  {
    //0-防误触-150
    Serial.println("move level small");
  }
  else if(level_num > DEVIATION_ANGLE1 && level_num < DEVIATION_ANGLE2)
  {
    //150-加1-600
    temp_int = temp_int + symbol * COMPARE_UPDOWN_LEVEL0;
  }
  else if(level_num > DEVIATION_ANGLE2 )
  {
    //600-加5-1200
    temp_int = temp_int + symbol * COMPARE_UPDOWN_LEVEL1;
  }


//  if(p_new.y > DEVIATION_ANGLE1 && p_new.y < DEVIATION_ANGLE2 )
//  {
//    temp_int++;
//  }
//  else if(p_new.y < -DEVIATION_ANGLE1)
//    temp_int--;


  if (temp_int < 0)
    temp_int = NUM_WORD - 1;
  else if (temp_int > NUM_WORD - 1)
    temp_int = 0;

  delay(200);
  temp_char = array_word[temp_int];
//  temp_char = String(temp_int);
  
  //}----------------------------------------------------

//  //----------------------------------------------------{
//  // func1: judge y acceleration to set 0-9
//  // y means board roll as axle wire
//  temp_compare = p_new.y - p_old.y ;
//  Serial.print("_y = :  \t");
//  Serial.print(temp_compare);
//
//  //================================={
//  Serial.print("\tY = ");
//  Serial.println(p_new.y);
//
//  
//  if(abs(p_new.y) > 2000 )
//  {
//    Serial.println("change line");
//    
//    if (middle_num < 3)
//    {
//      // 
//      temp_int = array_middle[middle_num];
//      middle_num++;
//    }
//    else
//      middle_num = 0;
//
//    delay(2000);    
//  }
//
//  //}=================================
//
//  
//  // | 0-防误触-150 | 150-加1-600 | 600-加5-1200 | 1200-加10-2000 |
//  int16_t symbol = 1;
//  int16_t level_num = 0;
//  if (temp_compare > 0)
//    symbol = 1;
//  else
//    symbol = -1;
//
//  level_num = abs(temp_compare);
//
//  if (level_num < JUDGE_UPDOWN_LEVEL0)
//  {
//    //0-防误触-150
//    Serial.println("move level small");
//  }
//  else if(level_num > JUDGE_UPDOWN_LEVEL0 && level_num < JUDGE_UPDOWN_LEVEL1)
//  {
//    //150-加1-600
//    temp_int = temp_int + symbol * COMPARE_UPDOWN_LEVEL0;
//  }
//  else if(level_num > JUDGE_UPDOWN_LEVEL1 && level_num < JUDGE_UPDOWN_LEVEL2)
//  {
//    //600-加5-1200
//    temp_int = temp_int + symbol * COMPARE_UPDOWN_LEVEL1;
//  }
//  else if(level_num > JUDGE_UPDOWN_LEVEL2 && level_num < JUDGE_UPDOWN_LEVEL3)
//  {
//    //1200-加10-2000
//    temp_int = temp_int + symbol * COMPARE_UPDOWN_LEVEL2;
//  }
//  
////  if (temp_compare > JUDGE_UPDOWN_LEVEL0)
////  {
////    if (temp_compare > JUDGE_UPDOWN_LEVEL1)
////    {
////      Serial.println("up large");
////      temp_int = temp_int + COMPARE_UPDOWN_LEVEL1;
////    }
////    else
////    {
////      Serial.println("up small");
////      temp_int = temp_int + COMPARE_UPDOWN_LEVEL0;
////    }
////
////  }
////  else if(temp_compare < -JUDGE_UPDOWN_LEVEL0)
////  {
////    if (temp_compare < -JUDGE_UPDOWN_LEVEL1)
////    {
////      Serial.println("down large");
////      temp_int = temp_int - COMPARE_UPDOWN_LEVEL1;
////    }
////    else
////    {
////      Serial.println("down small");
////      temp_int = temp_int - COMPARE_UPDOWN_LEVEL0;
////    }
////  }
//
//
//
//  
//
//  if (temp_int < 0)
//    temp_int = NUM_WORD - 1;
//  else if (temp_int > NUM_WORD - 1)
//    temp_int = 0;
//
//  delay(200);
//  temp_char = array_word[temp_int];
////  temp_char = String(temp_int);
//  
//  //}----------------------------------------------------

//  //----------------------------------------------------{
//  // func2: judge z acceleration to set 0-9
//  sensors_event_t event; 
//  msa.getEvent(&event);
//  
////  /* Display the results (acceleration is measured in m/s^2) */
////  Serial.print("\t\tX: "); Serial.print(event.acceleration.x);
////  Serial.print(" \tY: "); Serial.print(event.acceleration.y); 
////  Serial.print(" \tZ: "); Serial.print(event.acceleration.z); 
////  Serial.println(" m/s^2 ");
//
////  Serial.print("Z: "); Serial.print(event.acceleration.z); 
////  Serial.println();
//
//  // display now char
//  temp_int = int(abs(event.acceleration.z));
//  if (temp_int > 9)
//  {
//    Serial.print("temp_int: ");
//    Serial.println(temp_int);
//    temp_int = 0;
//  }
//    
////  Serial.println(temp_char);
//  temp_char = String(temp_int);
//  //}----------------------------------------------------

  
  
  display.clear_line(2);
//  display.drawString(0, 0, "touch light to choose word");
  display.drawString(0, 0, "touch keyB to choose word");
  display.drawString(0, 16, stock_num);
  display.drawString(temp_num*6+6, 16, "<");
//  display.drawString(temp_num*6, 16, temp_char);
  display.drawString(120, 16, temp_char);
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
bool status_check_word = false;

// keyB to check word
void get_keyB_setword()
{
  buttonState_B = digitalRead(KEY_B);
//  Serial.print("buttonState_B: ");
//  Serial.println(buttonState_B);

  if (buttonState_B == LOW) {
    delay(500);  //延迟除抖

    led_on();
    //
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


//    if(keyB_status == false){
//
//      keyB_status = true;
//    }
//    else
//    {
//      keyB_status = false;
//    }
  }


}

void led_on(){
  pixels.setPixelColor(0, pixels.Color(10, 0, 0));
  pixels.show();
  Serial.println("led_on");

  delay(500);
  led_off();
}
void led_off(){
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.show();
  Serial.println("led_off");
}

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
  status_check_word = true;
  temp_num = 0;
}

void exit_input_status()
{
  Serial.println("exit_input_status");
  keyA_status = false;
  status_check_word = false;
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

//  Serial.print("status_array_num_current\t");
//  Serial.println(status_array_num_current);

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

  // led
  pinMode(ledPin, OUTPUT);
  
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
      display.drawString(0, 0, "Tilt left or right");
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
  if(status_check_word == true)
    get_keyB_setword();
//    get_light_setstring();

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

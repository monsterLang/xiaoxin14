#include <Arduino.h>
#include<string>

#define MAX_LIST_NUM 70
#define MAX_DISPLAY_NUM 2   // can display 2 line
#define STOCK_LONG 2    // length of stock num , if stock num has 6 numbers, set 6


// pattern change

int buttonState_A = 0;
int pattern_num = -1;  // which pattern: 1. display weather/time 2. display picture 3. input word

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
# define KEY_A 0    // key A ---- the number of the pushbutton pin
bool keyA_status = false;  // in input status
int buttonState = 0; 

//keyB
# define KEY_B 2 
bool keyB_status = false;  // in input status
int buttonState_B = 0; 

bool status_enter_patter3 = false;

// display
#include "SH1106Wire.h"
SH1106Wire display(0x3c, 23, 22);   
int16_t line = 1;


int status_array_num_current = 0; // now show which stock num
int status_array_num_max = 0;     // how many of stock num
bool status_display_next = false; // can display next 2 stock num


String device_list_array[MAX_LIST_NUM] = {};
//int num_current_save_stock = 0;

int delaytime =0;
#define DELAY_TIME_STAY 15  // roll delay

String temp_string = "";

// gsensor
#include <Adafruit_MSA301.h>
Adafruit_MSA301 msa;

bool status_msa300_getvalue =  false;

struct msa300_value
{
  int16_t x;
  int16_t y;
  int16_t z;
};
msa300_value p_stabe;  // use to save calibrated msa300 value
msa300_value p_old;
msa300_value p_new;


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

#define NUM_WORD 10+26*2+8
String array_word[] = {"0","1","2","3","4","5","6","7","8","9",
"a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z",
"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z",
" ","_",",",".","?","!","*",":"};
int middle_num = 0;
int array_middle[] = {5,24,50};

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

void print_character()
{
  //for array_word[]
  for (int a = 0; a<10;a++)
  {
    Serial.print('"');
    Serial.print(a);
    Serial.print(',');
  }
  for (char a = 'A'; a<='Z';a++)
  {
    Serial.print('"');
    Serial.print(a);
    Serial.print('"');
    Serial.print(',');
  }
}

void choose_word_func1()
{
  //----------------------------------------------------{
  // func1: judge z acceleration(-10,10) to set 0-9
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
    
//  Serial.println(temp_char);
  temp_char = String(temp_int);
  //}----------------------------------------------------
}

void choose_word_func2()
{
  //----------------------------------------------------{
  // func2: judge the difference value between new.y and old.y to choose word
  // 1. if new.y > old.y choose next word (judge by compare)
  // 2. 
  // PS: y means board roll as axle wire
  //}----------------------------------------------------
  
  temp_compare = p_new.y - p_old.y ;
  Serial.print("_y = :  \t");
  Serial.print(temp_compare);

  //*************************{
  Serial.print("\tY = ");
  Serial.println(p_new.y);
  
  if(abs(p_new.y) > 2000 )
  {
    Serial.println("change line");
    
    if (middle_num < 3)
    {
      // 
      temp_int = array_middle[middle_num];
      middle_num++;
    }
    else
      middle_num = 0;

    delay(2000);    
  }

  //}*************************

  // | 0-prevent_contact-150 | 150-plus_1-600 | 600-plus_5-1200 | 1200-plus_10-2000 |
  int16_t symbol = 1;
  int16_t level_num = 0;
  if (temp_compare > 0)
    symbol = 1;
  else
    symbol = -1;

  level_num = abs(temp_compare);

  if (level_num < JUDGE_UPDOWN_LEVEL0)
  {
    //0-prevent_contact-150
//    Serial.println("move level small");
  }
  else if(level_num > JUDGE_UPDOWN_LEVEL0 && level_num < JUDGE_UPDOWN_LEVEL1)
  {
    //150-plus_1-600
    temp_int = temp_int + symbol * COMPARE_UPDOWN_LEVEL0;
  }
  else if(level_num > JUDGE_UPDOWN_LEVEL1 && level_num < JUDGE_UPDOWN_LEVEL2)
  {
    //600-plus_5-1200
    temp_int = temp_int + symbol * COMPARE_UPDOWN_LEVEL1;
  }
  else if(level_num > JUDGE_UPDOWN_LEVEL2 && level_num < JUDGE_UPDOWN_LEVEL3)
  {
    //1200-plus_10-2000
    temp_int = temp_int + symbol * COMPARE_UPDOWN_LEVEL2;
  }
  
//  if (temp_compare > JUDGE_UPDOWN_LEVEL0)
//  {
//    if (temp_compare > JUDGE_UPDOWN_LEVEL1)
//    {
//      Serial.println("up large");
//      temp_int = temp_int + COMPARE_UPDOWN_LEVEL1;
//    }
//    else
//    {
//      Serial.println("up small");
//      temp_int = temp_int + COMPARE_UPDOWN_LEVEL0;
//    }
//
//  }
//  else if(temp_compare < -JUDGE_UPDOWN_LEVEL0)
//  {
//    if (temp_compare < -JUDGE_UPDOWN_LEVEL1)
//    {
//      Serial.println("down large");
//      temp_int = temp_int - COMPARE_UPDOWN_LEVEL1;
//    }
//    else
//    {
//      Serial.println("down small");
//      temp_int = temp_int - COMPARE_UPDOWN_LEVEL0;
//    }
//  }

  if (temp_int < 0)
    temp_int = NUM_WORD - 1;
  else if (temp_int > NUM_WORD - 1)
    temp_int = 0;

  delay(200);
  temp_char = array_word[temp_int];
//  temp_char = String(temp_int);
  
}


void choose_word_func3()
{
  //----------------------------------------------------{
  // func3: judge y minus , word plus

//  Serial.print("y = :  \t");
//  Serial.println(p_new.y);
    // | 0-prevent_contact-500 | 500-plus_1-1500 | 1500-plus_5-2000 |
  int16_t symbol = 1;
  int16_t level_num = 0;
  if (p_new.y > 0)
    symbol = -1;  // left down
  else
    symbol = 1;   // right up
  level_num = abs(p_new.y);

  if (level_num < DEVIATION_ANGLE1)
  {
    //0-prevent_contact-150
//    Serial.println("move level small");
  }
  else if(level_num > DEVIATION_ANGLE1 && level_num < DEVIATION_ANGLE2)
  {
    //150-plus_1-600
    temp_int = temp_int + symbol * COMPARE_UPDOWN_LEVEL0;
  }
  else if(level_num > DEVIATION_ANGLE2 )
  {
    //600-plus_5-1200
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
}

void display_input_word()
{
  display.clear_line(2);
//  display.drawString(0, 0, "touch light to choose word");
  display.drawString(0, 0, "Press keyB to choose word");

  // input bit flay -- <
//  display.drawString(0, 16, stock_num);
//  display.drawString(temp_num*6+6, 16, "<");

  // input bit flay -- add "_"
  String temp_add_cursor = stock_num;
  temp_add_cursor.concat("_");
  display.drawString(0, 16, temp_add_cursor);
  
//  display.drawString(temp_num*6, 16, temp_char);
  display.drawString(120, 16, temp_char);
  display.display();
}

void save_old_new_xyz()
{
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
}

void get_msa300(){
  
  //get msa xyz value
  msa.read();      // get X Y and Z data at once

//  // Then print out the raw data
//  Serial.print("X:  "); Serial.print(msa.x); 
//  Serial.print("  \tY:  "); Serial.print(msa.y); 
//  Serial.print("  \tZ:  "); Serial.print(msa.z); 
//  Serial.println();

  // save value
  save_old_new_xyz();


  // display input string -- support 3 func choose word

  if (pattern_num == 2){
    choose_word_func3();
    display_input_word();
  }


}

// light
# define SENSOR_LIGHT 39
int sensorValue;
bool status_check_word = false;


// key 
int buttonState_temp = 0; 

bool status_display_line34 = false;

int keyB_down_num = 0;

// keyB func
// 1. enter input status 
// 2. choose word
// 3. exit input status
void check_keyB_status()
{
  Serial.print("keyB: ");
  Serial.println(keyB_down_num);
  if(keyB_down_num == 0)
  {
    //enter input status
    Serial.println("enter input status");
    enter_input_status();
    event_keyA();
//    status_msa300_getvalue = true;

    if(status_enter_patter3 == false)
    {
      status_enter_patter3 = true;
      status_display_line34 = true;
      display.clear();
      display.display();
    }
    
  }
  else if((keyB_down_num > 0) && (keyB_down_num < STOCK_LONG ) )
  {
    Serial.println("save char");
    save_char();
    led_on();
  }
  else if(keyB_down_num == STOCK_LONG )
  {
    // close input status
    Serial.println("exit input status");
    keyB_down_num = -1;
    
    led_on();
    save_char();
    save_word();
    
    exit_input_status();
    
  }
  else
    Serial.println("error keyA status");

//  if (keyB_down_num >= 0)
    keyB_down_num++;

//  print_array();
}



void long_press_key()
{
  Serial.println("long_press_key");
  keyA_status = false;        // disable msa300
  status_check_word = false;  // disable choose word
  status_display_line34 = false;  // disable line34
  temp_num = 0;               // recover word init status, initial value is '0'
  
  display.clear();
  display.drawString(0, 0, "change pattern");
  display.display();
  delay(2000);

  display.clear();
  display.display();
  delay(2000);

  // display picture
  

}

void key_down(char key_name)
{
  //default use keyA
  int key_X = 0;

  if (key_name == 'A')
    key_X = KEY_A;
  else if (key_name == 'B')
    key_X = KEY_B;
  else
    Serial.println("unknown key name");
  
  buttonState_temp = digitalRead(key_X);
  if(buttonState_temp == LOW){
    delay(2000);  //delay shake elimination

    buttonState_temp = digitalRead(key_X);
    if(buttonState_temp == LOW){
      delay(300); //delay shake elimination
      Serial.println("key X long press");
      long_press_key();
    }
    
    Serial.println("key X down");
  }
  else if(buttonState_temp == HIGH)
  {
    delay(1);
//    Serial.println("key X up");
  }
  else
    Serial.println("unkown key status");

}

// led
void led_on(){
  pixels.setPixelColor(2, pixels.Color(10, 0, 0, 10));
  pixels.show();
  Serial.println("led_on");

  delay(500);
  led_off();
}
void led_off(){
  pixels.setPixelColor(2, pixels.Color(0, 0, 0));
  pixels.show();
  Serial.println("led_off");
}


void setword1_light()
{
  sensorValue = analogRead(SENSOR_LIGHT);
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

// keyB to check word

void save_char()
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
//    delay(1500);
}

void setword2_keyB()
{
  buttonState_B = digitalRead(KEY_B);
//  Serial.print("buttonState_B: ");
//  Serial.println(buttonState_B);

  if (buttonState_B == LOW) {
    delay(500);  //delay shake elimination

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
  }
}

void print_array()
{
  Serial.println("-------------------------");
  for(int temp = 0; temp < status_array_num_max ; temp++)
    Serial.println(device_list_array[temp]);
//    Serial.print("\t");
  Serial.println("-------------------------");
}

void display_stock_num()
{
  Serial.println("display_stock_num");
  display.clear_line(line);
  display.drawString(0, 0, "Press keyB to input num");
  display.drawString(0, 16, stock_num);
  display.display();
  delay(500);

  // clear input word
  display.clear_line_num(1);
  display.display();
}

void enter_input_status()
{
  Serial.println("enter_input_status");
  status_msa300_getvalue = true;
//  status_check_word = true;
  
  
  temp_num = 0;
}

void save_word()
{
  // save stock num
  if (status_array_num_max < MAX_LIST_NUM){
      device_list_array[status_array_num_max] = stock_num;
      status_array_num_max ++;
  }
//  status_display_line34 = true;

//  display.clear_line_num(1);
//  display.drawString(0, 16, stock_num);
//  display.display();
}

void exit_input_status()
{
 
  Serial.println("exit_input_status");
  status_msa300_getvalue = false;        // disable msa300
//  status_check_word = false;  // disable choose word
  temp_num = 0;               // recover word init status, initial value is '0'

  if(pattern_num == 2)
    display_stock_num();        // display now input word

}


void display_line_34()
{
//  Serial.println("enter display_line_34");
//  print_array();
  if(status_array_num_max == 0)
    return;

    // show now display 2 lines stock num in 3/4 line
    for (int temp=0; MAX_DISPLAY_NUM > temp ; temp++){
      //to_string(status_array_num_current)+

      int a = status_array_num_current + temp;
      if((a == status_array_num_max-1)&&(status_array_num_max < 2))
        continue;
//        a = status_array_num_max-1;
      if(a == status_array_num_max )
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

//  if(status_array_num_max <= 1)
//  {
//    status_display_next = false;
//  }

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

//  if((status_array_num_max <= 1) && (status_array_num_current <= 1))
//    status_array_num_current = 0;
}


void setup() {
  Serial.begin(115200);
  Serial.println("enter setup");

  // led
//  pinMode(ledPin, OUTPUT);
//  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
//  pixels.show();

  // init msa300
  msa300_init();
  
  // init display
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

  // //display "Press keyA"
  display.clear();
  display.drawString(0, 0, "Press keyA to change pattern");
  display.display();
  delay(3000);
//  display "Press keyB"
//  display.clear();
//  display.drawString(0, 16, "Press keyB");
//  display.display();
//  display.clear_line(line);
//  display.display();


  

  
  Serial.println("exit setup");
}

void event_keyA()
{
      // enter input status
      Serial.println("keyB down, enter input status");
      stock_num = "";

//      display.clear();
//      display.display();
      
//      display.clear_line(line);
//      display.drawString(0, 0, "enter input status");
////      display.drawString(6, 16, "<");
//      display.display();
//
//      delay(1000);
//
//      display.clear_line(line);
//      display.drawString(0, 0, "Tilt left or right");
////      display.drawString(6, 16, "<");
//      display.display();
//
//      delay(1000);

      
      // can get gsensor and choose char
//      enter_input_status();
//      check_now_pattern();
}

// patter0 watch display
#include "watch_bmp.h"
int watch_image_current = 0;   // save now display image number
#define WATCH_IMAGE_NUM 4
// height width set 64??????????????
const uint8_t * watch_image_addr[WATCH_IMAGE_NUM]={Watch_Image1,Watch_Image2,Watch_Image3,Watch_Image4};
void drawImageDemo_128_64_w(int picture_num) {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  if(picture_num <0 || picture_num > 3)
  {
    Serial.println("error picture_num");
    picture_num = 0;
  }  

//  display.drawXbm(0, 0, WiFi_Logo_width, WiFi_Logo_height, watch_image_addr[picture_num]);
  display.drawXbm(0, 0, Watch_Image_width, Watch_Image_height, watch_image_addr[picture_num]);

}

// patter1 image display 
#include "images_bmp.h"
int image_num_current = 0;   // save now display image number

#define IMAGE_NUM 5
const uint8_t * image_addr[IMAGE_NUM]={Image1,Image2,Image3,Image4,Image5};
void drawImageDemo_128_64(int picture_num) {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  if(picture_num <0 || picture_num > 4)
  {
    Serial.println("error picture_num");
    picture_num = 0;
  }  

  display.drawXbm(0, 0, WiFi_Logo_width, WiFi_Logo_height, image_addr[picture_num]);
}

void display_bmp_watch(){
  // 1. display
  // clear the display
  display.clear();
  // draw the current demo method
  // demos[watch_image_current]();
  drawImageDemo_128_64_w(watch_image_current);

//  display.setFont(ArialMT_Plain_10);
//  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  //display.drawString(128, 54, String(millis()));
  // write the buffer to the display
  display.display();
}

void display_bmp(){
  // 1. display
  // clear the display
  display.clear();
  // draw the current demo method
  // demos[image_num_current]();
  drawImageDemo_128_64(image_num_current);

//  display.setFont(ArialMT_Plain_10);
//  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  //display.drawString(128, 54, String(millis()));
  // write the buffer to the display
  display.display();
}


//1. display weather/time
void enter_pattern1()
{
  Serial.println("enter_pattern1: display weather/time");

  // close pattern 3 display
  status_display_line34 = false;
  
  // led -- red
  // pixels.setPixelColor(0, pixels.Color(10, 0, 0));
  //test light level
  pixels.setPixelColor(0, pixels.Color(10, 0, 0, 10));
  pixels.show();
  Serial.println("led_on 1");

  display.clear();
  display.drawString(0, 0, "Pattern 1: ");
  display.drawString(0, 16, "display weather/time");
  // display.drawString(0, 0, "4. Repete 1-3");
  display.display();
  delay(3000);

  exit_input_status();
  display.clear();
  display.display();
}

//2. display picture 
void enter_pattern2()
{
  Serial.println("enter_pattern2: display picture");
  // led -- green
  pixels.setPixelColor(0, pixels.Color(0, 10, 0, 10));
  pixels.show();
  Serial.println("led_on 2");

  display.clear();
  display.drawString(0, 0, "Pattern 2: display image");
  display.drawString(0, 16, "Tilt the borad to choose");
  display.drawString(0, 32, "image");
  // display.drawString(0, 0, "4. Repete 1-3");
  display.display();

  delay(3000);
}


//3. input word
void enter_pattern3()
{
  Serial.println("enter_pattern3: input word");
  // led -- blue
  pixels.setPixelColor(0, pixels.Color(0, 0, 10, 10));
  pixels.show();
  Serial.println("led_on 3");

  Serial.println("start enter_pattern3 display");
  display.clear();
  display.drawString(0, 0, "Pattern 3: input word");
  display.drawString(0, 16, "1. Press B to start input");
  display.drawString(0, 32, "2. Tilt the borad");
  display.drawString(0, 48, "3. Press B to choose word");
  // display.drawString(0, 48, "4. Repete 1-3");
  display.display();
//  delay(2000);
  Serial.println("end enter_pattern3 display");

  status_enter_patter3 = false;
  
}


void judge_pattern()
{
   // judge pattern status
//   Serial.print("pattern_num");
//   Serial.println(pattern_num);

  // change pattern ---- judge keyA touch
  buttonState_A = HIGH;
  buttonState_A = digitalRead(KEY_A);
  if (buttonState_A == LOW) {
    delay(300);  // delay for debouncing

    pattern_num++;
    display.clear();
    display.display();
    
    switch (pattern_num)
    {
      case 0: enter_pattern1(); break;
      case 1: enter_pattern2();break;
      case 2: enter_pattern3();break;
      default: pattern_num = -1;break;

    }
    
//    display.clear();
//    display.display();
  }
}

// enter pattern3(input) ---- check keyA
void pattern_solution3()
{
//    display.setTextAlignment(TEXT_ALIGN_LEFT);
  
    //1. judge keyB -- read the state of the pushbutton value:
    buttonState_B = HIGH;
    buttonState_B = digitalRead(KEY_B);
  
    //check if the pushbutton is pressed. If it is, the buttonState is HIGH:
    if (buttonState_B == LOW) {
      delay(300);  //delay shake elimination
      if(keyB_status == false){
        check_keyB_status();
  //      event_keyA();
      }
  //    else
  //    {
  //      // exit input status
  //      Serial.println("keyB down, exit input status");
  //      exit_input_status();
  //    }
    }

    // 2. get msa300
    if(status_msa300_getvalue == true)
      get_msa300();
  //  if(status_check_word == true)
  //    setword2_keyB();
  //    setword1_light();
  
    // 3. display line34
    if (status_display_line34 == true)
      display_line_34();
    
    delay(100);
    delaytime = delaytime + 1;
  
    //  key_down('A');
    

}

//enter pattern2(image) ---- check msa300 to choose image
void pattern_solution2()
{
//    display.clear();
//    display.display();
    //get msa300
    get_msa300();

    // judge next or prior image
    if(p_new.y > 1500)
    {
      Serial.println("next image");

      if (image_num_current == IMAGE_NUM-1){
        Serial.print("right end\n");
      }else
      {
        // image_num_current = (image_num_current + 1)  % IMAGE_NUM;
        image_num_current = image_num_current + 1;
      }

    }
    else if (p_new.y < -1500)
    {
      Serial.println("prior image");

      if (image_num_current == 0){
        Serial.print("left end\n");
      }else
      {
        // image_num_current = (image_num_current - 1)  % IMAGE_NUM;
        image_num_current = image_num_current - 1;
      }

    }
//    else
//      Serial.println("don't change image");

    //display image
    display_bmp();
    delay(500);
//    delay(100);
}

//enter pattern1(weather/time) ---- 
void pattern_solution1()
{

    //display watch -- 100ms change picture
//    drawImageDemo_64_64();
  
  //display watch image
  if (watch_image_current < WATCH_IMAGE_NUM -1 )
    watch_image_current++;
  else 
    watch_image_current=0;

  display_bmp_watch();
  delay(100);


  //get weather/time

}

void loop() {

  judge_pattern();

  // // enter pattern3(input) ---- check keyA
  // if(pattern_num == 2)
  // {
  //   pattern_solution3();
  // }

  // //enter pattern2(image) ---- check msa300 to choose image
  // if(pattern_num == 1)
  // {
  //   pattern_solution2();
  // }

  // //enter pattern1(weather) ---- 
  // if(pattern_num == 0)
  // {
  //   pattern_solution1();
  // }

  switch(pattern_num)
  {
    case 0: pattern_solution1();break;
    case 1: pattern_solution2();break;
    case 2: pattern_solution3();break;
    case -1: 
    {
      pattern_num=0 ;
      enter_pattern1();
      break;
    }
    default: Serial.println("error pattern");break;
  }

}

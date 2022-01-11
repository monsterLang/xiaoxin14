#include <Arduino.h>
#include<string>

#define MAX_LIST_NUM 70
#define MAX_DISPLAY_NUM 2   // can display 2 line
#define STOCK_LONG 13    // length of stock num , if stock num has 6 numbers, set 6



// led light
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel pixels(3,17, NEO_GRB + NEO_KHZ800);
const int ledPin =  13;      // the number of the LED pin

// pattern
int status_pattern = 0;

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

  // | 0-防误触-150 | 150-加1-600 | 600-加5-1200 | 1200-加10-2000 |
  int16_t symbol = 1;
  int16_t level_num = 0;
  if (temp_compare > 0)
    symbol = 1;
  else
    symbol = -1;

  level_num = abs(temp_compare);

  if (level_num < JUDGE_UPDOWN_LEVEL0)
  {
    //0-防误触-150
//    Serial.println("move level small");
  }
  else if(level_num > JUDGE_UPDOWN_LEVEL0 && level_num < JUDGE_UPDOWN_LEVEL1)
  {
    //150-加1-600
    temp_int = temp_int + symbol * COMPARE_UPDOWN_LEVEL0;
  }
  else if(level_num > JUDGE_UPDOWN_LEVEL1 && level_num < JUDGE_UPDOWN_LEVEL2)
  {
    //600-加5-1200
    temp_int = temp_int + symbol * COMPARE_UPDOWN_LEVEL1;
  }
  else if(level_num > JUDGE_UPDOWN_LEVEL2 && level_num < JUDGE_UPDOWN_LEVEL3)
  {
    //1200-加10-2000
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
//    Serial.println("move level small");
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
}

void display_input_word()
{
  display.clear_line(2);
//  display.drawString(0, 0, "touch light to choose word");
  display.drawString(0, 0, "touch keyB to choose word");

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

  if (status_pattern == 0){
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

void check_now_pattern(){
  if (status_pattern == 0)
  {
    status_display_line34 = true;
  }
  else
    status_display_line34 = false;
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
  

  if (status_pattern == 0)
    status_pattern = 1 ;
   else if (status_pattern == 1 )
    status_pattern = 0 ;
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
    delay(2000);  //延迟除抖

    buttonState_temp = digitalRead(key_X);
    if(buttonState_temp == LOW){
      delay(300); //延迟除抖
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
  display.clear_line(line);
  display.drawString(0, 0, "Press keyA to input num");
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
  status_display_line34 = true;

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

void event_keyA()
{
      // enter input status
      Serial.println("keyB down, enter input status");
      stock_num = "";
      
      display.clear_line(line);
      display.drawString(0, 0, "enter input status");
//      display.drawString(6, 16, "<");
      display.display();

      delay(1000);

      display.clear_line(line);
      display.drawString(0, 0, "Tilt left or right");
//      display.drawString(6, 16, "<");
      display.display();

      delay(1000);
      // can get gsensor and choose char
//      enter_input_status();
//      check_now_pattern();
}



void loop() {

   // judge pattern status
   

  
  //1. read the state of the pushbutton value:
  buttonState = digitalRead(KEY_B);

  //check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == LOW) {
    delay(300);  //延迟除抖
    if(keyA_status == false){
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


  if(status_msa300_getvalue == true)
    get_msa300();
//  if(status_check_word == true)
//    setword2_keyB();
//    setword1_light();

  if (status_display_line34 == true)
    display_line_34();

//  key_down('A');
  
  delay(100);
  delaytime = delaytime + 1;
}

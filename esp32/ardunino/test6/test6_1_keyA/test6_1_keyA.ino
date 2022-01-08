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
  Serial.println(temp_char);
  
  
  display.clear();
  display.drawString(0, 0, "touch light");
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
    
    if (temp_num > 5)
    {
      exit_input_status();
    }
    else if(temp_num == 5)
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
  display.clear();
  display.drawString(0, 0, "display_stock_num");
  display.drawString(0, 16, stock_num);
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
  display.drawString(0, 0, "Press keyA");
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
  delay(100);
  
}

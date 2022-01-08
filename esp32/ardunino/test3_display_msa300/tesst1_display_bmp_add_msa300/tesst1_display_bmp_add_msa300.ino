// Include the correct display library
// For a connection via I2C using the Arduino Wire include:
#include <Wire.h>               // Only needed for Arduino 1.6.5 and earlier
#include "SH1106Wire.h"        // legacy: #include "SSD1306.h"

// Optionally include custom images
#include "images_bmp.h"

// Initialize the OLED display using Arduino Wire:
SH1106Wire display(0x3c, 23, 22);   // ADDRESS, SDA, SCL  -  SDA and SCL usually populate automatically based on your board's pins_arduino.h e.g. https://github.com/esp8266/Arduino/blob/master/variants/nodemcu/pins_arduino.h

// Include the correct msa300 library
#include <Adafruit_MSA301.h>

Adafruit_MSA301 msa;

#include "math.h"

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

#define MSA_Y_CHANGE_RANGE_MAX 1500
#define MSA_Y_CHANGE_RANGE_MIN -1500
#define DEMO_DURATION 3000
typedef void (*Demo)(void);

int demoMode = 0;
int counter = 1;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();


  // Initialising the UI will init the display too.
  display.init();

  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

  msa300_init();
}



void drawFontFaceDemo() {
  // Font Demo1
  // create more fonts at http://oleddisplay.squix.ch/
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0, 0, "Hello world");
  display.setFont(ArialMT_Plain_16);
  display.drawString(0, 10, "Hello world");
  display.setFont(ArialMT_Plain_24);
  display.drawString(0, 26, "Hello world");
}

void drawTextFlowDemo() {
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawStringMaxWidth(0, 0, 128,
                             "Lorem ipsum\n dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore." );
}

void drawTextAlignmentDemo() {
  // Text alignment demo
  display.setFont(ArialMT_Plain_10);

  // The coordinates define the left starting point of the text
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 10, "Left aligned (0,10)");

  // The coordinates define the center of the text
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 22, "Center aligned (64,22)");

  // The coordinates define the right end of the text
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.drawString(128, 33, "Right aligned (128,33)");
}

void drawRectDemo() {
  // Draw a pixel at given position
  for (int i = 0; i < 10; i++) {
    display.setPixel(i, i);
    display.setPixel(10 - i, i);
  }
  display.drawRect(12, 12, 20, 20);

  // Fill the rectangle
  display.fillRect(14, 14, 17, 17);

  // Draw a line horizontally
  display.drawHorizontalLine(0, 40, 20);

  // Draw a line horizontally
  display.drawVerticalLine(40, 0, 20);
}

void drawCircleDemo() {
  for (int i = 1; i < 8; i++) {
    display.setColor(WHITE);
    display.drawCircle(32, 32, i * 3);
    if (i % 2 == 0) {
      display.setColor(BLACK);
    }
    display.fillCircle(96, 32, 32 - i * 3);
  }
}

void drawProgressBarDemo() {
  int progress = (counter / 5) % 100;
  // draw the progress bar
  display.drawProgressBar(0, 32, 120, 10, progress);

  // draw the percentage as String
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 15, String(progress) + "%");
}

void drawImageDemo() {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(0, 0, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits);
}

void drawImageDemo_lang() {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(0, 0, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits_lang);
}

//WiFi_Logo_bits_1,WiFi_Logo_bits_2,WiFi_Logo_bits_3
void drawImageDemo_1() {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(0, 0, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits_1);
}

void drawImageDemo_2() {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(0, 0, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits_2);
}

void drawImageDemo_3() {
  // see http://blog.squix.org/2015/05/esp8266-nodemcu-how-to-create-xbm.html
  // on how to create xbm files
  display.drawXbm(0, 0, WiFi_Logo_width, WiFi_Logo_height, WiFi_Logo_bits_3);
}

// Demo demos[] = {drawFontFaceDemo, drawTextFlowDemo, drawTextAlignmentDemo, drawRectDemo, drawCircleDemo, drawProgressBarDemo, drawImageDemo};
Demo demos[] = {drawImageDemo, drawImageDemo_lang, drawImageDemo_1, drawImageDemo_2, drawImageDemo_3};
int demoLength = (sizeof(demos) / sizeof(Demo));
long timeSinceLastModeSwitch = 0;

void change_display_bmp(){
  demoMode = (demoMode + 1)  % demoLength;
//  delay(2000);
}

//int16_t x_old,x_new,y_old,y_new,z_old,z_new;
int16_t x_old = 0;
int16_t x_new = 0;
int16_t y_old = 0;
int16_t y_new = 0;
int16_t z_old = 0;
int16_t z_new = 0;

//int16_t x_old,x_new,y_old,y_new,z_old,z_new;
//x_old = 0;
//x_new = 0;
//y_old = 0;
//y_new = 0;
//z_old = 0;
//z_new = 0;

void get_msa300(){
  //get msa xyz value
  msa.read();      // get X Y and Z data at once
  // Then print out the raw data
  Serial.print("X:  "); Serial.print(msa.x); 
  Serial.print("  \tY:  "); Serial.print(msa.y); 
  Serial.print("  \tZ:  "); Serial.print(msa.z); 

  Serial.println();
  x_old = x_new;
  y_old = y_new;
  z_old = z_new;
  x_new = msa.x;
  y_new = msa.y;
  z_new = msa.z;
  Serial.print(x_old);Serial.print("  \t");
  Serial.print(x_new);Serial.print(";  \t");
  Serial.print(y_old);Serial.print("  \t");
  Serial.print(y_new);Serial.print(";  \t");
  Serial.print(z_old);Serial.print("  \t");
  Serial.print(z_new);Serial.print(";  \t");
  Serial.println();
}

void reset_old_value(){
  msa.read();
  Serial.print("\nreset old value start\n");
  Serial.print("print y_old&y_new \t");
  Serial.print(y_old);Serial.print("  \t");
  Serial.print(y_new);Serial.print(";  \t");
  x_old = msa.x;
  y_old = msa.y;
  z_old = msa.z;
  Serial.print("\nreset old value end\n");
  Serial.print("print y_old&y_new \t");
  Serial.print(y_old);Serial.print("  \t");
  Serial.print(y_new);Serial.print(";  \t");
}

void display_bmp(){
  // 1. display
  // clear the display
  display.clear();
  // draw the current demo method
  demos[demoMode]();

  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  //display.drawString(128, 54, String(millis()));
  // write the buffer to the display
  display.display();
}

void loop() {
  

//// shili
//  if (millis() - timeSinceLastModeSwitch > DEMO_DURATION) {
//    demoMode = (demoMode + 1)  % demoLength;
//    timeSinceLastModeSwitch = millis();
//  }
//  counter++;
//  delay(10);

  //2. get msa300 value  
  get_msa300();

  Serial.print("print y_old&y_new \t");
  Serial.print(y_old);Serial.print("  \t");
  Serial.print(y_new);Serial.print(";  \t");
  Serial.println();
  int16_t temp = y_new - y_old;
  Serial.print("temp");Serial.print(temp);
  Serial.println();

  //3. judge if change picture
//  if (abs(temp) > MSA_Y_CHANGE_RANGE_MAX) 
  if (temp > MSA_Y_CHANGE_RANGE_MAX)
  {
    //(abs(y_new) < 500)
//    change_display_bmp();

    if( abs(y_new) < 500 )
    {
      Serial.print("\nturn to 0\n");
    }else{
        Serial.print("change_display_bmp left\n");
        if (demoMode == 0){
          Serial.print("left end\n");
        }else
        {
          demoMode = (demoMode - 1)  % demoLength;
        }
        
        display_bmp();
    
        //delay(2000);
        //wait_device_stable
        reset_old_value();
        temp=0;
    }


  }else if (temp < MSA_Y_CHANGE_RANGE_MIN)
  {

    if( abs(y_new) < 500 )
    {
      Serial.print("\nturn to 0\n");
    }else{
        //(abs(y_new) < 500)
        Serial.print("change_display_bmp right\n");
        if (demoMode == demoLength - 1){
          Serial.print("right end\n");
        }else
        {
          demoMode = (demoMode + 1)  % demoLength;
        }
    
        display_bmp();
    
        //delay(2000);
        //wait_device_stable
        reset_old_value();
        temp=0;
    }
  }
  
  //wait device change picture until status is balance
  delay(500);
//  change_display_bmp();

}

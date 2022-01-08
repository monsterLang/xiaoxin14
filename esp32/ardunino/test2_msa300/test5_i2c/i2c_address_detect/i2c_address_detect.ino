#include <Adafruit_I2CDevice.h>

Adafruit_I2CDevice i2c_dev = Adafruit_I2CDevice(0x26);
typedef unsigned char uint8_t;
//static const uint8_t SDA = 23;
//static const uint8_t SCL = 22;


void setup() {
  while (!Serial) { delay(10); }
  Serial.begin(115200);
  Serial.println("I2C address detection test");

  if (!i2c_dev.begin()) {
    Serial.print("Did not find device at 0x");
    Serial.println(i2c_dev.address(), HEX);
    while (1);
  }
  Serial.print("Device found on address 0x");
  Serial.println(i2c_dev.address(), HEX);
}

void loop() {
  
}

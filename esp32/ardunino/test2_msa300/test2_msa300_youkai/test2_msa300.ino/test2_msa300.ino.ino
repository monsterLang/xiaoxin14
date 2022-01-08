#include <msa300_youkai.h>
#include "Wire.h"
#include <Arduino.h>
#include "math.h"
#include <stdlib.h> //fabs
#include <stdint.h> 

MSA300 msa300;
void setup(){
  Serial.begin(115200);
  delay(100);
  Serial.print("begin msa300\n");

  msa300.init();
}

void loop(){  
  float x1,x2,y1,y2,z1,z2;
  x1=msa300.getX();

  y1=msa300.getY();

  z1=msa300.getZ();

  Serial.print(x1);Serial.print("\t");
  Serial.print(y1);Serial.print("\t");
  Serial.print(z1);Serial.print("\t");
  Serial.println();
  delay(1000);
}

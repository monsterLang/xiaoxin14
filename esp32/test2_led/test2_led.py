# 板载RGB灯
from mpython import *
import neopixel
import time

ws2812_P7 = neopixel.NeoPixel(Pin(Pin.P7), n=3, bpp=3, timing=1)
ws2812_P7.brightness(10 / 255)
ws2812_P7[0]=(255, 0, 0)
ws2812_P7.write()




#绘制坐标轴
# MindPlus
# mpython
from mpython import *

while True:
  oled.line(0,3,3,0,1)
  oled.line(3,0,6,3,1)
  oled.line(123,57,127,60,1)
  oled.line(123,63,127,60,1)
  oled.line(3,0,3,63,1)
  oled.line(0,60,127,60,1)
  oled.show()


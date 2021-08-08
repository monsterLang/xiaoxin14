#显示精度x127,y63（不用这段，只是用来判断精度的）
# MindPlus
# mpython
from mpython import *

x=0
y=0
while True:
  #oled.pixel(x,y,1)
  oled.pixel(127,63,1)
  oled.show()
  #x=x+2
  #y=y+1
  #print("x",x)
  #print("y",y)
  #time.sleep(0.1)



# 字符显示
from mpython import *

oled.fill(0)
# 注意这些显示积木块，只把信息写入OLED屏幕缓存，并不是直接在OLED屏幕上显示
oled.DispChar("你好世界", 38, 0, 1)
oled.DispChar("hello,world", 32, 16, 1)
oled.DispChar("안녕하세요", 35, 32, 1)
oled.DispChar("こんにちは世界", 23, 48, 1)
# 运行这个积木块，将我们之前写入缓存的信息，显示在OLED屏幕上
oled.show()

# 一个字符占6*16
from mpython import *

x=0
y=0
while True:
  oled.fill(0)
  oled.DispChar("a", 0, 0, 1)
  oled.DispChar("cc", 0, 16, 1)
  oled.pixel( 11, 16, 1)
  oled.show()
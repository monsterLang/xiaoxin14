# 1. 显示圆形时钟
# MindPlus
# mpython
from mpython import *


while True:
  my_clock = Clock(oled, 64, 32, 30)
  my_clock.settime()
  my_clock.drawClock()
  oled.show()
  oled.fill(0)  # 清空显示，防止秒针保留


# 2. 连接wifi获取网络时钟
# https://mpython.readthedocs.io/zh/master/classic/clock.html?highlight=clock#id3

import ntptime,network
from mpython import*
from machine import Timer

# 十进制数字转换为字符
def int2char (x):
  numbers = {
    0 : '0',
    1 : '1',
    2 : '2',
    3 : '3',
    4 : '4',
    5 : '5',
    6 : '6',
    7 : '7',
    8 : '8',
    9 : '9',
    10 : 'a',
    11 : 'b',
    12 : 'c',
    13 : 'd',
    14 : 'e',
    15 : 'f',
  }
  return numbers.get(x,None)


# 十进制int转十进制字符串
def int_2_dec( value , key_num):
    a=value%key_num
    result=''
    #print("value",value)
    if (a == 0) & (value == 0 ):
      result = '0'
      return result
    #print("result",result)
    #print("a!=0",a!=0)
    #print("value > 0",value > 0)
    while True  :
        if a == 0:
          if value <= 0:
            break
        #print("enter")
        #print("a",a)
        temp=int2char(a)
        #temp=hex(a).replace('0x','')
        result=temp+result
        #print("result",result)
        value=(value-a)//key_num
        #print("value",value)
        a=value%key_num
        #print("a",a)
    return result

# 连接wifi
mywifi=wifi()
mywifi.connectWiFi("youkai","19971226")

try:
    # 获取UTC国际标准时间
    ntptime.settime()
    # ntptime.settime(timezone=8, server = 'ntp.ntsc.ac.cn')
    # timezone为时区时间差，默认为东八区补偿8h，指定授时服务器,server为字符串类型，默认
except OSError :
    oled.DispChar("ntp链接超时,请重启!",0,20)
    oled.show()
else:
    # 绘制时钟，设置圆心坐标(64，32)，半径30
    clock=Clock(oled,64,32,30)

    # 定义刷新函数，获取时间并显示
    def Refresh(_):
        # 读取并显示音量
        sound_value_int=sound.read()
        sound_value_char=int_2_dec(sound_value_int,10)
        print('sound',sound_value_char)
        oled.DispChar('sound', 0, 0, 1)
        oled.DispChar(sound_value_char, 0, 16, 1)

        # 读取并显示光亮
        light_value_int=light.read()
        light_value_char=int_2_dec(light_value_int,10)
        print('light',light_value_char)
        oled.DispChar('light', 97, 0, 1)
        oled.DispChar(light_value_char, 97, 16, 1)

        # 刷新时间
        clock.settime()
        clock.drawClock()
        oled.show()
        clock.clear()
        oled.fill(0)

    # 定时器1
    tim1 = Timer(1)

    # 初始化定时器1，设置时间1s，Timer.PERIODIC周期性，回调函数Refresh
    tim1.init(period=100, mode=Timer.PERIODIC, callback=Refresh)


# 待验证，貌似没有rtc模块
from mpython import *

rtc = mpython.RTC();

rtc.datetime((2018, 7, 14, 6, 23, 18, 0, 0));#用当前时间设置这个RTC

 

while True:

  print(rtc.datetime());

  mpython.delay(1000);#延时1秒

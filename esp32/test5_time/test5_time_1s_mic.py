#---------------------------------------------
# func1: 获取网络时间及显示
#
#---------------------------------------------

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

#---------------------------------------------
# func2: 测试rtc  貌似没有rtc模块————代码无法执行
#
#---------------------------------------------
from mpython import *

rtc = mpython.RTC();

rtc.datetime((2018, 7, 14, 6, 23, 18, 0, 0));#用当前时间设置这个RTC

while True:

  print(rtc.datetime());

  mpython.delay(1000);#延时1秒

#---------------------------------------------
# func3: 测试rtc  可以设置时间
# 使用rtc库
#---------------------------------------------
import machine
from machine import RTC
rtc = machine.RTC()
rtc.init((2018, 11, 21, 3, 9, 0, 0, 0))
print(rtc.datetime())

#---------------------------------------------
# func4: 尝试设置1s定时打印字符串
# 
#---------------------------------------------
import ntptime,network
from mpython import*
from machine import Timer

# 定义刷新函数，获取时间并显示
def Refresh(_):
    global temp_num #定义全局变量
    temp_value = temp_num%2
    if temp_value == 0:
      print('sound')
      oled.DispChar('sound', 0, 0, 1)
    elif temp_value == 1:
      print('light')
      oled.DispChar('light', 97, 0, 1)
    else:
      print("wrong value")
    
    oled.show()
    oled.fill(0)
    temp_num=temp_num+1
    
    return

temp_num=0

# 定时器1
tim1 = Timer(1)

# 初始化定时器1，设置时间1s，Timer.PERIODIC周期性，回调函数Refresh
tim1.init(period=1000, mode=Timer.PERIODIC, callback=Refresh)


#---------------------------------------------
# func5: 测试一秒采音量数据个数
# PS   : 由于将数值转换为字符串以及显示会耗费时间，导致增加了转换及显示1s只能采到8个数值
# 但是只设置取值以及计采样次数，可以采到16643个，所以音频的采样率大致为16643hz
#---------------------------------------------
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


# 定义刷新函数，获取时间并显示
def Refresh(_):
    global temp_num #定义全局变量
    temp_value=int_2_dec(temp_num,10)
    print(temp_value)
    temp_num=0
    
    return

temp_num=0

# 定时器1
tim1 = Timer(1)

# 初始化定时器1，设置时间1s，Timer.PERIODIC周期性，回调函数Refresh
tim1.init(period=1000, mode=Timer.PERIODIC, callback=Refresh)

while 1 :
  temp_num=temp_num+1
  sound_value_int=sound.read()
  # sound_value_char=int_2_dec(sound_value_int,10)
  # print('sound',sound_value_char)
  # oled.DispChar('sound', 0, 0, 1)
  # oled.DispChar(sound_value_char, 0, 16, 1)


#---------------------------------------------
# func6: 尝试利用数组将采样的数值进行存储
# 
#---------------------------------------------
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


# 定义刷新函数，获取时间并显示
def Refresh(_):
    global temp_num #定义全局变量
    global sound_list
    temp_value=int_2_dec(temp_num,10)
    print("temp_value",temp_value)
    print("sound_list",len(sound_list))
    sound_list=[]   # 需要清空数组,否则会报错内存不够
    temp_num=0
    
    return

temp_num=0
sound_list=[]
# 定时器1
tim1 = Timer(1)

# 初始化定时器1，设置时间1s，Timer.PERIODIC周期性，回调函数Refresh
tim1.init(period=1000, mode=Timer.PERIODIC, callback=Refresh)

while 1 :
  temp_num=temp_num+1
  sound_value_int=sound.read()
  sound_list.append(sound_value_int)

  # sound_value_char=int_2_dec(sound_value_int,10)
  # print('sound',sound_value_char)
  # oled.DispChar('sound', 0, 0, 1)
  # oled.DispChar(sound_value_char, 0, 16, 1)

print("test end")

#---------------------------------------------
# func7: 尝试利用数组将采样的数值进行存储(1s停止抓取数据)
# 
#---------------------------------------------
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


# 定义刷新函数，获取时间并显示
def Refresh(_):
    global temp_num #定义全局变量
    global sound_list
    global status
    status=0
    temp_value=int_2_dec(temp_num,10)
    print("temp_value",temp_value)
    print("sound_list",len(sound_list))
    sound_list=[]   # 需要清空数组,否则会报错内存不够
    temp_num=0
    
    return

temp_num=0
sound_list=[]
status=1
# 定时器1
tim1 = Timer(1)

# 初始化定时器1，设置时间1s，Timer.PERIODIC周期性，回调函数Refresh
tim1.init(period=1000, mode=Timer.ONE_SHOT, callback=Refresh)

while status :
  temp_num=temp_num+1
  sound_value_int=sound.read()
  sound_list.append(sound_value_int)

  # sound_value_char=int_2_dec(sound_value_int,10)
  # print('sound',sound_value_char)
  # oled.DispChar('sound', 0, 0, 1)
  # oled.DispChar(sound_value_char, 0, 16, 1)

print("test end")

#---------------------------------------------
# func8: 数组转化及存储
# 
#---------------------------------------------
import os

path="test\\c.txt"	#define file path
xxx = open(path,"a")
print(type(xxx))

sound_list=['12','123','1222']
print(sound_list)
print(type(sound_list))

xxx.write('sound_list\n')
#xxx.write(sound_list)
#file_mic.write("\n")
#print(file_mic.read())
print('write ok')
xxx.close()

#---------------------------------------------
# func9: 数组转化及存储（int型转字符型数组）
# 
#---------------------------------------------
# MindPlus
# mpython
import os

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


def intlist_2_char(intlist):
  list_len=len(intlist)
  char=''
  print("list_len",list_len)
  #1index=0
  for index in range(list_len):
    print("index",index)
    temp=int_2_dec(intlist[index],10)
    print("temp",temp)
    char=char+'\n'+temp
  print(char)
  return char

path="test\\c.txt"	#define file path
xxx = open(path,"a")
print(type(xxx))

sound_list=[1,2,3]
print(sound_list)
print(type(sound_list))
charlist=intlist_2_char(sound_list)

xxx.write('sound_list')
xxx.write(charlist)

#file_mic.write("\n")
#print(file_mic.read())
print('write ok')
xxx.close()


#---------------------------------------------
# func10: 尝试利用数组将采样的数值进行存储(1s停止抓取数据)+数组转换
# 
#---------------------------------------------
import ntptime,network
from mpython import*
from machine import Timer

# int型数组转换为字符串
def intlist_2_char(intlist):
  list_len=len(intlist)
  char=''
  #print("list_len",list_len)
  #1index=0
  for index in range(list_len):
    print("index",index)
    temp=int_2_dec(intlist[index],10)
    print("temp",temp)
    char=char+'\n'+temp
  print(char)
  return char

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


# 定义刷新函数，获取时间并显示
def Refresh(_):
    global temp_num #定义全局变量
    global sound_list
    global status
    status=0
    temp_value=int_2_dec(temp_num,10)
    print("temp_value",temp_value)
    print("sound_list",len(sound_list))
    result_char=intlist_2_char(sound_list)
    print("result_char",result_char)
    sound_list=[]   # 需要清空数组,否则会报错内存不够
    temp_num=0
    
    return

temp_num=0
sound_list=[]
status=1
# 定时器1
tim1 = Timer(1)

# 初始化定时器1，设置时间1s，Timer.PERIODIC周期性，回调函数Refresh
tim1.init(period=1000, mode=Timer.ONE_SHOT, callback=Refresh)

while status :
  temp_num=temp_num+1
  sound_value_int=sound.read()
  sound_list.append(sound_value_int)

  # sound_value_char=int_2_dec(sound_value_int,10)
  # print('sound',sound_value_char)
  # oled.DispChar('sound', 0, 0, 1)
  # oled.DispChar(sound_value_char, 0, 16, 1)

print("test end")


#---------------------------------------------
# func11: 测试字符串最多能存多少组数据
# 
#---------------------------------------------
import ntptime,network
from mpython import*
from machine import Timer

# int型数组转换为字符串
def intlist_2_char(intlist):
  list_len=len(intlist)
  charlist=[]
  char=''
  #print("list_len",list_len)
  #1index=0
  #for index_y in range(list_len):
  print(type(list_len))
  for index_x in range(list_len): #测试结果最多2090个，尝试设置2000个
    print("index",index_x)
    temp=int_2_dec(intlist[index_x],10)
    #print("temp",temp)
    char=char+'\n'+temp
  #print(char)
  print("-----stop change-----")
  return char

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


# 定义刷新函数，获取时间并显示
def Refresh(_):
    print("-----in 1s-----")
    global temp_num #定义全局变量
    global sound_list
    global status
    status=0
    temp_value=int_2_dec(temp_num,10)
    print("temp_value",temp_value)
    print("sound_list",len(sound_list))
    print("-----start change-----")
    result_char=intlist_2_char(sound_list)
    #print("result_char",result_char)
    sound_list=[]   # 需要清空数组,否则会报错内存不够
    temp_num=0
    
    return

temp_num=0
sound_list=[]
status=1
# 定时器1
tim1 = Timer(1)

# 初始化定时器1，设置时间1s，Timer.PERIODIC周期性，回调函数Refresh
tim1.init(period=1000, mode=Timer.ONE_SHOT, callback=Refresh)

while status :
  temp_num=temp_num+1
  sound_value_int=sound.read()
  sound_list.append(sound_value_int)

  # sound_value_char=int_2_dec(sound_value_int,10)
  # print('sound',sound_value_char)
  # oled.DispChar('sound', 0, 0, 1)
  # oled.DispChar(sound_value_char, 0, 16, 1)

print("test end")


#---------------------------------------------
# func12: 数组转化及存储（int型转字符型数组）+字符串数组
# in     [1, 2, 3, 4, 5, 6, 7]
# out    ['\n1\n2', '\n3\n4', '\n5\n6', '\n7']
#---------------------------------------------
import os

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


def intlist_2_char(intlist):
  list_len=len(intlist)
  x_max=2
  y_max=list_len//x_max+1
  print("list_len",list_len)
  print("x_max",x_max)
  print("y_max",y_max)
  char=''
  char_list=[]
  
  #1index=0
  for index_y in range(y_max):
    #print("index_y",index_y)
    #print('\n')
    if index_y == y_max-1 :
      print('---in last list change xmax---')
      x_max2=list_len%x_max
      for index_x in range(x_max2):
        #print("index_x",index_x)
        index=index_x+x_max*index_y
        #print("index",index)
        temp=int_2_dec(intlist[index],10)
        print("temp",temp)
        char=char+'\n'+temp
    else:
      print('----xmax is 2----')
      for index_x in range(x_max):
        #print("index_x",index_x)
        index=index_x+x_max*index_y
        #print("index",index)
        temp=int_2_dec(intlist[index],10)
        print("temp",temp)
        char=char+'\n'+temp
    char_list.append(char)
    print('char',char)
    char=''
  print("char_list",char_list)
  print("char_list len",len(char_list))
  return char_list

path="test\\c.txt"	#define file path
xxx = open(path,"a")
print(type(xxx))

sound_list=[1,2,3,4,5,6,7]
print(sound_list)
print(type(sound_list))
charlist=intlist_2_char(sound_list)

xxx.write('sound_list')

charlist_len=len(charlist)
for index_list_len in range(charlist_len):
  temp_list=charlist[index_list_len]
  xxx.write(temp_list)

#xxx.write(charlist)

#file_mic.write("\n")
#print(file_mic.read())
print('write ok')
xxx.close()



#---------------------------------------------
# func13: 整合2，修改了时间为140ms,采1977个数据，因为1s数据太多，即使是使用字符数组也一样，所以func11、12没啥用
# 
#---------------------------------------------
import ntptime,network
from mpython import*
from machine import Timer

# int型数组转换为字符串
def intlist_2_char(intlist):
  list_len=len(intlist)
  x_max=2000
  y_max=list_len//x_max+1
  #print("list_len",list_len)
  #print("x_max",x_max)
  #print("y_max",y_max)
  char=''
  char_list=[]
  
  #1index=0
  for index_y in range(y_max):
    #print("index_y",index_y)
    #print('\n')
    if index_y == y_max-1 :
      print('====in last list change xmax====')
      x_max2=list_len%x_max
      for index_x in range(x_max2):
        #print("index_x",index_x)
        index=index_x+x_max*index_y
        #print("index",index)
        temp=int_2_dec(intlist[index],10)
        #print("temp",temp)
        char=char+'\n'+temp
    else:
      print('====xmax is 2====')
      for index_x in range(x_max):
        #print("index_x",index_x)
        index=index_x+x_max*index_y
        #print("index",index)
        temp=int_2_dec(intlist[index],10)
        #print("temp",temp)
        char=char+'\n'+temp
    char_list.append(char)
    #print('char',char)
    char=''
  #print("char_list",char_list)
  #print("char_list len",len(char_list))
  return char_list

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


# 定义刷新函数，获取时间并显示
def Refresh(_):
    print("-----in 1s-----")
    global temp_num #定义全局变量
    global sound_list
    global status
    status=0
    temp_value=int_2_dec(temp_num,10)
    print("data num is ",temp_value)
    #print("sound_list",len(sound_list))
    print("-----change start-----")
    result_char=intlist_2_char(sound_list)
    #print("result_char",result_char)
    print("-----change end-----")

    # save
    print("-----save start-----")
    path="test\\c.txt"	#define file path
    xxx = open(path,"a")
    xxx.write('sound_list')

    charlist_len=len(result_char)
    for index_list_len in range(charlist_len):
      temp_list=result_char[index_list_len]
      xxx.write(temp_list)
    xxx.close()
    print("-----save end-----")


    # clear
    sound_list=[]   # 需要清空数组,否则会报错内存不够
    temp_num=0
    
    return

temp_num=0
sound_list=[]
status=1
# 定时器1
tim1 = Timer(1)

# 初始化定时器1，设置时间1s，Timer.PERIODIC周期性，回调函数Refresh
time_len=140
tim1.init(period=time_len, mode=Timer.ONE_SHOT, callback=Refresh)
print("time_len = ",time_len,'s')

print('start blow')
while status :
  temp_num=temp_num+1
  sound_value_int=sound.read()
  sound_list.append(sound_value_int)

  # sound_value_char=int_2_dec(sound_value_int,10)
  # print('sound',sound_value_char)
  # oled.DispChar('sound', 0, 0, 1)
  # oled.DispChar(sound_value_char, 0, 16, 1)

print("test end")

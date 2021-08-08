# MindPlus
# mpython
from mpython import *
import os
a=0
path="test\\b.txt"
xxx=open(path,"w")
num='0'

#print('10/3 = ',10/3)      3.333333
#print('9/3 = ',9/3)        3.0
#print('10//3 = ',10//3)    3
#print('10%3 = ',10%3)      1

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
  
#print("result",int_2_dec(10))


while True:
  #print(a)
  #oled.fill(0)
  y=sound.read()
  #print(type(y))
  #print("y = ",int_2_dec(y))
  value=int_2_dec(y,10)
  num=int_2_dec(a,16)       #使用16进制，因为空间不够
  value=num+'_'+value
  print("value",value)

  # 用于显示两列数据
  x=a%8
  if x == 0:
    oled.fill(0)
  #print("x//4",x//4)       #用于lcd显示两列数据分列
  #print("x%4",x%4)

  # 横坐标设置9（7+2），7是一组数据有7个字符串，2是一行两组数据之间间隔两个字符
  oled.DispChar(value, 9*7*(x//4), (x%4)*16, 1)
  
  #oled.DispChar('aaa', 33, 33, 1)
  oled.show()
  #print("y",y)
  
  xxx.write(value)
  xxx.write("\n")
  a = a + 1
  if a == 20:
    xxx.close()
    #print('close file')
    break
  


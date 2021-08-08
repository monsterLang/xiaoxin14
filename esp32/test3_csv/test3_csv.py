from mpython import *
import neopixel
import time

def light():
  ws2812_P7 = neopixel.NeoPixel(Pin(Pin.P7), n=3, bpp=3, timing=1)
  ws2812_P7.brightness(10 / 255)
  ws2812_P7[0]=(255, 0, 0)
  ws2812_P7.write()
  return

def light_close():
  ws2812_P7 = neopixel.NeoPixel(Pin(Pin.P7), n=3, bpp=3, timing=1)
  ws2812_P7.brightness(10 / 255)
  ws2812_P7[0]=(0, 0, 0)
  ws2812_P7.write()
  return

#数值必须是同位数，若存在2位和3位，则2位补零。原因对比的是字符串，若实现字符串转数字可以不用该限制
def max_val(data):
  max_value="0"
  i=0
  point_y=data[i]
  x=len(data)
  print(x)
  while i<(x-1):
    print("max",int(max_value))
    print("point_y",int(point_y))
    if int(point_y)>int(max_value):
    #if point_y>max_value: #数据同位数判断
      max_value=point_y
      print("point_y>max_value")
    i=i+1
    point_y=data[i]
  print("i",i)
  print("point_y",point_y)
  print("max=",max_value)
  
  return max_value

def show_max_value(data):
    #point_y=result[0]
    point_y=max_val(data)
    #point_y=',1234'
    print(point_y)
    oled.fill(0)
    oled.DispChar((point_y), 0, (1-1)*16, 1)
    #oled.DispChar(('12345'), 0, (2-1)*16, 1)
    #oled.DispChar((result[2]), 0, (3-1)*16, 1)
    #oled.DispChar((result[3]), 0, (4-1)*16, 1)
    oled.show()
    return


def show_x_y(x,y,x_max,y_max,x_pointmax,y_pointmax):
	a=int(x*x_pointmax/x_max)
	b=int(y*y_pointmax/y_max)
	oled.pixel(7+a,56-b,1)	#(0,0)->(7,56) (x,y)->(7+x,56-y)
	oled.show()
	return 

#右上角显示停止的时间
def show_stop_time(stop_x):
	start_x=50-7*3		#两位数值，一位7*16，显示屏最大宽为127
	#stop_x显示的数值，start_x横坐标起始点，0纵坐标起始点，1显示方式为白色点
	oled.DispChar((str(stop_x)), start_x, 0, 1)	
	oled.show()
	return

def search_array(data):
  point_x=0					#x
  point_y=int(data[point_x])		#y
  y_max=int(max_val(data))
  x_max=len(result)	#是否需要减一
  x_pointmax=120
  y_pointmax=56
  print("xline=",x_max)

  #point_y max
  last_x=0
  last_y=0
  stop_x=0
  #stop_y=0

  while point_x<(x_max-1):
  	show_x_y(point_x,point_y,x_max,y_max,x_pointmax,y_pointmax)
  	diff=point_y-last_y
  	last_y=point_y
  	last_x=point_x
  	#if diff<0:
  	#	last_y=0
  	#elif diff>50:
  	if diff>50:
  		stop_x=point_x
  		#stop_y=point_y
  		print("stop_x",stop_x)
  		print("point_x",point_x)
  		print("stop time")
  		show_stop_time(stop_x)
	 	#oled.line(stop_x,stop_y,stop_x,60,1)	#(0,0)->(7,56) (x,y)->(7+x,56-y)
		#oled.show()
  		#light()
  	#elif point_x< 50:
  	  #light_close()
  	print("point_x",point_x)
  	point_x=point_x+1
  	point_y=int(data[point_x])
  	#time.sleep(0.25)
  return

def f_xy():
  oled.line(0,3,3,0,1)
  oled.line(3,0,6,3,1)
  oled.line(123,57,127,60,1)
  oled.line(123,63,127,60,1)
  oled.line(3,0,3,63,1)
  oled.line(0,60,127,60,1)
  oled.show()
  return



def max_val(data):
  max_value="0"
  i=0
  point_y=data[i]
  x=len(data)
  print(x)
  while i<(x-1):
    #print("max",int(max_value))
    #print("point_y",int(point_y))
    if int(point_y)>int(max_value):
    #if point_y>max_value: #数据同位数判断
      max_value=point_y
      print("point_y>max_value")
    i=i+1
    point_y=data[i]
  #print("i",i)
  #print("point_y",point_y)
  #print("max=",max_value)
  return max_value
  
def test_show(data):
  point_x=0                     #x
  point_y=int(data[point_x])    #y
  y_max=int(max_val(data))
  x_max=len(data)  #是否需要减一
  x_pointmax=120
  y_pointmax=56
  #print("xline=",x_max)
  print("x_max",x_max)
  while point_x<x_max:
    print("point_x",point_x)
    print("point_y",data[point_x])
    show_newest_10data(data,point_x,x_max,y_max,x_pointmax,y_pointmax)
    point_x=point_x+1
    #time.sleep(0.025)
  return

def show_newest_10data(data,point_x,x_max,y_max,x_pointmax,y_pointmax):
    i=0
    oled.fill(0)
    lasty=0
    one_frame=65
    while i<one_frame:
        temp_xindex=point_x-one_frame+i
        #print(temp_xindex)
        if temp_xindex<0:
          i=i+1
          continue
        point_y=int(y_pointmax*int(data[point_x-i])/y_max)
        #print("point_y_temp",point_y)
        key_num=2
        #key_num决定显示屏一次显示多少位，one_frame只要符合条件
        #127-key_num*(i-1)<0
        oled.line(127-key_num*i,63-point_y,127-key_num*(i-1),63-lasty,1)
        #oled.pixel(10*i,point_y,1)
        
        lasty=point_y
        i=i+1
    oled.DispChar((str(point_x)),0,0,1)
    oled.DispChar((data[point_x]),29,0,1)
    
    oled.show()
    return



#___main
file_path="a.txt"
f = open(file_path, 'r')
result = f.read().split('\r\n')	#get data
f.close()
print(result)
f_xy()
search_array(result)        # test1  fix display
time.sleep(3)


#___main
file_path="a.txt"
f = open(file_path, 'r')
result = f.read().split('\r\n')  #get data
f.close()
print(result)
test_show(result)           # test2 dynamic display





# MindPlus
# mpython
from mpython import *
import os

path="test\\c.txt"
xxx = open(path,"a")
print(type(xxx))

xxx.write('ccc\n')
#file_mic.write("\n")
#print(file_mic.read())
print('write ok')
xxx.close()


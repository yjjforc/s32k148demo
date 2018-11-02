# coding:utf-8
import time, serial
from struct import *
import binascii
import serial
from time import sleep
file = open('E:\\kaiti32.bin', 'rb')
ser = serial.Serial('COM5', 115200, timeout=1)
i = 1
df=1



stopdata=b'@'
ssss = str(binascii.b2a_hex(stopdata))[2:-1]

while 1:
    data=ser.read(1)
    print (data)
    if data ==b'a':
        break
    time.sleep(1.000)
    print("没有写入等待中")
    ser.write(bytes().fromhex(ssss))# 将16进制转换为字节
    ser.write(bytes().fromhex(ssss))# 将16进制转换为字节
    ser.write(bytes().fromhex(ssss))# 将16进制转换为字节 
 

while 1:
    c = file.read(1)
    # 将字节转换成16进制；
    ssss = str(binascii.b2a_hex(c))[2:-1]
    print(str(binascii.b2a_hex(c))[2:-1])
    if not c:
        break

    ser.write(bytes().fromhex(ssss))# 将16进制转换为字节
    #print('c')
    if i % 4096 == 0:
    # if ser.read(1)!=0x63: 

        while 1:
            time.sleep(0.050)
            data=ser.read(1)
            print (data)

            if data==b'c': 
                break
            time.sleep(1.000)
            print("没有写入等待中")


    time.sleep(0.001)
    #写每一行等待的时间

    i += 1

stopdata=b'#'
ssss = str(binascii.b2a_hex(stopdata))[2:-1]
ser.write(bytes().fromhex(ssss))# 将16进制转换为字节
ser.write(bytes().fromhex(ssss))# 将16进制转换为字节
ser.write(bytes().fromhex(ssss))# 将16进制转换为字节
while 1:
    data=ser.read(1)
    print (data)
    if data ==b'd':
        break
    time.sleep(1.000)
    print("没有写入等待中")
ser.close()
file.close()

# progressbar.py
 
from PyQt5.QtWidgets import QApplication, QProgressBar, QPushButton
from PyQt5 import QtWidgets
from PyQt5.QtCore import Qt, QBasicTimer
import os
# coding:utf-8
import time, serial
from struct import *
import binascii
import serial
import threading
from time import ctime,sleep

class ProgressBar(QtWidgets.QWidget):
    def __init__(self, parent= None):
        QtWidgets.QWidget.__init__(self)
        
        self.setGeometry(300, 300, 250, 150)
        self.setWindowTitle('ProgressBar')
        self.pbar = QProgressBar(self)
        self.pbar.setGeometry(30, 40, 200, 25)
        
        self.button = QPushButton('Start', self)
        self.button.setFocusPolicy(Qt.NoFocus)
        self.button.move(40, 80)
        
        self.button.clicked.connect(self.onStart)
        self.timer = QBasicTimer()
        self.step = 0
        self.file = open('E:\\QWidget.bin', 'rb')
        self.ser = serial.Serial('COM6', 115200, timeout=1)
        self.i = 1
        self.df=1
        self.filesize=os.path.getsize('E:\\QWidget.bin')


        
    def timerEvent(self, event):
        if self.step >=100:
            self.timer.stop()
            return
        self.step = self.step + 1
        
        
    def onStart(self):
        while 1:
            c = self.file.read(1)
            # 将字节转换成16进制；
            ssss = str(binascii.b2a_hex(c))[2:-1]
            print(str(binascii.b2a_hex(c))[2:-1])
            if not c:
                break
            self.ser.write(bytes().fromhex(ssss))# 将16进制转换为字节
            #print('c')
            if self.i % 4096 == 0:
            # if ser.read(1)!=0x63:

                while 1:
                    time.sleep(0.050)
                    data=self.ser.read(1)
                    print (data)
                    di=ord(data)
                    self.df=self.df+1
                    if di==self.df: 
                        self.pbar.setValue(50)
                        break
                    time.sleep(1.000)
                    print("没有写入等待中")

            time.sleep(0.001)
            #写每一行等待的时间

            self.i += 1

        stopdata=b'#'
        ssss = str(binascii.b2a_hex(stopdata))[2:-1]
        self.ser.write(bytes().fromhex(ssss))# 将16进制转换为字节
        self.ser.write(bytes().fromhex(ssss))# 将16进制转换为字节
        while 1:
            data=self.ser.read(1)
            print (data)
            if data ==b'd':
                break
            time.sleep(1.000)
            print("没有写入等待中")
        self.ser.close()
        self.file.close()

    
        
 
if __name__ == "__main__":
    import sys
    app = QApplication(sys.argv)
    qb = ProgressBar()
    qb.show()
    sys.exit(app.exec_())

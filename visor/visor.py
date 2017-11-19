#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys, serial, threading
from PyQt5.QtWidgets import QApplication, QWidget, QLabel
from PyQt5.QtGui import QIcon, QPixmap
from PIL import Image, ImageDraw, ImageQt


class App(QWidget):
 
    def __init__(self):
      super().__init__()
      self.title = 'PyQt5 Mousecam ADNS 5020'
      self.left = 10
      self.top = 10
      self.width = 640
      self.height = 480
      self.initUI()
 
    def initUI(self):
      self.setWindowTitle(self.title)
      self.setGeometry(self.left, self.top, self.width, self.height)
      # Create widget
      self.label = QLabel(self)
      pixmap = QPixmap('fondo.jpg')
      self.label.setPixmap(pixmap)
      self.resize(pixmap.width(),pixmap.height())

      self.show()

    def mousecam(self):
      moo = serial.Serial('/dev/ttyUSB0', 9600)
      while True:
        msg = moo.readline().decode()
        print(msg)
        img = list(map(int,msg.strip()[:-1].split(',')))
        print(img)
        im=Image.new('RGB', (15,15), None)
        draw=ImageDraw.Draw(im)
        
        for i, value in enumerate(img):
          j = i // 15
          i = i - j * 15
          c = int(value * 2.4)
          
          draw.point((i,j),(c,c,c))
      
        im2 = im.resize((512,512))
        qim = ImageQt.ImageQt(im2)
        pix = QPixmap.fromImage(qim)
        self.label.setPixmap(pix)

if __name__ == '__main__':
  app = QApplication(sys.argv)
  ex = App()
  MouseCamThread = threading.Thread(target=ex.mousecam)
  MouseCamThread.daemon = True
  MouseCamThread.start()
  sys.exit(app.exec_())

    

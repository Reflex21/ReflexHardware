# -*- coding: utf-8 -*-
"""
Created on Sat Apr  3 19:13:53 2021

@author: Anthony Stewart
"""

import serial
import csv
import time
import matplotlib.pyplot as plt
import numpy as np

i=0
x=list()
y=list()
ser = serial.Serial('COM6',9600)
ser.flushInput()

plot_window = 20
y_var = np.array(np.zeros([plot_window]))

plt.ion()
fig, ax = plt.subplots()
line, = ax.plot(y_var)

while True:
    try:
        ser_bytes = ser.readline()
        print(ser_bytes)
        try:
            decoded_bytes = float(ser_bytes[0:len(ser_bytes)-2].decode("utf-8"))
            print(decoded_bytes)
        except:
            continue
        y_var = np.append(y_var,decoded_bytes)
        y_var = y_var[1:plot_window+1]
        line.set_ydata(y_var)
        ax.relim()
        ax.autoscale_view()
        fig.canvas.draw()
        fig.canvas.flush_events()
    except:
        print("Keyboard Interrupt")
        ser.close()
        break

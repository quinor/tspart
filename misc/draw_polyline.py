#!/bin/env python

import sys
import serial
import time

def draw_polyline (l):
  ser = serial.Serial("/dev/ttyUSB0", 9600)
  time.sleep(3)
  ser.write(b"1 0 0\n")
  print(ser.readline())
  for x, y in l:
    ser.write(bytes("2 {} {}\n".format(x, y), "utf-8"))
    print(ser.readline())
  ser.write(b"2 0 -1500\n")
  print(ser.readline())
  ser.write(b"3 0 0\n")
  print(ser.readline())

file = sys.argv[1]

s = open(file).read().split("\n")
l = [x.split() for x in s if x]

draw_polyline(l)

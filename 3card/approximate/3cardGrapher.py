#!python3

import numpy as np
import matplotlib.pyplot as plt

x = range(22100)

f1 = open('3LowApprox1Full.txt')
y1=[line.split(',')[-1] for line in f1][1:]

f2 = open('3LowApprox2Full.txt')
y2=[line.split(',')[-1] for line in f2]

f3 = open('3LowApprox5Full.txt')
y3=[line.split(',')[-1] for line in f3]

f4 = open('3LowApprox10Full.txt')
y4=[line.split(',')[-1] for line in f4]

f5 = open('3LowApprox50Full.txt')
y5=[line.split(',')[-1] for line in f5]

f6 = open('3LowApprox100Full.txt')
y6=[line.split(',')[-1] for line in f6]

f7 = open('3LowApprox1000Full.txt')
y7=[line.split(',')[-1] for line in f7]

f8 = open('3aceLowResultsFull.txt')
y8=[line.split(',')[-1] for line in f8]

plt.plot(x, y1, label='1.0')
plt.plot(x, y2, label='0.5')
plt.plot(x, y3, label='0.2')
plt.plot(x, y4, label='0.1')
plt.plot(x, y5, label='0.05')
plt.plot(x, y6, label='0.01')
plt.plot(x, y7, label='0.001')
plt.plot(x, y8, label='Actual')
plt.axis([0,22100,11,20])
#plt.title("Summary of E(h) for Different Tolerances")
plt.ylabel("Expected Turns to Rummy")
legend = plt.legend(loc='lower right')
plt.xlabel("Cumulative Hands Evaluated")

plt.show()
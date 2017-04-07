#!python3

import numpy as np
import matplotlib.pyplot as plt

x = range(270725)

f1 = open('4lowApprox1full.txt')
y1 = [line.rsplit(None,1)[-1] for line in f1]

f2 = open('4lowApprox2full.txt')
y2 = [line.rsplit(None,1)[-1] for line in f2]

f3 = open('4lowApprox5full.txt')
y3 = [line.rsplit(None,1)[-1] for line in f3]

f4 = open('4lowApprox20full.txt')
y4 = [line.rsplit(None,1)[-1] for line in f4]

f5 = open('4lowApprox50full.txt')
y5 = [line.rsplit(None,1)[-1] for line in f5]

f6 = open('4lowApprox100full.txt')
y6 = [line.rsplit(None,1)[-1] for line in f6]

f7 = open('4lowApprox1000full.txt')
y7 = [line.rsplit(None,1)[-1] for line in f7]

plt.plot(x, y1, label='1.0')
plt.plot(x, y2, label='0.5')
plt.plot(x, y3, label='0.2')
plt.plot(x, y4, label='0.05')
plt.plot(x, y5, label='0.01')
plt.plot(x, y6, label='0.001')
plt.plot(x, y7, label='Actual')
plt.axis([0,270725,22,45])
plt.title("Summary of E(h) for Different Tolerances")
plt.ylabel("Expected Turns to Rummy")
legend = plt.legend(loc='lower right')
plt.xlabel("Cumulative Hands Evaluated")

plt.show()
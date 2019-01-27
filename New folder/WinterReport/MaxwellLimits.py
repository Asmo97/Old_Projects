import matplotlib.pyplot as plt
import numpy as np

o = np.linspace(0, 1, 256, endpoint=True) #Vol fraction
kp = 1
kf = 1

Par = (o*(kp/kf)) + (1 - o) #parralel limit k/kf
Ser = (1/o) + (1/(1-o)) * ( #series limit k/kf


BLow = (kp - kf)/(kp + (2*kf))
BUp = (kf - kp)/(kf + (2*kp))
MaxLow = (1 + (2 * BLow *o))/(1 - (BLow * o))
MaxUp = (kp*((1 + (2 * BUp * (1 - o)))/(1 - (BUp * (1 - o)))))/kf

plt.plot(o, Par, label = "Parralel Limit")
plt.plot(o, Ser, label = "Series Limit")
plt.plot(o, MaxLow, label = "Lower Maxwell")
plt.plot(o, MaxUp, label = "Upper Maxwell")

plt.legend()
plt.xlabel('Vol. Fraction')
plt.ylabel('K/Kf')
plt.title("kp = %i , kf = %i" % (kp,kf))
#plt.xlim(0,1)
#plt.ylim(0,kp)

plt.show()

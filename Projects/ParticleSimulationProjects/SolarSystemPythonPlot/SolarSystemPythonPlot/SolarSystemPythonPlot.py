import matplotlib as mpl
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import pyplot as plt
import numpy as np

fig = plt.figure()
ax1 = fig.add_subplot(2,1,1, projection = '3d')
ax2 = fig.add_subplot(2,1,2)

x = []
y = []
z = []

for i in range(0,9):
  x.append([i])
print(x[1])
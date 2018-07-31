import matplotlib as mpl
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import pyplot as plt
import numpy as np

fig = plt.figure()
ax1 = fig.add_subplot(2,1,1, projection = '3d')
ax2 = fig.add_subplot(2,1,2)

for i in range(1,10):
    print(i)

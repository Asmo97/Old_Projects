import matplotlib as mpl
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import pyplot as plt
import numpy as np

fig = plt.figure()
ax1 = fig.add_subplot(111, projection = '3d')
x,y,z = np.loadtxt('ParticlePythonDataExplicit.csv', unpack=True, delimiter=',', skiprows = 1)
x1,y1,z1 = np.loadtxt('ParticlePythonDataImplicit.csv', unpack=True, delimiter=',', skiprows = 1)

ax1.plot(x,y,z)
ax1.plot(x1,y1,z1)
ax1.scatter(0,0,0, c='orange', s = 100)

ax1.set_xlim3d([-2.0, 2.0])
ax1.set_xlabel('X')

ax1.set_ylim3d([-2.0, 2.0])
ax1.set_ylabel('Y')

ax1.set_zlim3d([-2.0, 2.0])
ax1.set_zlabel('Z')

ax1.set_title('Euler Test')

plt.show()
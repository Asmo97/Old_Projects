import matplotlib as mpl
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import pyplot as plt
import numpy as np

fig = plt.figure()
ax1 = fig.add_subplot(2,1,1, projection = '3d')
ax2 = fig.add_subplot(2,1,2)
x,y,z,e,t = np.loadtxt('C:\\Users\\Saad\\Desktop\\Fun_stuff\\Projects\\ParticleSimulationProjects\\EulerParticleSimulation\\EulerParticleSimulation\\PythonParticleDataExplicit.csv', unpack=True, delimiter=',', skiprows = 1)
x1,y1,z1,e1,t1 = np.loadtxt('C:\\Users\\Saad\\Desktop\\Fun_stuff\\Projects\\ParticleSimulationProjects\\EulerParticleSimulation\\EulerParticleSimulation\\PythonParticleDataImplicit.csv', unpack=True, delimiter=',', skiprows = 1)

ax1.plot(x,y,z, label = "Explicit") #Explicit
ax1.plot(x1,y1,z1, label = "Implicit" ) #Implicit
ax1.scatter(0,0,0, c='orange', s = 100)

ax2.plot(t,e,label = "Explicit")
ax2.plot(t1,e1,label = "Implicit")

ax1.set_xlim3d([-2.0, 2.0])
ax1.set_xlabel('X')
ax1.set_ylim3d([-2.0, 2.0])
ax1.set_ylabel('Y')
ax1.set_zlim3d([-2.0, 2.0])
ax1.set_zlabel('Z')
ax1.set_title('Euler Test')

ax2.plot
ax2.set_xlabel('Time')
ax2.set_ylabel('Energy')
ax2.set_title('Total Energy of planets')

ax2.legend()
plt.show()
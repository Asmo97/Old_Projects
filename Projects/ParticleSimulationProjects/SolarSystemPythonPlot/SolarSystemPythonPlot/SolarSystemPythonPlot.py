import matplotlib as mpl
from mpl_toolkits.mplot3d import Axes3D
from matplotlib import pyplot as plt
import numpy as np

fig = plt.figure()
ax1 = fig.add_subplot(111, projection = '3d')

Planetnames = ["Mercury" , "Venus", "Earth", "Mars", "Jupiter", "Saturn", "Uranus", "Neptune", "Pluto"]

for i in range(1,9):
    Px,Py,Pz,e,t = np.loadtxt('C:\\Users\\Saad\\Desktop\\Fun_stuff\\Projects\\ParticleSimulationProjects\\SolarSystem\\SolarSystem\\PythonParticleData' + str(i) +'.csv', dtype="double", unpack=True, delimiter=',', skiprows = 1)
    ax1.plot(Px,Py,Pz, label = Planetnames[i-1])

ax1.scatter(0,0,0, c='orange', s = 100, label="sun")

ax1.set_title('Solar System')
ax1.set_xlabel('X')
ax1.set_ylabel('Y')
ax1.set_zlabel('Z')
ax1.legend()
 
plt.show()


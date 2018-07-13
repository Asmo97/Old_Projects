import csv
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from mpl_toolkits.mplot3d import axes3d

#--------------------------- Read data file ---------------------------------

Particle = [];
Mass = [];
Px = [];
Py = [];
Pz = [];



#Python will view the single \ as an escape character and your file will not open so use \\
with open('C:\\Users\\Saad\\Desktop\\Fun_stuff\\Projects\\ParticleSimulationProjects\\EulerParticleSimulation1\\EulerParticleSimulation1\\ParticleData.csv', 'r') as ParticleDataFile: 
    ParticleData = csv.reader(ParticleDataFile) # read the contents of the data file and store them into a generator object
    #quick note about generators: they do NOT hold the entire result in memory, it yeilds one result at a time when asked for

    next(ParticleData) #tell the generator object to skip the first line in ParticleData as they are headers

    for line in ParticleData: 
        Px = line[2];
        Py = line[3];
        Pz = line[4];
        
        
#--------------------------- Plot data ---------------------------------------

fig = plt.figure()
ax = axes3d.Axes3D(fig)

# Setting the axes properties
ax.set_xlim3d([0, 1])
ax.set_xlabel('X')

ax.set_ylim3d([0, 1])
ax.set_ylabel('Y')

ax.set_zlim3d([0, 1])
ax.set_zlabel('Z')

plt.show()

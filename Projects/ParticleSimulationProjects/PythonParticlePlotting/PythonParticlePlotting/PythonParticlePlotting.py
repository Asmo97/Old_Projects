import csv
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from mpl_toolkits.mplot3d import axes3d

#--------------------------- Read data file ---------------------------------

Position = [];
Velocity = [];
Force = [];


def Sort3DVector(Px,Py,Pz):
    vec = [];
    vec.append(Px);
    vec.append(Py);
    vec.append(Pz);

    return vec;

def CreateAxis(PositionAxis):
    ax = axes3d.Axes3D(fig)

    # Setting the axes properties
    ax.set_xlim3d([min(PositionAxis[0])-10,max(PositionAxis[0])+10])
    ax.set_xlabel('X')

    ax.set_ylim3d([min(PositionAxis[1])-10,max(PositionAxis[1])+10])
    ax.set_ylabel('Y')

    ax.set_zlim3d([min(PositionAxis[2])-10,max(PositionAxis[2])+10])
    ax.set_zlabel('Z')



#Python will view the single \ as an escape character and your file will not open so use \\
with open('C:\\Users\\Saad\\Desktop\\Fun_stuff-master\\Projects\\ParticleSimulationProjects\\EulerParticleSimulation1\\EulerParticleSimulation1\\ParticleData.csv', 'r') as ParticleDataFile: 
    ParticleData = csv.reader(ParticleDataFile) # read the contents of the data file and store them into a generator object
    #quick note about generators: they do NOT hold the entire result in memory, it yeilds one result at a time when asked for

    next(ParticleData) #tell the generator object to skip the first line in ParticleData as they are headers

    for line in ParticleData: 
        Position.append(Sort3DVector(line[2],line[3],line[4]));
        Velocity.append(Sort3DVector(line[5],line[6],line[7]));
        Force.append(Sort3DVector(line[8],line[9],line[10]));
       
print(Position)

    #fig = plt.figure();
    #CreateAxis(Position);
    #plt.show()
        
        

import csv
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from mpl_toolkits.mplot3d import axes3d



fig = plt.figure()
ax1 = axes3d.Axes3D(fig)

def animate(i):
    graph_data = open('example.txt','r').read()
    lines = graph_data.split('\n')
    xs = []
    ys = []
    zs = []
    for line in lines:
        if line == 0:
            continue
        if len(line) > 1:
            x, y, z = line.split(',')
            xs.append(x)
            ys.append(y)
            zs.append(z)
    ax1.clear()
    ax1.plot(xs, ys, zs)

ani = animation.FuncAnimation(fig, animate, interval=1000)
plt.show()


##--------------------------- Read data file ---------------------------------
#fig = plt.figure()
#ax = fig.add_subplot(111, projection='3d')

#def animate(i):
#    graph_data = open('testdata.txt', 'r').read()
#    lines = graph_data.split('\n')
#    x = []
#    y = []
#    z = []
#    for line in lines:
#        if line == 0:
#            continue
#        if len(line) > 1:
#            x_data, y_data, z_data = line.split(',')
#            x.append(x_data)
#            y.append(y_data)
#            z.append(z_data)

#    ax.clear()
#    ax.plot(x,y,z)
#    #ax.scatter(x, y, z, color="g", s=20*2**4)

#ani = animation.FuncAnimation(fig, animate, interval = 1000)
#plt.show()i
import sys
import matplotlib.pylab as plt
import numpy as np 

if(len(sys.argv) != 2):
    print("Please enter path to filename containing data")
    exit(-1)

dataFile = open(sys.argv[1],"r")
numRotations = {}   #g->numOfRotations
timeRotations = {}  #g->timeForEachRotation
insertionSearch = {}
insertionRotation = {}

#deletion stuff
d_numRotations = {}   #g->numOfRotations
d_timeRotations = {}  #g->timeForEachRotation
dSearch = {}
dRotation = {}

for dataPoint in dataFile:

    data = dataPoint.split(",")

    numRotations[int(data[0])] = int(data[1])
    timeRotations[int(data[0])] = float(data[2])
    insertionSearch[int(data[0])] = float(data[3])
    insertionRotation[int(data[0])] = float(data[4])

    d_numRotations[int(data[0])] = int(data[5])
    d_timeRotations[int(data[0])] = float(data[6])
    dSearch[int(data[0])] = float(data[7])
    dRotation[int(data[0])] = float(data[8])

rotations = sorted(numRotations.items())
rotations_x, rotations_y = zip(*rotations)

time = sorted(timeRotations.items())
time_x, time_y = zip(*time)

insertionRotationTime = sorted(insertionRotation.items())
rotTime_x, rotTime_y = zip(*insertionRotationTime)

insertionSearchTime = sorted(insertionSearch.items())
searchTime_x, searchTime_y = zip(*insertionSearchTime)

d_rotations = sorted(d_numRotations.items())
d_rotations_x, d_rotations_y = zip(*d_rotations)

d_time = sorted(d_timeRotations.items())
d_time_x, d_time_y = zip(*d_time)

dRotationTime = sorted(dRotation.items())
d_rotTime_x, d_rotTime_y = zip(*dRotationTime)

dSearchTime = sorted(dSearch.items())
d_searchTime_x, d_searchTime_y = zip(*dSearchTime)

fig, axs = plt.subplots(4, 1, sharex='col', constrained_layout=True)
fig.suptitle('AVL-G Tree Performance for Insertions', fontsize=16)
plt.xticks(np.arange(1, 21, 1)) 

fig2, axs2 = plt.subplots(4, 1, sharex='col', constrained_layout=True)
fig2.suptitle('AVL-G Tree Performance for Deletions', fontsize=16)
plt.xticks(np.arange(1, 21, 1)) 

axs[0].plot(rotations_x, rotations_y, '-')
axs[0].set_title('Effect of MaxImbalance on Number of Rotations')
axs[0].set_xlabel('MaxImbalnce (g)')
axs[0].set_ylabel('Number of Rotations')

axs[1].plot(time_x, time_y, '-')
axs[1].set_title('Effect of MaxImbalance on Total CPU Time')
axs[1].set_xlabel('MaxImbalnce (g)')
axs[1].set_ylabel('Time (ms)')

axs[2].plot(searchTime_x, searchTime_y, '-')
axs[2].set_title('Effect of MaxImbalance on Search CPU Time')
axs[2].set_xlabel('MaxImbalnce (g)')
axs[2].set_ylabel('Time (ms)')

axs[3].plot(rotTime_x, rotTime_y, '-')
axs[3].set_title('Effect of MaxImbalance on Roatation CPU Time')
axs[3].set_xlabel('MaxImbalnce (g)')
axs[3].set_ylabel('Time (ms)')

axs2[0].plot(d_rotations_x, d_rotations_y, '-')
axs2[0].set_title('Effect of MaxImbalance on Number of Rotations')
axs2[0].set_xlabel('MaxImbalnce (g)')
axs2[0].set_ylabel('Number of Rotations')

axs2[1].plot(d_time_x, d_time_y, '-')
axs2[1].set_title('Effect of MaxImbalance on Total CPU Time')
axs2[1].set_xlabel('MaxImbalnce (g)')
axs2[1].set_ylabel('Time (ms)')

axs2[2].plot(d_searchTime_x, d_searchTime_y, '-')
axs2[2].set_title('Effect of MaxImbalance on Search CPU Time')
axs2[2].set_xlabel('MaxImbalnce (g)')
axs2[2].set_ylabel('Time (ms)')

axs2[3].plot(d_rotTime_x, d_rotTime_y, '-')
axs2[3].set_title('Effect of MaxImbalance on Roatation CPU Time')
axs2[3].set_xlabel('MaxImbalnce (g)')
axs2[3].set_ylabel('Time (ms)')

plt.show()
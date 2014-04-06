#!/usr/bin/python2.7

import os;
import sys;
from subprocess import call;
from datetime import datetime;
import time;

photocache = "/dev/shm/photocache";

# Check to see if the temporary RAM filesystem is mounted, if not create it
if not os.path.exists(photocache) :
    ret = call(["mkdir", "-p", photocache]);
    if not (ret == 0) :
        print("CameraDaemon - ERROR: Could not create shared memory photocache!");
        quit(-1);

# check the command line arguments
interval = 0.1;
count = 0;
for arg in sys.argv:
    if arg == "-i":
        interval = float(sys.argv[count+1]);
    count = count + 1;

count = 0;
maxPhotos = 5;
index = 0;
lastFiles = []

while 1:
    tstamp = time.time();
    fname = datetime.fromtimestamp(tstamp).strftime('%H_%M_%S_%m');
    fname = photocache + os.sep + str(fname) + ".jpg";
    os.system("raspistill -awb auto -mm backlit -ex auto -w 640 -h 480 -t "+str(interval*1000.) + " -o " + str(fname));
    if count < maxPhotos :
        count = count + 1;
        index = index + 1;
        lastFiles.append(fname);
    else :
        rmCmd = "rm " + str(lastFiles[index]);
        os.system(rmCmd);
        lastFiles[index] = fname;
        index = (index + 1) % maxPhotos;

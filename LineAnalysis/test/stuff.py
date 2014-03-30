#!/usr/bin/python

import os;
import re;
from subprocess import call;

struct = os.walk("../../doc/testImages");

for root, dirs, files in struct :
    for f in files:
        if re.match(".*\.png", f) != None :
            call(["./LineDetector.test", "../../doc/testImages/"+f]);


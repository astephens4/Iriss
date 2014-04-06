#!/usr/bin/python

import os;
import re;
from subprocess import call;

struct = os.walk("../../doc/sampleData");

for root, dirs, files in struct :
    for f in files:
        matchessss = re.match("(.*)\.jpg", f);
        if re.match(".*\.jpg", f) != None :
            call(["./herp", "../../doc/sampleData/"+str(matchessss.group(1))+".line", "../../doc/sampleData/"+f]);


#!/usr/bin/python27

import os;
import re;
from subprocess import call;

dirStruct = os.walk("../../raspi/LineAnalysis/testImages");

for roots, dirs, files in dirStruct :
    for fname in files :
        if re.find("jpg", fname) != None :
            ret = call(["./stats"], ["-i"], ["

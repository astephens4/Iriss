#!/usr/bin/python

import os
import re

places = os.walk(".");

for root, subdirs, files in places :
    if re.match(".*include.*", root) == None :
        for fname in files :
            if re.match(".*\.[ch]pp", fname) :
                os.system("echo File:"+root+str(os.sep)+fname+" >> ~/allinone.cpp");
                os.system("cat "+root+str(os.sep)+fname+" >> ~/allinone.cpp");
                os.system("echo >> ~/allinone.cpp");

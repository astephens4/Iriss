#!/usr/bin/python27

import sys
import subprocess
import os;
import shlex;
import re;

def get_cmd_output(cmd) :
    args = shlex.split(cmd);
    proc = subprocess.Popen(args, stdout = subprocess.PIPE, stderr = subprocess.PIPE);
    out,err = proc.communicate();
    ret = proc.returncode;
    return ret, out, err;

wd = os.getcwd();

passfail = open(wd+"/passfail.txt", "w");

count = 0;
for arg in sys.argv :
    if count == 0 :
        count = 1;
        continue;
    cmd = wd + "/" + arg
    ret,out,err = get_cmd_output(cmd);
    resultsFname = cmd+".results.txt";
    resultsFname = re.sub("([a-zA-Z]+)\.test\.results\.txt", "results.\g<1>.txt", resultsFname);
    results = open(resultsFname, "w");
    results.write(out);
    if ret < 0 :
        passfail.write(cmd+": Fail\n");
    else :
        passfail.write(cmd+": Pass\n");

# do somestuff to display test results

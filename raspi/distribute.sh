#!/bin/bash

# Pack up everything for copying onto the Pi

BUILD_VERSION="release"
OUTDIR="output"

if [ "$#" -eq 0 ]; then
    echo "usage: ./distribute.sh {buildVersion}"
    echo "Where {buildVersion} is debug OR release"
    exit
elif [ "$1" == "release" ]; then
    BUILD_VERSION="$1"
    OUTDIR="iriss"
elif [ "$1" == "debug" ]; then
    BUILD_VERSION="$1"
    OUTDIR="iriss_d"
else
    echo "$1 is not a recognized option"
    exit
fi

# make the output directory
mkdir -p ./$OUTDIR/{lib,bin}

# copy the 3rdParty libraries into $OUTDIR/lib
cp -a ./common/$BUILD_VERSION/lib/* $OUTDIR/lib

# copy the non-third party libraries into $OUTDIR/lib
cp -a ./dist/$BUILD_VERSION/lib/* $OUTDIR/lib

# copy the 3rdParty binaries into $OUTDIR/bin
cp -a ./common/$BUILD_VERSION/bin/* $OUTDIR/bin

# copy the non-third party binaries into $OUTDIR/bin
cp -a ./dist/$BUILD_VERSION/bin/* $OUTDIR/bin

# pack $OUTDIR into a zipped tar
tar czfp $OUTDIR.tgz ./$OUTDIR && rm -rf $OUTDIR


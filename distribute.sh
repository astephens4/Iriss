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

# copy the libraries into $OUTDIR/lib
cp -a ./target/$BUILD_VERSION/lib/* $OUTDIR/lib

# copy the binaries into $OUTDIR/bin
cp -a ./target/$BUILD_VERSION/bin/* $OUTDIR/bin

# write the install script
echo "#!/bin/sh" > ./$OUTDIR/install.sh 
echo "sudo cp -rf lib/* /usr/local/lib" >> ./$OUTDIR/install.sh
echo "sudo ldconf" >> ./$OUTDIR/install.sh
chmod +x ./$OUTDIR/install.sh

# pack $OUTDIR into a zipped tar
tar czfp $OUTDIR.tgz ./$OUTDIR && rm -rf $OUTDIR


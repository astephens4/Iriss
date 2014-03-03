# Make access to the cross-compiler simple
export PATH=$PATH:/opt/raspi/buildtools/bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/raspi/buildtools/lib

# Type of libraries and executables to build
export BUILD_VERSION="release"
echo "$1"
if [ "$#" -eq 0 ]; then
    echo "Usage: ./buildAll {buildVersion}"
    echo "Where buildVersion is release or debug"
    exit
elif [ "$1" == "release" ]; then
    BUILD_VERSION="$1"
elif [ "$1" == "debug" ]; then
    BUILD_VERSION="$1"
fi
mkdir -p ../common/$BUILD_VERSION

# get the number of CPU cores
NUM_CORES=$(grep -c ^processor /proc/cpuinfo)

# Build opencv
unzip opencv-2.4.8.zip && cd opencv-2.4.8
mkdir build && cd build
cmake ../ -DCMAKE_TOOLCHAIN_FILE=../../rpi.toolchain.cmake -DENABLE_VFPV3=TRUE -DCMAKE_INSTALL_PREFIX=../../../common/$BUILD_VERSION -DCMAKE_BUILD_TYPE="$BUILD_VERSION" -DCMAKE_INSTALL_RPATH_USE_LINK_PATH=FALSE -DCMAKE_INSTALL_RPATH=/usr/local/lib

make -j $NUM_CORES && make install

cd ../../ && rm -rf opencv-2.4.8

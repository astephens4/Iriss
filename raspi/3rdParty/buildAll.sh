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
mkdir -p ./$BUILD_VERSION

# Build opencv
unzip opencv-2.4.8.zip && cd opencv-2.4.8
mkdir build && cd build
cmake ../ -DCMAKE_TOOLCHAIN_FILE=../../rpi.toolchain.cmake -DENABLE_NEON=ON -DENABLE_VFPV3=ON -DCMAKE_INSTALL_PREFIX=../../../common/$BUILD_VERSION -DCMAKE_BUILD_TYPE="$BUILD_VERSION"
make -j4 && make install

cd ../../ && rm -rf opencv-2.4.8

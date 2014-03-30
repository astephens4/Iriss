#/bin/sh

# install the executables for the build environment
echo "Downloading local dependencies from Ubuntu repositories..."
sudo apt-get install git cmake ia32-libs wget umlet build-essential arduino-core

echo "Getting Raspberry Pi toolchain from the official Github"
git clone https://github.com/raspberrypi/tools.git ./raspi_tc
sudo mkdir -p /opt/raspi/{buildtools,mkimage}
sudo chown -R $(whoami) /opt/raspi/{buildtools,mkimage}
sudo chgrp -R $(whoami) /opt/raspi/{buildtools,mkimage}
cp -rf ./raspi_tc/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/* /opt/raspi/buildtools
cp -rf ./raspi_tc/mkimage/* /opt/raspi/mkimage
rm -rf ./raspi_tc

# download the 3rdParty libraries
echo "Downloading 3rdPary Libraries"
wget "http://sourceforge.net/projects/opencvlibrary/files/opencv-unix/2.4.8/opencv-2.4.8.zip/download" -O ../3rdParty/opencv-2.4.8.zip

#/bin/sh

# install the executables for the build environment
sudo apt-get install git cmake ia32-libs wget umlet build-essential

# download the 3rdParty libraries
wget "http://sourceforge.net/projects/opencvlibrary/files/opencv-unix/2.4.8/opencv-2.4.8.zip/download" -O ../raspi/3rdParty/opencv-2.4.8.zip

#!/bin/bash
export HB_QTPATH="/usr/lib/qt5/bin/"
export HB_WITH_QT="/usr/include/x86_64-linux-gnu/qt5"
export HB_BUILD_DEBUG=yes
#HB_QTPATH, HB_QTSUFFIX HB_WITH_QT
make $*
sudo make install 
#sudo cp ./lib/linux/gcc/libgtqtc.a /usr/local/lib/harbour/libgtqtc.a 
#sudo chmod 755 /usr/local/lib/harbour/libgtqtc.a

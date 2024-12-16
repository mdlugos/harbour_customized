export HB_QTPATH="/usr/lib/qt6/libexec/"
export HB_WITH_QT="/home/ja/e/Qt/6.8.1/gcc_64/include"
#HB_QTPATH, HB_QTSUFFIX HB_WITH_QT
make $*
sudo make install
#sudo cp ./lib/linux/gcc/libgtqtc.a /usr/local/lib/harbour/libgtqtc.a 
#sudo chmod 755 /usr/local/lib/harbour/libgtqtc.a

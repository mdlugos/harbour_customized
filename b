#export HB_QTPATH="/usr/lib/qt6/libexec/"
#export HB_WITH_QT="/usr/include/x86_64-linux-gnu/qt6"
#HB_QTPATH, HB_QTSUFFIX HB_WITH_QT
make HB_QTPATH="/usr/lib/qt6/libexec/" HB_WITH_QT="/usr/include/x86_64-linux-gnu/qt6" $*
sudo make HB_QTPATH="/usr/lib/qt6/libexec/" HB_WITH_QT="/usr/include/x86_64-linux-gnu/qt6" install 
#sudo cp ./lib/linux/gcc/libgtqtc.a /usr/local/lib/harbour/libgtqtc.a 
#sudo chmod 755 /usr/local/lib/harbour/libgtqtc.a

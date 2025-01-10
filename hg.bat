set path=%~dp0..\h\comp\mingw64\bin;%~dp0bin\win\mingw64;%path%
set path=%~dp0;%~dp0..\tools;%~dp0..\ads120\acesdk;%~dp0..\MySQL.x64\LIB;%~dp0..\Firebird.x64\;%path%
set HB_WITH_MYSQL=%~dp0..\MySQL.x64\include
set HB_WITH_ADS=%~dp0..\ads120\acesdk
set HB_WITH_FIREBIRD=%~dp0..\Firebird.x64\include
set HB_WITH_QT=%~dp0..\h\comp\mingw64\include
rem set HB_BUILD_DEBUG=yes
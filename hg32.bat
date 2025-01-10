set path=%~dp0..\h\comp\mingw\bin;%~dp0bin\win\mingw;%path%
set path=%~dp0;%~dp0..\tools;%~dp0..\ads120\acesdk;%~dp0..\MySQL\LIB;%~dp0..\Firebird;%path%
set HB_WITH_MYSQL=%~dp0..\MySQL\include
set HB_WITH_ADS=%~dp0..\ads120\acesdk
set HB_WITH_FIREBIRD=%~dp0..\Firebird\include
rem set HB_BUILD_DEBUG=yes
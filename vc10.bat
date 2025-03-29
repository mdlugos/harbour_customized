SET Path=%~dp0bin\win\msvc;%path%
set path=%~dp0;%~dp0..\tools;%~dp0..\ads101\acesdk;%~dp0..\MySQL\LIB;%~dp0..\Firebird;%path%
set HB_WITH_MYSQL=%~dp0..\MySQL\include
set HB_WITH_ADS=%~dp0..\ads101\acesdk
set HB_WITH_FIREBIRD=%~dp0..\Firebird\include
rem set HB_BUILD_DEBUG=yes
rem # to enable additional system specific logging output,
rem # OutputDebugString() on Windows, syslog() on *nix systems:
rem https://github.com/harbour/core/tree/master?tab=readme-ov-file#debugging-options
rem set HB_TR_LEVEL=debug
rem set HB_TR_OUTPUT=tracelog.txt
rem set HB_TR_SYSOUT=yes

SET INCLUDE=%~dp0..\h\comp\VC10\include
SET LIB=%~dp0..\h\comp\VC10\lib
SET Path=%~dp0..\h\comp\VC10\bin;%path%


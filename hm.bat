SET Path=%~dp0bin\win\msvc64;%~d0\tools;%~d0\ng;%~d0\MySQL.x64\LIB;%~d0\ads120\acesdk;%~d0\Firebird.x64\;%path%
set HB_WITH_MYSQL=%~d0\MySQL.x64\include
set HB_WITH_ADS=%~d0\ads120\acesdk
set HB_WITH_FIREBIRD=%~d0\Firebird.x64\include
rem # to enable additional system specific logging output,
rem # OutputDebugString() on Windows, syslog() on *nix systems:
rem https://github.com/harbour/core/tree/master?tab=readme-ov-file#debugging-options
rem set HB_TR_LEVEL=debug
rem set HB_TR_OUTPUT=tracelog.txt
rem set HB_TR_SYSOUT=yes
IF NOT "%VCINSTALLDIR%"=="" GOTO EXIT
"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
SET INCLUDE=%~d0\h\comp\VC\include;%~d0\h\comp\KITS\include\ucrt;%~d0\h\comp\KITS\include\winrt;%~d0\h\comp\KITS\include\shared;%~d0\h\comp\KITS\include\um;%~d0\h\comp\KITS\include\cppwinrt
SET LIB=%~d0\h\comp\VC\lib\x64;%~d0\h\comp\KITS\lib\ucrt\x64;%~d0\h\comp\KITS\lib\um\x64;
SET Path=%~d0\h\comp\VC\bin\HostX64\x64;%~d0\h\comp\KITS\bin\x64;%~dp0bin\win\msvc64;%Path%
:EXIT

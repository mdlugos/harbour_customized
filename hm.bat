SET Path=%~dp0bin\win\msvc64;%path%
set path=%~dp0;%~dp0..\tools;%~dp0..\ads120\acesdk;%~dp0..\MySQL.x64\LIB;%~dp0..\Firebird.x64\;%path%
set HB_WITH_MYSQL=%~dp0..\MySQL.x64\include
set HB_WITH_ADS=%~dp0..\ads120\acesdk
set HB_WITH_FIREBIRD=%~dp0..\Firebird.x64\include
rem set HB_BUILD_DEBUG=yes
rem # to enable additional system specific logging output,
rem # OutputDebugString() on Windows, syslog() on *nix systems:
rem https://github.com/harbour/core/tree/master?tab=readme-ov-file#debugging-options
rem set HB_TR_LEVEL=debug
rem set HB_TR_OUTPUT=tracelog.txt
rem set HB_TR_SYSOUT=yes
"%ProgramFiles%\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
"%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
SET INCLUDE=%~dp0..\h\comp\VC\include;%~dp0..\h\comp\KITS\include\ucrt;%~dp0..\h\comp\KITS\include\winrt;%~dp0..\h\comp\KITS\include\shared;%~dp0..\h\comp\KITS\include\um;%~dp0..\h\comp\KITS\include\cppwinrt
SET LIB=%~dp0..\h\comp\VC\lib\x64;%~dp0..\h\comp\KITS\lib\ucrt\x64;%~dp0..\h\comp\KITS\lib\um\x64;
SET Path=%~dp0..\h\comp\VC\bin\HostX64\x64;%~dp0..\h\comp\KITS\bin\x64;%Path%

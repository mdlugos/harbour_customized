cd /D %~dp0
IF "%LIB%"=="" call hm32.bat
win-make HB_BUILD_DEBUG=yes %*

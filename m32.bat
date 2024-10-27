IF "%LIB%"=="" call %~dp0\hm32.bat
win-make HB_BUILD_DEBUG=yes HB_SHELL=nt %*

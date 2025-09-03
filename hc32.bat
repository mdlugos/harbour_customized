set PATH=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\Llvm\bin;%~dp0bin\win\clangx86;%path%
set path=%~dp0;%~dp0..\tools;%~dp0..\ads101\acesdk;%~dp0..\MySQL\LIB;%~dp0..\Firebird;%path%
set HB_WITH_MYSQL=%~dp0..\MySQL\include
set HB_WITH_ADS=%~dp0..\ads101\acesdk
set HB_WITH_FIREBIRD=%~dp0..\Firebird\include
set HB_MT=yes

del cd.txt
del setdbgp.bat
cd > cd.txt
copy ..\set.tx + cd.txt setdbgp.bat
echo SET HB_DBG_PATH=%%CD%%;%%CD%%\..\common >> setdbgp.bat
hbmk2 @prg -u+head/%1 %2 %3 %4 %5 %6 %7 %8 %9
setdbgp.bat

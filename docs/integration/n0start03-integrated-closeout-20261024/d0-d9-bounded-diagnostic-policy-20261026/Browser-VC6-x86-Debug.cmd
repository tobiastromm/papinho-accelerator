@echo off
call "C:/MSVC600-master/VC98/Bin/VCVARS32.BAT"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /MD /WX /W3 /Od /Z7 /I"C:\Projetos\PapinhoBrowser\src" /I"C:\Projetos\PapinhoBrowser\third_party\pst-sdk-0.6.5-normal-4dbd2783\include" /Fo"C:\Projetos\PapinhoAccelerator\build\d0-d9-bounded-diagnostic-20261026\getter-fixtures\Browser-VC6-x86\Debug\test.obj" /Fd"C:\Projetos\PapinhoAccelerator\build\d0-d9-bounded-diagnostic-20261026\getter-fixtures\Browser-VC6-x86\Debug\test.pdb" /Fe"C:\Projetos\PapinhoAccelerator\build\d0-d9-bounded-diagnostic-20261026\getter-fixtures\Browser-VC6-x86\Debug\actual-getter-test.exe" "C:\Projetos\PapinhoAccelerator\docs\integration\n0start03-integrated-closeout-20261024\d0-d9-bounded-diagnostic-policy-20261026\browser-actual-getter-test.c" /link /incremental:no
exit /b %errorlevel%

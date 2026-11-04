@echo off
set VSLANG=1033
call "C:\MSVC600-master\VC98\Bin\VCVARS32.BAT"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /MD /WX /W3 /DQUANT_NSS=1 /O2 /I"C:\Projetos\PapinhoBrowser\third_party\pst-sdk-0.6.5-normal-4dbd2783\include" /Fo"C:\Projetos\PapinhoAccelerator\build\composite-target-quantitative-20261026\NSS-VC6-x86\Release\fixture.obj" /Fd"C:\Projetos\PapinhoAccelerator\build\composite-target-quantitative-20261026\NSS-VC6-x86\Release\fixture.pdb" /Fe"C:\Projetos\PapinhoAccelerator\build\composite-target-quantitative-20261026\NSS-VC6-x86\Release\runtime-core-observation.exe" "C:\Projetos\PapinhoAccelerator\docs\integration\n0start03-integrated-closeout-20261024\composite-target-quantitative-closure-20261026\runtime-core-observation.c" "C:\Projetos\PapinhoBrowser\third_party\pst-sdk-0.6.5-normal-4dbd2783\lib\win32-x86-vc6-retrozilla-nss-md\papinho_secure_transport.lib" wsock32.lib /link /subsystem:console /incremental:no
exit /b %errorlevel%

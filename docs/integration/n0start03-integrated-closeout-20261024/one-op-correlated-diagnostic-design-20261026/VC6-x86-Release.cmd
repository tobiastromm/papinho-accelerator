@echo off
call "C:/MSVC600-master/VC98/Bin/VCVARS32.BAT"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /MD /WX /W3 /O2 /I"C:\Projetos\PapinhoBrowser\third_party\pst-sdk-0.6.5-normal-4dbd2783\include" /Fo"C:\Projetos\PapinhoAccelerator\build\one-op-correlated-diagnostic-20261026\VC6-x86-Release\test.obj" /Fd"C:\Projetos\PapinhoAccelerator\build\one-op-correlated-diagnostic-20261026\VC6-x86-Release\test.pdb" /Fe"C:\Projetos\PapinhoAccelerator\build\one-op-correlated-diagnostic-20261026\VC6-x86-Release\provider-query-test.exe" "C:\Projetos\PapinhoAccelerator\docs\integration\n0start03-integrated-closeout-20261024\one-op-correlated-diagnostic-design-20261026\owner-provider-readonly-probe-test.c" /link /incremental:no
exit /b %errorlevel%

@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /W4 /WX /TC /D_WIN32_WINNT=0x0603 /Od /Zi /RTC1 /MDd /D_DEBUG /Fo"C:\Projetos\PapinhoAccelerator\build\q4-independent-token-diagnostic-20261025\Legacy\Debug\identity-observation.obj" /Fd"C:\Projetos\PapinhoAccelerator\build\q4-independent-token-diagnostic-20261025\Legacy\Debug\identity-observation.pdb" /Fe"C:\Projetos\PapinhoAccelerator\build\q4-independent-token-diagnostic-20261025\Legacy\Debug\identity-observation.exe" "C:\Projetos\PapinhoAccelerator\docs\integration\n0start03-integrated-closeout-20261024\q0-q9-acceptance-and-integration-20261025\identity-observation-test.c" /link advapi32.lib
if errorlevel 1 exit /b %errorlevel%
cl /nologo /W4 /WX /TC /D_WIN32_WINNT=0x0603 /Od /Zi /RTC1 /MDd /D_DEBUG /Fo"C:\Projetos\PapinhoAccelerator\build\q4-independent-token-diagnostic-20261025\Legacy\Debug\q-census.obj" /Fd"C:\Projetos\PapinhoAccelerator\build\q4-independent-token-diagnostic-20261025\Legacy\Debug\q-census.pdb" /Fe"C:\Projetos\PapinhoAccelerator\build\q4-independent-token-diagnostic-20261025\Legacy\Debug\q-census.exe" "C:\Projetos\PapinhoAccelerator\docs\integration\n0start03-integrated-closeout-20261024\q0-q9-acceptance-and-integration-20261025\census-fault-test.c" /link advapi32.lib
if errorlevel 1 exit /b %errorlevel%
exit /b 0

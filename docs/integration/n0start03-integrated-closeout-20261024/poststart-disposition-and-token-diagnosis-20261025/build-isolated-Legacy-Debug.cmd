@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /W4 /WX /TC /D_WIN32_WINNT=0x0603 /Od /Zi /MDd /Fo"C:\Projetos\PapinhoAccelerator\build\ed11-positive-private-20261025\Legacy\Debug\poststart-identity-observation.obj" /Fd"C:\Projetos\PapinhoAccelerator\build\ed11-positive-private-20261025\Legacy\Debug\poststart-identity-observation.pdb" /Fe"C:\Projetos\PapinhoAccelerator\build\ed11-positive-private-20261025\Legacy\Debug\poststart-identity-observation.exe" /FAcs /Fa"C:\Projetos\PapinhoAccelerator\build\ed11-positive-private-20261025\Legacy\Debug\poststart-identity-observation.asm" "C:\Projetos\PapinhoAccelerator\docs\integration\n0start03-integrated-closeout-20261024\poststart-disposition-and-token-diagnosis-20261025\identity-observation-test.c" /link advapi32.lib
exit /b %errorlevel%

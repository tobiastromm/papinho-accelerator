@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /W4 /WX /TC /D_WIN32_WINNT=0x0603 /DPAPACC_Q4_TOKEN_DIAGNOSTIC=1 /DPAPACC_N0START03_PRIVATE_POSITIVE=1 /DPAPACC_N0_POSITIVE_PINNED_LAB=1 /DPAPACC_N0_PRIVATE_STDOUT=1 /Od /Zi /RTC1 /MDd /D_DEBUG /Fo"C:\Projetos\PapinhoAccelerator\build\q7-readback-unit-20261025\Legacy\Debug\Q4\q4-binding-test.obj" /Fd"C:\Projetos\PapinhoAccelerator\build\q7-readback-unit-20261025\Legacy\Debug\Q4\q4-binding-test.pdb" /Fe"C:\Projetos\PapinhoAccelerator\build\q7-readback-unit-20261025\Legacy\Debug\Q4\q4-binding-test.exe" "C:\Projetos\PapinhoAccelerator\docs\integration\n0start03-integrated-closeout-20261024\q7-scm-readback-correction-20261025\actual-readback-test.c" /link advapi32.lib
exit /b %errorlevel%

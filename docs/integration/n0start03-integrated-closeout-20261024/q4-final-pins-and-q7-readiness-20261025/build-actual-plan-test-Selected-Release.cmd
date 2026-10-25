@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /W4 /WX /TC /D_WIN32_WINNT=0x0603 /DPAPACC_Q4_TOKEN_DIAGNOSTIC=1 /DPAPACC_N0START03_PRIVATE_POSITIVE=1 /DPAPACC_N0_POSITIVE_PINNED_LAB=1 /DPAPACC_N0_PRIVATE_STDOUT=1 /O2 /DNDEBUG /MD /DPAPACC_ED11_WINDOWS_3MIB=1 /Fo"C:\Projetos\PapinhoAccelerator\build\q4-final-binding-20261025\Selected\Release\q4-actual-plan-test.obj" /Fd"C:\Projetos\PapinhoAccelerator\build\q4-final-binding-20261025\Selected\Release\q4-actual-plan-test.pdb" /Fe"C:\Projetos\PapinhoAccelerator\build\q4-final-binding-20261025\Selected\Release\q4-actual-plan-test.exe" "C:\Projetos\PapinhoAccelerator\docs\integration\n0start03-integrated-closeout-20261024\q4-final-pins-and-q7-readiness-20261025\q4-actual-plan-test.c" /link advapi32.lib
exit /b %errorlevel%

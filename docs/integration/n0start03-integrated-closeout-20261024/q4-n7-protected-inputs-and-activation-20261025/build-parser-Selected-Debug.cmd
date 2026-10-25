@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /W4 /WX /TC /D_WIN32_WINNT=0x0603 /DPAPACC_Q4_TOKEN_DIAGNOSTIC=1 /Od /Zi /RTC1 /MDd /D_DEBUG /DPAPACC_ED11_WINDOWS_3MIB=1 /Fo"C:\Projetos\PapinhoAccelerator\build\q4-input-candidate-20261025\Selected\Debug\candidate-parser.obj" /Fd"C:\Projetos\PapinhoAccelerator\build\q4-input-candidate-20261025\Selected\Debug\candidate-parser.pdb" /Fe"C:\Projetos\PapinhoAccelerator\build\q4-input-candidate-20261025\Selected\Debug\candidate-parser.exe" "C:\Projetos\PapinhoAccelerator\docs\integration\n0start03-integrated-closeout-20261024\q4-n7-protected-inputs-and-activation-20261025\candidate-input-parser-test.c" /link advapi32.lib
exit /b %errorlevel%

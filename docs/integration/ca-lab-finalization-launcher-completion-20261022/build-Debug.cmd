@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
if not exist "build\ca-lab-finalization-launcher-20261022\Debug" mkdir "build\ca-lab-finalization-launcher-20261022\Debug"
cl /nologo /TC /W4 /WX /Od /Zi /MDd /RTC1 /Iapps\server /Fo"build\ca-lab-finalization-launcher-20261022\Debug\\" /Fd"build\ca-lab-finalization-launcher-20261022\Debug\lab.pdb" tools\qualification\ca-ksp-next-lab\next-lab.c tools\qualification\ca-ksp-next-lab\backend-lab.c /Fe"build\ca-lab-finalization-launcher-20261022\Debug\next-lab.exe" /link ncrypt.lib advapi32.lib
if errorlevel 1 exit /b %errorlevel%
cl /nologo /TC /W4 /WX /Od /Zi /MDd /RTC1 /DPAPACC_LAB_NEGATIVE /Iapps\server /Fo"build\ca-lab-finalization-launcher-20261022\Debug\negative.obj" /Fd"build\ca-lab-finalization-launcher-20261022\Debug\negative.pdb" tests\ca_lab_nonoverwrite_negative_test.c /Fe"build\ca-lab-finalization-launcher-20261022\Debug\negative.exe"
exit /b %errorlevel%
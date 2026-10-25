@echo off
call "C:/Program Files (x86)/Microsoft Visual Studio/18/BuildTools/VC/Auxiliary/Build/vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cd /d "C:\Users\tobias\AppData\Local\Temp\pacc-storage-hash-lab-20261022\msvc-Release"
cl /nologo /TC /W4 /WX /MD /O2 "C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule\bcrypt_oracle.c" /Febcrypt_oracle.exe /link bcrypt.lib
if errorlevel 1 exit /b %errorlevel%
bcrypt_oracle.exe
exit /b %errorlevel%

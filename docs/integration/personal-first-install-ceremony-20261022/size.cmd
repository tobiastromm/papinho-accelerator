@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cd /d %TEMP%\pacc-personal-ceremony-20261022
cl /nologo /W4 /WX /TC size.c /Fe:size.exe
if errorlevel 1 exit /b %errorlevel%
size.exe

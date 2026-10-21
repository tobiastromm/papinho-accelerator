@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /TC /W4 /WX /O2 /MD /c /FAs /Fabuild\papacc-conf-parser-listeners-20261019\Release\config-source-layout.asm /Fobuild\papacc-conf-parser-listeners-20261019\Release\config-source-layout.obj src\config\config_source.c
exit /b %errorlevel%

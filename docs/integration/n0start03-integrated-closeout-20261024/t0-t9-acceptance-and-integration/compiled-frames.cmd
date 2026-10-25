@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
dumpbin /DISASM build\ed11-private-stdout-20261025\Selected\Release\CMakeFiles\papacc_n0_private_stdout.dir\tools\qualification\scm-n0-private-stdout-native.c.obj
exit /b %errorlevel%

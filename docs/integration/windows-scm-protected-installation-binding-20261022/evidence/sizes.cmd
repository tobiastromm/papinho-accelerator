@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /W4 /WX /MD /I apps/server /I include /I src/config /Fe"C:\Users\tobias\AppData\Local\Temp\pacc-scm-protected-binding-20261022\sizes.exe" /Fo"C:\Users\tobias\AppData\Local\Temp\pacc-scm-protected-binding-20261022\sizes.obj" /Fd"C:\Users\tobias\AppData\Local\Temp\pacc-scm-protected-binding-20261022\sizes.pdb" "C:\Users\tobias\AppData\Local\Temp\pacc-scm-protected-binding-20261022\sizes.c"
if errorlevel 1 exit /b %errorlevel%
"C:\Users\tobias\AppData\Local\Temp\pacc-scm-protected-binding-20261022\sizes.exe"

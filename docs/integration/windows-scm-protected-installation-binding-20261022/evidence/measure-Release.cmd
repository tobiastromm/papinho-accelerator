@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /W4 /WX /c /O2 /FAs /Fa"C:\Users\tobias\AppData\Local\Temp\pacc-scm-protected-binding-20261022\service-Release.asm" /Fo"C:\Users\tobias\AppData\Local\Temp\pacc-scm-protected-binding-20261022\service-Release.obj" /Fd"C:\Users\tobias\AppData\Local\Temp\pacc-scm-protected-binding-20261022\service-Release.pdb" /I apps/server /I include /I tools/qualification/storage-hash-capsule/vendor/bearssl-0.6/inc apps/server/server_service_win32.c
if errorlevel 1 exit /b %errorlevel%
dumpbin /symbols "C:\Users\tobias\AppData\Local\Temp\pacc-scm-protected-binding-20261022\service-Release.obj" > "C:\Users\tobias\AppData\Local\Temp\pacc-scm-protected-binding-20261022\symbols-Release.log"

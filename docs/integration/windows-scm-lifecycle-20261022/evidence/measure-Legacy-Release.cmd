@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /W4 /WX /MD /O2 /Ob2 /I"C:\Projetos\PapinhoAccelerator\include" /I"C:\Projetos\PapinhoAccelerator\apps\server" /Fd"C:\Users\tobias\AppData\Local\Temp\pacc-scm-lifecycle-20261022\measure-Legacy-Release.pdb" /FAcs /Fa"C:\Users\tobias\AppData\Local\Temp\pacc-scm-lifecycle-20261022\measure-Legacy-Release.asm" /Fo"C:\Users\tobias\AppData\Local\Temp\pacc-scm-lifecycle-20261022\measure-Legacy-Release.obj" /Fe"C:\Users\tobias\AppData\Local\Temp\pacc-scm-lifecycle-20261022\measure-Legacy-Release.exe" "C:\Users\tobias\AppData\Local\Temp\pacc-scm-lifecycle-20261022\measure.c" "C:\Projetos\PapinhoAccelerator\build\windows-scm-lifecycle-20261022\Legacy\Release\papacc_server_service.lib" "C:\Projetos\PapinhoAccelerator\build\windows-scm-lifecycle-20261022\Legacy\Release\papacc_server_console_win32.lib" /link advapi32.lib
if errorlevel 1 exit /b %errorlevel%
"C:\Users\tobias\AppData\Local\Temp\pacc-scm-lifecycle-20261022\measure-Legacy-Release.exe"
if errorlevel 1 exit /b %errorlevel%
dumpbin /headers "C:\Projetos\PapinhoAccelerator\build\windows-scm-lifecycle-20261022\Legacy\Release\CMakeFiles\papacc_server_service.dir\apps\server\server_service_win32.c.obj" > "C:\Users\tobias\AppData\Local\Temp\pacc-scm-lifecycle-20261022\sections-Legacy-Release.log"
dumpbin /imports "C:\Projetos\PapinhoAccelerator\build\windows-scm-lifecycle-20261022\Legacy\Release\papacc_server.exe" > "C:\Users\tobias\AppData\Local\Temp\pacc-scm-lifecycle-20261022\imports-Legacy-Release.log"
exit /b %errorlevel%


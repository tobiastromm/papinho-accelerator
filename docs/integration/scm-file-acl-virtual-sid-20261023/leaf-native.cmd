@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /W4 /WX /O2 /I"C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule\vendor\bearssl-0.6\inc" /TC /Fe:"C:\Users\tobias\AppData\Local\Temp\pacc-scm-file-sid-20261023\leaf-native.exe" /Fo:"C:\Users\tobias\AppData\Local\Temp\pacc-scm-file-sid-20261023\leaf-native.obj" "C:\Users\tobias\AppData\Local\Temp\pacc-scm-file-sid-20261023\leaf-native.c" /link advapi32.lib
if errorlevel 1 exit /b %errorlevel%
"C:\Users\tobias\AppData\Local\Temp\pacc-scm-file-sid-20261023\leaf-native.exe"
exit /b %errorlevel%

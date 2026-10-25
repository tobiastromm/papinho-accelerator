@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cl /I"C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule\vendor\bearssl-0.6\inc" /nologo /W4 /WX /TC /O2 /Fe:"C:\Users\tobias\AppData\Local\Temp\pacc-scm-acl-postverify-20261023\native-readonly.exe" /Fo:"C:\Users\tobias\AppData\Local\Temp\pacc-scm-acl-postverify-20261023\native-readonly.obj" "C:\Users\tobias\AppData\Local\Temp\pacc-scm-acl-postverify-20261023\native-readonly.c" /link advapi32.lib
if errorlevel 1 exit /b %errorlevel%
"C:\Users\tobias\AppData\Local\Temp\pacc-scm-acl-postverify-20261023\native-readonly.exe"
exit /b %errorlevel%


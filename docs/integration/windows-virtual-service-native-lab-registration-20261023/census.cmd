@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /W4 /WX /O2 /MD /I"C:\Projetos\PapinhoAccelerator\apps\server" /I"C:\Projetos\PapinhoAccelerator\include" /I"C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule\vendor\bearssl-0.6\inc" /TC /Fe:"C:\Users\tobias\AppData\Local\Temp\pacc-scm-lab-registration-20261023\census.exe" /Fo:"C:\Users\tobias\AppData\Local\Temp\pacc-scm-lab-registration-20261023\census.obj" "C:\Users\tobias\AppData\Local\Temp\pacc-scm-lab-registration-20261023\census.c" /link "C:\Projetos\PapinhoAccelerator\build\windows-virtual-service-native-lab-registration-20261023\Selected\Release\papacc_server_service.lib" "C:\Projetos\PapinhoAccelerator\build\windows-virtual-service-native-lab-registration-20261023\Selected\Release\papacc_server_console_win32.lib" "C:\Projetos\PapinhoAccelerator\build\windows-virtual-service-native-lab-registration-20261023\Selected\Release\papacc_core.lib" "C:\Projetos\PapinhoAccelerator\build\windows-virtual-service-native-lab-registration-20261023\Selected\Release\papacc_storage_sha256.lib" advapi32.lib
if errorlevel 1 exit /b %errorlevel%
"C:\Users\tobias\AppData\Local\Temp\pacc-scm-lab-registration-20261023\census.exe"
exit /b %errorlevel%

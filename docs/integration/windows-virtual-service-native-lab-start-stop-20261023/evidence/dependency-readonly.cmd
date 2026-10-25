@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /W4 /WX /I "C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule\vendor\bearssl-0.6\inc" /I "C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule\adapter_include" /I "C:\Projetos\PapinhoAccelerator\include" "C:\Projetos\_tmp\PapinhoAccelerator\scm-lab-start-stop-20261023\dependency-readonly.c" /Fo"C:\Projetos\_tmp\PapinhoAccelerator\scm-lab-start-stop-20261023\dependency-readonly.obj" /Fe"C:\Projetos\_tmp\PapinhoAccelerator\scm-lab-start-stop-20261023\dependency-readonly.exe" /link advapi32.lib
if errorlevel 1 exit /b %errorlevel%
dumpbin /dependents "C:\Projetos\PapinhoAccelerator\build\windows-service-protected-runtime-20261023\Selected\Release\papacc_server_lab_host.exe" > "C:\Projetos\_tmp\PapinhoAccelerator\scm-lab-start-stop-20261023\host-imports.txt"
if errorlevel 1 exit /b %errorlevel%
dumpbin /dependents "C:\Projetos\PapinhoAccelerator\build\scm-protected-installation-binding-20261022\Selected\Release\libssl-3-x64.dll" > "C:\Projetos\_tmp\PapinhoAccelerator\scm-lab-start-stop-20261023\libssl-imports.txt"
if errorlevel 1 exit /b %errorlevel%
dumpbin /dependents "C:\Projetos\PapinhoAccelerator\build\scm-protected-installation-binding-20261022\Selected\Release\libcrypto-3-x64.dll" > "C:\Projetos\_tmp\PapinhoAccelerator\scm-lab-start-stop-20261023\libcrypto-imports.txt"
if errorlevel 1 exit /b %errorlevel%
"C:\Projetos\_tmp\PapinhoAccelerator\scm-lab-start-stop-20261023\dependency-readonly.exe" > "C:\Projetos\_tmp\PapinhoAccelerator\scm-lab-start-stop-20261023\dependency-readonly.txt"
exit /b %errorlevel%

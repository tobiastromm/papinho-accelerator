@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cd /d C:\Projetos\PapinhoAccelerator
dumpbin /disasm build\root08-issuer-factual-transaction-binding-20261020\Selected\Debug\CMakeFiles\papacc_configuration_commit.dir\apps\server\server_configuration_commit_win32.c.obj > "C:\Users\tobias\AppData\Local\Temp\pacc-v4-factual-source-ed11-20261022\worker-Debug-disasm.txt"
dumpbin /disasm build\root08-issuer-factual-transaction-binding-20261020\Selected\Release\CMakeFiles\papacc_configuration_commit.dir\apps\server\server_configuration_commit_win32.c.obj > "C:\Users\tobias\AppData\Local\Temp\pacc-v4-factual-source-ed11-20261022\worker-Release-disasm.txt"

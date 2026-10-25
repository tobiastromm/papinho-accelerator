@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cd /d C:\Projetos\PapinhoAccelerator
dumpbin /disasm build\root08-issuer-factual-transaction-binding-20261020\Selected\Debug\CMakeFiles\papacc_server_run_win32.dir\apps\server\server_config_source_binding_win32.c.obj > "C:\Users\tobias\AppData\Local\Temp\pacc-bootstrap-scratch-20261022/binding-Debug-disasm.txt"
if errorlevel 1 exit /b %errorlevel%
dumpbin /disasm build\root08-issuer-factual-transaction-binding-20261020\Selected\Release\CMakeFiles\papacc_server_run_win32.dir\apps\server\server_config_source_binding_win32.c.obj > "C:\Users\tobias\AppData\Local\Temp\pacc-bootstrap-scratch-20261022/binding-Release-disasm.txt"
if errorlevel 1 exit /b %errorlevel%

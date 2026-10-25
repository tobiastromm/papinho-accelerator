@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cd /d C:\Projetos\PapinhoAccelerator
cmake --build build\root08-issuer-factual-transaction-binding-20261020\Selected\Debug --target papacc_issuer_root_store_test
if errorlevel 1 exit /b %errorlevel%
build\root08-issuer-factual-transaction-binding-20261020\Selected\Debug\papacc_issuer_root_store_test.exe --codec-only "C:\Users\tobias\AppData\Local\Temp\pacc-prepared-storage-20261022\root06-Debug-48119d3e\codec-only"
if errorlevel 1 exit /b %errorlevel%
build\root08-issuer-factual-transaction-binding-20261020\Selected\Debug\papacc_issuer_root_store_test.exe --pending-success "C:\Users\tobias\AppData\Local\Temp\pacc-prepared-storage-20261022\root06-Debug-48119d3e\pending-success"
if errorlevel 1 exit /b %errorlevel%
build\root08-issuer-factual-transaction-binding-20261020\Selected\Debug\papacc_issuer_root_store_test.exe --pending-conflict "C:\Users\tobias\AppData\Local\Temp\pacc-prepared-storage-20261022\root06-Debug-48119d3e\pending-conflict"
if errorlevel 1 exit /b %errorlevel%
build\root08-issuer-factual-transaction-binding-20261020\Selected\Debug\papacc_issuer_root_store_test.exe --pending-unknown "C:\Users\tobias\AppData\Local\Temp\pacc-prepared-storage-20261022\root06-Debug-48119d3e\pending-unknown"
if errorlevel 1 exit /b %errorlevel%

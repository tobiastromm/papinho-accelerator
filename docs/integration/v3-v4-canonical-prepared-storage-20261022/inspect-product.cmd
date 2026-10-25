@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cd /d C:\Projetos\PapinhoAccelerator
dumpbin /symbols build\root08-issuer-factual-transaction-binding-20261020\Selected\Debug\papacc_storage_sha256.lib > "C:\Users\tobias\AppData\Local\Temp\pacc-prepared-storage-20261022/product-capsule-Debug-symbols.txt"
if errorlevel 1 exit /b %errorlevel%
dumpbin /headers build\root08-issuer-factual-transaction-binding-20261020\Selected\Debug\papacc_storage_sha256.lib > "C:\Users\tobias\AppData\Local\Temp\pacc-prepared-storage-20261022/product-capsule-Debug-headers.txt"
if errorlevel 1 exit /b %errorlevel%
dumpbin /symbols build\root08-issuer-factual-transaction-binding-20261020\Selected\Release\papacc_storage_sha256.lib > "C:\Users\tobias\AppData\Local\Temp\pacc-prepared-storage-20261022/product-capsule-Release-symbols.txt"
if errorlevel 1 exit /b %errorlevel%
dumpbin /headers build\root08-issuer-factual-transaction-binding-20261020\Selected\Release\papacc_storage_sha256.lib > "C:\Users\tobias\AppData\Local\Temp\pacc-prepared-storage-20261022/product-capsule-Release-headers.txt"
if errorlevel 1 exit /b %errorlevel%

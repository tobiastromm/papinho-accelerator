@echo off
cd /d C:\Projetos\PapinhoAccelerator
build\root08-issuer-factual-transaction-binding-20261020\Selected\Release\papacc_v3_transaction_worker_test.exe success "C:\Users\tobias\AppData\Local\Temp\pacc-prepared-storage-20261022\native-worker-Release-f5be1f75\success"
if errorlevel 1 exit /b %errorlevel%
build\root08-issuer-factual-transaction-binding-20261020\Selected\Release\papacc_v3_transaction_worker_test.exe conflict "C:\Users\tobias\AppData\Local\Temp\pacc-prepared-storage-20261022\native-worker-Release-f5be1f75\conflict"
if errorlevel 1 exit /b %errorlevel%
build\root08-issuer-factual-transaction-binding-20261020\Selected\Release\papacc_v3_transaction_worker_test.exe unknown "C:\Users\tobias\AppData\Local\Temp\pacc-prepared-storage-20261022\native-worker-Release-f5be1f75\unknown"
if errorlevel 1 exit /b %errorlevel%
build\root08-issuer-factual-transaction-binding-20261020\Selected\Release\papacc_v3_transaction_worker_test.exe stop "C:\Users\tobias\AppData\Local\Temp\pacc-prepared-storage-20261022\native-worker-Release-f5be1f75\stop"
if errorlevel 1 exit /b %errorlevel%

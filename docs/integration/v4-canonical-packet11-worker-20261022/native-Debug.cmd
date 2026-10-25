@echo off
cd /d C:\Projetos\PapinhoAccelerator
build\root08-issuer-factual-transaction-binding-20261020\Selected\Debug\papacc_v4_transaction_worker_test.exe success "C:\Users\tobias\AppData\Local\Temp\pacc-v4-packet11-20261022\native-Debug-e785538a\3success" 3
if errorlevel 1 exit /b %errorlevel%
build\root08-issuer-factual-transaction-binding-20261020\Selected\Debug\papacc_v4_transaction_worker_test.exe conflict "C:\Users\tobias\AppData\Local\Temp\pacc-v4-packet11-20261022\native-Debug-e785538a\3conflict" 3
if errorlevel 1 exit /b %errorlevel%
build\root08-issuer-factual-transaction-binding-20261020\Selected\Debug\papacc_v4_transaction_worker_test.exe unknown "C:\Users\tobias\AppData\Local\Temp\pacc-v4-packet11-20261022\native-Debug-e785538a\3unknown" 3
if errorlevel 1 exit /b %errorlevel%
build\root08-issuer-factual-transaction-binding-20261020\Selected\Debug\papacc_v4_transaction_worker_test.exe stop "C:\Users\tobias\AppData\Local\Temp\pacc-v4-packet11-20261022\native-Debug-e785538a\3stop" 3
if errorlevel 1 exit /b %errorlevel%
build\root08-issuer-factual-transaction-binding-20261020\Selected\Debug\papacc_v4_transaction_worker_test.exe success "C:\Users\tobias\AppData\Local\Temp\pacc-v4-packet11-20261022\native-Debug-e785538a\4success" 4
if errorlevel 1 exit /b %errorlevel%
build\root08-issuer-factual-transaction-binding-20261020\Selected\Debug\papacc_v4_transaction_worker_test.exe conflict "C:\Users\tobias\AppData\Local\Temp\pacc-v4-packet11-20261022\native-Debug-e785538a\4conflict" 4
if errorlevel 1 exit /b %errorlevel%
build\root08-issuer-factual-transaction-binding-20261020\Selected\Debug\papacc_v4_transaction_worker_test.exe unknown "C:\Users\tobias\AppData\Local\Temp\pacc-v4-packet11-20261022\native-Debug-e785538a\4unknown" 4
if errorlevel 1 exit /b %errorlevel%
build\root08-issuer-factual-transaction-binding-20261020\Selected\Debug\papacc_v4_transaction_worker_test.exe stop "C:\Users\tobias\AppData\Local\Temp\pacc-v4-packet11-20261022\native-Debug-e785538a\4stop" 4
if errorlevel 1 exit /b %errorlevel%
build\root08-issuer-factual-transaction-binding-20261020\Selected\Debug\papacc_v3_transaction_worker_test.exe success "C:\Users\tobias\AppData\Local\Temp\pacc-v4-packet11-20261022\native-Debug-e785538a\v3success"
if errorlevel 1 exit /b %errorlevel%
build\root08-issuer-factual-transaction-binding-20261020\Selected\Debug\papacc_v3_transaction_worker_test.exe conflict "C:\Users\tobias\AppData\Local\Temp\pacc-v4-packet11-20261022\native-Debug-e785538a\v3conflict"
if errorlevel 1 exit /b %errorlevel%
build\root08-issuer-factual-transaction-binding-20261020\Selected\Debug\papacc_v3_transaction_worker_test.exe unknown "C:\Users\tobias\AppData\Local\Temp\pacc-v4-packet11-20261022\native-Debug-e785538a\v3unknown"
if errorlevel 1 exit /b %errorlevel%
build\root08-issuer-factual-transaction-binding-20261020\Selected\Debug\papacc_v3_transaction_worker_test.exe stop "C:\Users\tobias\AppData\Local\Temp\pacc-v4-packet11-20261022\native-Debug-e785538a\v3stop"
if errorlevel 1 exit /b %errorlevel%

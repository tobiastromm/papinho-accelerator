@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 if errorlevel 1 exit /b %errorlevel%
cd /d C:\Projetos\PapinhoAccelerator
cmake --build build/root08-issuer-factual-transaction-binding-20261020/Selected/Release --target papacc_v4_transaction_worker_test papacc_config_root8_test papacc_config_root8_v2_test papacc_config_root8_v3_test papacc_config_root8_v4_test papacc_config_lst1_test papacc_phase6_memory_census_test papacc_store_hash_owner_test papacc_store_hash_lifetime_test papacc_prepared_storage_model_test papacc_prepared_storage_native_test papacc_v3_update_cohort_test papacc_v3_backing_test papacc_v4_backing_test papacc_kind8_dispatch_model_test papacc_kind8_dispatch_native_test papacc_v3_current_negative_test papacc_v3_transaction_worker_test papacc_root8_kind8_model_test papacc_root8_kind8_native_gate_test
if errorlevel 1 if errorlevel 1 exit /b %errorlevel%
ctest --test-dir build/root08-issuer-factual-transaction-binding-20261020/Selected/Release --output-on-failure --timeout 180 -R "papacc.config_root8|papacc.config_lst1|papacc.phase6.memory_census|papacc.store_hash|papacc.prepared_storage|papacc.v3.update_cohort|papacc.v[34].immutable_backings|papacc.kind8.dispatch|papacc.v3.current_negative|papacc.root8.kind8"
if errorlevel 1 exit /b %errorlevel%
build\root08-issuer-factual-transaction-binding-20261020\Selected\Release\papacc_v4_transaction_worker_test.exe success "C:\Users\tobias\AppData\Local\Temp\pacc-v4-packet11-20261022\final-native-Release-63503957\3success" 3
if errorlevel 1 exit /b %errorlevel%
build\root08-issuer-factual-transaction-binding-20261020\Selected\Release\papacc_v4_transaction_worker_test.exe conflict "C:\Users\tobias\AppData\Local\Temp\pacc-v4-packet11-20261022\final-native-Release-63503957\3conflict" 3
if errorlevel 1 exit /b %errorlevel%
build\root08-issuer-factual-transaction-binding-20261020\Selected\Release\papacc_v4_transaction_worker_test.exe identity "C:\Users\tobias\AppData\Local\Temp\pacc-v4-packet11-20261022\final-native-Release-63503957\3identity" 3
if errorlevel 1 exit /b %errorlevel%
build\root08-issuer-factual-transaction-binding-20261020\Selected\Release\papacc_v4_transaction_worker_test.exe unknown "C:\Users\tobias\AppData\Local\Temp\pacc-v4-packet11-20261022\final-native-Release-63503957\3unknown" 3
if errorlevel 1 exit /b %errorlevel%
build\root08-issuer-factual-transaction-binding-20261020\Selected\Release\papacc_v4_transaction_worker_test.exe stop "C:\Users\tobias\AppData\Local\Temp\pacc-v4-packet11-20261022\final-native-Release-63503957\3stop" 3
if errorlevel 1 exit /b %errorlevel%
build\root08-issuer-factual-transaction-binding-20261020\Selected\Release\papacc_v4_transaction_worker_test.exe success "C:\Users\tobias\AppData\Local\Temp\pacc-v4-packet11-20261022\final-native-Release-63503957\4success" 4
if errorlevel 1 exit /b %errorlevel%
build\root08-issuer-factual-transaction-binding-20261020\Selected\Release\papacc_v4_transaction_worker_test.exe conflict "C:\Users\tobias\AppData\Local\Temp\pacc-v4-packet11-20261022\final-native-Release-63503957\4conflict" 4
if errorlevel 1 exit /b %errorlevel%
build\root08-issuer-factual-transaction-binding-20261020\Selected\Release\papacc_v4_transaction_worker_test.exe identity "C:\Users\tobias\AppData\Local\Temp\pacc-v4-packet11-20261022\final-native-Release-63503957\4identity" 4
if errorlevel 1 exit /b %errorlevel%
build\root08-issuer-factual-transaction-binding-20261020\Selected\Release\papacc_v4_transaction_worker_test.exe unknown "C:\Users\tobias\AppData\Local\Temp\pacc-v4-packet11-20261022\final-native-Release-63503957\4unknown" 4
if errorlevel 1 exit /b %errorlevel%
build\root08-issuer-factual-transaction-binding-20261020\Selected\Release\papacc_v4_transaction_worker_test.exe stop "C:\Users\tobias\AppData\Local\Temp\pacc-v4-packet11-20261022\final-native-Release-63503957\4stop" 4
if errorlevel 1 exit /b %errorlevel%
build\root08-issuer-factual-transaction-binding-20261020\Selected\Release\papacc_v3_transaction_worker_test.exe success "C:\Users\tobias\AppData\Local\Temp\pacc-v4-packet11-20261022\final-native-Release-63503957\v3success"
if errorlevel 1 exit /b %errorlevel%
build\root08-issuer-factual-transaction-binding-20261020\Selected\Release\papacc_v3_transaction_worker_test.exe conflict "C:\Users\tobias\AppData\Local\Temp\pacc-v4-packet11-20261022\final-native-Release-63503957\v3conflict"
if errorlevel 1 exit /b %errorlevel%
build\root08-issuer-factual-transaction-binding-20261020\Selected\Release\papacc_v3_transaction_worker_test.exe unknown "C:\Users\tobias\AppData\Local\Temp\pacc-v4-packet11-20261022\final-native-Release-63503957\v3unknown"
if errorlevel 1 exit /b %errorlevel%
build\root08-issuer-factual-transaction-binding-20261020\Selected\Release\papacc_v3_transaction_worker_test.exe stop "C:\Users\tobias\AppData\Local\Temp\pacc-v4-packet11-20261022\final-native-Release-63503957\v3stop"
if errorlevel 1 exit /b %errorlevel%
exit /b 0

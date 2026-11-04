@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat" >nul
cmake --build build/v3-backings-20261021/Selected/Debug --target papacc_server papacc_config_root8_v5_test papacc_config_root8_v4_test papacc_configuration_commit papacc_v5_transaction_worker_test papacc_v4_transaction_worker_test --parallel 4 > "docs\integration\n0start03-integrated-closeout-20261024\l1-accepted-tpm-manual-readiness-20261027\qualify-Selected-Debug.log" 2>&1
exit /b %errorlevel%

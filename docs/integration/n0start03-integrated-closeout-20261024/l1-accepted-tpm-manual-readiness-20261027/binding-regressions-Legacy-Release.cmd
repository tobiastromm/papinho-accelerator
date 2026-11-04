@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat" >nul
cmake --build build/v3-backings-20261021/Legacy/Release --target papacc_q4_root_binding_data_test papacc_ed11_profile_selected_test papacc_ed11_profile_legacy_test papacc_ed11_credit_selected_test papacc_ed11_credit_legacy_test --parallel 4 > "docs\integration\n0start03-integrated-closeout-20261024\l1-accepted-tpm-manual-readiness-20261027\binding-regressions-Legacy-Release.log" 2>&1
exit /b %errorlevel%

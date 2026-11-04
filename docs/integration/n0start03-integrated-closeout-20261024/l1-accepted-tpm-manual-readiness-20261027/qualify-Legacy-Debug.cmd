@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat" >nul
cmake --build build/v3-backings-20261021/Legacy/Debug --target papacc_server papacc_config_root8_v5_test papacc_config_root8_v4_test papacc_configuration_commit --parallel 4 > "docs\integration\n0start03-integrated-closeout-20261024\l1-accepted-tpm-manual-readiness-20261027\qualify-Legacy-Debug.log" 2>&1
exit /b %errorlevel%

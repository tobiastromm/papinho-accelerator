@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat" >nul
cmake -S . -B build/v3-backings-20261021/Selected/Debug -DPAPACC_ENABLE_NATIVE_TPM_PROOF_CANDIDATE=ON -DPAPACC_TPM_NATIVE_HOST_BOUND_TESTS=ON > "docs\integration\n0start03-integrated-closeout-20261024\native-tpm-host-e2e-20261027\Debug-configure.log" 2>&1
if errorlevel 1 exit /b 1
cmake --build build/v3-backings-20261021/Selected/Debug --target papacc_tpm_native_proof papacc_tpm_native_core_test papacc_tpm_native_platform_test papacc_tpm_native_artifact_test papacc_tpm_native_e2e_fixture --parallel 4 > "docs\integration\n0start03-integrated-closeout-20261024\native-tpm-host-e2e-20261027\Debug-integrated-build.log" 2>&1
exit /b %errorlevel%

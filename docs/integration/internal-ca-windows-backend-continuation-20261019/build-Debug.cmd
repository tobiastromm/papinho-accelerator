@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cmake -S . -B build/internal-ca-windows-backend-continuation-20261019/Debug -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_FLAGS=/WX -DBUILD_TESTING=ON -DPAPACC_ENABLE_PST_OUTBOUND_PROOF=OFF -DPAPACC_ENABLE_CANONICAL_API3_CANDIDATE=OFF
if errorlevel 1 exit /b %errorlevel%
cmake --build build/internal-ca-windows-backend-continuation-20261019/Debug --target papacc_issuer_windows_validation_test papacc_issuer papacc_issuer_offline_test papacc_issuer_production_gate_test papacc_issuer_checkpoint_test
exit /b %errorlevel%

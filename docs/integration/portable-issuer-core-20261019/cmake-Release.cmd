@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cmake -S . -B build/portable-issuer-core-20261019/cmake-Release -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_FLAGS=/WX -DBUILD_TESTING=ON -DPAPACC_ENABLE_PST_OUTBOUND_PROOF=OFF -DPAPACC_ENABLE_CANONICAL_API3_CANDIDATE=OFF
if errorlevel 1 exit /b %errorlevel%
cmake --build build/portable-issuer-core-20261019/cmake-Release --target papacc_issuer papacc_issuer_offline_test papacc_issuer_production_gate_test
exit /b %errorlevel%

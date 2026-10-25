@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cd /d "C:\Projetos\PapinhoAccelerator"
cmake --build build/v3-current-e0-20261021/Selected/Debug --target papacc_roughtime_test
if errorlevel 1 exit /b %errorlevel%
ctest --test-dir build/v3-current-e0-20261021/Selected/Debug --output-on-failure -R "papacc.issuer.roughtime_data|papacc.issuer.qualified_utc_data" --timeout 60
if errorlevel 1 exit /b %errorlevel%
cmake -S tests/roughtime -B build/roughtime-offline-20261021/Debug -G Ninja -DCMAKE_BUILD_TYPE=Debug -DPAPACC_ROUGHTIME_OPENSSL_358_OFFLINE=ON -DPAPACC_PACKAGE_OPENSSL_ROOT=C:/Projetos/PapinhoSecureTransport/third_party/openssl/prebuilt/win32-x64-msvc-19.51-openssl3/3.5.8
if errorlevel 1 exit /b %errorlevel%
cmake --build build/roughtime-offline-20261021/Debug
if errorlevel 1 exit /b %errorlevel%
ctest --test-dir build/roughtime-offline-20261021/Debug --output-on-failure --timeout 60
exit /b %errorlevel%

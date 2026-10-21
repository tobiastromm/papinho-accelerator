@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cmake -S tests/manual_package_validation -B build/manual-package-trust-authenticated-validation-20261020/Selected/Debug -G Ninja -DCMAKE_BUILD_TYPE=Debug -DPAPACC_ENABLE_WINDOWS_3MIB_MANAGED_PROFILE=ON -DPAPACC_MANUAL_CRYPTO_OPENSSL_358_OFFLINE=ON -DPAPACC_PACKAGE_OPENSSL_ROOT=C:/Projetos/PapinhoSecureTransport/third_party/openssl/prebuilt/win32-x64-msvc-19.51-openssl3/3.5.8
if errorlevel 1 exit /b %errorlevel%
cmake --build build/manual-package-trust-authenticated-validation-20261020/Selected/Debug
if errorlevel 1 exit /b %errorlevel%
ctest --test-dir build/manual-package-trust-authenticated-validation-20261020/Selected/Debug --output-on-failure --timeout 30
exit /b %errorlevel%

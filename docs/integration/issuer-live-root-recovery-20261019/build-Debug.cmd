@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cmake -S . -B build/issuer-live-root-recovery-20261019/Debug -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_FLAGS=/WX -DBUILD_TESTING=ON -DPAPACC_ENABLE_CANONICAL_API3_CANDIDATE=ON -DPAPACC_ENABLE_CANONICAL_C8_ROOT=ON -DPAPACC_ENABLE_PST_OUTBOUND_PROOF=ON -DPAPACC_PST_QUALIFICATION_SDK_ROOT=C:/Projetos/PapinhoAccelerator/build/ed08-r4-integrated-20261012/sdk-normal -DPAPACC_PST_QUALIFICATION_SDK_ARCHIVE=C:/Projetos/PapinhoSecureTransport/dist/packages/0.6.5/papinho-secure-transport-0.6.5-win32-x64-msvc-19.51-openssl3.zip
if errorlevel 1 exit /b %errorlevel%
cmake --build build/issuer-live-root-recovery-20261019/Debug --target papacc_issuer_root_store_test papacc_issuer_root_guard_test papacc_issuer_offline_test papacc_issuer_production_gate_test papacc_site_tls_catalog_offline_test papacc_canonical_root_layout_test papacc_canonical_root_transaction_test papacc_server
exit /b %errorlevel%

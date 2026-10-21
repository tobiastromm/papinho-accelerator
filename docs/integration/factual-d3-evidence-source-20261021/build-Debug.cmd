@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cmake -S . -B build/fd3-20261021/Selected/Debug -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_FLAGS=/WX -DBUILD_TESTING=ON -DPAPACC_ENABLE_WINDOWS_3MIB_MANAGED_PROFILE=ON -DPAPACC_ENABLE_DORMANT_C2_PARTICIPANT=OFF -DPAPACC_ENABLE_OFFLINE_ROOT8=ON -DPAPACC_ENABLE_OFFLINE_D3_WORKER=ON -DPAPACC_ENABLE_OFFLINE_LST1=ON -DPAPACC_ENABLE_OFFLINE_CONFIG_SOURCE=ON -DPAPACC_ENABLE_CONFIG_ADMIN_BINDING=ON -DPAPACC_ENABLE_CANONICAL_API3_CANDIDATE=ON -DPAPACC_ENABLE_CANONICAL_C8_ROOT=ON -DPAPACC_ENABLE_PST_OUTBOUND_PROOF=ON -DPAPACC_PST_QUALIFICATION_SDK_ROOT=C:/Projetos/PapinhoAccelerator/build/ed08-r4-integrated-20261012/sdk-normal -DPAPACC_PST_QUALIFICATION_SDK_ARCHIVE=C:/Projetos/PapinhoSecureTransport/dist/packages/0.6.5/papinho-secure-transport-0.6.5-win32-x64-msvc-19.51-openssl3.zip
if errorlevel 1 exit /b %errorlevel%
cmake --build build/fd3-20261021/Selected/Debug --target papacc_config_root8_v3_test papacc_config_root8_v2_test papacc_config_root8_test papacc_config_lst1_test papacc_canonical_root_layout_test papacc_issuer_offline_test papacc_issuer_production_gate_test papacc_issuer_checkpoint_test papacc_phase6_memory_census_test papacc_issuer_root_store_test papacc_ed11_profile_selected_test papacc_ed11_profile_legacy_test papacc_ed11_profile_other_test papacc_ed11_credit_selected_test papacc_ed11_credit_legacy_test
if errorlevel 1 exit /b %errorlevel%
ctest --test-dir build/fd3-20261021/Selected/Debug --output-on-failure --timeout 90 -R "papacc.config_root8|papacc.config_lst1|papacc.issuer.offline|papacc.issuer.production_gate|papacc.issuer.checkpoint|papacc.canonical.root_layout|papacc.ed11|papacc.phase6.memory_census"
exit /b %errorlevel%

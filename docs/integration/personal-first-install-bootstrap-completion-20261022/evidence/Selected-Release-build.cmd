@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cmake -S . -B build/personal-bootstrap-completion-20261022/Selected/Release -G Ninja -DCMAKE_C_FLAGS=/WX -DBUILD_TESTING=ON -DPAPACC_ENABLE_CONFIG_ADMIN_CLI=ON -DPAPACC_ENABLE_OFFLINE_ROOT8=ON -DPAPACC_ENABLE_OFFLINE_D3_WORKER=ON -DPAPACC_ENABLE_OFFLINE_LST1=ON -DPAPACC_ENABLE_OFFLINE_CONFIG_SOURCE=ON -DPAPACC_ENABLE_CONFIG_ADMIN_BINDING=ON -DPAPACC_ENABLE_CANONICAL_API3_CANDIDATE=ON -DPAPACC_ENABLE_CANONICAL_C8_ROOT=ON -DPAPACC_ENABLE_PST_OUTBOUND_PROOF=ON -DPAPACC_PST_QUALIFICATION_SDK_ROOT=C:/Projetos/PapinhoAccelerator/build/ed08-r4-integrated-20261012/sdk-normal -DPAPACC_PST_QUALIFICATION_SDK_ARCHIVE=C:/Projetos/PapinhoSecureTransport/dist/packages/0.6.5/papinho-secure-transport-0.6.5-win32-x64-msvc-19.51-openssl3.zip -DCMAKE_BUILD_TYPE=Release -DPAPACC_ENABLE_WINDOWS_3MIB_MANAGED_PROFILE=ON
if errorlevel 1 exit /b %errorlevel%
cmake --build build/personal-bootstrap-completion-20261022/Selected/Release --parallel 2 --target papacc_personal_effect_plan_test papacc_server papacc_personal_target_component_test papacc_personal_native_test papacc_personal_plan_data_test papacc_bootstrap_scratch_test papacc_config_binding_test papacc_config_binding_native_test papacc_phase6_memory_census_test papacc_store_hash_owner_test papacc_store_hash_lifetime_test papacc_issuer_production_gate_test papacc_issuer_cng_production_gate_test
exit /b %errorlevel%

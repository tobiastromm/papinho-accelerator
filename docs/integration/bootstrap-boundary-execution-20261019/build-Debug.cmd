@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cmake -S . -B build/bootstrap-boundary-execution-20261019/Debug -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_FLAGS=/WX -DBUILD_TESTING=ON -DPAPACC_ENABLE_CONFIG_ADMIN_CLI=ON -DPAPACC_ENABLE_OFFLINE_ROOT8=ON -DPAPACC_ENABLE_OFFLINE_D3_WORKER=ON -DPAPACC_ENABLE_OFFLINE_LST1=ON -DPAPACC_ENABLE_OFFLINE_CONFIG_SOURCE=ON -DPAPACC_ENABLE_CONFIG_ADMIN_BINDING=ON -DPAPACC_ENABLE_CANONICAL_API3_CANDIDATE=ON -DPAPACC_ENABLE_CANONICAL_C8_ROOT=ON -DPAPACC_ENABLE_PST_OUTBOUND_PROOF=ON -DPAPACC_PST_QUALIFICATION_SDK_ROOT=C:/Projetos/PapinhoAccelerator/build/ed08-r4-integrated-20261012/sdk-normal -DPAPACC_PST_QUALIFICATION_SDK_ARCHIVE=C:/Projetos/PapinhoSecureTransport/dist/packages/0.6.5/papinho-secure-transport-0.6.5-win32-x64-msvc-19.51-openssl3.zip
if errorlevel 1 exit /b %errorlevel%
cmake --build build/bootstrap-boundary-execution-20261019/Debug --target papacc_bootstrap_namespace_test papacc_server_cli_test papacc_d3_worker_test papacc_issuer_root_store_test papacc_issuer_root_guard_test papacc_config_root8_test papacc_config_lst1_test papacc_config_source_test papacc_config_binding_test papacc_config_binding_native_test papacc_server papacc_canonical_root_layout_test papacc_persistent_bind_selection_test papacc_network_interface_test papacc_bind_target_test papacc_authentication_authorization_test papacc_network_discovery_win32_test papacc_canonical_root_transaction_test
exit /b %errorlevel%

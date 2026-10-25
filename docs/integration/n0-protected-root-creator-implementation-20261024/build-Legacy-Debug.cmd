@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cmake -S . -B C:/Projetos/PapinhoAccelerator/build/n0-protected-root-creator-20261024/Legacy/Debug -G Ninja "-DCMAKE_C_FLAGS=/WX /DPAPACC_PSR_TRACE_LAB=1 /DPAPACC_PSR_FILE_RUNTIME_LAB=1" -DBUILD_TESTING=ON -DPAPACC_ENABLE_SINGLE_LAB_ADMISSION=ON -DPAPACC_ENABLE_CONFIG_ADMIN_CLI=ON -DPAPACC_ENABLE_OFFLINE_ROOT8=ON -DPAPACC_ENABLE_OFFLINE_D3_WORKER=ON -DPAPACC_ENABLE_OFFLINE_LST1=ON -DPAPACC_ENABLE_OFFLINE_CONFIG_SOURCE=ON -DPAPACC_ENABLE_CONFIG_ADMIN_BINDING=ON -DPAPACC_ENABLE_CANONICAL_API3_CANDIDATE=ON -DPAPACC_ENABLE_CANONICAL_C8_ROOT=ON -DPAPACC_ENABLE_PST_OUTBOUND_PROOF=ON -DPAPACC_PST_QUALIFICATION_SDK_ROOT=C:/Projetos/PapinhoAccelerator/build/ed08-r4-integrated-20261012/sdk-normal -DPAPACC_PST_QUALIFICATION_SDK_ARCHIVE=C:/Projetos/PapinhoSecureTransport/dist/packages/0.6.5/papinho-secure-transport-0.6.5-win32-x64-msvc-19.51-openssl3.zip -DCMAKE_BUILD_TYPE=Debug -DPAPACC_ENABLE_WINDOWS_3MIB_MANAGED_PROFILE=OFF
if errorlevel 1 exit /b %errorlevel%
cmake --build C:/Projetos/PapinhoAccelerator/build/n0-protected-root-creator-20261024/Legacy/Debug --parallel 2 --target papacc_n0_root_creator papacc_n0_root_model_test
exit /b %errorlevel%

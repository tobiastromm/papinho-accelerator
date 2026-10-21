@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cmake -S . -B build/ed11-windows-3mib-profile-20261020/IsolatedDormant/Release -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_FLAGS=/WX -DBUILD_TESTING=ON -DPAPACC_ENABLE_WINDOWS_3MIB_MANAGED_PROFILE=OFF -DPAPACC_ENABLE_DORMANT_C2_PARTICIPANT=ON -DPAPACC_ENABLE_OFFLINE_ROOT8=OFF -DPAPACC_ENABLE_OFFLINE_D3_WORKER=OFF -DPAPACC_ENABLE_OFFLINE_LST1=OFF -DPAPACC_ENABLE_OFFLINE_CONFIG_SOURCE=OFF -DPAPACC_ENABLE_CONFIG_ADMIN_BINDING=OFF -DPAPACC_ENABLE_CANONICAL_API3_CANDIDATE=ON -DPAPACC_ENABLE_CANONICAL_C8_ROOT=OFF -DPAPACC_ENABLE_PST_OUTBOUND_PROOF=ON -DPAPACC_PST_QUALIFICATION_SDK_ROOT=C:/Projetos/PapinhoAccelerator/build/ed08-r4-integrated-20261012/sdk-normal -DPAPACC_PST_QUALIFICATION_SDK_ARCHIVE=C:/Projetos/PapinhoSecureTransport/dist/packages/0.6.5/papinho-secure-transport-0.6.5-win32-x64-msvc-19.51-openssl3.zip
if errorlevel 1 exit /b %errorlevel%
cmake --build build/ed11-windows-3mib-profile-20261020/IsolatedDormant/Release --target papacc_c2_dormant_test
exit /b %errorlevel%

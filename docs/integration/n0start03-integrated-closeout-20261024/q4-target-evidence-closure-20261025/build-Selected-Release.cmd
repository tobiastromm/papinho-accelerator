@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cmake -S . -B C:/Projetos/PapinhoAccelerator/build/q4-target-evidence-20261025/Selected/Release -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_C_FLAGS=/WX -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON -DPAPACC_ENABLE_CANONICAL_API3_CANDIDATE=ON -DPAPACC_ENABLE_CANONICAL_C8_ROOT=ON -DPAPACC_ENABLE_PST_OUTBOUND_PROOF=ON -DPAPACC_PST_QUALIFICATION_SDK_ARCHIVE=C:/Projetos/PapinhoSecureTransport/dist/packages/0.6.5/papinho-secure-transport-0.6.5-win32-x64-msvc-19.51-openssl3.zip -DPAPACC_PST_QUALIFICATION_SDK_ROOT=C:/Projetos/PapinhoAccelerator/build/ed08-r4-integrated-20261012/sdk-normal -DPAPACC_ENABLE_SINGLE_LAB_ADMISSION=OFF -DPAPACC_ENABLE_WINDOWS_3MIB_MANAGED_PROFILE=ON
if errorlevel 1 exit /b %errorlevel%
cmake --build C:/Projetos/PapinhoAccelerator/build/q4-target-evidence-20261025/Selected/Release --parallel 2 --target papacc_server papacc_target_typed_evidence_test papacc_process_provenance_offline_test papacc_pair_receipt_offline_test
exit /b %errorlevel%

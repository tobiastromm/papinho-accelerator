@echo off
set VSLANG=1033
chcp 65001 >nul
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
C:\Strawberry\c\bin\cmake.exe -S . -B build\phase6-canonical-consolidation-20261018\MinimalPairDebug -G Ninja -DCMAKE_MAKE_PROGRAM=C:/Strawberry/c/bin/ninja.exe -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON -DPAPACC_ENABLE_CANONICAL_API3_CANDIDATE=ON -DPAPACC_ENABLE_CANONICAL_C8_ROOT=ON -DPAPACC_ENABLE_PST_OUTBOUND_PROOF=ON -DPAPACC_PST_QUALIFICATION_SDK_ARCHIVE=C:/Projetos/PapinhoSecureTransport/dist/packages/0.6.5/papinho-secure-transport-0.6.5-win32-x64-msvc-19.51-openssl3.zip -DPAPACC_PST_QUALIFICATION_SDK_ROOT=C:/Projetos/PapinhoAccelerator/build/ed08-r4-integrated-20261012/sdk-normal
if errorlevel 1 exit /b %errorlevel%
C:\Strawberry\c\bin\cmake.exe --build build\phase6-canonical-consolidation-20261018\MinimalPairDebug --target papacc_server papacc_canonical_root_layout_test papacc_pair_receipt_offline_test
exit /b %errorlevel%

@echo off
call "C:\Projetos\PapinhoSecureTransport\tools\msvc-env.bat"
if errorlevel 1 exit /b %errorlevel%
"C:\Projetos\PapinhoAccelerator\build\roughtime-offline-20261021\Debug\papacc_roughtime_crypto_test.exe"
if errorlevel 1 exit /b %errorlevel%
"C:\Projetos\PapinhoAccelerator\build\roughtime-offline-20261021\Debug\papacc_roughtime_transcript_test.exe" "C:\Projetos\PapinhoAccelerator\tests\fixtures\roughtime-lab-20261021"
if errorlevel 1 exit /b %errorlevel%
"C:\Projetos\PapinhoAccelerator\build\roughtime-offline-20261021\Release\papacc_roughtime_crypto_test.exe"
if errorlevel 1 exit /b %errorlevel%
"C:\Projetos\PapinhoAccelerator\build\roughtime-offline-20261021\Release\papacc_roughtime_transcript_test.exe" "C:\Projetos\PapinhoAccelerator\tests\fixtures\roughtime-lab-20261021"
if errorlevel 1 exit /b %errorlevel%
dumpbin /headers "C:\Projetos\_tmp\PapinhoAccelerator\openssl-strict-ed25519-20261021\Debug\strict-test.exe" > "C:\Projetos\_tmp\PapinhoAccelerator\openssl-strict-ed25519-20261021\Debug\pe-headers.txt"
dumpbin /headers "C:\Projetos\_tmp\PapinhoAccelerator\openssl-strict-ed25519-20261021\Release\strict-test.exe" > "C:\Projetos\_tmp\PapinhoAccelerator\openssl-strict-ed25519-20261021\Release\pe-headers.txt"
exit /b %errorlevel%

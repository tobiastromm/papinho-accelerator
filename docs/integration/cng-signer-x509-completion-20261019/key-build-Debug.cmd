@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
if not exist "build\cng-signer-x509-completion-20261019\key-Debug" mkdir "build\cng-signer-x509-completion-20261019\key-Debug"
cl /nologo /TC /W4 /WX /Od /Zi /MDd /Iapps\server  /Fo"build\cng-signer-x509-completion-20261019\key-Debug\\" /Fd"build\cng-signer-x509-completion-20261019\key-Debug\readonly_test.pdb" tests\ca_readonly_diagnostic_test.c /Fe"build\cng-signer-x509-completion-20261019\key-Debug\readonly_test.exe" /link advapi32.lib
if errorlevel 1 exit /b %errorlevel%
cl /nologo /TC /W4 /WX /Od /Zi /MDd /Iapps\server /DPAPACC_CA_READ_ONLY_DIAGNOSTIC /Fo"build\cng-signer-x509-completion-20261019\key-Debug\\" /Fd"build\cng-signer-x509-completion-20261019\key-Debug\readonly_diagnostic.pdb" apps\server\server_ca_key_win32.c tests\ca_key_readonly_diagnostic_win32.c /Fe"build\cng-signer-x509-completion-20261019\key-Debug\readonly_diagnostic.exe" /link ncrypt.lib advapi32.lib
if errorlevel 1 exit /b %errorlevel%
cl /nologo /TC /W4 /WX /Od /Zi /MDd /Iapps\server  /Fo"build\cng-signer-x509-completion-20261019\key-Debug\\" /Fd"build\cng-signer-x509-completion-20261019\key-Debug\production_guard_test.pdb" apps\server\server_ca_key_win32.c tests\ca_key_offline_test.c /Fe"build\cng-signer-x509-completion-20261019\key-Debug\production_guard_test.exe" /link ncrypt.lib advapi32.lib
if errorlevel 1 exit /b %errorlevel%
cl /nologo /TC /W4 /WX /Od /Zi /MDd /Iapps\server  /Fo"build\cng-signer-x509-completion-20261019\key-Debug\\" /Fd"build\cng-signer-x509-completion-20261019\key-Debug\production_differential.pdb" tests\ca_failure_instrumentation_test.c tests\ca_failure_baseline_bridge.c /Fe"build\cng-signer-x509-completion-20261019\key-Debug\production_differential.exe" /link advapi32.lib
if errorlevel 1 exit /b %errorlevel%
exit /b 0

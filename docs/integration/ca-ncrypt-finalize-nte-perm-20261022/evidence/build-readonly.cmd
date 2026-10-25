@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
if not exist build\ca-finalize-readonly-diagnosis-20261022 mkdir build\ca-finalize-readonly-diagnosis-20261022
cl /nologo /TC /W4 /WX /O2 /MD /Iapps\server /DPAPACC_CA_READ_ONLY_DIAGNOSTIC /Fo"build\ca-finalize-readonly-diagnosis-20261022\\" apps\server\server_ca_key_win32.c tests\ca_key_readonly_diagnostic_win32.c /Fe"build\ca-finalize-readonly-diagnosis-20261022\readonly_diagnostic.exe" /link ncrypt.lib advapi32.lib
exit /b %errorlevel%

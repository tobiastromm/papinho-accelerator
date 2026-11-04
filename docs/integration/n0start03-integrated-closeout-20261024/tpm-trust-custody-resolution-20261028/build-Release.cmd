@echo off

call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat" >nul

if errorlevel 1 exit /b 1

cl /nologo /std:c11 /W4 /WX /GS /sdl /O2 /MD /Itools\qualification\tpm-native-proof /I"docs\integration\n0start03-integrated-closeout-20261024\tpm-trust-custody-resolution-20261028" /Fodocs\integration\n0start03-integrated-closeout-20261024\tpm-trust-custody-resolution-20261028\Release\ /Fd"docs\integration\n0start03-integrated-closeout-20261024\tpm-trust-custody-resolution-20261028\Release\adapter.pdb" /Fe"docs\integration\n0start03-integrated-closeout-20261024\tpm-trust-custody-resolution-20261028\Release\nv_adapter_test.exe" "docs\integration\n0start03-integrated-closeout-20261024\tpm-trust-custody-resolution-20261028\nv_adapter.c" "docs\integration\n0start03-integrated-closeout-20261024\tpm-trust-custody-resolution-20261028\nv_adapter_test.c" tools\qualification\tpm-native-proof\proof_core.c /link bcrypt.lib psapi.lib > "docs\integration\n0start03-integrated-closeout-20261024\tpm-trust-custody-resolution-20261028\Release\build.log" 2>&1

exit /b %errorlevel%


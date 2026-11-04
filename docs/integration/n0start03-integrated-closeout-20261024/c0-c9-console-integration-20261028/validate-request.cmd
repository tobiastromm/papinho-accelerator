@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat" >nul
cl /nologo /std:c11 /W4 /WX /GS /sdl /O2 /MD /Itools/qualification/tpm-native-proof /Fodocs\integration\n0start03-integrated-closeout-20261024\c0-c9-console-integration-20261028\layout\ /Fedocs\integration\n0start03-integrated-closeout-20261024\c0-c9-console-integration-20261028\layout\request-data-check.exe docs\integration\n0start03-integrated-closeout-20261024\c0-c9-console-integration-20261028\request-data-check.c tools/qualification/tpm-native-proof/host_platform.c build/v3-backings-20261021/Selected/Release/papacc_tpm_native_core.lib /link bcrypt.lib psapi.lib advapi32.lib crypt32.lib wevtapi.lib user32.lib
if errorlevel 1 exit /b 1
docs\integration\n0start03-integrated-closeout-20261024\c0-c9-console-integration-20261028\layout\request-data-check.exe docs\integration\n0start03-integrated-closeout-20261024\c0-c9-console-integration-20261028\manual-candidate\NATIVE-POSSESSION-OPERATION-REQUEST.DRAFT.json 59c67f6d81d88c49c1d676bde2553aa8a843a1925c76a33deb14acbab3424898
exit /b %errorlevel%

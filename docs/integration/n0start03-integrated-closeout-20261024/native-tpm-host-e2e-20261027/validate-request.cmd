@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat" >nul
cl /nologo /std:c11 /W4 /WX /GS /sdl /O2 /MD /Itools/qualification/tpm-native-proof /Fodocs\integration\n0start03-integrated-closeout-20261024\native-tpm-host-e2e-20261027\layout\ /Fedocs\integration\n0start03-integrated-closeout-20261024\native-tpm-host-e2e-20261027\layout\request-data-check.exe docs\integration\n0start03-integrated-closeout-20261024\native-tpm-host-e2e-20261027\request-data-check.c tools/qualification/tpm-native-proof/host_platform.c build/v3-backings-20261021/Selected/Release/papacc_tpm_native_core.lib /link bcrypt.lib psapi.lib advapi32.lib crypt32.lib wevtapi.lib user32.lib
if errorlevel 1 exit /b 1
docs\integration\n0start03-integrated-closeout-20261024\native-tpm-host-e2e-20261027\layout\request-data-check.exe docs\integration\n0start03-integrated-closeout-20261024\native-tpm-host-e2e-20261027\manual-candidate\NATIVE-POSSESSION-OPERATION-REQUEST.DRAFT.json 95446475cad9b4666379b75d4b850b1a77e4fdf232d240d937416593387c6857
exit /b %errorlevel%

@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat" >nul
cl /nologo /c /std:c11 /W4 /WX /GS /sdl /O2 /Ob2 /DNDEBUG /MD /FAs /Fa"docs\integration\n0start03-integrated-closeout-20261024\native-tpm-host-e2e-20261027\layout\proof_core.asm" /Fo"docs\integration\n0start03-integrated-closeout-20261024\native-tpm-host-e2e-20261027\layout\proof_core.obj" tools\qualification\tpm-native-proof\proof_core.c
if errorlevel 1 exit /b 1
cl /nologo /c /std:c11 /W4 /WX /GS /sdl /O2 /Ob2 /DNDEBUG /MD /FAs /Fa"docs\integration\n0start03-integrated-closeout-20261024\native-tpm-host-e2e-20261027\layout\host_platform.asm" /Fo"docs\integration\n0start03-integrated-closeout-20261024\native-tpm-host-e2e-20261027\layout\host_platform.obj" tools\qualification\tpm-native-proof\host_platform.c
if errorlevel 1 exit /b 1
cl /nologo /c /std:c11 /W4 /WX /GS /sdl /O2 /Ob2 /DNDEBUG /MD /FAs /Fa"docs\integration\n0start03-integrated-closeout-20261024\native-tpm-host-e2e-20261027\layout\native_host.asm" /Fo"docs\integration\n0start03-integrated-closeout-20261024\native-tpm-host-e2e-20261027\layout\native_host.obj" tools\qualification\tpm-native-proof\native_host.c
if errorlevel 1 exit /b 1
cl /nologo /c /std:c11 /W4 /WX /GS /sdl /O2 /Ob2 /DNDEBUG /MD /FAs /Fa"docs\integration\n0start03-integrated-closeout-20261024\native-tpm-host-e2e-20261027\layout\json_data.asm" /Fo"docs\integration\n0start03-integrated-closeout-20261024\native-tpm-host-e2e-20261027\layout\json_data.obj" tools\qualification\tpm-native-proof\json_data.c
if errorlevel 1 exit /b 1
dumpbin /headers /imports build\v3-backings-20261021\Selected\Release\papacc_tpm_native_proof.exe > "docs\integration\n0start03-integrated-closeout-20261024\native-tpm-host-e2e-20261027\PE-IMPORTS-RELEASE.txt"
exit /b %errorlevel%

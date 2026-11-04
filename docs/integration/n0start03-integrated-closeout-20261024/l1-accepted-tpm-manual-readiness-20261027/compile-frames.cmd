@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat" >nul
cl /nologo /c /W4 /WX /O2 /Ob2 /DNDEBUG /MD /Iinclude /Isrc/config /Isrc/issuer /FAs /Fa"docs\integration\n0start03-integrated-closeout-20261024\l1-accepted-tpm-manual-readiness-20261027\v5-Release.asm" /Fo"docs\integration\n0start03-integrated-closeout-20261024\l1-accepted-tpm-manual-readiness-20261027\v5-Release.obj" /Fd"docs\integration\n0start03-integrated-closeout-20261024\l1-accepted-tpm-manual-readiness-20261027\v5-Release.pdb" src/config/config_root8_v5.c > "docs\integration\n0start03-integrated-closeout-20261024\l1-accepted-tpm-manual-readiness-20261027\frames-Release.log" 2>&1
if errorlevel 1 exit /b 1
cl /nologo /c /W4 /WX /Zi /Od /Ob0 /RTC1 /MDd /Iinclude /Isrc/config /Isrc/issuer /FAs /Fa"docs\integration\n0start03-integrated-closeout-20261024\l1-accepted-tpm-manual-readiness-20261027\v5-Debug.asm" /Fo"docs\integration\n0start03-integrated-closeout-20261024\l1-accepted-tpm-manual-readiness-20261027\v5-Debug.obj" /Fd"docs\integration\n0start03-integrated-closeout-20261024\l1-accepted-tpm-manual-readiness-20261027\v5-Debug.pdb" src/config/config_root8_v5.c > "docs\integration\n0start03-integrated-closeout-20261024\l1-accepted-tpm-manual-readiness-20261027\frames-Debug.log" 2>&1
if errorlevel 1 exit /b 1
exit /b 0

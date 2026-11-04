@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat" >nul
if errorlevel 1 exit /b 1
cl /nologo /W4 /WX /GS /sdl /O2 /MD /Fodocs\integration\n0start03-integrated-closeout-20261024\tpm-ownerauth-factual-resolution-20261028\ /Fe"docs\integration\n0start03-integrated-closeout-20261024\tpm-ownerauth-factual-resolution-20261028\OwnerAuthAvailability.exe" "docs\integration\n0start03-integrated-closeout-20261024\tpm-ownerauth-factual-resolution-20261028\ownerauth_availability.c" > "docs\integration\n0start03-integrated-closeout-20261024\tpm-ownerauth-factual-resolution-20261028\build.log" 2>&1
exit /b %errorlevel%

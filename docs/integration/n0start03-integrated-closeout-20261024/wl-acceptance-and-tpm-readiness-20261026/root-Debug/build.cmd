@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /W4 /WX /Od /Zi /MDd /I"C:\Projetos\PapinhoAccelerator\include" /I"C:\Projetos\PapinhoAccelerator\apps\server" /I"C:\Projetos\PapinhoAccelerator\src\issuer" /I"C:\Projetos\PapinhoAccelerator\src\protocol" /I"C:\Projetos\PapinhoAccelerator\src\network" /I"C:\Projetos\PapinhoAccelerator\src\security" "C:\Projetos\PapinhoAccelerator\docs\integration\n0start03-integrated-closeout-20261024\wl-acceptance-and-tpm-readiness-20261026\q4_root_binding_test.c" /Fe:q4_root_test.exe
exit /b %errorlevel%

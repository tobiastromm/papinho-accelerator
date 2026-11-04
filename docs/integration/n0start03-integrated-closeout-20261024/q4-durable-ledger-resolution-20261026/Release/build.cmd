@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /W4 /WX /O2 /MD /I"C:\Projetos\PapinhoAccelerator\include" /I"C:\Projetos\PapinhoAccelerator\src\issuer" "C:\Projetos\PapinhoAccelerator\docs\integration\n0start03-integrated-closeout-20261024\q4-durable-ledger-resolution-20261026\issuer_checkpoint_q4_suitability_test.c" "C:\Projetos\PapinhoAccelerator\src\issuer\issuer_codec.c" /Fe:q4_checkpoint_suitability.exe /link bcrypt.lib
exit /b %errorlevel%

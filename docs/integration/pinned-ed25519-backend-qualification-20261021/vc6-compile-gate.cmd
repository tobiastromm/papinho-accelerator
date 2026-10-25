@echo off
call "C:\MSVC600-master\VC98\Bin\VCVARS32.BAT"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /TC /W4 /WX /Od /Zi /c /DSODIUM_STATIC /DNATIVE_LITTLE_ENDIAN /I"C:\Projetos\_tmp\PapinhoAccelerator\ed25519-backend-20261021\libsodium-1.0.21\src\libsodium\include\sodium" "C:\Projetos\_tmp\PapinhoAccelerator\ed25519-backend-20261021\libsodium-1.0.21\src\libsodium\crypto_core\ed25519\core_ed25519.c" /Fo"C:\Projetos\_tmp\PapinhoAccelerator\ed25519-backend-20261021\vc6-core-ed25519.obj" /Fd"C:\Projetos\_tmp\PapinhoAccelerator\ed25519-backend-20261021\vc6-core-ed25519.pdb"
exit /b %errorlevel%

@echo off
call "C:\Projetos\PapinhoSecureTransport\tools\msvc-env.bat"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /TC /MD /W4 /WX /Gy /Zc:inline /FAs /O2 /I"C:\Projetos\_tmp\PapinhoAccelerator\openssl-strict-ed25519-20261021\openssl-3.5.8\include" /I"C:\Projetos\_tmp\PapinhoAccelerator\openssl-strict-ed25519-20261021\openssl-3.5.8\crypto\ec" /c "C:\Projetos\_tmp\PapinhoAccelerator\openssl-strict-ed25519-20261021\openssl-3.5.8\crypto\ec\curve25519.c" /Fo"C:\Projetos\_tmp\PapinhoAccelerator\openssl-strict-ed25519-20261021\Release\curve.obj" /Fa"C:\Projetos\_tmp\PapinhoAccelerator\openssl-strict-ed25519-20261021\Release\curve.asm" /Fd"C:\Projetos\_tmp\PapinhoAccelerator\openssl-strict-ed25519-20261021\Release\curve.pdb"
exit /b %errorlevel%

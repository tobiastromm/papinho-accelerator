@echo off
call "C:\Projetos\PapinhoSecureTransport\tools\msvc-env.bat"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /TC /MD /W4 /WX /Gy /Zc:inline /FAs /Od /Zi /I"C:\Projetos\_tmp\PapinhoAccelerator\openssl-strict-ed25519-20261021\openssl-3.5.8\include" /I"C:\Projetos\_tmp\PapinhoAccelerator\openssl-strict-ed25519-20261021\openssl-3.5.8\crypto\ec" /c "C:\Projetos\_tmp\PapinhoAccelerator\openssl-strict-ed25519-20261021\openssl-3.5.8\crypto\ec\curve25519.c" /Fo"C:\Projetos\_tmp\PapinhoAccelerator\openssl-strict-ed25519-20261021\Debug\curve.obj" /Fa"C:\Projetos\_tmp\PapinhoAccelerator\openssl-strict-ed25519-20261021\Debug\curve.asm" /Fd"C:\Projetos\_tmp\PapinhoAccelerator\openssl-strict-ed25519-20261021\Debug\curve.pdb"
exit /b %errorlevel%

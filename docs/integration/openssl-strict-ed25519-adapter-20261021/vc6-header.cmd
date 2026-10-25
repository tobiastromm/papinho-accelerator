@echo off
call "C:\Projetos\PapinhoSecureTransport\tools\vc6-env.bat"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /TC /Za /W4 /WX /MD /c /I"C:\Projetos\_tmp\PapinhoAccelerator\openssl-strict-ed25519-20261021" "C:\Projetos\_tmp\PapinhoAccelerator\openssl-strict-ed25519-20261021\header-consumer.c" /Fo"C:\Projetos\_tmp\PapinhoAccelerator\openssl-strict-ed25519-20261021\vc6-header.obj"
exit /b %errorlevel%

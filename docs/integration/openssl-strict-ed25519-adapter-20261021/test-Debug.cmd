@echo off
call "C:\Projetos\PapinhoSecureTransport\tools\msvc-env.bat"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /TC /MD /W4 /WX /Gy /Od /Zi /I"C:\Projetos\_tmp\PapinhoAccelerator\openssl-strict-ed25519-20261021" /I"C:\Projetos\PapinhoSecureTransport\third_party\openssl\prebuilt\win32-x64-msvc-19.51-openssl3\3.5.8\include" "C:\Projetos\_tmp\PapinhoAccelerator\openssl-strict-ed25519-20261021\strict-test.c" /Fo"C:\Projetos\_tmp\PapinhoAccelerator\openssl-strict-ed25519-20261021\Debug\test.obj" /Fe"C:\Projetos\_tmp\PapinhoAccelerator\openssl-strict-ed25519-20261021\Debug\strict-test.exe" /Fd"C:\Projetos\_tmp\PapinhoAccelerator\openssl-strict-ed25519-20261021\Debug\test.pdb" /link "C:\Projetos\_tmp\PapinhoAccelerator\openssl-strict-ed25519-20261021\Debug\curve.obj" /OPT:REF /MAP:"C:\Projetos\_tmp\PapinhoAccelerator\openssl-strict-ed25519-20261021\Debug\strict-test.map" "C:\Projetos\PapinhoSecureTransport\third_party\openssl\prebuilt\win32-x64-msvc-19.51-openssl3\3.5.8\lib\libcrypto.lib"
if errorlevel 1 exit /b %errorlevel%
"C:\Projetos\_tmp\PapinhoAccelerator\openssl-strict-ed25519-20261021\Debug\strict-test.exe"
exit /b %errorlevel%

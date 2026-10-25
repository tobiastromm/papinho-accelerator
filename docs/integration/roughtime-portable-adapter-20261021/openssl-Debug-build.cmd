@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cd /d "C:\Users\tobias\AppData\Local\Temp\pacc-roughtime-portable-adapter-20261021\openssl-Debug"
cl /nologo /TC /W4 /WX /MD /DRT_OPENSSL_TEST /Od /Zi /I"C:\Projetos\PapinhoAccelerator\include" /I"C:\Projetos\PapinhoAccelerator\src\issuer" /I"C:\Users\tobias\AppData\Local\Temp\pacc-roughtime-portable-adapter-20261021" /I"C:\Projetos\PapinhoSecureTransport\third_party\openssl\prebuilt\win32-x64-msvc-19.51-openssl3\3.5.8\include" "C:\Projetos\PapinhoAccelerator\src\issuer\roughtime.c" "C:\Projetos\PapinhoAccelerator\src\issuer\roughtime_openssl.c" "C:\Projetos\PapinhoAccelerator\tests\roughtime_test.c" C:\Projetos\PapinhoAccelerator\src\issuer\qualified_utc.c /Fert_test.exe /link /INCREMENTAL:NO "C:\Projetos\PapinhoSecureTransport\third_party\openssl\prebuilt\win32-x64-msvc-19.51-openssl3\3.5.8\lib\libcrypto.lib"
if errorlevel 1 exit /b %errorlevel%
set PATH=C:\Projetos\PapinhoSecureTransport\third_party\openssl\prebuilt\win32-x64-msvc-19.51-openssl3\3.5.8\runtime;%PATH%
rt_test.exe
exit /b %errorlevel%

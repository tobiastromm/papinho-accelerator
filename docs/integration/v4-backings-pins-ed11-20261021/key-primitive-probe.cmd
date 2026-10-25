@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /TC /W4 /WX /MD /IC:\Projetos\PapinhoSecureTransport\third_party\openssl\prebuilt\win32-x64-msvc-19.51-openssl3\3.5.8\include C:\Users\tobias\AppData\Local\Temp\pacc-roughtime-full-transcript-ed11-20261021\v4-key-check-probe.c /FoC:\Users\tobias\AppData\Local\Temp\pacc-roughtime-full-transcript-ed11-20261021\v4-key-check-probe.obj /FeC:\Users\tobias\AppData\Local\Temp\pacc-roughtime-full-transcript-ed11-20261021\v4-key-check-probe.exe /link C:\Projetos\PapinhoSecureTransport\third_party\openssl\prebuilt\win32-x64-msvc-19.51-openssl3\3.5.8\lib\libcrypto.lib
if errorlevel 1 exit /b %errorlevel%
"C:\Users\tobias\AppData\Local\Temp\pacc-roughtime-full-transcript-ed11-20261021\v4-key-check-probe.exe"
exit /b %errorlevel%

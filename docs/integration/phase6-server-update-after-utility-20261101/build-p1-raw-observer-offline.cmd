@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat" >nul
if errorlevel 1 exit /b 1
set "obsdir=docs\integration\phase6-server-update-after-utility-20261101"
cl /nologo /W4 /WX /D_WIN32_WINNT=0x0A00 /DUNICODE /D_UNICODE /O2 "%obsdir%\p1-raw-open-observer.c" /Fo"%obsdir%\p1-raw-open-observer.obj" /Fe"%obsdir%\p1-raw-open-observer.exe"
if errorlevel 1 exit /b 1
cl /nologo /W4 /WX /D_WIN32_WINNT=0x0A00 /DUNICODE /D_UNICODE /DOBSERVER_MODEL /Od "%obsdir%\p1-raw-open-observer.c" /Fo"%obsdir%\p1-raw-open-observer-model.obj" /Fe"%obsdir%\p1-raw-open-observer-model.exe"
if errorlevel 1 exit /b 1
"%obsdir%\p1-raw-open-observer-model.exe"
exit /b %errorlevel%

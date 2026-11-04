@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat" >nul
if errorlevel 1 exit /b %errorlevel%
cl /nologo /W4 /WX /MD /O2 target-boundary.fixture.c /Fe:target-boundary.Release.exe /Fo:target-boundary.Release.obj
if errorlevel 1 exit /b %errorlevel%
target-boundary.Release.exe
if errorlevel 1 exit /b %errorlevel%
cl /nologo /W4 /WX /MDd /Od /Zi target-boundary.fixture.c /Fe:target-boundary.Debug.exe /Fo:target-boundary.Debug.obj /Fd:target-boundary.Debug.pdb
if errorlevel 1 exit /b %errorlevel%
target-boundary.Debug.exe

@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
C:\Strawberry\c\bin\cmake.exe --build build\phase6-canonical-consolidation-20261018\RootDebug --target papacc_server papacc_canonical_root_layout_test
exit /b %errorlevel%

@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cmake --build C:/Projetos/PapinhoAccelerator/build/ed11-actual-observer-20261024/Selected/Release --target papacc_service_observer_stack_va_isolated_test --parallel 2
exit /b %errorlevel%

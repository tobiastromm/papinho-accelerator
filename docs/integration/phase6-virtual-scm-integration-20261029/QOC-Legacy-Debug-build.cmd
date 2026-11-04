@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cmake --build build/phase6-functional-20261028/Legacy/Debug --parallel 2 --target papacc_phase6_memory_census_test papacc_phase6_resource_observer_model_test
exit /b %errorlevel%

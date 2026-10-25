@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
if errorlevel 1 exit /b %errorlevel%
cmake --build build/windows-virtual-service-lab-runtime-20261023/Selected/Debug --parallel 2 --target papacc_service_host_lab_model papacc_service_host_product_model
exit /b %errorlevel%

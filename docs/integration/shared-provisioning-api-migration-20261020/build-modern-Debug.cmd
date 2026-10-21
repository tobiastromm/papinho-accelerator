@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cmake -S tests/shared_provisioning -B build/shared-provisioning-api-migration-20261020/modern/Debug -G Ninja -DCMAKE_BUILD_TYPE=Debug
if errorlevel 1 exit /b %errorlevel%
cmake --build build/shared-provisioning-api-migration-20261020/modern/Debug
if errorlevel 1 exit /b %errorlevel%
ctest --test-dir build/shared-provisioning-api-migration-20261020/modern/Debug --output-on-failure --timeout 60
exit /b %errorlevel%

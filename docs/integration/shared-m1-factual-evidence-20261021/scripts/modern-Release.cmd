@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cd /d "C:\Projetos\PapinhoAccelerator"
cmake -S tests/shared_m1 -B build/sm1-20261021/modern/Release -G Ninja -DCMAKE_BUILD_TYPE=Release
if errorlevel 1 exit /b %errorlevel%
cmake --build build/sm1-20261021/modern/Release
if errorlevel 1 exit /b %errorlevel%
ctest --test-dir build/sm1-20261021/modern/Release --output-on-failure --timeout 60
exit /b %errorlevel%

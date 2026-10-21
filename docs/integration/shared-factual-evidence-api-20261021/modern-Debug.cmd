@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cd /d "C:\Projetos\PapinhoAccelerator"
cmake -S tests/shared_evidence -B build/evidence-20261021/modern/Debug -G Ninja -DCMAKE_BUILD_TYPE=Debug
if errorlevel 1 exit /b %errorlevel%
cmake --build build/evidence-20261021/modern/Debug
if errorlevel 1 exit /b %errorlevel%
ctest --test-dir build/evidence-20261021/modern/Debug --output-on-failure --timeout 60
exit /b %errorlevel%

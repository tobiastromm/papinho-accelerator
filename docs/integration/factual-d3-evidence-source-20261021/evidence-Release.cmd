@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cd /d "C:\Projetos\PapinhoAccelerator"
cmake -S tests/shared_evidence -B build/fd3-20261021/evidence/Release -G Ninja -DCMAKE_BUILD_TYPE=Release
if errorlevel 1 exit /b %errorlevel%
cmake --build build/fd3-20261021/evidence/Release
if errorlevel 1 exit /b %errorlevel%
ctest --test-dir build/fd3-20261021/evidence/Release --output-on-failure --timeout 60
exit /b %errorlevel%

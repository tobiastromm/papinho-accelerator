@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cmake -S C:/Projetos/PapinhoAccelerator/build/q7-actual-prefix-regression-20261025/old -B C:/Projetos/PapinhoAccelerator/build/q7-actual-prefix-regression-20261025/old/build -G Ninja -DCMAKE_BUILD_TYPE=Release
if errorlevel 1 exit /b %errorlevel%
cmake --build C:/Projetos/PapinhoAccelerator/build/q7-actual-prefix-regression-20261025/old/build --target prefix_probe
exit /b %errorlevel%

@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cmake -S C:/Projetos/PapinhoAccelerator/build/q7-source-pin-incremental-fixture-20261025/automatic-missing -B C:/Projetos/PapinhoAccelerator/build/q7-source-pin-incremental-fixture-20261025/automatic-missing/build -G Ninja -DCMAKE_BUILD_TYPE=Release -DEXPLICIT_PIN_DEP=OFF
if errorlevel 1 exit /b %errorlevel%
cmake --build C:/Projetos/PapinhoAccelerator/build/q7-source-pin-incremental-fixture-20261025/automatic-missing/build --target pin_probe
exit /b %errorlevel%

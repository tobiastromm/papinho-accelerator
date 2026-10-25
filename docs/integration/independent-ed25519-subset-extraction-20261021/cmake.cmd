@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
cmake -S "C:\Projetos\PapinhoAccelerator\tools\qualification\strict-ed25519" -B "C:\Users\tobias\AppData\Local\Temp\pacc-ed25519-subset-extraction\cmake-Release" -G Ninja -DCMAKE_BUILD_TYPE=Release
if errorlevel 1 exit /b %errorlevel%
cmake --build "C:\Users\tobias\AppData\Local\Temp\pacc-ed25519-subset-extraction\cmake-Release"
if errorlevel 1 exit /b %errorlevel%
ctest --test-dir "C:\Users\tobias\AppData\Local\Temp\pacc-ed25519-subset-extraction\cmake-Release" --output-on-failure
exit /b %errorlevel%

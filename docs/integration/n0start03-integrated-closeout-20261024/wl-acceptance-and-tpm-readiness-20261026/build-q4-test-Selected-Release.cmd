@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cmake --build "C:\Projetos\PapinhoAccelerator\build\d0-d9-bounded-diagnostic-20261026\Selected\Release" --target papacc_q4_root_binding_data_test --parallel 1
exit /b %errorlevel%

@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cmake --build C:/Projetos/PapinhoAccelerator/build/q4-final-binding-20261025/Selected/Release --parallel 2 --target papacc_scm_instrumented_lab papacc_n0_private_stdout
exit /b %errorlevel%

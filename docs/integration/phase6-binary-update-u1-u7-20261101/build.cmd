@echo off
call docs\integration\phase6-staged-binary-update-20261101\build.cmd %1 %2
if errorlevel 1 exit /b 1
cmake --build build/windows-virtual-scm-integration-20261022/%1/%2 --parallel 1 --target papacc_phase6_resource_observer_model_test
exit /b %errorlevel%

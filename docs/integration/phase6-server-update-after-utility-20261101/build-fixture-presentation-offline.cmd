@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat" >nul
if errorlevel 1 exit /b 1
cmake --build build/windows-virtual-scm-integration-20261022/%1/%2 --parallel 1 --target papacc_server papacc_machine_fixture_source_test papacc_scq_first_controller_test papacc_scq_machine_read_chain_test papacc_phase6_memory_census_test
exit /b %errorlevel%

@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat" >nul
if errorlevel 1 exit /b 1
cmake --build build/windows-virtual-scm-integration-20261022/%1/%2 --parallel 1 --target papacc_server papacc_scq_cold_open_test papacc_scq_cold_controller_test papacc_scq_start_assurance_test papacc_scq_query_cold_chain_test papacc_phase6_memory_census_test papacc_personal_paths_test papacc_scq_native_selection_capture_test
exit /b %errorlevel%

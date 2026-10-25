@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cd /d C:\Projetos\PapinhoAccelerator
cmake --build build/local-bootstrap-config-cli-20261019/Release --target papacc_server papacc_issuer_production_gate_test papacc_issuer_cng_production_gate_test papacc_config_binding_test papacc_config_binding_native_test papacc_bootstrap_scratch_test papacc_phase6_memory_census_test papacc_store_hash_owner_test papacc_store_hash_lifetime_test
if errorlevel 1 exit /b %errorlevel%
ctest --test-dir build/local-bootstrap-config-cli-20261019/Release --output-on-failure --timeout 60 -R "papacc.issuer.production_gate|papacc.issuer.cng_production_gate|papacc.bootstrap.scratch|papacc.config_binding|papacc.phase6.memory_census|papacc.store_hash"
exit /b %errorlevel%


@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
dumpbin /disasm build\bootstrap-d3-commit-authorization-20261019\Debug\CMakeFiles\papacc_configuration_commit.dir\apps\server\server_configuration_commit_win32.c.obj build\bootstrap-d3-commit-authorization-20261019\Debug\CMakeFiles\papacc_server_run_win32.dir\apps\server\server_config_source_binding_win32.c.obj

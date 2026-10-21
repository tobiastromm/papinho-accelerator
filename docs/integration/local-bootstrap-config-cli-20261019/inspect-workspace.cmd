@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
dumpbin /disasm build\local-bootstrap-config-cli-20261019\Debug\CMakeFiles\papacc_server.dir\apps\server\server_config_cli.c.obj

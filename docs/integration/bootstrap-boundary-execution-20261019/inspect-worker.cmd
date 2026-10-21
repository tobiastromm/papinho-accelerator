@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
dumpbin /disasm build\bootstrap-boundary-execution-20261019\Debug\CMakeFiles\papacc_configuration_commit.dir\apps\server\server_configuration_commit_win32.c.obj

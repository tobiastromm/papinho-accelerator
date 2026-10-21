@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
dumpbin /disasm build\config-interface-persistent-binding-20261019\Debug\CMakeFiles\papacc_config_binding.dir\src\config\config_binding.c.obj

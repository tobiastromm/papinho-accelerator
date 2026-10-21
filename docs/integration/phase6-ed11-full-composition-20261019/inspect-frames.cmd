@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
dumpbin /disasm build\phase6-ed11-full-composition-20261019\Debug\CMakeFiles\papacc_configuration_commit.dir\apps\server\server_configuration_commit_win32.c.obj
dumpbin /disasm build\phase6-ed11-full-composition-20261019\Release\CMakeFiles\papacc_configuration_commit.dir\apps\server\server_configuration_commit_win32.c.obj

@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
dumpbin /disasm /symbols "C:\Projetos\PapinhoAccelerator\build\local-bootstrap-config-cli-20261019\Debug\CMakeFiles\papacc_configuration_commit.dir\apps\server\server_configuration_personal_custody_data_win32.c.obj" > "C:\Users\tobias\AppData\Local\Temp\pacc-personal-target-ca-20261022\Debug-query-disasm.txt"
dumpbin /disasm /symbols "C:\Projetos\PapinhoAccelerator\build\local-bootstrap-config-cli-20261019\Release\CMakeFiles\papacc_configuration_commit.dir\apps\server\server_configuration_personal_custody_data_win32.c.obj" > "C:\Users\tobias\AppData\Local\Temp\pacc-personal-target-ca-20261022\Release-query-disasm.txt"

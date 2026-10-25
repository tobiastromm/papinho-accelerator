@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cd /d C:\Projetos\PapinhoAccelerator
dumpbin /headers /symbols build/local-bootstrap-config-cli-20261019/Debug/CMakeFiles/papacc_configuration_commit.dir/apps/server/personal_plan_data.c.obj > "%TEMP%\pacc-personal-ceremony-20261022\Debug-data-object.txt"
dumpbin /headers /symbols build/local-bootstrap-config-cli-20261019/Release/CMakeFiles/papacc_configuration_commit.dir/apps/server/personal_plan_data.c.obj > "%TEMP%\pacc-personal-ceremony-20261022\Release-data-object.txt"
dumpbin /disasm build/local-bootstrap-config-cli-20261019/Debug/CMakeFiles/papacc_configuration_commit.dir/apps/server/personal_plan_data.c.obj > "%TEMP%\pacc-personal-ceremony-20261022\Debug-data-disasm.txt"
dumpbin /disasm build/local-bootstrap-config-cli-20261019/Release/CMakeFiles/papacc_configuration_commit.dir/apps/server/personal_plan_data.c.obj > "%TEMP%\pacc-personal-ceremony-20261022\Release-data-disasm.txt"

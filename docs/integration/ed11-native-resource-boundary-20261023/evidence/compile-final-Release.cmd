@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /W4 /WX /O2 /Ob2 /DNDEBUG /MD /FAs /Fa"C:\Projetos\_tmp\PapinhoAccelerator\ed11-native-resource-boundary-20261023/probe-final-Release.asm" /Fo"C:\Projetos\_tmp\PapinhoAccelerator\ed11-native-resource-boundary-20261023/probe-final-Release.obj" /Fd"C:\Projetos\_tmp\PapinhoAccelerator\ed11-native-resource-boundary-20261023/probe-final-Release.pdb" /Fe"C:\Projetos\_tmp\PapinhoAccelerator\ed11-native-resource-boundary-20261023/probe-final-Release.exe" /I"C:/Projetos/PapinhoAccelerator/tools/qualification" "C:\Projetos\_tmp\PapinhoAccelerator\ed11-native-resource-boundary-20261023/native-file-probe.c" /link Advapi32.lib Psapi.lib User32.lib
exit /b %errorlevel%

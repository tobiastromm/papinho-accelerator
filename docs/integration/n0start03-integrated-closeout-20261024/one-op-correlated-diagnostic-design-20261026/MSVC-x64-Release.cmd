@echo off
call "C:/Program Files (x86)/Microsoft Visual Studio/18/BuildTools/VC/Auxiliary/Build/vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /MD /WX /W4 /O2 /I"C:\Projetos\PapinhoAccelerator\build\ed08-r4-integrated-20261012\sdk-normal\include" /Fo"C:\Projetos\PapinhoAccelerator\build\one-op-correlated-diagnostic-20261026\MSVC-x64-Release\test.obj" /Fd"C:\Projetos\PapinhoAccelerator\build\one-op-correlated-diagnostic-20261026\MSVC-x64-Release\test.pdb" /Fe"C:\Projetos\PapinhoAccelerator\build\one-op-correlated-diagnostic-20261026\MSVC-x64-Release\provider-query-test.exe" "C:\Projetos\PapinhoAccelerator\docs\integration\n0start03-integrated-closeout-20261024\one-op-correlated-diagnostic-design-20261026\owner-provider-readonly-probe-test.c" /link /incremental:no
exit /b %errorlevel%

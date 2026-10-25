@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
cl /nologo /W4 /WX /TC /O2 /LD /Fobuild/ed11-positive-private-20261025/Selected/Release/AdminHandleProbe.obj /Febuild/ed11-positive-private-20261025/Selected/Release/AdminHandleProbe.dll docs\integration\n0start03-integrated-closeout-20261024\admin-handle-budget-resolution-20261025\admin-handle-probe.c
if errorlevel 1 exit /b %errorlevel%
"C:\Windows\Microsoft.NET\Framework64\v4.0.30319\csc.exe" /nologo /target:library /platform:x64 /warnaserror /out:build/ed11-positive-private-20261025/Selected/Release/Pacc.AdminProbe.Test.dll docs\integration\n0start03-integrated-closeout-20261024\admin-handle-budget-resolution-20261025\admin-handle-probe.cs
exit /b %errorlevel%

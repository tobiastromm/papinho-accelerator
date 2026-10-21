@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cl.exe /nologo /TC /W4 /WX /MD /Od /Zi /Fobuild/mode-c-hybrid-external-observer-20261018/Debug/observer.obj /Fdbuild/mode-c-hybrid-external-observer-20261018/Debug/observer.pdb tests/diagnostic/ed11_total_probe/ed11_c1_external_observer.c /Febuild/mode-c-hybrid-external-observer-20261018/Debug/observer.exe /link psapi.lib bcrypt.lib
exit /b %errorlevel%

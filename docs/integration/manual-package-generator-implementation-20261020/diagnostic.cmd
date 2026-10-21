@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
cl /nologo /W4 /WX /I src/provisioning docs/integration/manual-package-generator-implementation-20261020/diagnostic.c /Fe:docs/integration/manual-package-generator-implementation-20261020/diagnostic.exe /Fo:docs/integration/manual-package-generator-implementation-20261020/diagnostic.obj
if errorlevel 1 exit /b %errorlevel%
docs\integration\manual-package-generator-implementation-20261020\diagnostic.exe

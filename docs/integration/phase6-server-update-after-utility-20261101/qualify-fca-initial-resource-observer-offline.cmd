@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat" >nul
if errorlevel 1 exit /b 1
cl /nologo /W4 /WX /MT /O2 /I include /I apps/server docs\integration\phase6-server-update-after-utility-20261101\fca-initial-resource-observer.c /Fe:docs\integration\phase6-server-update-after-utility-20261101\fca-initial-resource-observer.exe /Fo:docs\integration\phase6-server-update-after-utility-20261101\fca-initial-resource-observer.obj /link psapi.lib
if errorlevel 1 exit /b 1
cl /nologo /W4 /WX /MT /O2 /I include /I apps/server docs\integration\phase6-server-update-after-utility-20261101\fca-initial-resource-observer-model.c /Fe:docs\integration\phase6-server-update-after-utility-20261101\fca-initial-resource-observer-model.exe /Fo:docs\integration\phase6-server-update-after-utility-20261101\fca-initial-resource-observer-model.obj
if errorlevel 1 exit /b 1
docs\integration\phase6-server-update-after-utility-20261101\fca-initial-resource-observer-model.exe
exit /b %errorlevel%
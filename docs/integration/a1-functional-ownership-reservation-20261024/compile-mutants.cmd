@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cl /nologo /W4 /WX /TC /DA1_SELECTED=1 /Od /MD /FoC:/Projetos/PapinhoAccelerator/docs/integration/a1-functional-ownership-reservation-20261024/mutants/lost-reference/fixture.obj /FeC:/Projetos/PapinhoAccelerator/docs/integration/a1-functional-ownership-reservation-20261024/mutants/lost-reference/fixture.exe C:/Projetos/PapinhoAccelerator/docs/integration/a1-functional-ownership-reservation-20261024/mutants/lost-reference/fixture.c
if errorlevel 1 exit /b %errorlevel%
cl /nologo /W4 /WX /TC /DA1_SELECTED=1 /Od /MD /FoC:/Projetos/PapinhoAccelerator/docs/integration/a1-functional-ownership-reservation-20261024/mutants/ignore-frozen-acquisition/fixture.obj /FeC:/Projetos/PapinhoAccelerator/docs/integration/a1-functional-ownership-reservation-20261024/mutants/ignore-frozen-acquisition/fixture.exe C:/Projetos/PapinhoAccelerator/docs/integration/a1-functional-ownership-reservation-20261024/mutants/ignore-frozen-acquisition/fixture.c
if errorlevel 1 exit /b %errorlevel%
cl /nologo /W4 /WX /TC /DA1_SELECTED=1 /Od /MD /FoC:/Projetos/PapinhoAccelerator/docs/integration/a1-functional-ownership-reservation-20261024/mutants/overwrite-first-error/fixture.obj /FeC:/Projetos/PapinhoAccelerator/docs/integration/a1-functional-ownership-reservation-20261024/mutants/overwrite-first-error/fixture.exe C:/Projetos/PapinhoAccelerator/docs/integration/a1-functional-ownership-reservation-20261024/mutants/overwrite-first-error/fixture.c
if errorlevel 1 exit /b %errorlevel%
cl /nologo /W4 /WX /TC /DA1_SELECTED=1 /Od /MD /FoC:/Projetos/PapinhoAccelerator/docs/integration/a1-functional-ownership-reservation-20261024/mutants/unscoped-pair-borrow/fixture.obj /FeC:/Projetos/PapinhoAccelerator/docs/integration/a1-functional-ownership-reservation-20261024/mutants/unscoped-pair-borrow/fixture.exe C:/Projetos/PapinhoAccelerator/docs/integration/a1-functional-ownership-reservation-20261024/mutants/unscoped-pair-borrow/fixture.c
if errorlevel 1 exit /b %errorlevel%

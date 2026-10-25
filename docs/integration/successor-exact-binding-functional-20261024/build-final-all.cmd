@echo off
set VSLANG=1033
call "docs\integration\successor-exact-binding-functional-20261024\build-qualification-Selected-Debug.cmd" > "docs\integration\successor-exact-binding-functional-20261024\build-final-Selected-Debug.log" 2>&1
if errorlevel 1 exit /b %errorlevel%
call "docs\integration\successor-exact-binding-functional-20261024\build-qualification-Selected-Release.cmd" > "docs\integration\successor-exact-binding-functional-20261024\build-final-Selected-Release.log" 2>&1
if errorlevel 1 exit /b %errorlevel%
call "docs\integration\successor-exact-binding-functional-20261024\build-qualification-Legacy-Debug.cmd" > "docs\integration\successor-exact-binding-functional-20261024\build-final-Legacy-Debug.log" 2>&1
if errorlevel 1 exit /b %errorlevel%
call "docs\integration\successor-exact-binding-functional-20261024\build-qualification-Legacy-Release.cmd" > "docs\integration\successor-exact-binding-functional-20261024\build-final-Legacy-Release.log" 2>&1
if errorlevel 1 exit /b %errorlevel%
exit /b 0

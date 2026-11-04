@echo off
call docs\integration\phase6-binary-update-u1-u7-20261101\build.cmd %1 %2
if errorlevel 1 exit /b 1
call docs\integration\phase6-update-utility-20261101\build.cmd %1 %2
exit /b %errorlevel%

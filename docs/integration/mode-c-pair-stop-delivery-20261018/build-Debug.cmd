@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cl.exe /nologo /TC /W4 /WX /MD /Od /Zi /Iinclude /Iapps/server /Fobuild/mode-c-pair-stop-delivery-20261018/Debug/ /Fdbuild/mode-c-pair-stop-delivery-20261018/Debug/stop.pdb tests/diagnostic/ed11_total_probe/pair_stop_delivery_test.c apps/server/server_console_win32.c /Febuild/mode-c-pair-stop-delivery-20261018/Debug/stop-delivery.exe /link user32.lib
exit /b %errorlevel%

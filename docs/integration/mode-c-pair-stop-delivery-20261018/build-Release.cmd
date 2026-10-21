@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cl.exe /nologo /TC /W4 /WX /MD /O2 /DNDEBUG /Iinclude /Iapps/server /Fobuild/mode-c-pair-stop-delivery-20261018/Release/ /Fdbuild/mode-c-pair-stop-delivery-20261018/Release/stop.pdb tests/diagnostic/ed11_total_probe/pair_stop_delivery_test.c apps/server/server_console_win32.c /Febuild/mode-c-pair-stop-delivery-20261018/Release/stop-delivery.exe /link user32.lib
exit /b %errorlevel%

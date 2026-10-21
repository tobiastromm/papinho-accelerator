@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
if not exist "build\internal-ca-canonical-implementation-20261019\Release" mkdir "build\internal-ca-canonical-implementation-20261019\Release"
cl /nologo /TC /W4 /WX /O2 /MD /Iapps\server /Fo"build\internal-ca-canonical-implementation-20261019\Release\\" /Fd"build\internal-ca-canonical-implementation-20261019\Release\compile.pdb" apps\server\server_ca_key_win32.c tests\ca_key_offline_test.c /Fe"build\internal-ca-canonical-implementation-20261019\Release\ca_key_offline_test.exe" /link ncrypt.lib bcrypt.lib advapi32.lib
exit /b %errorlevel%
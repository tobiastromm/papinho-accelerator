@echo off
set VSLANG=1033
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
if not exist "build\internal-ca-canonical-implementation-20261019\Debug" mkdir "build\internal-ca-canonical-implementation-20261019\Debug"
cl /nologo /TC /W4 /WX /Od /Zi /MDd /Iapps\server /Fo"build\internal-ca-canonical-implementation-20261019\Debug\\" /Fd"build\internal-ca-canonical-implementation-20261019\Debug\compile.pdb" apps\server\server_ca_key_win32.c tests\ca_key_offline_test.c /Fe"build\internal-ca-canonical-implementation-20261019\Debug\ca_key_offline_test.exe" /link ncrypt.lib bcrypt.lib advapi32.lib
exit /b %errorlevel%
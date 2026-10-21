@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
dumpbin /imports build\internal-ca-windows-backend-continuation-20261019\Debug\papacc_issuer_windows_validation_test.exe
dumpbin /disasm build\internal-ca-windows-backend-continuation-20261019\Debug\CMakeFiles\papacc_issuer_windows_validation.dir\apps\server\server_issuer_windows_validation.c.obj
dumpbin /imports build\internal-ca-windows-backend-continuation-20261019\Release\papacc_issuer_windows_validation_test.exe
dumpbin /disasm build\internal-ca-windows-backend-continuation-20261019\Release\CMakeFiles\papacc_issuer_windows_validation.dir\apps\server\server_issuer_windows_validation.c.obj

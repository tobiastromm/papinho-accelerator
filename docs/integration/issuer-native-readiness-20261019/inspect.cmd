@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
dumpbin /imports build\issuer-native-readiness-20261019\Debug\papacc_issuer_cng_x509_test.exe
dumpbin /disasm build\issuer-native-readiness-20261019\Debug\CMakeFiles\papacc_issuer_cng_windows.dir\apps\server\server_issuer_x509_win32.c.obj
dumpbin /disasm build\issuer-native-readiness-20261019\Debug\CMakeFiles\papacc_issuer_cng_windows.dir\apps\server\server_issuer_cng_backend_win32.c.obj
dumpbin /disasm build\issuer-native-readiness-20261019\Debug\CMakeFiles\papacc_issuer_cng_windows.dir\apps\server\server_ca_key_win32.c.obj
dumpbin /imports build\issuer-native-readiness-20261019\Release\papacc_issuer_cng_x509_test.exe
dumpbin /disasm build\issuer-native-readiness-20261019\Release\CMakeFiles\papacc_issuer_cng_windows.dir\apps\server\server_issuer_x509_win32.c.obj
dumpbin /disasm build\issuer-native-readiness-20261019\Release\CMakeFiles\papacc_issuer_cng_windows.dir\apps\server\server_issuer_cng_backend_win32.c.obj
dumpbin /disasm build\issuer-native-readiness-20261019\Release\CMakeFiles\papacc_issuer_cng_windows.dir\apps\server\server_ca_key_win32.c.obj

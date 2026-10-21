@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
cl /nologo /TC /Za /W4 /WX /c build\issuer-live-transaction-safety-20261019\portable-contract.c /Fobuild\issuer-live-transaction-safety-20261019\portable-contract.obj
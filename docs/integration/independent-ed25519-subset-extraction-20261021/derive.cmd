@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
cd /d "C:\Users\tobias\AppData\Local\Temp\pacc-ed25519-subset-extraction"
cl /nologo /TC /Za /W4 /WX /MD /Od /I"C:\Projetos\PapinhoAccelerator\include" /I"C:\Projetos\PapinhoAccelerator\src\issuer" public-vector-derive.c /Fe:public-vector-derive.exe
if errorlevel 1 exit /b %errorlevel%
public-vector-derive.exe
exit /b %errorlevel%

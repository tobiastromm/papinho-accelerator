@echo off
call "C:/Program Files (x86)/Microsoft Visual Studio/18/BuildTools/VC/Auxiliary/Build/vcvars64.bat"
if errorlevel 1 exit /b %errorlevel%
cd /d "C:\Users\tobias\AppData\Local\Temp\pacc-storage-hash-lab-20261022\msvc-Debug"
cl /c /nologo /TC /Za /W4 /WX /MD /FAs /Od /Zl /GS- /Oi- /I"C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule" /I"C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule\vendor\bearssl-0.6\inc" /I"C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule\adapter_include" /FI"C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule\memory_bind.h" "C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule\vendor\bearssl-0.6\src\hash\sha2small.c" /Fosha2small.obj /Fasha2small.asm
if errorlevel 1 exit /b %errorlevel%
cl /c /nologo /TC /Za /W4 /WX /MD /FAs /Od /Zl /GS- /Oi- /I"C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule" /I"C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule\vendor\bearssl-0.6\inc" /I"C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule\adapter_include" /FI"C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule\memory_bind.h" "C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule\vendor\bearssl-0.6\src\codec\dec32be.c" /Fodec32be.obj /Fadec32be.asm
if errorlevel 1 exit /b %errorlevel%
cl /c /nologo /TC /Za /W4 /WX /MD /FAs /Od /Zl /GS- /Oi- /I"C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule" /I"C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule\vendor\bearssl-0.6\inc" /I"C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule\adapter_include" /FI"C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule\memory_bind.h" "C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule\vendor\bearssl-0.6\src\codec\enc32be.c" /Foenc32be.obj /Faenc32be.asm
if errorlevel 1 exit /b %errorlevel%
cl /c /nologo /TC /Za /W4 /WX /MD /FAs /Od /Zl /GS- /Oi- /I"C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule" /I"C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule\vendor\bearssl-0.6\inc" /I"C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule\adapter_include" /FI"C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule\memory_bind.h" "C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule\storage_sha256.c" /Fostorage_sha256.obj /Fastorage_sha256.asm
if errorlevel 1 exit /b %errorlevel%
cl /c /nologo /TC /Za /W4 /WX /MD /FAs /Od /Zl /GS- /Oi- /I"C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule" /I"C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule\vendor\bearssl-0.6\inc" /I"C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule\adapter_include" /FI"C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule\memory_bind.h" "C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule\memory_ops.c" /Fomemory_ops.obj /Famemory_ops.asm
if errorlevel 1 exit /b %errorlevel%
lib /nologo /out:capsule.lib sha2small.obj dec32be.obj enc32be.obj storage_sha256.obj memory_ops.obj
if errorlevel 1 exit /b %errorlevel%
cl /c /nologo /TC /Za /W4 /WX /MD /FAs /Od /Zl /GS- /Oi- /I"C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule" /I"C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule\vendor\bearssl-0.6\inc" /I"C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule\adapter_include" "C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule\closure_probe.c" /Foclosure_probe.obj /Faclosure_probe.asm
if errorlevel 1 exit /b %errorlevel%
link /nologo /NODEFAULTLIB /ENTRY:probe_main /SUBSYSTEM:CONSOLE /INCREMENTAL:NO /MAP:closure.map /OUT:closure.exe closure_probe.obj capsule.lib
if errorlevel 1 exit /b %errorlevel%
closure.exe
if errorlevel 1 exit /b %errorlevel%
cl /nologo /TC /Za /W4 /WX /MD /FAs /Od /I"C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule" /I"C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule\vendor\bearssl-0.6\inc" /I"C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule\adapter_include" "C:\Projetos\PapinhoAccelerator\tools\qualification\storage-hash-capsule\capsule_test.c" /Fecapsule_test.exe /Facapsule_test.asm /link /INCREMENTAL:NO /MAP:capsule_test.map capsule.lib
if errorlevel 1 exit /b %errorlevel%
capsule_test.exe
if errorlevel 1 exit /b %errorlevel%
dumpbin /imports closure.exe > closure-imports.txt
if errorlevel 1 exit /b %errorlevel%
dumpbin /disasm closure.exe > closure-disasm.txt
if errorlevel 1 exit /b %errorlevel%
dumpbin /symbols capsule.lib > capsule-symbols.txt
if errorlevel 1 exit /b %errorlevel%
dumpbin /headers closure.exe > closure-headers.txt
if errorlevel 1 exit /b %errorlevel%
dumpbin /imports capsule_test.exe > test-imports.txt
exit /b %errorlevel%

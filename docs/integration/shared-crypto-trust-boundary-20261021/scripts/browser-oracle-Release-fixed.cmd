@echo off
call C:\MSVC600-master\VC98\Bin\VCVARS32.BAT
if errorlevel 1 exit /b %errorlevel%
if not exist "C:\Projetos\PapinhoAccelerator\build\sct-20261021\browser-oracle\Release" mkdir "C:\Projetos\PapinhoAccelerator\build\sct-20261021\browser-oracle\Release"
pushd "C:\Projetos\PapinhoAccelerator\build\sct-20261021\browser-oracle\Release"
cl /nologo /TC /Za /W3 /WX /MD /O2 /I"C:\Projetos\PapinhoBrowser" /I"C:\Projetos\PapinhoBrowser\src" /I"C:\Projetos\PapinhoBrowser\third_party\bearssl\inc" /I"C:\Projetos\PapinhoBrowser\third_party\compat\vc6" /FI"C:\Projetos\PapinhoBrowser/third_party/compat/vc6/vc6_compat.h" "C:\Projetos\PapinhoBrowser\tests\papacc-import\crypto_adapter_test.c" "C:\Projetos\PapinhoBrowser\src\papinho_papacc_public.c" "C:\Projetos\PapinhoBrowser\src\papinho_papacc_frame.c" "C:\Projetos\PapinhoBrowser\src\papinho_c2_secret.c" "C:\Projetos\PapinhoBrowser\third_party\shared-provisioning-structure-snapshot1-a64a141ed2d2\structure.c" /Fecrypto_oracle.exe /link /INCREMENTAL:NO "C:\Projetos\PapinhoBrowser/build/spc-20261020/CANDIDATE_065/Release/bearssl/bearssls.lib"
if errorlevel 1 exit /b %errorlevel%
crypto_oracle.exe "C:\Projetos\PapinhoBrowser/build/papacc-crypto-adapter-vc6-20261020/fixtures" "C:\Projetos\PapinhoBrowser/build/papacc-simple-import-20261020/fixtures/signed.papacc"
exit /b %errorlevel%

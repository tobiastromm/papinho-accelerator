@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
cl /nologo /TC /c /W4 /WX /I src/core /DED11_EXPECTED=3145728U /DPAPACC_ED11_WINDOWS_3MIB=2 /Fo"build/ed11-windows-3mib-profile-20261020/invalid-selector.obj" tests/ed11_process_profile_test.c > docs/integration/ed11-windows-3mib-profile-20261020/invalid-selector.log 2>&1
if not errorlevel 1 exit /b 1
cl /nologo /TC /c /W4 /WX /I src/core /DED11_EXPECTED=3145728U /DPAPACC_ED11_WINDOWS_3MIB=1 /DED11_TEST_OTHER=1 /Fo"build/ed11-windows-3mib-profile-20261020/invalid-platform.obj" tests/ed11_process_profile_test.c > docs/integration/ed11-windows-3mib-profile-20261020/invalid-platform.log 2>&1
if not errorlevel 1 exit /b 1
cmake -S . -B build/ed11-windows-3mib-profile-20261020/InvalidBool -G Ninja -DPAPACC_ENABLE_WINDOWS_3MIB_MANAGED_PROFILE=garbage > docs/integration/ed11-windows-3mib-profile-20261020/invalid-cmake-selector.log 2>&1
if not errorlevel 1 exit /b 1
cmake -S . -B build/ed11-windows-3mib-profile-20261020/InvalidComposition -G Ninja -DPAPACC_ENABLE_WINDOWS_3MIB_MANAGED_PROFILE=ON > docs/integration/ed11-windows-3mib-profile-20261020/invalid-cmake-composition.log 2>&1
if not errorlevel 1 exit /b 1
exit /b 0

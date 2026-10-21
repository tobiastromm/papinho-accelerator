@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
dumpbin /disasm "build/canonical-issuer-carrier-integration-20261020/Debug/CMakeFiles/papacc_configuration_commit.dir/apps/server/server_configuration_commit_win32.c.obj"
dumpbin /disasm "build/canonical-issuer-carrier-integration-20261020/Debug/CMakeFiles/papacc_config_root8_v2_test.dir/apps/server/server_config_root8_data_win32.c.obj"
dumpbin /disasm "build/canonical-issuer-carrier-integration-20261020/Debug/CMakeFiles/papacc_issuer.dir/src/issuer/issuer_codec.c.obj"
dumpbin /disasm "build/canonical-issuer-carrier-integration-20261020/Release/CMakeFiles/papacc_configuration_commit.dir/apps/server/server_configuration_commit_win32.c.obj"
dumpbin /disasm "build/canonical-issuer-carrier-integration-20261020/Release/CMakeFiles/papacc_config_root8_v2_test.dir/apps/server/server_config_root8_data_win32.c.obj"
dumpbin /disasm "build/canonical-issuer-carrier-integration-20261020/Release/CMakeFiles/papacc_issuer.dir/src/issuer/issuer_codec.c.obj"

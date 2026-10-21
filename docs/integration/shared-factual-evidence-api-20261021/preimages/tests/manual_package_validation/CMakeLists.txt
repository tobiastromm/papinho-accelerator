cmake_minimum_required(VERSION 3.20)
project(papacc_manual_validation_offline C)
enable_testing()
# Explicit isolated-test profile; no root/runtime or individual quota change.
option(PAPACC_ENABLE_WINDOWS_3MIB_MANAGED_PROFILE "Explicit Windows process ceiling for isolated validation component" OFF)
get_filename_component(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../.." ABSOLUTE)
add_subdirectory("${ROOT}/tests/manual_package_crypto" crypto)
# Reuse canonical portable codecs, not another format/root or persistence path.
add_library(package_pdg_codec STATIC
 "${ROOT}/src/config/config_lst1.c" "${ROOT}/src/config/config_root8.c"
 "${ROOT}/src/config/config_root8_v2.c" "${ROOT}/src/config/config_root8_v3.c"
 "${ROOT}/src/issuer/issuer_codec.c")
target_include_directories(package_pdg_codec PUBLIC "${ROOT}/include" "${ROOT}/src/config" "${ROOT}/src/issuer")
target_compile_options(package_pdg_codec PRIVATE /W4 /WX)
add_library(package_validation STATIC "${ROOT}/src/provisioning/manual_package_validation.c" "${ROOT}/src/provisioning/manual_package_validation_openssl.c" "${ROOT}/src/provisioning/manual_package_pdg.c" "${ROOT}/src/provisioning/shared_structure/m1.c")
target_link_libraries(package_validation PUBLIC package_crypto package_pdg_codec)
target_compile_options(package_validation PRIVATE /W4 /WX)
add_library(package_validation_model STATIC "${ROOT}/src/provisioning/manual_package_validation.c" "${ROOT}/src/provisioning/manual_package.c" "${ROOT}/src/provisioning/shared_structure/m1.c")
target_include_directories(package_validation_model PUBLIC "${ROOT}/src/provisioning" "${ROOT}/include")
target_compile_definitions(package_validation_model PRIVATE MP_PACKAGE_VALIDATION_OFFLINE_TEST=1)
target_compile_options(package_validation_model PRIVATE /W4 /WX)
add_executable(package_validation_model_test "${ROOT}/tests/manual_package_validation_test.c")
target_link_libraries(package_validation_model_test PRIVATE package_validation_model)
target_compile_definitions(package_validation_model_test PRIVATE MP_PACKAGE_VALIDATION_OFFLINE_TEST=1)
target_compile_options(package_validation_model_test PRIVATE /W4 /WX)
if(PAPACC_ENABLE_WINDOWS_3MIB_MANAGED_PROFILE)
 if(NOT WIN32)
  message(FATAL_ERROR "Windows3MiB profile cannot be selected on another target")
 endif()
 target_compile_definitions(package_validation PRIVATE PAPACC_ED11_WINDOWS_3MIB=1)
 target_compile_definitions(package_validation_model PRIVATE PAPACC_ED11_WINDOWS_3MIB=1)
endif()
# Test hooks exist in a separate model binary, never package_validation.
# Existing public-fixture crypto test additionally exercises productive DENY.
target_link_libraries(package_crypto_test PRIVATE package_validation)
target_compile_definitions(package_crypto_test PRIVATE MP_AUTH_NATIVE_INTEGRATION_TEST=1)
add_test(NAME papacc.manual_package.validation_model COMMAND package_validation_model_test)

# Explicit dependencies also support hosts emitting localized /showIncludes.
set_property(SOURCE "${ROOT}/src/provisioning/manual_package_validation.c"
 "${ROOT}/src/provisioning/manual_package_validation_openssl.c"
 "${ROOT}/tests/manual_package_validation_test.c" APPEND PROPERTY OBJECT_DEPENDS
 "${ROOT}/src/provisioning/manual_package_validation.h;${ROOT}/src/provisioning/manual_package_validation_internal.h;${ROOT}/src/provisioning/manual_package_crypto.h;${ROOT}/src/core/ed11_process_profile.h")
set_property(SOURCE "${ROOT}/tests/manual_package_crypto_test.c" TARGET_DIRECTORY package_crypto_test APPEND PROPERTY OBJECT_DEPENDS
 "${ROOT}/src/provisioning/manual_package_validation.h;${ROOT}/src/provisioning/manual_package_crypto.h;${ROOT}/tests/manual_package_validation_native_test.inc")
set_property(SOURCE "${ROOT}/src/provisioning/manual_package_crypto_openssl.c" TARGET_DIRECTORY package_crypto APPEND PROPERTY OBJECT_DEPENDS
 "${ROOT}/src/provisioning/manual_package_crypto.h")

set_property(SOURCE "${ROOT}/src/provisioning/manual_package_pdg.c"
 "${ROOT}/src/provisioning/manual_package_validation.c" "${ROOT}/src/provisioning/manual_package_validation_openssl.c" APPEND PROPERTY OBJECT_DEPENDS
 "${ROOT}/src/provisioning/manual_package_pdg_internal.h;${ROOT}/src/provisioning/manual_package_validation.h;${ROOT}/src/config/config_root8_v3.h")
set_property(SOURCE "${ROOT}/src/config/config_root8_v3.c" APPEND PROPERTY OBJECT_DEPENDS "${ROOT}/src/config/config_root8_v3.h;${ROOT}/src/config/config_root8_internal.h")
set_property(SOURCE "${ROOT}/tests/manual_package_crypto_test.c" TARGET_DIRECTORY package_crypto_test APPEND PROPERTY OBJECT_DEPENDS "${ROOT}/tests/manual_package_pdg_fixture.inc;${ROOT}/src/config/config_root8_v3.h")

set_property(SOURCE "${ROOT}/src/provisioning/shared_structure/m1.c"
 "${ROOT}/src/provisioning/manual_package_validation.c" APPEND PROPERTY OBJECT_DEPENDS
 "${ROOT}/src/provisioning/shared_structure/m1.h;${ROOT}/src/provisioning/shared_structure/m1_internal.h")

cmake_minimum_required(VERSION 3.16)
project(shared_crypto_policy_offline C)
enable_testing()
get_filename_component(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../.." ABSOLUTE)
add_executable(shared_suite suite_test.c
 "${ROOT}/src/provisioning/shared_structure/suite.c"
 "${ROOT}/src/provisioning/shared_structure/structure.c"
 "${ROOT}/tests/shared_provisioning/oracles/papinho_papacc_frame.c")
target_include_directories(shared_suite PRIVATE "${ROOT}/src/provisioning/shared_structure" "${ROOT}/tests/shared_provisioning/oracles")
set_target_properties(shared_suite PROPERTIES C_STANDARD 90 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
if(MSVC)
 target_compile_options(shared_suite PRIVATE /W4 /WX)
else()
 target_compile_options(shared_suite PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
add_test(NAME shared.crypto.suite_policy COMMAND shared_suite)

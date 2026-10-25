cmake_minimum_required(VERSION 3.20)
project(papacc_roughtime_offline C)
enable_testing()
if(NOT WIN32 OR NOT MSVC OR NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
 message(FATAL_ERROR "Only explicitly qualified modern Windows x64 offline backend")
endif()
if(NOT PAPACC_ROUGHTIME_OPENSSL_358_OFFLINE OR NOT DEFINED PAPACC_PACKAGE_OPENSSL_ROOT)
 message(FATAL_ERROR "Explicit pinned existing OpenSSL artifact required; no fallback")
endif()
foreach(item "lib/libcrypto.lib|e6047dadbf2b9c85b3bd52598fd6dbac78f9789de9b1c0a837712584ac883509" "runtime/libcrypto-3-x64.dll|09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044")
 string(REPLACE "|" ";" pair "${item}")
 list(GET pair 0 name)
 list(GET pair 1 expected)
 file(SHA256 "${PAPACC_PACKAGE_OPENSSL_ROOT}/${name}" actual)
 if(NOT actual STREQUAL expected)
  message(FATAL_ERROR "Pinned OpenSSL artifact mismatch")
 endif()
endforeach()
get_filename_component(ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../.." ABSOLUTE)
add_executable(papacc_roughtime_crypto_test
 "${ROOT}/src/issuer/roughtime.c" "${ROOT}/src/issuer/qualified_utc.c"
 "${ROOT}/src/issuer/roughtime_openssl.c" "${ROOT}/tests/roughtime_test.c")
target_compile_definitions(papacc_roughtime_crypto_test PRIVATE RT_OPENSSL_TEST)
target_include_directories(papacc_roughtime_crypto_test PRIVATE "${ROOT}/include" "${ROOT}/src/issuer" "${PAPACC_PACKAGE_OPENSSL_ROOT}/include")
target_compile_options(papacc_roughtime_crypto_test PRIVATE /W4 /WX)
target_link_libraries(papacc_roughtime_crypto_test PRIVATE "${PAPACC_PACKAGE_OPENSSL_ROOT}/lib/libcrypto.lib")
add_custom_command(TARGET papacc_roughtime_crypto_test POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PAPACC_PACKAGE_OPENSSL_ROOT}/runtime/libcrypto-3-x64.dll" "$<TARGET_FILE_DIR:papacc_roughtime_crypto_test>")
add_test(NAME papacc.roughtime.offline_crypto COMMAND papacc_roughtime_crypto_test)

# Bounded negative-vector capacity scenario, never a qualified source.
add_executable(papacc_roughtime_capacity_test
 "${ROOT}/src/issuer/roughtime.c" "${ROOT}/src/issuer/qualified_utc.c"
 "${ROOT}/src/issuer/roughtime_openssl.c" "${ROOT}/tests/roughtime_capacity_test.c")
target_compile_definitions(papacc_roughtime_capacity_test PRIVATE RT_OPENSSL_TEST)
target_include_directories(papacc_roughtime_capacity_test PRIVATE "${ROOT}/include" "${ROOT}/src/issuer" "${PAPACC_PACKAGE_OPENSSL_ROOT}/include")
target_compile_options(papacc_roughtime_capacity_test PRIVATE /W4 /WX)
target_link_libraries(papacc_roughtime_capacity_test PRIVATE "${PAPACC_PACKAGE_OPENSSL_ROOT}/lib/libcrypto.lib")
add_test(NAME papacc.roughtime.offline_capacity COMMAND papacc_roughtime_capacity_test)

add_custom_command(TARGET papacc_roughtime_capacity_test POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PAPACC_PACKAGE_OPENSSL_ROOT}/runtime/libcrypto-3-x64.dll" "$<TARGET_FILE_DIR:papacc_roughtime_capacity_test>")

# Public LAB_ONLY transcript; no key generation occurs during configure/test.
foreach(item
 "long-term-keys.bin|23415be05b7976ca4cdd0dc831e87dece5c5913cf38c058b0a24bf223d8c8b32"
 "nonce-randoms.bin|c952c8c5228ea26a3947aed4cd4d4a0239bbc4b39c9db591ce7fb00a17bee3bc"
 "request-0.bin|e931ba4b04dd3ff8aa8c72baf21f032d96bf68246f65d54834c2f9534a88de4f"
 "request-1.bin|86d255d6bd3b1b3f13c61075aa13c2d734ccd344c122e38640f7bde9dd9d88a6"
 "request-2.bin|2320b5d440d8318ae887176139146afd883cb76e3295a86812e8303ea0ab9a33"
 "request-3.bin|265e9bcc6ab9dedf3e3c1aaa683b86b471151418e6a2187b4995ac78ebd13338"
 "request-4.bin|565c36e45491844aa278f57a5c4194dc44fe8b9e395abbbdca8933f6677ea6b9"
 "request-5.bin|e4c9d1c4a0503ae10ca8e54ccd85a09b5f1c0c342c0f22a18c3bc4e81a1b6ee9"
 "response-0.bin|72de3e506ae72f2654f2c50968e6d3a7bc9a2d1017248186dbf7654be51aa107"
 "response-1.bin|ec4b30b0af42ab92d29d797256e4c9faf1354e7bfca22b1001c70ff518066ca7"
 "response-2.bin|cfbbadaa884f66dd527714edfd59f0fc531525b5c05bfcd3489820be7a785016"
 "response-3.bin|78b3950a12b624a73cfdcd2903b13cd013e02d1921d7249cc2eb8a47b6d959d6"
 "response-4.bin|30732d3aa1f9542b7fb24e88e96de17d482e323d467a600050db44c99f1cab1f"
 "response-5.bin|89b4618a1d5f0c62f496002e3ded4da244aa6e85842dff40e4b8dfe89896f943"
)
 string(REPLACE "|" ";" pair "${item}")
 list(GET pair 0 name)
 list(GET pair 1 expected)
 file(SHA256 "${ROOT}/tests/fixtures/roughtime-lab-20261021/${name}" actual)
 if(NOT actual STREQUAL expected)
  message(FATAL_ERROR "Pinned LAB_ONLY public transcript mismatch")
 endif()
endforeach()
add_executable(papacc_roughtime_transcript_test
 "${ROOT}/src/issuer/roughtime.c" "${ROOT}/src/issuer/qualified_utc.c"
 "${ROOT}/src/issuer/roughtime_openssl.c" "${ROOT}/tests/roughtime_capacity_test.c")
target_compile_definitions(papacc_roughtime_transcript_test PRIVATE RT_OPENSSL_TEST RT_LAB_TRANSCRIPT _CRT_SECURE_NO_WARNINGS)
target_include_directories(papacc_roughtime_transcript_test PRIVATE "${ROOT}/include" "${ROOT}/src/issuer" "${PAPACC_PACKAGE_OPENSSL_ROOT}/include")
target_compile_options(papacc_roughtime_transcript_test PRIVATE /W4 /WX)
target_link_libraries(papacc_roughtime_transcript_test PRIVATE "${PAPACC_PACKAGE_OPENSSL_ROOT}/lib/libcrypto.lib")
add_custom_command(TARGET papacc_roughtime_transcript_test POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${PAPACC_PACKAGE_OPENSSL_ROOT}/runtime/libcrypto-3-x64.dll" "$<TARGET_FILE_DIR:papacc_roughtime_transcript_test>")
add_test(NAME papacc.roughtime.lab_transcript COMMAND papacc_roughtime_transcript_test "${ROOT}/tests/fixtures/roughtime-lab-20261021")

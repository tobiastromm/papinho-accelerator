# Private build boundary for consumers of the pinned PST release SDK.
# This module does not acquire PST and does not create any runtime lifecycle.

function(papacc_require_pst_file_sha256 file_path expected_sha256 label)
    if(NOT EXISTS "${file_path}")
        message(FATAL_ERROR "${label} is missing: ${file_path}")
    endif()
    file(SHA256 "${file_path}" PAPACC_PST_ACTUAL_SHA256)
    string(TOLOWER "${expected_sha256}" PAPACC_PST_EXPECTED_SHA256)
    string(TOLOWER "${PAPACC_PST_ACTUAL_SHA256}" PAPACC_PST_ACTUAL_SHA256)
    if(NOT PAPACC_PST_ACTUAL_SHA256 STREQUAL PAPACC_PST_EXPECTED_SHA256)
        message(FATAL_ERROR
            "${label} SHA256 mismatch: expected ${PAPACC_PST_EXPECTED_SHA256}, "
            "observed ${PAPACC_PST_ACTUAL_SHA256}: ${file_path}")
    endif()
endfunction()

function(papacc_define_pst_consumer_boundary)
    if(TARGET papacc_pst_consumer)
        return()
    endif()

    set(PAPACC_PST_PIN_FILE
        "${PROJECT_SOURCE_DIR}/dependencies/papinho-secure-transport.txt")
    file(STRINGS "${PAPACC_PST_PIN_FILE}" PAPACC_PST_RELEASE_LINE
        REGEX "^release_tag=")
    file(STRINGS "${PAPACC_PST_PIN_FILE}" PAPACC_PST_TARGET_LINE
        REGEX "^target=")
    list(LENGTH PAPACC_PST_RELEASE_LINE PAPACC_PST_RELEASE_COUNT)
    list(LENGTH PAPACC_PST_TARGET_LINE PAPACC_PST_TARGET_COUNT)
    if(NOT PAPACC_PST_RELEASE_COUNT EQUAL 1 OR
       NOT PAPACC_PST_TARGET_COUNT EQUAL 1)
        message(FATAL_ERROR
            "The PST pin must contain exactly one release_tag and target")
    endif()

    string(REGEX REPLACE "^release_tag=" ""
        PAPACC_PST_RELEASE_TAG "${PAPACC_PST_RELEASE_LINE}")
    string(REGEX REPLACE "^target=" ""
        PAPACC_PST_TARGET "${PAPACC_PST_TARGET_LINE}")

    # Production remains derived exclusively from the versioned release pin.
    # An isolated BUILD_TESTING tree may explicitly select the governed 0.6.5
    # qualification SDK; that selection never changes the production pin.
    unset(PAPACC_PST_SDK_ROOT CACHE)
    set(PAPACC_PST_QUALIFICATION_SDK_ROOT "" CACHE PATH
        "Extracted governed PST 0.6.5 normal SDK for isolated qualification")
    set(PAPACC_PST_QUALIFICATION_SDK_ARCHIVE "" CACHE FILEPATH
        "Governed PST 0.6.5 normal SDK archive for provenance verification")
    # Explicit API3 candidate has its own build directory and immutable normal SDK.
    # Default/production and the older API2 qualification path remain unchanged.
    set(PAPACC_EXPECTED_ARCHIVE "0c134e0fc842e22e80dd6b35f6e7f207a7859f097f0be5b7d0be9c08d9d05c67")
    set(PAPACC_EXPECTED_MANIFEST "969e6048dabd9000990a9c386e78dec9aa4caab3d8148e26ae7ba8d6d7cc5d5e")
    set(PAPACC_EXPECTED_HEADER "e3c682c97c98a67ef228bcebe3b8ee3d39607a51dbe0b40587667ed7e0cc0dc6")
    set(PAPACC_EXPECTED_LIBRARY "dca90f3ae869b19738ee259bcB0e620af8f31c9842c9ec15a47bbb04ba4038e1")
    set(PAPACC_EXPECTED_API "api_version=2.2.0")
    set(PAPACC_EXPECTED_SPI "spi_version=3.0")
    if(PAPACC_ENABLE_CANONICAL_API3_CANDIDATE)
        if(NOT BUILD_TESTING OR NOT PAPACC_PST_QUALIFICATION_SDK_ROOT OR
           NOT PAPACC_PST_QUALIFICATION_SDK_ARCHIVE OR
           PAPACC_PST_TEST_ONLY_SDK_ROOT OR PAPACC_PST_TEST_ONLY_SDK_ARCHIVE OR
           PAPACC_PST_TEST_ONLY_DIAGNOSTIC_LIBRARY)
            message(FATAL_ERROR "API3 candidate requires normal qualification SDK only; no TEST_ONLY mixing")
        endif()
        set(PAPACC_EXPECTED_ARCHIVE "79e53a2d47ad6d509ec893bd1c6d9b8c26b98c5a8a9ecd75e4adb33a240b132f")
        set(PAPACC_EXPECTED_MANIFEST "03130f62c40b0742b83a8af636112349a51cf1b1d949f6f8eb18ef2bad1877cb")
        set(PAPACC_EXPECTED_HEADER "8f2255d53d0e59ea5d9da7c999a536a9b0392eb25efe20ca63e0c1a784c5ce8c")
        set(PAPACC_EXPECTED_LIBRARY "ad27c1f36861f96b48e496391ba1589de42377d930aa735ac7297044a2fbc37f")
        set(PAPACC_EXPECTED_API "api_version=3.0.0")
        set(PAPACC_EXPECTED_SPI "spi_version=4.0")
    endif()
    if(PAPACC_PST_QUALIFICATION_SDK_ROOT OR
       PAPACC_PST_QUALIFICATION_SDK_ARCHIVE)
        if(NOT BUILD_TESTING)
            message(FATAL_ERROR
                "PST 0.6.5 qualification selection requires BUILD_TESTING")
        endif()
        if(NOT PAPACC_PST_QUALIFICATION_SDK_ROOT OR
           NOT PAPACC_PST_QUALIFICATION_SDK_ARCHIVE)
            message(FATAL_ERROR
                "PST qualification requires both SDK root and SDK archive")
        endif()
        set(PAPACC_PST_SDK_ROOT "${PAPACC_PST_QUALIFICATION_SDK_ROOT}")
        set(PAPACC_PST_QUALIFICATION_ACTIVE TRUE)
        papacc_require_pst_file_sha256(
            "${PAPACC_PST_QUALIFICATION_SDK_ARCHIVE}"
            ${PAPACC_EXPECTED_ARCHIVE}
            "Governed PST 0.6.5 normal SDK archive")
    else()
        set(PAPACC_PST_SDK_ROOT
            "${CMAKE_BINARY_DIR}/dependencies/papinho-secure-transport/${PAPACC_PST_RELEASE_TAG}/${PAPACC_PST_TARGET}")
        set(PAPACC_PST_QUALIFICATION_ACTIVE FALSE)
    endif()

    set(PAPACC_PST_LIB_DIR
        "${PAPACC_PST_SDK_ROOT}/lib/${PAPACC_PST_TARGET}")
    set(PAPACC_PST_RUNTIME_DIR
        "${PAPACC_PST_SDK_ROOT}/runtime/${PAPACC_PST_TARGET}")
    set(PAPACC_PST_RUNTIME_FILES
        "${PAPACC_PST_RUNTIME_DIR}/libssl-3-x64.dll"
        "${PAPACC_PST_RUNTIME_DIR}/libcrypto-3-x64.dll")
    set(PAPACC_PST_REQUIRED_FILES
        "${PAPACC_PST_SDK_ROOT}/include/papinho_secure_transport.h"
        "${PAPACC_PST_SDK_ROOT}/include/papinho_secure_transport_win32.h"
        "${PAPACC_PST_LIB_DIR}/papinho_secure_transport.lib"
        "${PAPACC_PST_LIB_DIR}/libssl.lib"
        "${PAPACC_PST_LIB_DIR}/libcrypto.lib"
        ${PAPACC_PST_RUNTIME_FILES})
    foreach(PAPACC_PST_REQUIRED_FILE IN LISTS PAPACC_PST_REQUIRED_FILES)
        if(NOT EXISTS "${PAPACC_PST_REQUIRED_FILE}")
            message(FATAL_ERROR
                "Selected PST SDK is missing or incomplete: "
                "${PAPACC_PST_REQUIRED_FILE}")
        endif()
    endforeach()

    if(PAPACC_PST_QUALIFICATION_ACTIVE)
        if(EXISTS "${PAPACC_PST_SDK_ROOT}/lib/papinho_secure_transport_TEST_ONLY.lib")
            message(FATAL_ERROR
                "Normal qualification SDK must not contain the test-only library")
        endif()
        papacc_require_pst_file_sha256(
            "${PAPACC_PST_SDK_ROOT}/manifest.ini"
            ${PAPACC_EXPECTED_MANIFEST}
            "PST 0.6.5 normal manifest")
        file(STRINGS "${PAPACC_PST_SDK_ROOT}/manifest.ini"
            PAPACC_PST_QUALIFICATION_MANIFEST_LINES)
        foreach(PAPACC_PST_EXPECTED_MANIFEST_LINE IN ITEMS
                "package_version=0.6.5"
                "library_version=0.6.5"
                "${PAPACC_EXPECTED_API}"
                "${PAPACC_EXPECTED_SPI}"
                "target_id=win32-x64-msvc-19.51-openssl3"
                "crt=dynamic-/MD"
                "linkage=static"
                "provider_ids=openssl")
            list(FIND PAPACC_PST_QUALIFICATION_MANIFEST_LINES
                "${PAPACC_PST_EXPECTED_MANIFEST_LINE}"
                PAPACC_PST_MANIFEST_LINE_INDEX)
            if(PAPACC_PST_MANIFEST_LINE_INDEX EQUAL -1)
                message(FATAL_ERROR
                    "PST 0.6.5 normal manifest identity mismatch: "
                    "${PAPACC_PST_EXPECTED_MANIFEST_LINE}")
            endif()
        endforeach()
        papacc_require_pst_file_sha256(
            "${PAPACC_PST_SDK_ROOT}/include/papinho_secure_transport.h"
            ${PAPACC_EXPECTED_HEADER}
            "PST 0.6.5 public header")
        papacc_require_pst_file_sha256(
            "${PAPACC_PST_SDK_ROOT}/include/papinho_secure_transport_win32.h"
            "3cb297b887e80fdcd5df75015e37829d7196032f990d9c3503ccb3266028d883"
            "PST 0.6.5 Win32 header")
        papacc_require_pst_file_sha256(
            "${PAPACC_PST_LIB_DIR}/papinho_secure_transport.lib"
            ${PAPACC_EXPECTED_LIBRARY}
            "PST 0.6.5 normal library")
        papacc_require_pst_file_sha256(
            "${PAPACC_PST_LIB_DIR}/libssl.lib"
            "e6d86b6fb9ad205f3b80ba80564b4ab8764c7bf45a793bc209385ad3c601a755"
            "PST 0.6.5 OpenSSL import library")
        papacc_require_pst_file_sha256(
            "${PAPACC_PST_LIB_DIR}/libcrypto.lib"
            "e6047dadbf2b9c85b3bd52598fd6dbac78f9789de9b1c0a837712584ac883509"
            "PST 0.6.5 crypto import library")
        papacc_require_pst_file_sha256(
            "${PAPACC_PST_RUNTIME_DIR}/libssl-3-x64.dll"
            "3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed"
            "PST 0.6.5 OpenSSL runtime")
        papacc_require_pst_file_sha256(
            "${PAPACC_PST_RUNTIME_DIR}/libcrypto-3-x64.dll"
            "09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044"
            "PST 0.6.5 crypto runtime")
    endif()

    set(PAPACC_PST_TEST_ONLY_SDK_ROOT "" CACHE PATH
        "Complete governed PST test-only SDK used only by opted-in test targets")
    set(PAPACC_PST_TEST_ONLY_SDK_ARCHIVE "" CACHE FILEPATH
        "Governed PST 0.6.5 test-only SDK archive for provenance verification")
    set(PAPACC_PST_TEST_ONLY_DIAGNOSTIC_LIBRARY "" CACHE FILEPATH
        "Approved isolated PST test-only diagnostic library")
    set(PAPACC_PST_TEST_ONLY_CORRELATION_HEADER "" CACHE FILEPATH
        "Approved isolated PST test-only correlation observer header")
    if(PAPACC_PST_TEST_ONLY_SDK_ROOT OR PAPACC_PST_TEST_ONLY_SDK_ARCHIVE)
        if(NOT BUILD_TESTING OR NOT PAPACC_PST_QUALIFICATION_ACTIVE)
            message(FATAL_ERROR
                "PST test-only SDK requires an isolated 0.6.5 qualification build")
        endif()
        if(NOT PAPACC_PST_TEST_ONLY_SDK_ROOT OR
           NOT PAPACC_PST_TEST_ONLY_SDK_ARCHIVE)
            message(FATAL_ERROR
                "PST test-only selection requires both SDK root and SDK archive")
        endif()
        get_filename_component(PAPACC_PST_NORMAL_REAL_ROOT
            "${PAPACC_PST_SDK_ROOT}" REALPATH)
        get_filename_component(PAPACC_PST_TEST_ONLY_REAL_ROOT
            "${PAPACC_PST_TEST_ONLY_SDK_ROOT}" REALPATH)
        if(PAPACC_PST_NORMAL_REAL_ROOT STREQUAL PAPACC_PST_TEST_ONLY_REAL_ROOT)
            message(FATAL_ERROR "Normal and test-only PST SDK roots must differ")
        endif()
        if(EXISTS "${PAPACC_PST_TEST_ONLY_SDK_ROOT}/lib/papinho_secure_transport.lib")
            message(FATAL_ERROR
                "Test-only SDK must not contain the normal PST library")
        endif()
        papacc_require_pst_file_sha256(
            "${PAPACC_PST_TEST_ONLY_SDK_ARCHIVE}"
            "4c92719a0495bc41d2b444abe3614ada9fd1fc0a0d3008c452bbab3b8e673974"
            "Governed PST 0.6.5 test-only SDK archive")
        set(PAPACC_PST_TEST_ONLY_HASHES
            "include/papinho_secure_transport.h|e3c682c97c98a67ef228bcebe3b8ee3d39607a51dbe0b40587667ed7e0cc0dc6"
            "include/papinho_secure_transport_win32.h|d5c22322b6945d67db1aac826b79d3a8f2004ca8a4f71056af4addd94a0f85bf"
            "lib/papinho_secure_transport_TEST_ONLY.lib|22883661fadb38f6562a3ae0c042ea290c81c7ea5d0329b925d6ed46f050f08e"
            "lib/libssl.lib|e6d86b6fb9ad205f3b80ba80564b4ab8764c7bf45a793bc209385ad3c601a755"
            "lib/libcrypto.lib|e6047dadbf2b9c85b3bd52598fd6dbac78f9789de9b1c0a837712584ac883509"
            "bin/libssl-3-x64.dll|3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed"
            "bin/libcrypto-3-x64.dll|09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044"
            "README.md|3fd25e8c4311302851169a2efbc847257c7cdd82637694e592d90f146e397651"
            "SHA256SUMS-TEST-ONLY-SDK.txt|65ceee960b08f6c4da749aa51b7f881c4664f90de5f743cab5ee5c31047fc2cb")
        foreach(PAPACC_PST_TEST_ONLY_HASH_ENTRY IN LISTS
                PAPACC_PST_TEST_ONLY_HASHES)
            string(REPLACE "|" ";" PAPACC_PST_TEST_ONLY_HASH_PARTS
                "${PAPACC_PST_TEST_ONLY_HASH_ENTRY}")
            list(GET PAPACC_PST_TEST_ONLY_HASH_PARTS 0
                PAPACC_PST_TEST_ONLY_RELATIVE_PATH)
            list(GET PAPACC_PST_TEST_ONLY_HASH_PARTS 1
                PAPACC_PST_TEST_ONLY_EXPECTED_HASH)
            papacc_require_pst_file_sha256(
                "${PAPACC_PST_TEST_ONLY_SDK_ROOT}/${PAPACC_PST_TEST_ONLY_RELATIVE_PATH}"
                "${PAPACC_PST_TEST_ONLY_EXPECTED_HASH}"
                "PST 0.6.5 test-only artifact")
        endforeach()
        file(STRINGS "${PAPACC_PST_TEST_ONLY_SDK_ROOT}/README.md"
            PAPACC_PST_TEST_ONLY_README_LINES)
        foreach(PAPACC_PST_TEST_ONLY_IDENTITY_LINE IN ITEMS
                "- PST_VERSION=0.6.5"
                "- PST_API=2.2.0"
                "- PST_SPI=3.0"
                "- PROVIDER=OPENSSL"
                "- TARGET=win32-x64-msvc-19.51-openssl3"
                "- CRT=/MD"
                "- TEST_ONLY_DEFINE=PST_OPENSSL_TEST_ONLY_SHUTDOWN_WRITE_GATE")
            list(FIND PAPACC_PST_TEST_ONLY_README_LINES
                "${PAPACC_PST_TEST_ONLY_IDENTITY_LINE}"
                PAPACC_PST_TEST_ONLY_IDENTITY_INDEX)
            if(PAPACC_PST_TEST_ONLY_IDENTITY_INDEX EQUAL -1)
                message(FATAL_ERROR
                    "PST 0.6.5 test-only identity mismatch: "
                    "${PAPACC_PST_TEST_ONLY_IDENTITY_LINE}")
            endif()
        endforeach()
        if(PAPACC_PST_TEST_ONLY_DIAGNOSTIC_LIBRARY)
            if(PAPACC_PST_TEST_ONLY_CORRELATION_HEADER)
                set(PAPACC_PST_TEST_ONLY_DIAGNOSTIC_EXPECTED_HASH
                    "fdb17091923cdac094bc58793115c52a62860661799f3c68dec4e9d3fd954cd7")
                papacc_require_pst_file_sha256(
                    "${PAPACC_PST_TEST_ONLY_CORRELATION_HEADER}"
                    "32e1b16dbc0a9e59ced892d4623faf7cbfbea3bff5653d995464b7122a54802e"
                    "Approved isolated PST correlation observer header")
            else()
                set(PAPACC_PST_TEST_ONLY_DIAGNOSTIC_EXPECTED_HASH
                    "a0b5496a13517d41212527a5be4996a7426711d2c7d9baf1688623694240928a")
            endif()
            papacc_require_pst_file_sha256(
                "${PAPACC_PST_TEST_ONLY_DIAGNOSTIC_LIBRARY}"
                "${PAPACC_PST_TEST_ONLY_DIAGNOSTIC_EXPECTED_HASH}"
                "Approved isolated PST 0.6.5 native-close diagnostic library")
            set(PAPACC_PST_TEST_ONLY_LIBRARY
                "${PAPACC_PST_TEST_ONLY_DIAGNOSTIC_LIBRARY}")
        else()
            set(PAPACC_PST_TEST_ONLY_LIBRARY
                "${PAPACC_PST_TEST_ONLY_SDK_ROOT}/lib/papinho_secure_transport_TEST_ONLY.lib")
        endif()
    endif()

    add_library(papacc_pst_consumer INTERFACE)
    if(PAPACC_PST_TEST_ONLY_DIAGNOSTIC_LIBRARY AND
       PAPACC_PST_TEST_ONLY_CORRELATION_HEADER)
        target_include_directories(papacc_pst_consumer INTERFACE
            "${PAPACC_PST_TEST_ONLY_SDK_ROOT}/include")
        target_link_libraries(papacc_pst_consumer INTERFACE
            "${PAPACC_PST_TEST_ONLY_LIBRARY}"
            "${PAPACC_PST_TEST_ONLY_SDK_ROOT}/lib/libssl.lib"
            "${PAPACC_PST_TEST_ONLY_SDK_ROOT}/lib/libcrypto.lib"
            ws2_32 crypt32)
        set_property(TARGET papacc_pst_consumer PROPERTY
            PAPACC_PST_DIAGNOSTIC_CORRELATION_ACTIVE TRUE)
    else()
        target_include_directories(papacc_pst_consumer INTERFACE
            "$<$<NOT:$<BOOL:$<TARGET_PROPERTY:PAPACC_PST_TEST_ONLY_CONSUMER>>>:${PAPACC_PST_SDK_ROOT}/include>"
            "$<$<BOOL:$<TARGET_PROPERTY:PAPACC_PST_TEST_ONLY_CONSUMER>>:${PAPACC_PST_TEST_ONLY_SDK_ROOT}/include>")
        target_link_libraries(papacc_pst_consumer INTERFACE
            "$<$<NOT:$<BOOL:$<TARGET_PROPERTY:PAPACC_PST_TEST_ONLY_CONSUMER>>>:${PAPACC_PST_LIB_DIR}/papinho_secure_transport.lib>"
            "$<$<NOT:$<BOOL:$<TARGET_PROPERTY:PAPACC_PST_TEST_ONLY_CONSUMER>>>:${PAPACC_PST_LIB_DIR}/libssl.lib>"
            "$<$<NOT:$<BOOL:$<TARGET_PROPERTY:PAPACC_PST_TEST_ONLY_CONSUMER>>>:${PAPACC_PST_LIB_DIR}/libcrypto.lib>"
            "$<$<BOOL:$<TARGET_PROPERTY:PAPACC_PST_TEST_ONLY_CONSUMER>>:${PAPACC_PST_TEST_ONLY_LIBRARY}>"
            "$<$<BOOL:$<TARGET_PROPERTY:PAPACC_PST_TEST_ONLY_CONSUMER>>:${PAPACC_PST_TEST_ONLY_SDK_ROOT}/lib/libssl.lib>"
            "$<$<BOOL:$<TARGET_PROPERTY:PAPACC_PST_TEST_ONLY_CONSUMER>>:${PAPACC_PST_TEST_ONLY_SDK_ROOT}/lib/libcrypto.lib>"
            ws2_32 crypt32)
    endif()
    set_property(TARGET papacc_pst_consumer PROPERTY
        PAPACC_PST_RUNTIME_FILES "${PAPACC_PST_RUNTIME_FILES}")
endfunction()

function(papacc_stage_pst_runtime consumer_target)
    if(NOT TARGET papacc_pst_consumer)
        message(FATAL_ERROR "The PST consumer boundary is not defined")
    endif()
    if(NOT TARGET ${consumer_target})
        message(FATAL_ERROR "Unknown PST consumer target: ${consumer_target}")
    endif()
    get_target_property(PAPACC_PST_TEST_ONLY_CONSUMER ${consumer_target}
        PAPACC_PST_TEST_ONLY_CONSUMER)
    get_target_property(PAPACC_PST_DIAGNOSTIC_CORRELATION_ACTIVE
        papacc_pst_consumer PAPACC_PST_DIAGNOSTIC_CORRELATION_ACTIVE)
    if(PAPACC_PST_TEST_ONLY_CONSUMER OR
       PAPACC_PST_DIAGNOSTIC_CORRELATION_ACTIVE)
        set(PAPACC_PST_RUNTIME_FILES
            "${PAPACC_PST_TEST_ONLY_SDK_ROOT}/bin/libssl-3-x64.dll"
            "${PAPACC_PST_TEST_ONLY_SDK_ROOT}/bin/libcrypto-3-x64.dll")
    else()
        get_target_property(PAPACC_PST_RUNTIME_FILES papacc_pst_consumer
            PAPACC_PST_RUNTIME_FILES)
    endif()
    foreach(PAPACC_PST_RUNTIME_FILE IN LISTS PAPACC_PST_RUNTIME_FILES)
        add_custom_command(TARGET ${consumer_target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${PAPACC_PST_RUNTIME_FILE}"
                "$<TARGET_FILE_DIR:${consumer_target}>"
            VERBATIM)
    endforeach()
endfunction()

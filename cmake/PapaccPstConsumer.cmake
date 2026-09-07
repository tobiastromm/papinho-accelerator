# Private build boundary for consumers of the pinned PST release SDK.
# This module does not acquire PST and does not create any runtime lifecycle.

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

    # A cache or command-line override must never select the dependency.
    unset(PAPACC_PST_SDK_ROOT CACHE)
    set(PAPACC_PST_SDK_ROOT
        "${CMAKE_BINARY_DIR}/dependencies/papinho-secure-transport/${PAPACC_PST_RELEASE_TAG}/${PAPACC_PST_TARGET}")
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
                "Pinned PST SDK is missing or incomplete. Run the "
                "papacc_acquire_pst target before enabling a PST consumer: "
                "${PAPACC_PST_REQUIRED_FILE}")
        endif()
    endforeach()

    add_library(papacc_pst_consumer INTERFACE)
    target_include_directories(papacc_pst_consumer INTERFACE
        "${PAPACC_PST_SDK_ROOT}/include")
    target_link_libraries(papacc_pst_consumer INTERFACE
        "${PAPACC_PST_LIB_DIR}/papinho_secure_transport.lib"
        "${PAPACC_PST_LIB_DIR}/libssl.lib"
        "${PAPACC_PST_LIB_DIR}/libcrypto.lib"
        ws2_32 crypt32)
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
    get_target_property(PAPACC_PST_RUNTIME_FILES papacc_pst_consumer
        PAPACC_PST_RUNTIME_FILES)
    foreach(PAPACC_PST_RUNTIME_FILE IN LISTS PAPACC_PST_RUNTIME_FILES)
        add_custom_command(TARGET ${consumer_target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${PAPACC_PST_RUNTIME_FILE}"
                "$<TARGET_FILE_DIR:${consumer_target}>"
            VERBATIM)
    endforeach()
endfunction()

cmake_minimum_required(VERSION 3.16)

project(PapinhoAccelerator VERSION 0.1.0 LANGUAGES C)

include(CTest)

function(papacc_enable_warnings target)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endfunction()

add_library(papacc_core STATIC
    src/core/version.c
)

target_include_directories(papacc_core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

set_target_properties(papacc_core PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED YES
    C_EXTENSIONS NO
)

papacc_enable_warnings(papacc_core)

add_executable(papacc_server
    apps/server/main.c
)

target_link_libraries(papacc_server PRIVATE papacc_core)

set_target_properties(papacc_server PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED YES
    C_EXTENSIONS NO
)

papacc_enable_warnings(papacc_server)

if(BUILD_TESTING)
    add_executable(papacc_version_test
        tests/version_test.c
    )

    target_link_libraries(papacc_version_test PRIVATE papacc_core)

    set_target_properties(papacc_version_test PROPERTIES
        C_STANDARD 99
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO
    )

    papacc_enable_warnings(papacc_version_test)

    add_test(NAME papacc.version COMMAND papacc_version_test)
endif()

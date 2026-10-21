cmake_minimum_required(VERSION 3.16)
project(PapinhoPACC VERSION 1.1.0 LANGUAGES C)

add_library(papacc_protocol STATIC
    src/frame.c
    src/frame_parser.c
    src/control_protocol.c
    src/data_association_protocol.c
    src/capability_wire.c
    src/capability_negotiation_protocol.c
    src/network_offload_protocol.c)
target_include_directories(papacc_protocol PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/papacc/pacc)
set_target_properties(papacc_protocol PROPERTIES
    C_STANDARD 90 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
if(MSVC)
    target_compile_options(papacc_protocol PRIVATE /W4)
else()
    target_compile_options(papacc_protocol PRIVATE -Wall -Wextra -Wpedantic)
endif()

include(CTest)
if(BUILD_TESTING)
    add_executable(papinhopacc_consumer_smoke tests/consumer_smoke.c)
    target_link_libraries(papinhopacc_consumer_smoke PRIVATE papacc_protocol)
    add_test(NAME papinhopacc.consumer_smoke COMMAND papinhopacc_consumer_smoke)
endif()

cmake_minimum_required(VERSION 3.16)

project(PapinhoAccelerator VERSION 0.1.0 LANGUAGES C)

if(MSVC AND CMAKE_GENERATOR MATCHES "Ninja")
    set(CMAKE_C_COMPILER_LAUNCHER
        "${CMAKE_CURRENT_SOURCE_DIR}/tools/msvc-ninja-utf8.cmd")
    # UTF-8 bytes match the existing chcp65001 compiler launcher. Keep source ASCII.
    string(ASCII 195 167 195 163 _papacc_msvc_utf8_accents)
    set(CMAKE_CL_SHOWINCLUDES_PREFIX "Observa${_papacc_msvc_utf8_accents}o: incluindo arquivo: ")
    unset(_papacc_msvc_utf8_accents)
endif()

include(CTest)
include(cmake/PapaccPstConsumer.cmake)

option(PAPACC_ENABLE_PST_OUTBOUND_PROOF
    "Build opt-in PST consumers, tests, and TLS 1.3 Internet proof" OFF)
option(PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS
    "Build private owner-terminal observer/injection hooks for tests" OFF)

option(PAPACC_ENABLE_CANONICAL_API3_CANDIDATE
    "Isolated normal API3 SDK candidate; production pin and owner unchanged" OFF)
if(PAPACC_ENABLE_CANONICAL_API3_CANDIDATE)
    set(CMAKE_MSVC_RUNTIME_LIBRARY MultiThreadedDLL)
    add_compile_definitions(PAPACC_ENABLE_CANONICAL_API3_CANDIDATE=1)
endif()

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

# Portable issuer rules only. No native adapter or product activation implied.
# Keep these dependencies explicit even with localized MSVC include scanning.
set_source_files_properties(src/issuer/issuer_core.c tests/issuer_core_test.c
    tests/issuer_production_gate_test.c PROPERTIES OBJECT_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/issuer/issuer_core.h;${CMAKE_CURRENT_SOURCE_DIR}/include/papacc/types.h")
add_library(papacc_issuer STATIC src/issuer/issuer_core.c src/issuer/issuer_codec.c src/issuer/qualified_utc.c)
# Private in-memory proof/profile/root-port layout is shared by these consumers.
# Keep indirect includes explicit for the localized MSVC/Ninja build boundary.
set_property(SOURCE src/issuer/issuer_codec.c apps/server/server_launcher_owner_win32.c
    apps/server/server_c2_owner_win32.c apps/server/server_configuration_commit_win32.c
    apps/server/server_issuer_cng_backend_win32.c apps/server/server_issuer_x509_win32.c
    tests/issuer_checkpoint_test.c tests/issuer_cng_x509_test.c
    APPEND PROPERTY OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/issuer/issuer_core.h")
target_include_directories(papacc_issuer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/issuer)
target_link_libraries(papacc_issuer PUBLIC papacc_core)
set_target_properties(papacc_issuer PROPERTIES C_STANDARD 90 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
papacc_enable_warnings(papacc_issuer)
if(BUILD_TESTING)
    # Separate translation unit: test acceptance can never enter papacc_issuer.
    add_executable(papacc_issuer_offline_test tests/issuer_core_test.c src/issuer/issuer_core.c)
    target_compile_definitions(papacc_issuer_offline_test PRIVATE PAPACC_ISSUER_OFFLINE_TEST=1)
    target_include_directories(papacc_issuer_offline_test PRIVATE src/issuer include)
    set_target_properties(papacc_issuer_offline_test PROPERTIES C_STANDARD 90 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
    papacc_enable_warnings(papacc_issuer_offline_test)
    add_test(NAME papacc.issuer.offline COMMAND papacc_issuer_offline_test)
    add_executable(papacc_issuer_production_gate_test tests/issuer_production_gate_test.c)
    target_link_libraries(papacc_issuer_production_gate_test PRIVATE papacc_issuer)
    papacc_enable_warnings(papacc_issuer_production_gate_test)
    add_test(NAME papacc.issuer.production_gate COMMAND papacc_issuer_production_gate_test)
    add_executable(papacc_qualified_utc_test tests/qualified_utc_test.c)
    target_link_libraries(papacc_qualified_utc_test PRIVATE papacc_issuer)
    set_target_properties(papacc_qualified_utc_test PROPERTIES C_STANDARD 90 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
    papacc_enable_warnings(papacc_qualified_utc_test)
    add_test(NAME papacc.issuer.qualified_utc_data COMMAND papacc_qualified_utc_test)
    if(WIN32)
    add_executable(papacc_issuer_checkpoint_test tests/issuer_checkpoint_test.c)
    target_link_libraries(papacc_issuer_checkpoint_test PRIVATE papacc_issuer bcrypt)
    set_target_properties(papacc_issuer_checkpoint_test PROPERTIES C_STANDARD 90 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
    papacc_enable_warnings(papacc_issuer_checkpoint_test)
    add_test(NAME papacc.issuer.checkpoint COMMAND papacc_issuer_checkpoint_test)
    endif()
endif()

# Offline Roughtime DATA codec; never linked to a source/worker authority.
add_library(papacc_roughtime STATIC src/issuer/roughtime.c)
target_include_directories(papacc_roughtime PUBLIC src/issuer)
target_link_libraries(papacc_roughtime PUBLIC papacc_issuer)
set_target_properties(papacc_roughtime PROPERTIES C_STANDARD 90 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
papacc_enable_warnings(papacc_roughtime)
if(BUILD_TESTING)
 add_executable(papacc_roughtime_test tests/roughtime_test.c)
 target_link_libraries(papacc_roughtime_test PRIVATE papacc_roughtime)
 set_target_properties(papacc_roughtime_test PROPERTIES C_STANDARD 90 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
 papacc_enable_warnings(papacc_roughtime_test)
 add_test(NAME papacc.issuer.roughtime_data COMMAND papacc_roughtime_test)
endif()

# Bounded offline manual-package DATA codec, never a crypto/provider authority.
add_library(papacc_manual_package STATIC src/provisioning/manual_package.c)
target_include_directories(papacc_manual_package PUBLIC src/provisioning)
set_target_properties(papacc_manual_package PROPERTIES C_STANDARD 90 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
papacc_enable_warnings(papacc_manual_package)
set_source_files_properties(src/provisioning/manual_package.c tests/manual_package_test.c PROPERTIES OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/provisioning/manual_package.h")
if(BUILD_TESTING)
 add_executable(papacc_manual_package_test tests/manual_package_test.c)
 target_link_libraries(papacc_manual_package_test PRIVATE papacc_manual_package)
 set_target_properties(papacc_manual_package_test PROPERTIES C_STANDARD 90 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
 papacc_enable_warnings(papacc_manual_package_test)
 add_test(NAME papacc.manual_package.structure COMMAND papacc_manual_package_test)
endif()

# One authoritative, transport-independent implementation of PACC wire logic.
add_library(papacc_protocol STATIC
    src/protocol/frame.c
    src/protocol/frame_parser.c
    src/protocol/control_protocol.c
    src/protocol/data_association_protocol.c
    src/protocol/capability_wire.c
    src/protocol/capability_negotiation_protocol.c
    src/protocol/network_offload_protocol.c)
target_include_directories(papacc_protocol PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(papacc_protocol PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/protocol)
set_target_properties(papacc_protocol PROPERTIES
    C_STANDARD 90 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
papacc_enable_warnings(papacc_protocol)

if(BUILD_TESTING)
    add_executable(papacc_papinhopacc_wire_golden_test
        tests/papinhopacc_wire_golden_test.c)
    target_link_libraries(papacc_papinhopacc_wire_golden_test PRIVATE
        papacc_protocol)
    papacc_enable_warnings(papacc_papinhopacc_wire_golden_test)
    add_test(NAME papacc.papinhopacc.wire_golden
        COMMAND papacc_papinhopacc_wire_golden_test)
endif()

add_custom_target(papacc_protocol_package
    COMMAND ${CMAKE_COMMAND}
        -DPAPACC_SOURCE_ROOT=${CMAKE_CURRENT_SOURCE_DIR}
        -DPAPACC_BINARY_ROOT=${CMAKE_BINARY_DIR}
        -P ${CMAKE_CURRENT_SOURCE_DIR}/tools/package-papinhopacc.cmake
    VERBATIM)

add_library(papacc_capability STATIC
    src/capability/capability.c
    src/capability/builtin_capabilities.c
    src/capability/session_capability_store.c
)
target_include_directories(papacc_capability PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/capability)
target_link_libraries(papacc_capability PUBLIC papacc_core papacc_protocol)
set_target_properties(papacc_capability PROPERTIES
    C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
papacc_enable_warnings(papacc_capability)

if(BUILD_TESTING)
    add_executable(papacc_capability_test tests/capability_test.c)
    target_link_libraries(papacc_capability_test PRIVATE papacc_capability)
    set_target_properties(papacc_capability_test PROPERTIES
        C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
    papacc_enable_warnings(papacc_capability_test)
    add_test(NAME papacc.capability.framework COMMAND papacc_capability_test)
endif()

add_library(papacc_network STATIC
    src/network/bind_selection.c
    src/network/bind_target.c
    src/network/ip_address.c
    src/network/network_endpoint.c
    src/network/network_interface.c
    src/network/interface_snapshot_copy.c
    src/network/persistent_bind_selection.c
)

target_include_directories(papacc_network
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/network
)

target_link_libraries(papacc_network PUBLIC papacc_core)

set_target_properties(papacc_network PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED YES
    C_EXTENSIONS NO
)

papacc_enable_warnings(papacc_network)

add_library(papacc_connection STATIC
    src/runtime/connection.c
    src/transports/transport_connection.c
)

target_include_directories(papacc_connection
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime
        ${CMAKE_CURRENT_SOURCE_DIR}/src/transports
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/network
)

target_link_libraries(papacc_connection PUBLIC papacc_network)

set_target_properties(papacc_connection PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED YES
    C_EXTENSIONS NO
)

papacc_enable_warnings(papacc_connection)

add_library(papacc_session STATIC
    src/runtime/session.c
)

target_include_directories(papacc_session
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime
)

target_link_libraries(papacc_session PUBLIC papacc_core)

set_target_properties(papacc_session PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED YES
    C_EXTENSIONS NO
)

papacc_enable_warnings(papacc_session)

add_library(papacc_compute STATIC
    src/compute/job.c
)
target_include_directories(papacc_compute PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/compute
    ${CMAKE_CURRENT_SOURCE_DIR}/src/capability
    ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime)
target_link_libraries(papacc_compute PUBLIC
    papacc_core papacc_capability papacc_session)
set_target_properties(papacc_compute PROPERTIES
    C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
papacc_enable_warnings(papacc_compute)

add_library(papacc_network_egress STATIC
    src/network_egress/exact_policy.c
    src/network_egress/egress_runtime_config.c
    src/network_egress/egress_policy.c
    src/network_egress/destination.c
    src/network_egress/network_authorization.c
    src/network_egress/network_stream.c)
target_include_directories(papacc_network_egress PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/network_egress
    ${CMAKE_CURRENT_SOURCE_DIR}/src/network
    ${CMAKE_CURRENT_SOURCE_DIR}/src/protocol)
target_link_libraries(papacc_network_egress PUBLIC
    papacc_core papacc_network papacc_capability papacc_network_offload_protocol)
set_target_properties(papacc_network_egress PROPERTIES
    C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
papacc_enable_warnings(papacc_network_egress)

if(BUILD_TESTING)
    add_executable(papacc_phase6_portable_test tests/phase6_portable_test.c)
    target_link_libraries(papacc_phase6_portable_test PRIVATE
        papacc_network_egress papacc_network_offload_protocol
        papacc_capability_negotiation_protocol papacc_capability)
    set_target_properties(papacc_phase6_portable_test PROPERTIES
        C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
    papacc_enable_warnings(papacc_phase6_portable_test)
    add_test(NAME papacc.phase6.portable COMMAND papacc_phase6_portable_test)

    add_executable(papacc_phase6_security_matrix_test
        tests/phase6_security_matrix_test.c)
    target_link_libraries(papacc_phase6_security_matrix_test PRIVATE
        papacc_network_egress papacc_capability)
    set_target_properties(papacc_phase6_security_matrix_test PROPERTIES
        C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
    papacc_enable_warnings(papacc_phase6_security_matrix_test)
    add_test(NAME papacc.phase6.security_matrix
        COMMAND papacc_phase6_security_matrix_test)

    add_executable(papacc_phase6_wire_runtime_test
        tests/phase6_wire_runtime_test.c)
    target_link_libraries(papacc_phase6_wire_runtime_test PRIVATE
        papacc_network_offload_protocol
        papacc_capability_negotiation_protocol)
    set_target_properties(papacc_phase6_wire_runtime_test PROPERTIES
        C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
    papacc_enable_warnings(papacc_phase6_wire_runtime_test)
    add_test(NAME papacc.phase6.wire_runtime
        COMMAND papacc_phase6_wire_runtime_test)
endif()

if(BUILD_TESTING)
    add_executable(papacc_job_test tests/job_test.c)
    target_link_libraries(papacc_job_test PRIVATE papacc_compute)
    set_target_properties(papacc_job_test PROPERTIES
        C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
    papacc_enable_warnings(papacc_job_test)
    add_test(NAME papacc.compute.job_framework COMMAND papacc_job_test)
endif()

add_library(papacc_channel STATIC
    src/runtime/channel.c
)

target_include_directories(papacc_channel
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/network
        ${CMAKE_CURRENT_SOURCE_DIR}/src/transports
)

target_link_libraries(
    papacc_channel PUBLIC papacc_core papacc_connection papacc_session)

set_target_properties(papacc_channel PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED YES
    C_EXTENSIONS NO
)

papacc_enable_warnings(papacc_channel)

add_library(papacc_protocol_framing INTERFACE)
target_link_libraries(papacc_protocol_framing INTERFACE papacc_protocol)
target_include_directories(papacc_protocol_framing INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/protocol)

add_library(papacc_framed_reader STATIC
    src/protocol/framed_reader.c
)

target_include_directories(papacc_framed_reader
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/protocol
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/transports
)

target_link_libraries(
    papacc_framed_reader
    PUBLIC papacc_protocol_framing papacc_connection)

set_target_properties(papacc_framed_reader PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED YES
    C_EXTENSIONS NO
)

papacc_enable_warnings(papacc_framed_reader)

add_library(papacc_framed_writer STATIC
    src/protocol/framed_writer.c
)

target_include_directories(papacc_framed_writer
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/protocol
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/transports
)

target_link_libraries(
    papacc_framed_writer
    PUBLIC papacc_protocol_framing papacc_connection)

set_target_properties(papacc_framed_writer PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED YES
    C_EXTENSIONS NO
)

papacc_enable_warnings(papacc_framed_writer)

add_library(papacc_control_protocol INTERFACE)
target_link_libraries(papacc_control_protocol INTERFACE papacc_protocol)
target_include_directories(papacc_control_protocol INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/protocol)

add_library(papacc_capability_negotiation_protocol STATIC
    src/protocol/capability_negotiation.c)
target_include_directories(papacc_capability_negotiation_protocol PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/protocol
    ${CMAKE_CURRENT_SOURCE_DIR}/src/capability)
target_link_libraries(papacc_capability_negotiation_protocol PUBLIC
    papacc_protocol papacc_capability)
set_target_properties(papacc_capability_negotiation_protocol PROPERTIES
    C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
papacc_enable_warnings(papacc_capability_negotiation_protocol)

add_library(papacc_network_offload_protocol STATIC
    src/protocol/network_data_state.c)
target_include_directories(papacc_network_offload_protocol PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/protocol)
target_link_libraries(papacc_network_offload_protocol PUBLIC papacc_protocol)
set_target_properties(papacc_network_offload_protocol PROPERTIES
    C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
papacc_enable_warnings(papacc_network_offload_protocol)

add_library(papacc_control_processor STATIC
    src/protocol/control_processor.c
)
target_include_directories(papacc_control_processor
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/protocol
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime
            ${CMAKE_CURRENT_SOURCE_DIR}/src/transports
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network)
target_link_libraries(papacc_control_processor PUBLIC
    papacc_control_protocol papacc_framed_reader papacc_framed_writer
    papacc_channel papacc_session papacc_connection)
set_target_properties(papacc_control_processor PROPERTIES
    C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
papacc_enable_warnings(papacc_control_processor)

add_library(papacc_data_association_protocol INTERFACE)
target_link_libraries(papacc_data_association_protocol INTERFACE papacc_protocol)
target_include_directories(papacc_data_association_protocol INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/protocol)

add_library(papacc_data_association STATIC
    src/protocol/data_association.c)
target_include_directories(papacc_data_association PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/protocol
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network_egress
            ${CMAKE_CURRENT_SOURCE_DIR}/src/transports
            ${CMAKE_CURRENT_SOURCE_DIR}/src/capability)
target_link_libraries(papacc_data_association PUBLIC
    papacc_data_association_protocol papacc_channel papacc_session)
set_target_properties(papacc_data_association PROPERTIES
    C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
papacc_enable_warnings(papacc_data_association)

add_library(papacc_post_control_processor STATIC
    src/protocol/post_control_processor.c)
target_include_directories(papacc_post_control_processor PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/protocol
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network
            ${CMAKE_CURRENT_SOURCE_DIR}/src/transports)
target_link_libraries(papacc_post_control_processor PUBLIC
    papacc_data_association papacc_framed_reader papacc_framed_writer
    papacc_connection papacc_channel papacc_session
    papacc_capability_negotiation_protocol papacc_capability)
set_target_properties(papacc_post_control_processor PROPERTIES
    C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
papacc_enable_warnings(papacc_post_control_processor)

add_library(papacc_connection_classifier STATIC
    src/protocol/connection_classifier.c)
target_include_directories(papacc_connection_classifier PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/protocol
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network
            ${CMAKE_CURRENT_SOURCE_DIR}/src/transports)
target_link_libraries(papacc_connection_classifier PUBLIC
    papacc_framed_reader papacc_connection papacc_control_protocol
    papacc_data_association_protocol)
set_target_properties(papacc_connection_classifier PROPERTIES
    C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
papacc_enable_warnings(papacc_connection_classifier)

add_library(papacc_data_attach_processor STATIC
    src/protocol/data_attach_processor.c)
target_include_directories(papacc_data_attach_processor PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/protocol
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network
            ${CMAKE_CURRENT_SOURCE_DIR}/src/transports)
target_link_libraries(papacc_data_attach_processor PUBLIC
    papacc_data_association papacc_data_association_protocol
    papacc_framed_reader papacc_framed_writer papacc_connection
    papacc_channel papacc_session)
set_target_properties(papacc_data_attach_processor PROPERTIES
    C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
papacc_enable_warnings(papacc_data_attach_processor)

if(WIN32)
    add_library(papacc_tcp_win32 STATIC
        src/platform/win32/tcp_accept_win32.c
        src/platform/win32/tcp_platform_win32.c
        src/platform/win32/tcp_listener_set_win32.c
        src/platform/win32/tcp_socket_win32.c
    )

    target_include_directories(papacc_tcp_win32
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network
            ${CMAKE_CURRENT_SOURCE_DIR}/src/transports/tcp
    )

    target_link_libraries(
        papacc_tcp_win32
        PUBLIC papacc_core papacc_network
        PRIVATE ws2_32
    )

    set_target_properties(papacc_tcp_win32 PROPERTIES
        C_STANDARD 99
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO
    )

    papacc_enable_warnings(papacc_tcp_win32)

    add_library(papacc_tcp_connection_win32 STATIC
        src/platform/win32/tcp_connection_transport_win32.c
        src/platform/win32/tcp_outbound_win32.c
    )

    target_include_directories(papacc_tcp_connection_win32
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network
            ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/win32
            ${CMAKE_CURRENT_SOURCE_DIR}/src/transports
            ${CMAKE_CURRENT_SOURCE_DIR}/src/transports/tcp
    )

    target_link_libraries(
        papacc_tcp_connection_win32
        PUBLIC papacc_connection papacc_tcp_win32
        PRIVATE ws2_32
    )

    set_target_properties(papacc_tcp_connection_win32 PROPERTIES
        C_STANDARD 99
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO
    )

    papacc_enable_warnings(papacc_tcp_connection_win32)

    add_library(papacc_network_win32 STATIC
        src/platform/win32/network_interfaces_win32.c
    )

    target_include_directories(papacc_network_win32
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network
    )

    target_link_libraries(
        papacc_network_win32
        PUBLIC papacc_network
        PRIVATE iphlpapi
    )

    set_target_properties(papacc_network_win32 PROPERTIES
        C_STANDARD 99
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO
    )

    papacc_enable_warnings(papacc_network_win32)

    add_library(papacc_pal STATIC
        src/platform/win32/pal_time_win32.c
    )

    target_include_directories(papacc_pal
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/platform
    )

    target_link_libraries(papacc_pal PUBLIC papacc_core)

    set_target_properties(papacc_pal PROPERTIES
        C_STANDARD 99
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO
    )

    papacc_enable_warnings(papacc_pal)

    add_library(papacc_runtime STATIC
        src/runtime/log.c
        src/runtime/runtime.c
    )

    target_include_directories(papacc_runtime
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/platform
            ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime
    )

    target_link_libraries(papacc_runtime PUBLIC papacc_pal)

    set_target_properties(papacc_runtime PROPERTIES
        C_STANDARD 99
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO
    )

    papacc_enable_warnings(papacc_runtime)
endif()

add_library(papacc_server_config STATIC
    apps/server/server_config.c
)

target_include_directories(papacc_server_config
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/server
        ${CMAKE_CURRENT_SOURCE_DIR}/src/network
)

target_link_libraries(papacc_server_config PUBLIC papacc_network)

set_target_properties(papacc_server_config PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED YES
    C_EXTENSIONS NO
)

papacc_enable_warnings(papacc_server_config)

add_library(papacc_server_cli STATIC
    apps/server/server_cli.c
)

target_include_directories(papacc_server_cli
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/server
        ${CMAKE_CURRENT_SOURCE_DIR}/src/network
        ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime
)

target_link_libraries(papacc_server_cli PUBLIC papacc_server_config papacc_runtime)

set_target_properties(papacc_server_cli PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED YES
    C_EXTENSIONS NO
)

papacc_enable_warnings(papacc_server_cli)

if(WIN32)
    unset(PAPACC_POWERSHELL_EXECUTABLE CACHE)

    add_custom_target(papacc_acquire_pst
        COMMAND powershell
            -NoProfile -ExecutionPolicy Bypass
            -File ${CMAKE_CURRENT_SOURCE_DIR}/tools/acquire-pst-release.ps1
            -OutputRoot
            ${CMAKE_BINARY_DIR}/dependencies/papinho-secure-transport
        COMMENT "Acquire and verify the pinned PapinhoSecureTransport release"
        VERBATIM)

    add_executable(papacc_server
        apps/server/main.c
    )

    add_library(papacc_server_cli_interfaces STATIC
        apps/server/server_cli_interfaces.c
    )

    target_include_directories(papacc_server_cli_interfaces
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/apps/server
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network
    )

    target_link_libraries(
        papacc_server_cli_interfaces PUBLIC papacc_network_win32)

    set_target_properties(papacc_server_cli_interfaces PROPERTIES
        C_STANDARD 99
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO
    )

    papacc_enable_warnings(papacc_server_cli_interfaces)

    add_library(papacc_server_network STATIC
        apps/server/server_network.c
    )

    target_include_directories(papacc_server_network
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/apps/server
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network
            ${CMAKE_CURRENT_SOURCE_DIR}/src/transports/tcp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/win32
    )

    target_link_libraries(
        papacc_server_network
        PUBLIC papacc_server_config papacc_network_win32 papacc_tcp_win32)

    set_target_properties(papacc_server_network PROPERTIES
        C_STANDARD 99
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO
    )

    papacc_enable_warnings(papacc_server_network)

    add_library(papacc_server_acceptor_win32 STATIC
        apps/server/server_acceptor_win32.c
    )

    target_include_directories(papacc_server_acceptor_win32
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/apps/server
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network
            ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/win32
            ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime
            ${CMAKE_CURRENT_SOURCE_DIR}/src/transports
            ${CMAKE_CURRENT_SOURCE_DIR}/src/transports/tcp
    )

    target_link_libraries(
        papacc_server_acceptor_win32
        PUBLIC
            papacc_server_network
            papacc_connection
            papacc_tcp_win32
            papacc_tcp_connection_win32
    )

    set_target_properties(papacc_server_acceptor_win32 PROPERTIES
        C_STANDARD 99
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO
    )

    papacc_enable_warnings(papacc_server_acceptor_win32)

    add_library(papacc_server_io_loop_win32 STATIC
        apps/server/server_io_loop_win32.c
        apps/server/server_data_ticket_generator.c
    )
    target_include_directories(papacc_server_io_loop_win32
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/apps/server
            ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network
            ${CMAKE_CURRENT_SOURCE_DIR}/src/platform
            ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/win32
            ${CMAKE_CURRENT_SOURCE_DIR}/src/protocol
            ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime
            ${CMAKE_CURRENT_SOURCE_DIR}/src/transports
            ${CMAKE_CURRENT_SOURCE_DIR}/src/transports/tcp
    )
    target_link_libraries(papacc_server_io_loop_win32 PUBLIC
        papacc_server_acceptor_win32 papacc_connection_classifier
        papacc_control_processor papacc_post_control_processor
        papacc_data_attach_processor papacc_pal papacc_runtime)
    target_link_libraries(papacc_server_io_loop_win32 PUBLIC
        papacc_compute papacc_network_egress papacc_capability)
    set_target_properties(papacc_server_io_loop_win32 PROPERTIES
        C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
    papacc_enable_warnings(papacc_server_io_loop_win32)
    if(PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS)
        target_compile_definitions(papacc_server_io_loop_win32 PUBLIC
            PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS=1)
    endif()

    add_library(papacc_server_console_win32 STATIC
        apps/server/server_console_win32.c
    )

    target_include_directories(papacc_server_console_win32
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/apps/server
    )

    target_link_libraries(papacc_server_console_win32 PUBLIC papacc_core)

    set_target_properties(papacc_server_console_win32 PROPERTIES
        C_STANDARD 99
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO
    )

    papacc_enable_warnings(papacc_server_console_win32)

    add_library(papacc_server_run_win32 STATIC
        apps/server/server_run_win32.c
        apps/server/server_application_lifetime_win32.c
        apps/server/server_egress_source_win32.c
    )

    target_include_directories(papacc_server_run_win32
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/apps/server
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network
            ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/win32
            ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime
            ${CMAKE_CURRENT_SOURCE_DIR}/src/transports
            ${CMAKE_CURRENT_SOURCE_DIR}/src/transports/tcp
    )

    target_link_libraries(
        papacc_server_run_win32
        PUBLIC
            papacc_server_acceptor_win32
            papacc_server_io_loop_win32
            papacc_server_console_win32
            papacc_server_network
    )

    set_target_properties(papacc_server_run_win32 PROPERTIES
        C_STANDARD 99
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO
    )

    papacc_enable_warnings(papacc_server_run_win32)

    target_include_directories(papacc_server
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/apps/server
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network
            ${CMAKE_CURRENT_SOURCE_DIR}/src/transports/tcp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/win32
    )
    target_link_libraries(
        papacc_server
        PRIVATE
            papacc_server_cli
            papacc_server_cli_interfaces
            papacc_server_network
            papacc_server_console_win32
            papacc_server_run_win32)

    set_target_properties(papacc_server PROPERTIES
        C_STANDARD 99
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO
    )

    papacc_enable_warnings(papacc_server)

    if(PAPACC_ENABLE_PST_OUTBOUND_PROOF)
        papacc_define_pst_consumer_boundary()

        add_library(papacc_security_composition STATIC
            src/security/authentication_authorization.c
            src/security/security_composition.c
            src/security/pst_log_adapter.c
            src/security/pst_peer_evidence.c
            src/security/pst_secure_processor.c
            src/security/pst_secure_scheduler.c
            src/security/pst_secure_transport_adapter.c
            src/security/server_listener_configuration.c
            src/security/server_security_configuration.c
            src/security/secure_data_association.c
            src/security/platform/win32/pst_external_source_win32.c
            src/security/platform/win32/pst_provider_bootstrap_win32.c)
        target_sources(papacc_security_composition PRIVATE
            src/network_egress/pst_outbound_tls.c)
        target_include_directories(papacc_security_composition
            PUBLIC
                ${CMAKE_CURRENT_SOURCE_DIR}/src/security
                ${CMAKE_CURRENT_SOURCE_DIR}/src/security/platform/win32
                ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime
                ${CMAKE_CURRENT_SOURCE_DIR}/src/network
                ${CMAKE_CURRENT_SOURCE_DIR}/src/network_egress
                ${CMAKE_CURRENT_SOURCE_DIR}/src/transports
                ${CMAKE_CURRENT_SOURCE_DIR}/src/protocol)
        target_link_libraries(papacc_security_composition
            PUBLIC papacc_core papacc_runtime papacc_data_association papacc_channel
            PRIVATE papacc_pst_consumer)
        set_target_properties(papacc_security_composition PROPERTIES
            C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        papacc_enable_warnings(papacc_security_composition)
        if(PAPACC_PST_TEST_ONLY_CORRELATION_HEADER)
            get_filename_component(PAPACC_PST_TEST_ONLY_CORRELATION_INCLUDE_DIR
                "${PAPACC_PST_TEST_ONLY_CORRELATION_HEADER}" DIRECTORY)
            target_compile_definitions(papacc_security_composition PUBLIC
                PAPACC_ENABLE_PST_TEST_CORRELATION=1)
            target_include_directories(papacc_security_composition PRIVATE
                "${PAPACC_PST_TEST_ONLY_CORRELATION_INCLUDE_DIR}")
            target_compile_definitions(papacc_server_io_loop_win32 PUBLIC
                PAPACC_ENABLE_PST_TEST_CORRELATION=1)
        endif()

        target_compile_definitions(papacc_server_io_loop_win32 PUBLIC            PAPACC_ENABLE_PST_OUTBOUND_TLS=1)
        target_include_directories(papacc_server_io_loop_win32 PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/src/security
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network_egress
            ${PAPACC_PST_INCLUDE_DIR})
        target_link_libraries(papacc_server_io_loop_win32 PUBLIC
            papacc_security_composition papacc_pst_consumer)

        target_sources(papacc_server_run_win32 PRIVATE
            apps/server/server_security_source_win32.c
            apps/server/server_outbound_tls_source_win32.c)
        target_compile_definitions(papacc_server_run_win32 PUBLIC
            PAPACC_ENABLE_PST_OUTBOUND_TLS=1)
        target_include_directories(papacc_server_run_win32 PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/security
            ${CMAKE_CURRENT_SOURCE_DIR}/src/security/platform/win32
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network_egress
            ${PAPACC_PST_INCLUDE_DIR})
        target_link_libraries(papacc_server_run_win32 PUBLIC
            papacc_server_secure_io_win32 papacc_security_composition
            papacc_pst_consumer crypt32)

        add_executable(papacc_security_composition_test
            tests/security_composition_test.c)
        target_link_libraries(papacc_security_composition_test PRIVATE
            papacc_security_composition papacc_pst_consumer)
        set_target_properties(papacc_security_composition_test PROPERTIES
            C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        papacc_enable_warnings(papacc_security_composition_test)
        papacc_stage_pst_runtime(papacc_security_composition_test)
        add_test(NAME papacc_security_composition_test
            COMMAND papacc_security_composition_test)

        add_executable(papacc_pst_log_adapter_test
            tests/pst_log_adapter_test.c)
        target_link_libraries(papacc_pst_log_adapter_test PRIVATE
            papacc_security_composition papacc_pst_consumer)
        set_target_properties(papacc_pst_log_adapter_test PROPERTIES
            C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        papacc_enable_warnings(papacc_pst_log_adapter_test)
        papacc_stage_pst_runtime(papacc_pst_log_adapter_test)
        add_test(NAME papacc_pst_log_adapter_test
            COMMAND papacc_pst_log_adapter_test)

        add_executable(papacc_pst_secure_scheduler_test
            tests/pst_secure_scheduler_test.c)
        target_include_directories(papacc_pst_secure_scheduler_test PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime
            ${CMAKE_CURRENT_SOURCE_DIR}/src/transports)
        target_link_libraries(papacc_pst_secure_scheduler_test PRIVATE
            papacc_security_composition papacc_security_test_support
            papacc_connection papacc_pst_consumer ws2_32)
        set_target_properties(papacc_pst_secure_scheduler_test PROPERTIES
            C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        if(PAPACC_PST_TEST_ONLY_SDK_ROOT)
            set_property(TARGET papacc_pst_secure_scheduler_test
                PROPERTY PAPACC_PST_TEST_ONLY_CONSUMER TRUE)
        endif()
        papacc_enable_warnings(papacc_pst_secure_scheduler_test)
        papacc_stage_pst_runtime(papacc_pst_secure_scheduler_test)
        add_test(NAME papacc_pst_secure_scheduler_test
            COMMAND papacc_pst_secure_scheduler_test)
        add_test(NAME papacc.adr0032.p1.real_pst_pre_close
            COMMAND papacc_pst_secure_scheduler_test --adr0032-p1)

        add_executable(papacc_authentication_authorization_test
            tests/authentication_authorization_test.c)
        target_link_libraries(papacc_authentication_authorization_test PRIVATE
            papacc_security_composition papacc_pst_consumer)
        set_target_properties(papacc_authentication_authorization_test PROPERTIES
            C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        papacc_enable_warnings(papacc_authentication_authorization_test)
        papacc_stage_pst_runtime(papacc_authentication_authorization_test)
        add_test(NAME papacc_authentication_authorization_test
            COMMAND papacc_authentication_authorization_test)

        add_executable(papacc_authentication_authorization_real_mtls_test
            tests/authentication_authorization_real_mtls_test.c)
        target_compile_definitions(papacc_authentication_authorization_real_mtls_test PRIVATE
            PAPACC_TEST_PKI_DIR="${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/security")
        target_link_libraries(papacc_authentication_authorization_real_mtls_test PRIVATE
            papacc_security_composition papacc_pst_consumer
            papacc_security_test_support ws2_32 crypt32)
        set_target_properties(papacc_authentication_authorization_real_mtls_test PROPERTIES
            C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        papacc_enable_warnings(papacc_authentication_authorization_real_mtls_test)
        papacc_stage_pst_runtime(papacc_authentication_authorization_real_mtls_test)
        add_test(NAME papacc_authentication_authorization_real_mtls_test
            COMMAND papacc_authentication_authorization_real_mtls_test)

        add_library(papacc_security_test_support STATIC
            tests/security_test_support.c)
        target_include_directories(papacc_security_test_support PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/tests
            ${CMAKE_CURRENT_SOURCE_DIR}/include)
        target_compile_definitions(papacc_security_test_support PRIVATE
            PAPACC_TEST_PKI_DIR="${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/security")
        target_link_libraries(papacc_security_test_support PUBLIC
            papacc_pst_consumer crypt32)
        set_target_properties(papacc_security_test_support PROPERTIES
            C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        papacc_enable_warnings(papacc_security_test_support)

        add_executable(papacc_pst_outbound_tls_test
            tests/pst_outbound_tls_test.c)
        target_link_libraries(papacc_pst_outbound_tls_test PRIVATE
            papacc_security_composition papacc_security_test_support
            papacc_pst_consumer ws2_32)
        set_target_properties(papacc_pst_outbound_tls_test PROPERTIES
            C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        papacc_enable_warnings(papacc_pst_outbound_tls_test)
        papacc_stage_pst_runtime(papacc_pst_outbound_tls_test)
        add_test(NAME papacc_pst_outbound_tls_test
            COMMAND papacc_pst_outbound_tls_test)

        add_executable(papacc_pst_outbound_tls_lifecycle_test
            tests/pst_outbound_tls_lifecycle_test.c)
        target_include_directories(papacc_pst_outbound_tls_lifecycle_test PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network_egress)
        target_link_libraries(papacc_pst_outbound_tls_lifecycle_test PRIVATE
            papacc_network_egress papacc_security_composition
            papacc_security_test_support papacc_pst_consumer ws2_32)
        set_target_properties(papacc_pst_outbound_tls_lifecycle_test PROPERTIES
            C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        papacc_enable_warnings(papacc_pst_outbound_tls_lifecycle_test)
        papacc_stage_pst_runtime(papacc_pst_outbound_tls_lifecycle_test)
        add_test(NAME papacc_pst_outbound_tls_lifecycle_test
            COMMAND papacc_pst_outbound_tls_lifecycle_test)

        add_executable(papacc_network_stream_ai_injection_test
            tests/network_stream_ai_injection_test.c)
        target_include_directories(papacc_network_stream_ai_injection_test PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network_egress)
        target_link_libraries(papacc_network_stream_ai_injection_test PRIVATE
            papacc_network_egress)
        set_target_properties(papacc_network_stream_ai_injection_test PROPERTIES
            C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        papacc_enable_warnings(papacc_network_stream_ai_injection_test)
        # D-I remain explicit commands for the governed PST 0.6.5 qualification.
        add_test(NAME papacc.network_stream.ai_oracle_selftest
            COMMAND papacc_network_stream_ai_injection_test selftest)
        add_executable(papacc_secure_reference_client
            tests/reference_client/secure_reference_client.c)
        target_link_libraries(papacc_secure_reference_client PRIVATE
            papacc_security_test_support papacc_pst_consumer
            papacc_protocol ws2_32)
        set_target_properties(papacc_secure_reference_client PROPERTIES
            C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        papacc_enable_warnings(papacc_secure_reference_client)
        papacc_stage_pst_runtime(papacc_secure_reference_client)

        add_executable(papacc_secure_data_association_test
            tests/secure_data_association_test.c)
        target_link_libraries(papacc_secure_data_association_test PRIVATE
            papacc_security_composition papacc_connection)
        set_target_properties(papacc_secure_data_association_test PROPERTIES
            C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        papacc_enable_warnings(papacc_secure_data_association_test)
        papacc_stage_pst_runtime(papacc_secure_data_association_test)
        add_test(NAME papacc_secure_data_association_test
            COMMAND papacc_secure_data_association_test)

        add_executable(papacc_server_security_configuration_test
            tests/server_security_configuration_test.c)
        target_link_libraries(papacc_server_security_configuration_test PRIVATE
            papacc_security_composition papacc_network papacc_pst_consumer)
        set_target_properties(papacc_server_security_configuration_test PROPERTIES
            C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        papacc_enable_warnings(papacc_server_security_configuration_test)
        papacc_stage_pst_runtime(papacc_server_security_configuration_test)
        add_test(NAME papacc_server_security_configuration_test
            COMMAND papacc_server_security_configuration_test)

        add_library(papacc_server_secure_io_win32 STATIC
            apps/server/server_secure_io_win32.c)
        target_include_directories(papacc_server_secure_io_win32
            PUBLIC
                ${CMAKE_CURRENT_SOURCE_DIR}/apps/server
            PRIVATE
                ${CMAKE_CURRENT_SOURCE_DIR}/src/network
                ${CMAKE_CURRENT_SOURCE_DIR}/src/platform
                ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/win32
                ${CMAKE_CURRENT_SOURCE_DIR}/src/protocol
                ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime
                ${CMAKE_CURRENT_SOURCE_DIR}/src/security
                ${CMAKE_CURRENT_SOURCE_DIR}/src/security/platform/win32
                ${CMAKE_CURRENT_SOURCE_DIR}/src/transports
                ${CMAKE_CURRENT_SOURCE_DIR}/src/transports/tcp)
        target_link_libraries(papacc_server_secure_io_win32
            PUBLIC papacc_server_io_loop_win32 papacc_security_composition
            PRIVATE papacc_pst_consumer papacc_tcp_win32 ws2_32)
        set_target_properties(papacc_server_secure_io_win32 PROPERTIES
            C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        papacc_enable_warnings(papacc_server_secure_io_win32)
        if(PAPACC_PST_TEST_ONLY_CORRELATION_HEADER)
            target_compile_definitions(papacc_server_secure_io_win32 PUBLIC
                PAPACC_ENABLE_PST_TEST_CORRELATION=1)
            target_include_directories(papacc_server_secure_io_win32 PRIVATE
                "${PAPACC_PST_TEST_ONLY_CORRELATION_INCLUDE_DIR}")
        endif()

        add_executable(papacc_server_secure_io_win32_test            tests/server_secure_io_win32_test.c)
        target_include_directories(papacc_server_secure_io_win32_test PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network
            ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/win32
            ${CMAKE_CURRENT_SOURCE_DIR}/src/protocol
            ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime
            ${CMAKE_CURRENT_SOURCE_DIR}/src/security
            ${CMAKE_CURRENT_SOURCE_DIR}/src/transports
            ${CMAKE_CURRENT_SOURCE_DIR}/src/transports/tcp)
        target_link_libraries(papacc_server_secure_io_win32_test PRIVATE
            papacc_server_secure_io_win32 papacc_pst_consumer)
        set_target_properties(papacc_server_secure_io_win32_test PROPERTIES
            C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        papacc_enable_warnings(papacc_server_secure_io_win32_test)
        papacc_stage_pst_runtime(papacc_server_secure_io_win32_test)
        add_test(NAME papacc_server_secure_io_win32_test
            COMMAND papacc_server_secure_io_win32_test)

        add_executable(papacc_server_secure_io_win32_integration_test
            tests/server_secure_io_win32_integration_test.c)
        option(PAPACC_ENABLE_P10_NORMAL_NATIVE_OBSERVER "Normal SDK P10 external native observation harness" OFF)
        if(PAPACC_ENABLE_P10_NORMAL_NATIVE_OBSERVER)
            if(NOT MSVC OR NOT PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS OR NOT PAPACC_PST_QUALIFICATION_SDK_ROOT OR PAPACC_PST_TEST_ONLY_SDK_ROOT OR PAPACC_PST_TEST_ONLY_CORRELATION_HEADER)
                message(FATAL_ERROR "P10 normal observer requires MSVC, consumer hooks and a qualified normal SDK")
            endif()
            target_compile_definitions(papacc_server_secure_io_win32_integration_test PRIVATE PAPACC_P10_NORMAL_NATIVE_OBSERVER=1)
            target_link_options(papacc_server_secure_io_win32_integration_test PRIVATE /MAP /DYNAMICBASE:NO)
            foreach(p10_normal_case IN ITEMS stop once persistent need-write queued-stop)
                add_test(NAME papacc.p10.normal.${p10_normal_case}
                    COMMAND papacc_server_secure_io_win32_integration_test
                        --isolated-p10-normal-${p10_normal_case})
                set_tests_properties(papacc.p10.normal.${p10_normal_case} PROPERTIES TIMEOUT 30 RUN_SERIAL TRUE)
            endforeach()
        endif()
        if(PAPACC_PST_TEST_ONLY_CORRELATION_HEADER)
            target_include_directories(
                papacc_server_secure_io_win32_integration_test PRIVATE
                "${PAPACC_PST_TEST_ONLY_CORRELATION_INCLUDE_DIR}")
        endif()
        target_include_directories(
            papacc_server_secure_io_win32_integration_test PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/apps/server
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network_egress
            ${CMAKE_CURRENT_SOURCE_DIR}/src/platform
            ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/win32
            ${CMAKE_CURRENT_SOURCE_DIR}/src/protocol
            ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime
            ${CMAKE_CURRENT_SOURCE_DIR}/src/security
            ${CMAKE_CURRENT_SOURCE_DIR}/src/security/platform/win32
            ${CMAKE_CURRENT_SOURCE_DIR}/src/transports
            ${CMAKE_CURRENT_SOURCE_DIR}/src/transports/tcp)
        target_link_libraries(
            papacc_server_secure_io_win32_integration_test PRIVATE
            papacc_server_secure_io_win32 papacc_security_test_support ws2_32)
        set_target_properties(
            papacc_server_secure_io_win32_integration_test PROPERTIES
            C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        if(PAPACC_PST_TEST_ONLY_SDK_ROOT)
            set_property(TARGET papacc_server_secure_io_win32_integration_test
                PROPERTY PAPACC_PST_TEST_ONLY_CONSUMER TRUE)
        endif()
        papacc_enable_warnings(papacc_server_secure_io_win32_integration_test)
        papacc_stage_pst_runtime(
            papacc_server_secure_io_win32_integration_test)

        add_executable(papacc_secure_browser_fixture
            tests/secure_fixture_server_win32.c)
        target_include_directories(papacc_secure_browser_fixture PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/apps/server
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network_egress
            ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/win32
            ${CMAKE_CURRENT_SOURCE_DIR}/src/protocol
            ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime
            ${CMAKE_CURRENT_SOURCE_DIR}/src/security
            ${CMAKE_CURRENT_SOURCE_DIR}/src/security/platform/win32
            ${CMAKE_CURRENT_SOURCE_DIR}/src/transports
            ${CMAKE_CURRENT_SOURCE_DIR}/src/transports/tcp)
        target_compile_definitions(papacc_secure_browser_fixture PRIVATE
            PAPACC_TEST_PKI_DIR="${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/security")
        target_link_libraries(papacc_secure_browser_fixture PRIVATE
            papacc_server_secure_io_win32 papacc_security_test_support ws2_32)
        set_target_properties(papacc_secure_browser_fixture PROPERTIES
            C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        papacc_enable_warnings(papacc_secure_browser_fixture)
        papacc_stage_pst_runtime(papacc_secure_browser_fixture)

        add_executable(papacc_secure_fixture_lifetime_test
            tests/secure_fixture_lifetime_win32_test.c)
        # This test embeds the fixture implementation. Keep the dependency
        # explicit even when localized MSVC include scanning is unavailable.
        set_property(SOURCE tests/secure_fixture_lifetime_win32_test.c APPEND
            PROPERTY OBJECT_DEPENDS
            "${CMAKE_CURRENT_SOURCE_DIR}/tests/secure_fixture_server_win32.c")
        get_target_property(fixture_includes papacc_secure_browser_fixture INCLUDE_DIRECTORIES)
        get_target_property(fixture_defines papacc_secure_browser_fixture COMPILE_DEFINITIONS)
        target_include_directories(papacc_secure_fixture_lifetime_test PRIVATE ${fixture_includes})
        target_compile_definitions(papacc_secure_fixture_lifetime_test PRIVATE ${fixture_defines})
        target_link_libraries(papacc_secure_fixture_lifetime_test PRIVATE
            papacc_server_secure_io_win32 papacc_security_test_support ws2_32)
        set_target_properties(papacc_secure_fixture_lifetime_test PROPERTIES
            C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        papacc_enable_warnings(papacc_secure_fixture_lifetime_test)
        papacc_stage_pst_runtime(papacc_secure_fixture_lifetime_test)
        foreach(lifetime_case A B C F)
            add_test(NAME papacc.fixture_lifetime.${lifetime_case}
                COMMAND papacc_secure_fixture_lifetime_test ${lifetime_case})
            set_tests_properties(papacc.fixture_lifetime.${lifetime_case}
                PROPERTIES TIMEOUT 30)
        endforeach()

        add_executable(papacc_lab_egress_targets
            tests/lab_egress_targets_win32.c)
        set_source_files_properties(tests/lab_egress_targets_win32.c PROPERTIES
            OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests/secure_fixture_server_win32.c")
        target_include_directories(papacc_lab_egress_targets PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/apps/server
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network_egress
            ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/win32
            ${CMAKE_CURRENT_SOURCE_DIR}/src/protocol
            ${CMAKE_CURRENT_SOURCE_DIR}/src/security
            ${CMAKE_CURRENT_SOURCE_DIR}/src/security/platform/win32
            ${CMAKE_CURRENT_SOURCE_DIR}/src/transports
            ${CMAKE_CURRENT_SOURCE_DIR}/src/transports/tcp)
        target_compile_definitions(papacc_lab_egress_targets PRIVATE
            PAPACC_TEST_PKI_DIR="${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/security")
        target_link_libraries(papacc_lab_egress_targets PRIVATE
            papacc_server_secure_io_win32 papacc_security_test_support ws2_32)
        set_target_properties(papacc_lab_egress_targets PROPERTIES
            C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        papacc_enable_warnings(papacc_lab_egress_targets)
        papacc_stage_pst_runtime(papacc_lab_egress_targets)

        add_executable(papacc_lab_egress_targets_multi_connection_test
            tests/lab_egress_targets_multi_connection_test.c)
        target_include_directories(
            papacc_lab_egress_targets_multi_connection_test PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/security/platform/win32)
        target_link_libraries(
            papacc_lab_egress_targets_multi_connection_test PRIVATE
            papacc_security_test_support papacc_security_composition
            papacc_pst_consumer ws2_32)
        set_target_properties(
            papacc_lab_egress_targets_multi_connection_test PROPERTIES
            C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        papacc_enable_warnings(papacc_lab_egress_targets_multi_connection_test)
        papacc_stage_pst_runtime(
            papacc_lab_egress_targets_multi_connection_test)
        add_dependencies(papacc_lab_egress_targets_multi_connection_test
            papacc_lab_egress_targets)
        add_test(NAME papacc.lab_egress_targets.multi_connection
            COMMAND papacc_lab_egress_targets_multi_connection_test
                $<TARGET_FILE:papacc_lab_egress_targets>)

        add_executable(papacc_mode_a_tls12_target
            tests/mode_a_tls12_target_win32.c)
        target_include_directories(papacc_mode_a_tls12_target PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/security/platform/win32)
        target_compile_definitions(papacc_mode_a_tls12_target PRIVATE
            PAPACC_TEST_PKI_DIR="${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/security")
        target_link_libraries(papacc_mode_a_tls12_target PRIVATE
            papacc_security_test_support papacc_security_composition
            papacc_pst_consumer ws2_32)
        set_target_properties(papacc_mode_a_tls12_target PROPERTIES
            C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        papacc_enable_warnings(papacc_mode_a_tls12_target)
        papacc_stage_pst_runtime(papacc_mode_a_tls12_target)

        add_executable(papacc_mode_a_tls12_target_integration_test
            tests/mode_a_tls12_target_integration_test.c)
        target_include_directories(
            papacc_mode_a_tls12_target_integration_test PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/security/platform/win32)
        target_compile_definitions(
            papacc_mode_a_tls12_target_integration_test PRIVATE
            PAPACC_TEST_PKI_DIR="${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/security")
        target_link_libraries(
            papacc_mode_a_tls12_target_integration_test PRIVATE
            papacc_security_test_support papacc_security_composition
            papacc_pst_consumer ws2_32)
        set_target_properties(
            papacc_mode_a_tls12_target_integration_test PROPERTIES
            C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        papacc_enable_warnings(papacc_mode_a_tls12_target_integration_test)
        papacc_stage_pst_runtime(papacc_mode_a_tls12_target_integration_test)
        add_dependencies(papacc_mode_a_tls12_target_integration_test
            papacc_mode_a_tls12_target)
        add_test(NAME papacc.mode_a_tls12_target.integration
            COMMAND papacc_mode_a_tls12_target_integration_test
                $<TARGET_FILE:papacc_mode_a_tls12_target>)

        add_executable(papacc_secure_fixture_launcher_test
            tests/secure_fixture_launcher_win32_test.c)
        set_target_properties(papacc_secure_fixture_launcher_test PROPERTIES
            C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        papacc_enable_warnings(papacc_secure_fixture_launcher_test)
        add_test(NAME papacc_secure_fixture_launcher_test
            COMMAND papacc_secure_fixture_launcher_test
                $<TARGET_FILE:papacc_secure_browser_fixture>
                $<TARGET_FILE:papacc_mode_b_fixture_integration_test>
                $<TARGET_FILE:papacc_secure_reference_client>)

        add_executable(papacc_mode_b_fixture_integration_test
            tests/mode_b_fixture_integration_win32_test.c)
        set_target_properties(papacc_mode_b_fixture_integration_test PROPERTIES
            C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        papacc_enable_warnings(papacc_mode_b_fixture_integration_test)
        add_dependencies(papacc_mode_b_fixture_integration_test
            papacc_secure_browser_fixture papacc_secure_reference_client)
        add_dependencies(papacc_secure_fixture_launcher_test
            papacc_mode_b_fixture_integration_test)
        add_test(NAME papacc_mode_b_fixture_integration_test
            COMMAND papacc_mode_b_fixture_integration_test
                $<TARGET_FILE:papacc_secure_browser_fixture>
                $<TARGET_FILE:papacc_secure_reference_client>)
        add_test(NAME papacc_terminal_latency_trace_test
            COMMAND papacc_mode_b_fixture_integration_test
                $<TARGET_FILE:papacc_secure_browser_fixture>
                $<TARGET_FILE:papacc_secure_reference_client>
                --terminal-latency-trace)
        add_test(NAME papacc_tcp_plain_fixture_integration_test
            COMMAND papacc_mode_b_fixture_integration_test
                $<TARGET_FILE:papacc_secure_browser_fixture>
                $<TARGET_FILE:papacc_secure_reference_client>
                --tcp-plain-only)
        add_test(NAME papacc_mixed_egress_fixture_integration_test
            COMMAND papacc_mode_b_fixture_integration_test
                $<TARGET_FILE:papacc_secure_browser_fixture>
                $<TARGET_FILE:papacc_secure_reference_client>
                --mixed-only)
        add_test(NAME papacc.browser_fixture.tls_clean_close
            COMMAND papacc_mode_b_fixture_integration_test
                $<TARGET_FILE:papacc_secure_browser_fixture>
                $<TARGET_FILE:papacc_secure_reference_client>
                --tls-clean-close-only)
        add_test(NAME papacc.browser_fixture.tls_terminal_matrix
            COMMAND papacc_mode_b_fixture_integration_test
                $<TARGET_FILE:papacc_secure_browser_fixture>
                $<TARGET_FILE:papacc_secure_reference_client>
                --tls-terminal-matrix)
        add_test(NAME papacc.browser_fixture.secure_v2_to_v1_fallback
            COMMAND papacc_mode_b_fixture_integration_test
                $<TARGET_FILE:papacc_secure_browser_fixture>
                $<TARGET_FILE:papacc_secure_reference_client>
                --fallback-matrix)
        add_test(NAME papacc.browser_fixture.fallback_negative_matrix
            COMMAND papacc_mode_b_fixture_integration_test
                $<TARGET_FILE:papacc_secure_browser_fixture>
                $<TARGET_FILE:papacc_secure_reference_client>
                --negative-fallback-matrix)
        add_test(NAME papacc.browser_fixture.protocol_error_repetition
            COMMAND papacc_mode_b_fixture_integration_test
                $<TARGET_FILE:papacc_secure_browser_fixture>
                $<TARGET_FILE:papacc_secure_reference_client>
                --protocol-error-repetition)

        add_executable(papacc_production_secure_listener_test
            tests/production_secure_listener_win32_test.c)
        target_link_libraries(papacc_production_secure_listener_test PRIVATE
            ws2_32)
        set_target_properties(papacc_production_secure_listener_test PROPERTIES
            C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        papacc_enable_warnings(papacc_production_secure_listener_test)
        add_dependencies(papacc_production_secure_listener_test
            papacc_server papacc_secure_reference_client
            papacc_lab_egress_targets)
        add_test(NAME papacc.production.secure_listener
            COMMAND papacc_production_secure_listener_test
                $<TARGET_FILE:papacc_server>
                $<TARGET_FILE:papacc_secure_reference_client>
                ${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/security
                $<TARGET_FILE:papacc_lab_egress_targets>)

        add_executable(papacc_legacy_browser_fixture
            tests/legacy_fixture_server_win32.c)
        target_include_directories(papacc_legacy_browser_fixture PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/apps/server
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network_egress
            ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/win32
            ${CMAKE_CURRENT_SOURCE_DIR}/src/protocol
            ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime
            ${CMAKE_CURRENT_SOURCE_DIR}/src/security
            ${CMAKE_CURRENT_SOURCE_DIR}/src/transports
            ${CMAKE_CURRENT_SOURCE_DIR}/src/transports/tcp)
        target_link_libraries(papacc_legacy_browser_fixture PRIVATE
            papacc_server_io_loop_win32 ws2_32)
        set_target_properties(papacc_legacy_browser_fixture PROPERTIES
            C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        papacc_enable_warnings(papacc_legacy_browser_fixture)

        add_executable(papacc_legacy_fixture_integration_test
            tests/legacy_fixture_integration_win32_test.c)
        target_link_libraries(papacc_legacy_fixture_integration_test PRIVATE
            papacc_protocol ws2_32)
        set_target_properties(papacc_legacy_fixture_integration_test PROPERTIES
            C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        papacc_enable_warnings(papacc_legacy_fixture_integration_test)
        add_test(NAME papacc_legacy_fixture_integration_test
            COMMAND papacc_legacy_fixture_integration_test
                $<TARGET_FILE:papacc_legacy_browser_fixture>)
        add_dependencies(papacc_server_secure_io_win32_integration_test
            papacc_secure_reference_client)
        add_test(NAME papacc_server_secure_io_win32_integration_test
            COMMAND papacc_server_secure_io_win32_integration_test)
        add_test(NAME papacc_secure_reference_client_interop
            COMMAND papacc_server_secure_io_win32_integration_test)
        add_test(NAME papacc.phase6.reference.no_negotiation
            COMMAND papacc_server_secure_io_win32_integration_test
                --isolated-negative-no-negotiation)
        add_test(NAME papacc.phase6.reference.missing_tls_offload
            COMMAND papacc_server_secure_io_win32_integration_test
                --isolated-negative-missing-tls)
        add_test(NAME papacc.phase6.reference.destination_denied
            COMMAND papacc_server_secure_io_win32_integration_test
                --isolated-negative-destination)
        add_test(NAME papacc.phase6.reference.tls_hostname_negative
            COMMAND papacc_server_secure_io_win32_integration_test
                --isolated-negative-tls-name)
        add_test(NAME papacc.phase6.reference.tls_required_tcp_reset
            COMMAND papacc_server_secure_io_win32_integration_test
                --isolated-tls-reset)
        add_test(NAME papacc.phase6.reference.resource_oracle_selftest
            COMMAND papacc_server_secure_io_win32_integration_test
                --resource-oracle-selftest)
        add_test(NAME papacc.phase6.reference.tcp_abort
            COMMAND papacc_server_secure_io_win32_integration_test
                --isolated-tcp-abort)
        add_test(NAME papacc.phase6.reference.tls_half_close
            COMMAND papacc_server_secure_io_win32_integration_test
                --isolated-tls-half-close)
        add_test(NAME papacc.phase6.reference.tls_truncation_matrix
            COMMAND papacc_server_secure_io_win32_integration_test
                --isolated-tls-truncated-matrix)
        add_test(NAME papacc.phase6.reference.multiple_data_recovery
            COMMAND papacc_server_secure_io_win32_integration_test
                --isolated-multiple-recovery)

        add_executable(papacc_pst_tls13_outbound_proof
            tests/pst_tls13_outbound_proof.c)
        target_link_libraries(papacc_pst_tls13_outbound_proof PRIVATE
            papacc_pst_consumer)
        set_target_properties(papacc_pst_tls13_outbound_proof PROPERTIES
            C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        papacc_enable_warnings(papacc_pst_tls13_outbound_proof)
        papacc_stage_pst_runtime(papacc_pst_tls13_outbound_proof)
        add_custom_target(papacc_pst_tls13_google
            COMMAND $<TARGET_FILE:papacc_pst_tls13_outbound_proof>
            DEPENDS papacc_pst_tls13_outbound_proof
            COMMENT "Run opt-in PST TLS 1.3 proof against google.com:443"
            VERBATIM)
        add_custom_target(papacc_pst_tls13_google_wrong_hostname
            COMMAND $<TARGET_FILE:papacc_pst_tls13_outbound_proof>
                --wrong-hostname
            DEPENDS papacc_pst_tls13_outbound_proof
            COMMENT "Run opt-in PST wrong-hostname negative proof"
            VERBATIM)
    endif()
endif()

if(BUILD_TESTING)
    add_executable(papacc_data_association_protocol_test
        tests/data_association_protocol_test.c)
    target_link_libraries(papacc_data_association_protocol_test PRIVATE
        papacc_data_association_protocol)
    set_target_properties(papacc_data_association_protocol_test PROPERTIES
        C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
    papacc_enable_warnings(papacc_data_association_protocol_test)
    add_test(NAME papacc.protocol.data_association
        COMMAND papacc_data_association_protocol_test)

    add_executable(papacc_data_association_test
        tests/data_association_test.c)
    target_include_directories(papacc_data_association_test PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime
        ${CMAKE_CURRENT_SOURCE_DIR}/src/network
        ${CMAKE_CURRENT_SOURCE_DIR}/src/transports)
    target_link_libraries(papacc_data_association_test PRIVATE
        papacc_data_association papacc_connection)
    set_target_properties(papacc_data_association_test PROPERTIES
        C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
    papacc_enable_warnings(papacc_data_association_test)
    add_test(NAME papacc.runtime.data_association
        COMMAND papacc_data_association_test)

    add_executable(papacc_data_association_transaction_test
        tests/data_association_transaction_test.c)
    target_include_directories(papacc_data_association_transaction_test PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime
        ${CMAKE_CURRENT_SOURCE_DIR}/src/network
        ${CMAKE_CURRENT_SOURCE_DIR}/src/transports)
    target_link_libraries(papacc_data_association_transaction_test PRIVATE
        papacc_data_association papacc_connection)
    set_target_properties(papacc_data_association_transaction_test PROPERTIES
        C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
    papacc_enable_warnings(papacc_data_association_transaction_test)
    add_test(NAME papacc.runtime.data_association_transaction
        COMMAND papacc_data_association_transaction_test)

    add_executable(papacc_post_control_processor_test
        tests/post_control_processor_test.c)
    target_include_directories(papacc_post_control_processor_test PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime
        ${CMAKE_CURRENT_SOURCE_DIR}/src/network
        ${CMAKE_CURRENT_SOURCE_DIR}/src/transports)
    target_link_libraries(papacc_post_control_processor_test PRIVATE
        papacc_post_control_processor)
    set_target_properties(papacc_post_control_processor_test PROPERTIES
        C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
    papacc_enable_warnings(papacc_post_control_processor_test)
    add_test(NAME papacc.protocol.post_control
        COMMAND papacc_post_control_processor_test)

    add_executable(papacc_connection_classifier_test
        tests/connection_classifier_test.c)
    target_include_directories(papacc_connection_classifier_test PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime
        ${CMAKE_CURRENT_SOURCE_DIR}/src/network
        ${CMAKE_CURRENT_SOURCE_DIR}/src/transports)
    target_link_libraries(papacc_connection_classifier_test PRIVATE
        papacc_connection_classifier papacc_control_processor
        papacc_post_control_processor)
    set_target_properties(papacc_connection_classifier_test PROPERTIES
        C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
    papacc_enable_warnings(papacc_connection_classifier_test)
    add_test(NAME papacc.protocol.connection_classifier
        COMMAND papacc_connection_classifier_test)

    add_executable(papacc_data_attach_processor_test
        tests/data_attach_processor_test.c)
    target_include_directories(papacc_data_attach_processor_test PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime
        ${CMAKE_CURRENT_SOURCE_DIR}/src/network
        ${CMAKE_CURRENT_SOURCE_DIR}/src/transports)
    target_link_libraries(papacc_data_attach_processor_test PRIVATE
        papacc_data_attach_processor papacc_connection_classifier)
    set_target_properties(papacc_data_attach_processor_test PROPERTIES
        C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
    papacc_enable_warnings(papacc_data_attach_processor_test)
    add_test(NAME papacc.protocol.data_attach
        COMMAND papacc_data_attach_processor_test)

    add_executable(papacc_control_protocol_test tests/control_protocol_test.c)
    target_link_libraries(papacc_control_protocol_test PRIVATE
        papacc_control_protocol)
    set_target_properties(papacc_control_protocol_test PROPERTIES
        C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
    papacc_enable_warnings(papacc_control_protocol_test)
    add_test(NAME papacc.protocol.control_codec
        COMMAND papacc_control_protocol_test)

    add_executable(papacc_control_processor_test tests/control_processor_test.c)
    target_include_directories(papacc_control_processor_test PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/network
        ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime
        ${CMAKE_CURRENT_SOURCE_DIR}/src/transports)
    target_link_libraries(papacc_control_processor_test PRIVATE
        papacc_control_processor)
    set_target_properties(papacc_control_processor_test PROPERTIES
        C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
    papacc_enable_warnings(papacc_control_processor_test)
    add_test(NAME papacc.protocol.control_processor
        COMMAND papacc_control_processor_test)

    add_executable(papacc_framed_writer_test
        tests/framed_writer_test.c
    )

    target_include_directories(papacc_framed_writer_test
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/transports
    )

    target_link_libraries(
        papacc_framed_writer_test PRIVATE papacc_framed_writer)

    set_target_properties(papacc_framed_writer_test PROPERTIES
        C_STANDARD 99
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO
    )

    papacc_enable_warnings(papacc_framed_writer_test)

    add_test(
        NAME papacc.protocol.framed_writer
        COMMAND papacc_framed_writer_test)

    add_executable(papacc_framed_reader_test
        tests/framed_reader_test.c
    )

    target_include_directories(papacc_framed_reader_test
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/transports
    )

    target_link_libraries(
        papacc_framed_reader_test PRIVATE papacc_framed_reader)

    set_target_properties(papacc_framed_reader_test PROPERTIES
        C_STANDARD 99
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO
    )

    papacc_enable_warnings(papacc_framed_reader_test)

    add_test(
        NAME papacc.protocol.framed_reader
        COMMAND papacc_framed_reader_test)

    add_executable(papacc_transport_connection_test
        tests/transport_connection_test.c
    )

    target_include_directories(papacc_transport_connection_test
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/transports
    )

    target_link_libraries(
        papacc_transport_connection_test PRIVATE papacc_connection)

    set_target_properties(papacc_transport_connection_test PROPERTIES
        C_STANDARD 99
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO
    )

    papacc_enable_warnings(papacc_transport_connection_test)

    add_test(
        NAME papacc.transport.connection
        COMMAND papacc_transport_connection_test)

    add_executable(papacc_frame_test
        tests/frame_test.c
    )

    target_link_libraries(
        papacc_frame_test PRIVATE papacc_protocol_framing)

    set_target_properties(papacc_frame_test PROPERTIES
        C_STANDARD 99
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO
    )

    papacc_enable_warnings(papacc_frame_test)

    add_test(NAME papacc.protocol.frame COMMAND papacc_frame_test)

    add_executable(papacc_frame_parser_test
        tests/frame_parser_test.c
    )

    target_link_libraries(
        papacc_frame_parser_test PRIVATE papacc_protocol_framing)

    set_target_properties(papacc_frame_parser_test PROPERTIES
        C_STANDARD 99
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO
    )

    papacc_enable_warnings(papacc_frame_parser_test)

    add_test(NAME papacc.protocol.frame_parser COMMAND papacc_frame_parser_test)

    add_executable(papacc_channel_test
        tests/channel_test.c
    )

    target_include_directories(papacc_channel_test
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network
            ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime
            ${CMAKE_CURRENT_SOURCE_DIR}/src/transports
    )

    target_link_libraries(papacc_channel_test PRIVATE papacc_channel)

    set_target_properties(papacc_channel_test PROPERTIES
        C_STANDARD 99
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO
    )

    papacc_enable_warnings(papacc_channel_test)

    add_test(NAME papacc.channel.relationship COMMAND papacc_channel_test)

    add_executable(papacc_session_test
        tests/session_test.c
    )

    target_include_directories(papacc_session_test
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime
    )

    target_link_libraries(papacc_session_test PRIVATE papacc_session)

    set_target_properties(papacc_session_test PROPERTIES
        C_STANDARD 99
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO
    )

    papacc_enable_warnings(papacc_session_test)

    add_test(NAME papacc.session.lifecycle COMMAND papacc_session_test)

    add_executable(papacc_connection_test
        tests/connection_test.c
    )

    target_include_directories(papacc_connection_test
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network
            ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime
            ${CMAKE_CURRENT_SOURCE_DIR}/src/transports
    )

    target_link_libraries(papacc_connection_test PRIVATE papacc_connection)

    set_target_properties(papacc_connection_test PROPERTIES
        C_STANDARD 99
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO
    )

    papacc_enable_warnings(papacc_connection_test)

    add_test(NAME papacc.connection.lifecycle COMMAND papacc_connection_test)

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

    add_executable(papacc_types_test
        tests/types_test.c
    )

    target_link_libraries(papacc_types_test PRIVATE papacc_core)

    set_target_properties(papacc_types_test PROPERTIES
        C_STANDARD 99
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO
    )

    papacc_enable_warnings(papacc_types_test)

    add_test(NAME papacc.types COMMAND papacc_types_test)

    add_executable(papacc_server_config_test
        tests/server_config_test.c
    )

    target_include_directories(papacc_server_config_test
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/apps/server
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network
    )

    target_link_libraries(
        papacc_server_config_test
        PRIVATE papacc_server_config
    )

    set_target_properties(papacc_server_config_test PROPERTIES
        C_STANDARD 99
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO
    )

    papacc_enable_warnings(papacc_server_config_test)

    add_test(NAME papacc.server.configuration COMMAND papacc_server_config_test)

    add_executable(papacc_server_cli_test
        tests/server_cli_test.c
    )

    target_include_directories(papacc_server_cli_test
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/apps/server
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network
    )

    target_link_libraries(papacc_server_cli_test PRIVATE papacc_server_cli)

    set_target_properties(papacc_server_cli_test PROPERTIES
        C_STANDARD 99
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO
    )

    papacc_enable_warnings(papacc_server_cli_test)

    add_test(NAME papacc.server.cli COMMAND papacc_server_cli_test)

    if(WIN32)
        add_executable(papacc_server_cli_interfaces_test
            tests/server_cli_interfaces_test.c
        )

        target_include_directories(papacc_server_cli_interfaces_test
            PRIVATE
                ${CMAKE_CURRENT_SOURCE_DIR}/apps/server
                ${CMAKE_CURRENT_SOURCE_DIR}/src/network
        )

        target_link_libraries(
            papacc_server_cli_interfaces_test
            PRIVATE papacc_server_cli_interfaces)

        set_target_properties(papacc_server_cli_interfaces_test PROPERTIES
            C_STANDARD 99
            C_STANDARD_REQUIRED YES
            C_EXTENSIONS NO
        )

        papacc_enable_warnings(papacc_server_cli_interfaces_test)

        add_test(
            NAME papacc.server.cli_interfaces
            COMMAND papacc_server_cli_interfaces_test)

        add_executable(papacc_server_network_test
            tests/server_network_test.c
        )

        target_include_directories(papacc_server_network_test
            PRIVATE
                ${CMAKE_CURRENT_SOURCE_DIR}/apps/server
                ${CMAKE_CURRENT_SOURCE_DIR}/src/network
                ${CMAKE_CURRENT_SOURCE_DIR}/src/transports/tcp
                ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/win32
        )

        target_link_libraries(
            papacc_server_network_test PRIVATE papacc_server_network)

        set_target_properties(papacc_server_network_test PROPERTIES
            C_STANDARD 99
            C_STANDARD_REQUIRED YES
            C_EXTENSIONS NO
        )

        papacc_enable_warnings(papacc_server_network_test)

        add_test(
            NAME papacc.server.network
            COMMAND papacc_server_network_test)

        add_executable(papacc_server_acceptor_win32_test
            tests/server_acceptor_win32_test.c
        )

        target_include_directories(papacc_server_acceptor_win32_test
            PRIVATE
                ${CMAKE_CURRENT_SOURCE_DIR}/apps/server
                ${CMAKE_CURRENT_SOURCE_DIR}/src/network
                ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/win32
                ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime
                ${CMAKE_CURRENT_SOURCE_DIR}/src/transports
                ${CMAKE_CURRENT_SOURCE_DIR}/src/transports/tcp
        )

        target_link_libraries(
            papacc_server_acceptor_win32_test
            PRIVATE papacc_server_acceptor_win32
        )

        set_target_properties(papacc_server_acceptor_win32_test PROPERTIES
            C_STANDARD 99
            C_STANDARD_REQUIRED YES
            C_EXTENSIONS NO
        )

        papacc_enable_warnings(papacc_server_acceptor_win32_test)

        add_test(
            NAME papacc.server.acceptor.win32
            COMMAND papacc_server_acceptor_win32_test
        )

        add_executable(papacc_server_io_loop_win32_test
            tests/server_io_loop_win32_test.c)
        target_include_directories(papacc_server_io_loop_win32_test PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/apps/server
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network
            ${CMAKE_CURRENT_SOURCE_DIR}/src/platform
            ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/win32
            ${CMAKE_CURRENT_SOURCE_DIR}/src/protocol
            ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime
            ${CMAKE_CURRENT_SOURCE_DIR}/src/transports
            ${CMAKE_CURRENT_SOURCE_DIR}/src/transports/tcp)
        target_link_libraries(papacc_server_io_loop_win32_test PRIVATE
            papacc_server_io_loop_win32)
        set_target_properties(papacc_server_io_loop_win32_test PROPERTIES
            C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        papacc_enable_warnings(papacc_server_io_loop_win32_test)
        add_test(NAME papacc.server.io_loop.win32
            COMMAND papacc_server_io_loop_win32_test)

        if(PAPACC_ENABLE_TEST_OWNER_TERMINAL_HOOKS)
            add_executable(papacc_owner_terminal_hook_test
                tests/server_owner_terminal_hook_test.c)
            target_include_directories(papacc_owner_terminal_hook_test PRIVATE
                ${CMAKE_CURRENT_SOURCE_DIR}/apps/server
                ${CMAKE_CURRENT_SOURCE_DIR}/src/network
                ${CMAKE_CURRENT_SOURCE_DIR}/src/network_egress
                ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/win32
                ${CMAKE_CURRENT_SOURCE_DIR}/src/protocol
                ${CMAKE_CURRENT_SOURCE_DIR}/src/transports/tcp)
            target_link_libraries(papacc_owner_terminal_hook_test PRIVATE
                papacc_server_io_loop_win32)
            set_target_properties(papacc_owner_terminal_hook_test PROPERTIES
                C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
            papacc_enable_warnings(papacc_owner_terminal_hook_test)
            add_test(NAME papacc.server.owner_terminal_hook
                COMMAND papacc_owner_terminal_hook_test)
        endif()

        add_executable(papacc_server_egress_source_win32_test
            tests/server_egress_source_win32_test.c
            apps/server/server_outbound_tls_source_win32.c)
        target_include_directories(papacc_server_egress_source_win32_test PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/apps/server
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network_egress
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network)
        target_link_libraries(papacc_server_egress_source_win32_test PRIVATE
            papacc_server_run_win32 crypt32)
        set_target_properties(papacc_server_egress_source_win32_test PROPERTIES
            C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        papacc_enable_warnings(papacc_server_egress_source_win32_test)
        add_test(NAME papacc.server.egress_source.win32
            COMMAND papacc_server_egress_source_win32_test
                ${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/security)

        add_executable(papacc_server_data_ticket_generator_test
            tests/server_data_ticket_generator_test.c)
        target_include_directories(papacc_server_data_ticket_generator_test PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/apps/server
            ${CMAKE_CURRENT_SOURCE_DIR}/src/protocol
            ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network
            ${CMAKE_CURRENT_SOURCE_DIR}/src/transports)
        target_link_libraries(papacc_server_data_ticket_generator_test PRIVATE
            papacc_server_io_loop_win32)
        set_target_properties(papacc_server_data_ticket_generator_test PROPERTIES
            C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        papacc_enable_warnings(papacc_server_data_ticket_generator_test)
        add_test(NAME papacc.server.data_ticket_generator
            COMMAND papacc_server_data_ticket_generator_test)

        add_executable(papacc_server_run_win32_test
            tests/server_run_win32_test.c
        )

        target_include_directories(papacc_server_run_win32_test
            PRIVATE
                ${CMAKE_CURRENT_SOURCE_DIR}/apps/server
                ${CMAKE_CURRENT_SOURCE_DIR}/src/network
                ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/win32
                ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime
                ${CMAKE_CURRENT_SOURCE_DIR}/src/transports
                ${CMAKE_CURRENT_SOURCE_DIR}/src/transports/tcp
        )

        target_link_libraries(
            papacc_server_run_win32_test
            PRIVATE papacc_server_run_win32
        )

        set_target_properties(papacc_server_run_win32_test PROPERTIES
            C_STANDARD 99
            C_STANDARD_REQUIRED YES
            C_EXTENSIONS NO
        )

        papacc_enable_warnings(papacc_server_run_win32_test)

        add_test(
            NAME papacc.server.run.win32
            COMMAND papacc_server_run_win32_test
        )

        add_executable(papacc_server_console_win32_test
            tests/server_console_win32_test.c
        )

        target_include_directories(papacc_server_console_win32_test
            PRIVATE
                ${CMAKE_CURRENT_SOURCE_DIR}/apps/server
        )

        target_link_libraries(
            papacc_server_console_win32_test
            PRIVATE papacc_server_console_win32)

        set_target_properties(papacc_server_console_win32_test PROPERTIES
            C_STANDARD 99
            C_STANDARD_REQUIRED YES
            C_EXTENSIONS NO
        )

        papacc_enable_warnings(papacc_server_console_win32_test)

        add_test(
            NAME papacc.server.console_win32
            COMMAND papacc_server_console_win32_test)
    endif()

    add_executable(papacc_ip_address_test
        tests/ip_address_test.c
    )

    target_include_directories(papacc_ip_address_test
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network
    )

    target_link_libraries(papacc_ip_address_test PRIVATE papacc_network)

    set_target_properties(papacc_ip_address_test PROPERTIES
        C_STANDARD 99
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO
    )

    papacc_enable_warnings(papacc_ip_address_test)

    add_test(NAME papacc.network.ip_address COMMAND papacc_ip_address_test)

    add_executable(papacc_network_endpoint_test
        tests/network_endpoint_test.c
    )

    target_include_directories(papacc_network_endpoint_test
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network
    )

    target_link_libraries(papacc_network_endpoint_test PRIVATE papacc_network)

    set_target_properties(papacc_network_endpoint_test PROPERTIES
        C_STANDARD 99
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO
    )

    papacc_enable_warnings(papacc_network_endpoint_test)

    add_test(
        NAME papacc.network.endpoint
        COMMAND papacc_network_endpoint_test)

    add_executable(papacc_bind_selection_test
        tests/bind_selection_test.c
    )

    target_include_directories(papacc_bind_selection_test
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network
    )

    target_link_libraries(papacc_bind_selection_test PRIVATE papacc_network)

    set_target_properties(papacc_bind_selection_test PROPERTIES
        C_STANDARD 99
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO
    )

    papacc_enable_warnings(papacc_bind_selection_test)

    add_test(NAME papacc.network.bind_selection COMMAND papacc_bind_selection_test)

    add_executable(papacc_persistent_bind_selection_test
        tests/persistent_bind_selection_test.c
    )

    target_include_directories(papacc_persistent_bind_selection_test
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network
    )

    target_link_libraries(
        papacc_persistent_bind_selection_test PRIVATE papacc_network)

    set_target_properties(papacc_persistent_bind_selection_test PROPERTIES
        C_STANDARD 99
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO
    )

    papacc_enable_warnings(papacc_persistent_bind_selection_test)

    add_test(
        NAME papacc.network.persistent_bind_selection
        COMMAND papacc_persistent_bind_selection_test)

    add_executable(papacc_bind_target_test
        tests/bind_target_test.c
    )

    target_include_directories(papacc_bind_target_test
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network
    )

    target_link_libraries(papacc_bind_target_test PRIVATE papacc_network)

    set_target_properties(papacc_bind_target_test PROPERTIES
        C_STANDARD 99
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO
    )

    papacc_enable_warnings(papacc_bind_target_test)

    add_test(NAME papacc.network.bind_target COMMAND papacc_bind_target_test)

    add_executable(papacc_network_interface_test
        tests/network_interface_test.c
    )

    target_include_directories(papacc_network_interface_test
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/src/network
    )

    target_link_libraries(papacc_network_interface_test PRIVATE papacc_network)

    set_target_properties(papacc_network_interface_test PROPERTIES
        C_STANDARD 99
        C_STANDARD_REQUIRED YES
        C_EXTENSIONS NO
    )

    papacc_enable_warnings(papacc_network_interface_test)

    add_test(NAME papacc.network.interface_model COMMAND papacc_network_interface_test)

    if(WIN32)
        add_executable(papacc_tcp_platform_win32_test
            tests/tcp_platform_win32_test.c
        )

        target_include_directories(papacc_tcp_platform_win32_test
            PRIVATE
                ${CMAKE_CURRENT_SOURCE_DIR}/src/transports/tcp
        )

        target_link_libraries(
            papacc_tcp_platform_win32_test
            PRIVATE papacc_tcp_win32
        )

        set_target_properties(papacc_tcp_platform_win32_test PROPERTIES
            C_STANDARD 99
            C_STANDARD_REQUIRED YES
            C_EXTENSIONS NO
        )

        papacc_enable_warnings(papacc_tcp_platform_win32_test)

        add_test(
            NAME papacc.transport.tcp_platform.win32
            COMMAND papacc_tcp_platform_win32_test
        )

        add_executable(papacc_tcp_socket_win32_test
            tests/tcp_socket_win32_test.c
        )

        target_include_directories(papacc_tcp_socket_win32_test
            PRIVATE
                ${CMAKE_CURRENT_SOURCE_DIR}/src/network
                ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/win32
                ${CMAKE_CURRENT_SOURCE_DIR}/src/transports/tcp
        )

        target_link_libraries(
            papacc_tcp_socket_win32_test
            PRIVATE papacc_tcp_win32 papacc_network
        )

        set_target_properties(papacc_tcp_socket_win32_test PROPERTIES
            C_STANDARD 99
            C_STANDARD_REQUIRED YES
            C_EXTENSIONS NO
        )

        papacc_enable_warnings(papacc_tcp_socket_win32_test)

        add_test(
            NAME papacc.transport.tcp_socket_bind.win32
            COMMAND papacc_tcp_socket_win32_test
        )

        add_executable(papacc_tcp_accept_win32_test
            tests/tcp_accept_win32_test.c
        )

        target_include_directories(papacc_tcp_accept_win32_test
            PRIVATE
                ${CMAKE_CURRENT_SOURCE_DIR}/src/network
                ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/win32
                ${CMAKE_CURRENT_SOURCE_DIR}/src/transports/tcp
        )

        target_link_libraries(
            papacc_tcp_accept_win32_test
            PRIVATE papacc_tcp_win32 papacc_network
        )

        set_target_properties(papacc_tcp_accept_win32_test PROPERTIES
            C_STANDARD 99
            C_STANDARD_REQUIRED YES
            C_EXTENSIONS NO
        )

        papacc_enable_warnings(papacc_tcp_accept_win32_test)

        add_test(
            NAME papacc.transport.tcp_accept.win32
            COMMAND papacc_tcp_accept_win32_test
        )

        add_executable(papacc_tcp_connection_transport_win32_test
            tests/tcp_connection_transport_win32_test.c
        )

        target_include_directories(papacc_tcp_connection_transport_win32_test
            PRIVATE
                ${CMAKE_CURRENT_SOURCE_DIR}/src/network
                ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/win32
                ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime
                ${CMAKE_CURRENT_SOURCE_DIR}/src/transports
                ${CMAKE_CURRENT_SOURCE_DIR}/src/transports/tcp
        )

        target_link_libraries(
            papacc_tcp_connection_transport_win32_test
            PRIVATE
                papacc_tcp_connection_win32
                papacc_framed_reader
                papacc_framed_writer
        )

        set_target_properties(
            papacc_tcp_connection_transport_win32_test PROPERTIES
            C_STANDARD 99
            C_STANDARD_REQUIRED YES
            C_EXTENSIONS NO
        )

        papacc_enable_warnings(papacc_tcp_connection_transport_win32_test)

        add_test(
            NAME papacc.connection.transport.win32
            COMMAND papacc_tcp_connection_transport_win32_test
        )

        add_executable(papacc_tcp_listener_set_win32_test
            tests/tcp_listener_set_win32_test.c
        )

        target_include_directories(papacc_tcp_listener_set_win32_test
            PRIVATE
                ${CMAKE_CURRENT_SOURCE_DIR}/src/network
                ${CMAKE_CURRENT_SOURCE_DIR}/src/platform/win32
                ${CMAKE_CURRENT_SOURCE_DIR}/src/transports/tcp
        )

        target_link_libraries(
            papacc_tcp_listener_set_win32_test
            PRIVATE papacc_tcp_win32 papacc_network
        )

        set_target_properties(
            papacc_tcp_listener_set_win32_test PROPERTIES
            C_STANDARD 99
            C_STANDARD_REQUIRED YES
            C_EXTENSIONS NO
        )

        papacc_enable_warnings(papacc_tcp_listener_set_win32_test)

        add_test(
            NAME papacc.transport.tcp_listener_set.win32
            COMMAND papacc_tcp_listener_set_win32_test
        )

        add_executable(papacc_network_discovery_win32_test
            tests/network_discovery_win32_test.c
        )

        target_include_directories(papacc_network_discovery_win32_test
            PRIVATE
                ${CMAKE_CURRENT_SOURCE_DIR}/src/network
        )

        target_link_libraries(
            papacc_network_discovery_win32_test
            PRIVATE papacc_network_win32
        )

        set_target_properties(papacc_network_discovery_win32_test PROPERTIES
            C_STANDARD 99
            C_STANDARD_REQUIRED YES
            C_EXTENSIONS NO
        )

        papacc_enable_warnings(papacc_network_discovery_win32_test)

        add_test(
            NAME papacc.network.discovery.win32
            COMMAND papacc_network_discovery_win32_test
        )

        add_executable(papacc_pal_time_test
            tests/pal_time_test.c
        )

        target_include_directories(papacc_pal_time_test
            PRIVATE
                ${CMAKE_CURRENT_SOURCE_DIR}/src/platform
        )

        target_link_libraries(papacc_pal_time_test PRIVATE papacc_pal)

        set_target_properties(papacc_pal_time_test PROPERTIES
            C_STANDARD 99
            C_STANDARD_REQUIRED YES
            C_EXTENSIONS NO
        )

        papacc_enable_warnings(papacc_pal_time_test)

        add_test(NAME papacc.pal.monotonic_time COMMAND papacc_pal_time_test)

        add_executable(papacc_log_test
            tests/log_test.c
        )

        target_include_directories(papacc_log_test
            PRIVATE
                ${CMAKE_CURRENT_SOURCE_DIR}/src/platform
                ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime
        )

        target_link_libraries(papacc_log_test PRIVATE papacc_runtime)

        set_target_properties(papacc_log_test PROPERTIES
            C_STANDARD 99
            C_STANDARD_REQUIRED YES
            C_EXTENSIONS NO
        )

        papacc_enable_warnings(papacc_log_test)

        add_test(NAME papacc.runtime.logging COMMAND papacc_log_test)

        add_executable(papacc_runtime_test
            tests/runtime_test.c
        )

        target_include_directories(papacc_runtime_test
            PRIVATE
                ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime
        )

        target_link_libraries(papacc_runtime_test PRIVATE papacc_runtime)

        set_target_properties(papacc_runtime_test PROPERTIES
            C_STANDARD 99
            C_STANDARD_REQUIRED YES
            C_EXTENSIONS NO
        )

        papacc_enable_warnings(papacc_runtime_test)

        add_test(NAME papacc.runtime.lifecycle COMMAND papacc_runtime_test)
    endif()
endif()

# Pure SDK ABI probe: never creates PST runtime, Connection, wait-set or socket.
if(PAPACC_ENABLE_CANONICAL_API3_CANDIDATE)
    if(NOT BUILD_TESTING OR NOT MSVC OR NOT WIN32 OR
       NOT PAPACC_ENABLE_PST_OUTBOUND_PROOF OR NOT TARGET papacc_pst_consumer)
        message(FATAL_ERROR "API3 candidate requires isolated MSVC/Win32 testing and PST proof composition")
    endif()
    add_executable(papacc_pst_api3_offline_contract
        tests/pst_api3_offline_contract_test.c)
    target_link_libraries(papacc_pst_api3_offline_contract PRIVATE papacc_pst_consumer)
    target_compile_options(papacc_pst_api3_offline_contract PRIVATE /W4 /WX)
    set_target_properties(papacc_pst_api3_offline_contract PROPERTIES
        C_STANDARD 90 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
    papacc_stage_pst_runtime(papacc_pst_api3_offline_contract)
    add_test(NAME papacc.pst.api3_offline_contract
        COMMAND papacc_pst_api3_offline_contract)
endif()
# Private, dormant configuration participant only; no application dispatch link.
option(PAPACC_ENABLE_DORMANT_C2_PARTICIPANT "Build closed C2 configuration participant" OFF)
if(WIN32 AND PAPACC_ENABLE_DORMANT_C2_PARTICIPANT)
    add_library(papacc_c2_dormant STATIC
        src/protocol/c2_operation_dormant_win32.c
        apps/server/server_c2_participant_win32.c)
    target_include_directories(papacc_c2_dormant PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/server
        ${CMAKE_CURRENT_SOURCE_DIR}/src/protocol
        ${CMAKE_CURRENT_SOURCE_DIR}/src/security
        ${CMAKE_CURRENT_SOURCE_DIR}/src/network)
    target_link_libraries(papacc_c2_dormant PUBLIC papacc_server_config papacc_capability)
    target_compile_definitions(papacc_c2_dormant PUBLIC PAPACC_C8_PRIVATE_STORAGE_PROFILE)
    target_compile_options(papacc_c2_dormant PRIVATE /W4 /WX)
    if(BUILD_TESTING)
        add_executable(papacc_c2_dormant_test tests/c2_dormant_participant_test.c)
        target_link_libraries(papacc_c2_dormant_test PRIVATE papacc_c2_dormant)
        target_compile_options(papacc_c2_dormant_test PRIVATE /W4 /WX)
        add_test(NAME papacc.c2.dormant_participant COMMAND papacc_c2_dormant_test)
    endif()
endif()

option(PAPACC_ENABLE_SECURE_SOURCE_PARTICIPANT "Build private transactional security material participant" OFF)
if(WIN32 AND PAPACC_ENABLE_SECURE_SOURCE_PARTICIPANT)
    if(NOT PAPACC_ENABLE_CANONICAL_API3_CANDIDATE OR NOT TARGET papacc_pst_consumer)
        message(FATAL_ERROR "Secure source participant requires explicit normal API3 candidate and PST proof boundary")
    endif()
    add_library(papacc_secure_source_participant STATIC
        apps/server/server_security_source_win32.c
        src/security/authentication_authorization.c
        src/security/server_security_configuration.c
        src/security/security_composition.c
        src/security/pst_log_adapter.c
        src/security/platform/win32/pst_provider_bootstrap_win32.c)
    target_include_directories(papacc_secure_source_participant PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/server
        ${CMAKE_CURRENT_SOURCE_DIR}/src/security
        ${CMAKE_CURRENT_SOURCE_DIR}/src/security/platform/win32
        ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime)
    target_link_libraries(papacc_secure_source_participant PUBLIC papacc_core papacc_runtime papacc_pst_consumer crypt32)
    target_compile_options(papacc_secure_source_participant PRIVATE /W4 /WX)
    if(BUILD_TESTING)
        add_executable(papacc_secure_source_participant_test tests/secure_source_participant_test.c)
        target_link_libraries(papacc_secure_source_participant_test PRIVATE papacc_secure_source_participant)
        target_compile_options(papacc_secure_source_participant_test PRIVATE /W4 /WX)
        add_test(NAME papacc.security.source_participant COMMAND papacc_secure_source_participant_test)
    endif()
endif()
if(TARGET papacc_secure_source_participant_test)
    set_tests_properties(papacc.security.source_participant PROPERTIES WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
endif()

if(WIN32 AND BUILD_TESTING)
    add_executable(papacc_application_lifetime_test
        tests/application_lifetime_test.c
        apps/server/server_application_lifetime_win32.c)
    target_include_directories(papacc_application_lifetime_test PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/server ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime)
    target_link_libraries(papacc_application_lifetime_test PRIVATE papacc_runtime)
    target_compile_options(papacc_application_lifetime_test PRIVATE /W4 /WX)
    add_test(NAME papacc.application.lifetime COMMAND papacc_application_lifetime_test)
endif()
if(WIN32 AND BUILD_TESTING)
    add_test(NAME papacc.application.lifetime_retained COMMAND papacc_application_lifetime_test retained)
endif()
# Compile the real Secure application sources against normal API3 without
# linking/executing the still-API2 processor. No alternative service is built.
if(WIN32 AND PAPACC_ENABLE_CANONICAL_API3_CANDIDATE)
    add_library(papacc_application_graph_compile OBJECT
        apps/server/main.c apps/server/server_run_win32.c)
    file(GLOB_RECURSE PAPACC_GRAPH_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
    set(PAPACC_GRAPH_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/apps/server" "${CMAKE_CURRENT_SOURCE_DIR}/include")
    foreach(header IN LISTS PAPACC_GRAPH_HEADERS)
        get_filename_component(header_dir "${header}" DIRECTORY)
        list(APPEND PAPACC_GRAPH_INCLUDE_DIRS "${header_dir}")
    endforeach()
    list(REMOVE_DUPLICATES PAPACC_GRAPH_INCLUDE_DIRS)
    target_include_directories(papacc_application_graph_compile PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS})
    target_link_libraries(papacc_application_graph_compile PRIVATE papacc_pst_consumer)
    target_compile_definitions(papacc_application_graph_compile PRIVATE PAPACC_ENABLE_PST_OUTBOUND_TLS=1)
    target_compile_options(papacc_application_graph_compile PRIVATE /W4 /WX)
endif()

if(WIN32 AND PAPACC_ENABLE_CANONICAL_API3_CANDIDATE AND BUILD_TESTING)
    add_executable(papacc_application_graph_census tests/application_graph_census.c
        apps/server/server_application_lifetime_win32.c)
    target_include_directories(papacc_application_graph_census PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS})
    target_link_libraries(papacc_application_graph_census PRIVATE papacc_core papacc_pst_consumer)
    target_compile_definitions(papacc_application_graph_census PRIVATE PAPACC_ENABLE_PST_OUTBOUND_TLS=1)
    target_compile_options(papacc_application_graph_census PRIVATE /W4 /WX)
    add_test(NAME papacc.application.graph_census COMMAND papacc_application_graph_census)
endif()

if(WIN32 AND PAPACC_ENABLE_CANONICAL_API3_CANDIDATE)
    target_sources(papacc_tcp_win32 PRIVATE src/platform/win32/native_teardown_win32.c)
endif()

if(WIN32 AND PAPACC_ENABLE_CANONICAL_API3_CANDIDATE)
    target_sources(papacc_security_composition PRIVATE src/security/pst_socket_attachment_win32.c)
endif()

if(WIN32 AND PAPACC_ENABLE_CANONICAL_API3_CANDIDATE AND BUILD_TESTING)
    add_executable(papacc_socket_ownership_offline_test
        tests/socket_ownership_offline_test.c src/security/pst_socket_attachment_win32.c
        src/platform/win32/native_teardown_win32.c src/platform/win32/tcp_accept_win32.c
        src/platform/win32/tcp_outbound_win32.c src/platform/win32/tcp_connection_transport_win32.c
        src/platform/win32/tcp_platform_win32.c)
    target_include_directories(papacc_socket_ownership_offline_test PRIVATE
        ${PAPACC_GRAPH_INCLUDE_DIRS} "${PAPACC_PST_QUALIFICATION_SDK_ROOT}/include")
    target_compile_definitions(papacc_socket_ownership_offline_test PRIVATE PAPACC_NATIVE_TEARDOWN_TEST_ONLY=1)
    target_link_libraries(papacc_socket_ownership_offline_test PRIVATE papacc_network papacc_connection ws2_32)
    target_compile_options(papacc_socket_ownership_offline_test PRIVATE /W4 /WX)
    add_test(NAME papacc.socket.ownership_offline COMMAND papacc_socket_ownership_offline_test)
endif()

if(WIN32 AND PAPACC_ENABLE_CANONICAL_API3_CANDIDATE AND BUILD_TESTING)
    add_executable(papacc_socket_retention_offline_test tests/socket_retention_offline_test.c)
    target_include_directories(papacc_socket_retention_offline_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS})
    target_link_libraries(papacc_socket_retention_offline_test PRIVATE papacc_server_secure_io_win32 papacc_pst_consumer)
    target_compile_options(papacc_socket_retention_offline_test PRIVATE /W4 /WX)
    papacc_stage_pst_runtime(papacc_socket_retention_offline_test)
    add_test(NAME papacc.socket.retention_offline COMMAND papacc_socket_retention_offline_test)
endif()

# Existing complete C2 core is available for the future factual root wiring.
# The closed participant profile is not installed twice; no runtime owner is added.
if(WIN32 AND PAPACC_ENABLE_CANONICAL_API3_CANDIDATE)
    add_library(papacc_c2_core STATIC src/protocol/c2_codec.c src/protocol/c2_session.c
        src/protocol/c2_tokens_win32.c src/protocol/c2_token_compare.c src/protocol/c2_operation_win32.c)
    target_include_directories(papacc_c2_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/protocol)
    target_link_libraries(papacc_c2_core PUBLIC papacc_capability papacc_security_composition bcrypt)
    target_compile_options(papacc_c2_core PRIVATE /W4 /WX)
    target_link_libraries(papacc_server_run_win32 PRIVATE papacc_c2_core)
endif()

if(WIN32 AND PAPACC_ENABLE_CANONICAL_API3_CANDIDATE)
    add_library(papacc_c3_bridge STATIC src/network_egress/c3_bridge_win32.c)
    target_include_directories(papacc_c3_bridge PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/network_egress)
    target_link_libraries(papacc_c3_bridge PUBLIC papacc_security_composition papacc_pst_consumer papacc_connection ws2_32)
    target_compile_options(papacc_c3_bridge PRIVATE /W4 /WX)
    target_link_libraries(papacc_server_run_win32 PRIVATE papacc_c3_bridge)
endif()

if(WIN32 AND PAPACC_ENABLE_CANONICAL_API3_CANDIDATE AND BUILD_TESTING)
    target_link_libraries(papacc_socket_retention_offline_test PRIVATE papacc_c3_bridge)
    add_executable(papacc_c2_core_empty_offline_test tests/c2_core_empty_offline_test.c)
    target_link_libraries(papacc_c2_core_empty_offline_test PRIVATE papacc_c2_core)
    target_compile_options(papacc_c2_core_empty_offline_test PRIVATE /W4 /WX)
    papacc_stage_pst_runtime(papacc_c2_core_empty_offline_test)
    add_test(NAME papacc.c2.core_empty_offline COMMAND papacc_c2_core_empty_offline_test)
endif()

option(PAPACC_ENABLE_CANONICAL_C8_ROOT "Compose the existing factual closed root with C2/C3" OFF)
option(PAPACC_ENABLE_WINDOWS_3MIB_MANAGED_PROFILE "Explicit Windows Accelerator process ED11 3MiB candidate profile" OFF)
if(NOT "${PAPACC_ENABLE_WINDOWS_3MIB_MANAGED_PROFILE}" MATCHES "^(ON|OFF)$")
    message(FATAL_ERROR "Windows3MiB ED11 selector must be explicitly ON or OFF")
endif()
if(PAPACC_ENABLE_WINDOWS_3MIB_MANAGED_PROFILE)
    if(NOT WIN32 OR NOT PAPACC_ENABLE_CANONICAL_API3_CANDIDATE OR NOT PAPACC_ENABLE_CANONICAL_C8_ROOT)
        message(FATAL_ERROR "Windows3MiB ED11 requires explicit Windows canonical API3/root candidate")
    endif()
    add_compile_definitions(PAPACC_ED11_WINDOWS_3MIB=1)
endif()
if(PAPACC_ENABLE_CANONICAL_C8_ROOT)
    if(NOT WIN32 OR NOT PAPACC_ENABLE_CANONICAL_API3_CANDIDATE OR NOT BUILD_TESTING)
        message(FATAL_ERROR "Canonical closed root requires normal API3 offline candidate profile")
    endif()
    add_compile_definitions(
      "$<$<NOT:$<BOOL:$<TARGET_PROPERTY:PAPACC_STANDALONE_SOCKET_FIXTURE>>>:PAPACC_ENABLE_CANONICAL_C8_ROOT=1>"
      "$<$<NOT:$<BOOL:$<TARGET_PROPERTY:PAPACC_STANDALONE_SOCKET_FIXTURE>>>:PAPACC_C8_PRIVATE_STORAGE_PROFILE=1>")
    target_sources(papacc_server_config PRIVATE apps/server/server_site_tls_profile.c)
    add_library(papacc_configuration_commit STATIC apps/server/server_configuration_commit_win32.c)
    target_include_directories(papacc_configuration_commit PUBLIC ${PAPACC_GRAPH_INCLUDE_DIRS})
    target_link_libraries(papacc_configuration_commit PUBLIC papacc_server_config papacc_issuer bcrypt ktmw32)
    target_compile_options(papacc_configuration_commit PRIVATE /W4 /WX)
    add_library(papacc_c2_root_owner STATIC apps/server/server_c2_owner_win32.c
        apps/server/server_c2_configuration_win32.c apps/server/server_c2_participant_win32.c)
    target_include_directories(papacc_c2_root_owner PUBLIC ${PAPACC_GRAPH_INCLUDE_DIRS})
    # Same IoLoop ABI as canonical owner/consumer; C8 requires outbound TLS.
    target_compile_definitions(papacc_c2_root_owner PUBLIC PAPACC_ENABLE_PST_OUTBOUND_TLS=1)
    target_link_libraries(papacc_c2_root_owner PUBLIC papacc_c2_core papacc_c3_bridge papacc_configuration_commit)
    target_compile_options(papacc_c2_root_owner PRIVATE /W4 /WX)
    add_library(papacc_c8_target_evidence STATIC apps/server/server_c8_target_evidence_win32.c)
    target_include_directories(papacc_c8_target_evidence PUBLIC ${PAPACC_GRAPH_INCLUDE_DIRS})
    target_link_libraries(papacc_c8_target_evidence PUBLIC papacc_pst_consumer papacc_pal psapi)
    target_compile_options(papacc_c8_target_evidence PRIVATE /W4 /WX)
    target_compile_definitions(papacc_c8_target_evidence PRIVATE PAPACC_OBSERVED_BUILD_DEBUG=$<IF:$<CONFIG:Debug>,1,0>)
    target_sources(papacc_server_run_win32 PRIVATE apps/server/server_launcher_owner_win32.c apps/server/server_c2_pair_receipt.c)
    target_link_libraries(papacc_server_run_win32 PRIVATE papacc_c2_root_owner papacc_c8_target_evidence)
endif()

if(PAPACC_ENABLE_CANONICAL_C8_ROOT AND BUILD_TESTING)
    add_executable(papacc_canonical_root_layout_test tests/canonical_root_layout_test.c)
    target_include_directories(papacc_canonical_root_layout_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS})
    target_link_libraries(papacc_canonical_root_layout_test PRIVATE papacc_c2_root_owner)
    target_compile_options(papacc_canonical_root_layout_test PRIVATE /W4 /WX)
    papacc_stage_pst_runtime(papacc_canonical_root_layout_test)
    add_test(NAME papacc.canonical.root_layout COMMAND papacc_canonical_root_layout_test)
endif()

if(PAPACC_ENABLE_CANONICAL_C8_ROOT AND BUILD_TESTING)
 # With C8 enabled the secure owner references the real launcher/C2 owners.
 # The isolated non-network test must link them too, never substitute stubs.
 if(TARGET papacc_server_secure_io_win32_test)
  target_link_libraries(papacc_server_secure_io_win32_test PRIVATE papacc_server_run_win32)
 endif()
 add_executable(papacc_canonical_root_transaction_test tests/canonical_root_transaction_test.c)
 add_executable(papacc_credential_revocation_test tests/credential_revocation_win32_test.c)
 target_include_directories(papacc_credential_revocation_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS})
 target_link_libraries(papacc_credential_revocation_test PRIVATE papacc_server_run_win32)
 target_compile_options(papacc_credential_revocation_test PRIVATE /W4 /WX)
 papacc_stage_pst_runtime(papacc_credential_revocation_test)
 add_test(NAME papacc.credential.revocation_cohort COMMAND papacc_credential_revocation_test)
 set_property(SOURCE apps/server/server_launcher_owner_win32.c tests/credential_revocation_win32_test.c APPEND PROPERTY OBJECT_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_credential_revocation_win32.inc;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_credential_revocation_win32.h")
 target_include_directories(papacc_canonical_root_transaction_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS})
 target_link_libraries(papacc_canonical_root_transaction_test PRIVATE papacc_server_run_win32)
 target_compile_options(papacc_canonical_root_transaction_test PRIVATE /W4 /WX)
 papacc_stage_pst_runtime(papacc_canonical_root_transaction_test)
endif()

# Standalone synthetic socket fixture does not instantiate a C8 root.
# Its low-level sources must retain the normal API3 fixture profile.
if(PAPACC_ENABLE_CANONICAL_C8_ROOT AND BUILD_TESTING)
 set_property(TARGET papacc_socket_ownership_offline_test PROPERTY PAPACC_STANDALONE_SOCKET_FIXTURE TRUE)
 target_link_libraries(papacc_socket_retention_offline_test PRIVATE papacc_server_run_win32)
 target_link_libraries(papacc_c2_core_empty_offline_test PRIVATE papacc_server_run_win32)
endif()

if(PAPACC_ENABLE_CANONICAL_C8_ROOT AND BUILD_TESTING)
 add_executable(papacc_site_tls_catalog_offline_test tests/site_tls_catalog_offline_test.c)
 target_link_libraries(papacc_site_tls_catalog_offline_test PRIVATE papacc_configuration_commit)
 target_compile_options(papacc_site_tls_catalog_offline_test PRIVATE /W4 /WX)
endif()

if(PAPACC_ENABLE_CANONICAL_C8_ROOT AND BUILD_TESTING)
 add_executable(papacc_pair_receipt_offline_test tests/pair_receipt_offline_test.c)
 target_include_directories(papacc_pair_receipt_offline_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS})
 target_link_libraries(papacc_pair_receipt_offline_test PRIVATE papacc_server_run_win32)
 target_compile_options(papacc_pair_receipt_offline_test PRIVATE /W4 /WX)
 papacc_stage_pst_runtime(papacc_pair_receipt_offline_test)
endif()
if(PAPACC_ENABLE_CANONICAL_C8_ROOT AND BUILD_TESTING)
 add_executable(papacc_process_provenance_offline_test tests/process_provenance_offline_test.c)
 target_include_directories(papacc_process_provenance_offline_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS})
 target_link_libraries(papacc_process_provenance_offline_test PRIVATE papacc_server_run_win32)
 target_compile_options(papacc_process_provenance_offline_test PRIVATE /W4 /WX)
 papacc_stage_pst_runtime(papacc_process_provenance_offline_test)
endif()
if(PAPACC_ENABLE_CANONICAL_C8_ROOT AND BUILD_TESTING)
 add_executable(papacc_target_typed_evidence_test tests/target_typed_evidence_test.c)
 target_include_directories(papacc_target_typed_evidence_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS})
 target_link_libraries(papacc_target_typed_evidence_test PRIVATE papacc_server_run_win32)
 target_compile_options(papacc_target_typed_evidence_test PRIVATE /W4 /WX)
 papacc_stage_pst_runtime(papacc_target_typed_evidence_test)
endif()
# Explicit private root dependencies: localized MSVC include scans can omit .inc.
set_source_files_properties(apps/server/server_launcher_owner_win32.c PROPERTIES OBJECT_DEPENDS
 "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_configuration_root_transaction_win32.inc;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_launcher_owner_win32.h;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_configuration_commit_win32.h;${CMAKE_CURRENT_SOURCE_DIR}/src/issuer/issuer_codec.h")
# ROOT06 offline component fixture; never linked into application startup.
if(PAPACC_ENABLE_CANONICAL_C8_ROOT AND BUILD_TESTING)
 add_executable(papacc_issuer_root_store_test tests/issuer_root_store_win32_test.c tests/issuer_root_preimage_size.c
     apps/server/server_configuration_commit_win32.c)
 target_include_directories(papacc_issuer_root_store_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS})
 target_compile_definitions(papacc_issuer_root_store_test PRIVATE PAPACC_ISSUER_STORAGE_COMPONENT_TEST=1)
 target_link_libraries(papacc_issuer_root_store_test PRIVATE papacc_server_config papacc_issuer bcrypt ktmw32)
 target_compile_options(papacc_issuer_root_store_test PRIVATE /W4 /WX)
 add_executable(papacc_issuer_root_guard_test tests/issuer_root_store_win32_test.c tests/issuer_root_preimage_size.c)
 target_link_libraries(papacc_issuer_root_guard_test PRIVATE papacc_configuration_commit)
 target_compile_options(papacc_issuer_root_guard_test PRIVATE /W4 /WX)
 set_source_files_properties(src/issuer/issuer_codec.c tests/issuer_root_store_win32_test.c
     apps/server/server_configuration_commit_win32.c PROPERTIES OBJECT_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/issuer/issuer_codec.h;${CMAKE_CURRENT_SOURCE_DIR}/src/issuer/issuer_core.h;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_configuration_commit_win32.h;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_configuration_issuer06_win32.inc")
endif()
# Localized MSVC dependency scans are not the only source of .inc edges.
set_property(SOURCE apps/server/server_launcher_owner_win32.c APPEND PROPERTY OBJECT_DEPENDS
 "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_issuer_publication_v2_win32.inc")
set_property(SOURCE src/issuer/issuer_codec.c APPEND PROPERTY OBJECT_DEPENDS
 "${CMAKE_CURRENT_SOURCE_DIR}/src/issuer/issuer_publication_v2.inc")
set_property(SOURCE tests/issuer_root_store_win32_test.c APPEND PROPERTY OBJECT_DEPENDS
 "${CMAKE_CURRENT_SOURCE_DIR}/tests/issuer_publication_v2_cases.inc")
# Pending ROOT06 native adapter is private; common outcome contract is portable.
set_property(SOURCE apps/server/server_configuration_commit_win32.c APPEND PROPERTY OBJECT_DEPENDS
 "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_configuration_pending_win32.inc"
 "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_configuration_pending_win32.h"
 "${CMAKE_CURRENT_SOURCE_DIR}/src/issuer/issuer_transaction.h")

# Session/live publication: .inc dependencies remain explicit for localized MSVC.
set_property(SOURCE apps/server/server_launcher_owner_win32.c APPEND PROPERTY OBJECT_DEPENDS
 "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_c2_root_binding_win32.inc"
 "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_admission_preflight_win32.inc"
 "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_listener_composition_win32.inc"
 "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_c2_owner_win32.h")
set_property(SOURCE apps/server/server_c2_owner_win32.c APPEND PROPERTY OBJECT_DEPENDS
 "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_launcher_owner_win32.h"
 "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_c2_owner_win32.h")

set_property(SOURCE apps/server/server_launcher_owner_win32.c APPEND PROPERTY OBJECT_DEPENDS
 "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_configuration_attachment_win32.inc")

# Same-root checkpoint: all consumers share capacity/accounting ABI.
set_property(SOURCE apps/server/server_launcher_owner_win32.c apps/server/server_c2_owner_win32.c
    tests/canonical_root_transaction_test.c tests/canonical_root_layout_test.c APPEND PROPERTY OBJECT_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_configuration_pending_win32.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_configuration_commit_win32.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/issuer/issuer_codec.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_c8_private_storage_layout_win32.h")

# Offline papacc.conf candidates only. Never linked into papacc_server.
option(PAPACC_ENABLE_OFFLINE_CONFIG_SOURCE "Build bounded offline configuration source" OFF)
if(PAPACC_ENABLE_OFFLINE_CONFIG_SOURCE)
    add_library(papacc_config_source STATIC src/config/config_source.c)
    target_include_directories(papacc_config_source PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/config)
    set_target_properties(papacc_config_source PROPERTIES C_STANDARD 90 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
    papacc_enable_warnings(papacc_config_source)
    if(BUILD_TESTING)
        add_executable(papacc_config_source_test tests/config_source_test.c)
        target_link_libraries(papacc_config_source_test PRIVATE papacc_config_source)
        set_target_properties(papacc_config_source_test PROPERTIES C_STANDARD 90 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        papacc_enable_warnings(papacc_config_source_test)
        add_test(NAME papacc.config_source.offline COMMAND papacc_config_source_test
            ${CMAKE_CURRENT_SOURCE_DIR}/docs/integration/squid-inspired-configuration-contract-20261019/papacc.conf)
    endif()
endif()

# Administrative resolution is read-only/opt-in; no product activation or codec.
option(PAPACC_ENABLE_CONFIG_ADMIN_BINDING "Build read-only canonical config binding" OFF)
if(PAPACC_ENABLE_CONFIG_ADMIN_BINDING)
    if(NOT PAPACC_ENABLE_OFFLINE_CONFIG_SOURCE OR NOT PAPACC_ENABLE_CANONICAL_C8_ROOT)
        message(FATAL_ERROR "Config binding requires existing offline source and canonical C8 models")
    endif()
    add_library(papacc_config_binding STATIC src/config/config_binding.c)
    target_include_directories(papacc_config_binding PUBLIC src/config apps/server src/network)
    target_compile_definitions(papacc_config_binding PUBLIC PAPACC_C8_PRIVATE_STORAGE_PROFILE=1)
    target_link_libraries(papacc_config_binding PUBLIC papacc_config_source papacc_server_config)
    set_target_properties(papacc_config_binding PROPERTIES C_STANDARD 90 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
    papacc_enable_warnings(papacc_config_binding)
    target_sources(papacc_server_run_win32 PRIVATE apps/server/server_config_source_binding_win32.c)
    target_link_libraries(papacc_server_run_win32 PRIVATE papacc_config_binding)
    if(BUILD_TESTING)
        add_executable(papacc_config_binding_test tests/config_binding_test.c)
        target_link_libraries(papacc_config_binding_test PRIVATE papacc_config_binding)
        papacc_enable_warnings(papacc_config_binding_test)
        add_test(NAME papacc.config_binding.model COMMAND papacc_config_binding_test)
        add_executable(papacc_config_binding_native_test tests/config_binding_native_test.c)
        target_include_directories(papacc_config_binding_native_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} src/config)
        target_link_libraries(papacc_config_binding_native_test PRIVATE papacc_server_run_win32 papacc_config_binding)
        papacc_enable_warnings(papacc_config_binding_native_test)
        papacc_stage_pst_runtime(papacc_config_binding_native_test)
        add_test(NAME papacc.config_binding.native_guard COMMAND papacc_config_binding_native_test)
    endif()
endif()


# LST1 accepted private OFFLINE fragment. Never linked into product targets.
option(PAPACC_ENABLE_OFFLINE_LST1 "Build private offline LST1 codec" OFF)
if(PAPACC_ENABLE_OFFLINE_LST1)
    add_library(papacc_config_lst1 STATIC src/config/config_lst1.c)
    target_include_directories(papacc_config_lst1 PUBLIC src/config include)
    set_target_properties(papacc_config_lst1 PROPERTIES C_STANDARD 90 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
    papacc_enable_warnings(papacc_config_lst1)
    if(BUILD_TESTING)
        if(NOT PAPACC_ENABLE_CONFIG_ADMIN_BINDING)
            message(FATAL_ERROR "LST1 native SHA regression requires existing canonical config binding test profile")
        endif()
        add_executable(papacc_config_lst1_test tests/config_lst1_test.c)
        target_include_directories(papacc_config_lst1_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} src/config)
        target_link_libraries(papacc_config_lst1_test PRIVATE papacc_config_lst1 papacc_server_run_win32 papacc_config_binding)
        set_target_properties(papacc_config_lst1_test PROPERTIES C_STANDARD 90 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        papacc_enable_warnings(papacc_config_lst1_test)
        papacc_stage_pst_runtime(papacc_config_lst1_test)
        add_test(NAME papacc.config_lst1.offline COMMAND papacc_config_lst1_test)
    endif()
endif()


# Accepted D3 private DATA codec only. No link into product root/runtime.
option(PAPACC_ENABLE_OFFLINE_ROOT8 "Build private offline ROOT8 codec" OFF)
if(PAPACC_ENABLE_OFFLINE_ROOT8)
    if(NOT PAPACC_ENABLE_OFFLINE_LST1)
        message(FATAL_ERROR "ROOT8 requires existing offline LST1 codec")
    endif()
    add_library(papacc_config_root8 STATIC src/config/config_root8.c src/config/config_root8_v2.c src/config/config_root8_v3.c src/config/config_root8_v4.c src/config/config_root8_v5.c src/config/config_root8_time_source_key.c src/issuer/strict_ed25519.c)
    target_include_directories(papacc_config_root8 PUBLIC src/config include)
    target_link_libraries(papacc_config_root8 PUBLIC papacc_config_lst1 papacc_issuer)
    set_target_properties(papacc_config_root8 PROPERTIES C_STANDARD 90 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
    papacc_enable_warnings(papacc_config_root8)
    if(BUILD_TESTING)
        add_executable(papacc_time_source_key_test tests/config_root8_time_source_key_test.c)
        target_include_directories(papacc_time_source_key_test PRIVATE include src/config src/issuer)
        target_link_libraries(papacc_time_source_key_test PRIVATE papacc_config_root8)
        set_target_properties(papacc_time_source_key_test PROPERTIES C_STANDARD 90 C_EXTENSIONS NO)
        papacc_enable_warnings(papacc_time_source_key_test)
        add_test(NAME papacc.time_source.key_data COMMAND papacc_time_source_key_test)
    endif()
    if(BUILD_TESTING)
        if(NOT PAPACC_ENABLE_CONFIG_ADMIN_BINDING)
            message(FATAL_ERROR "ROOT8 native data regression requires existing canonical config binding test profile")
        endif()
        add_executable(papacc_config_root8_test tests/config_root8_test.c apps/server/server_config_root8_data_win32.c)
        target_include_directories(papacc_config_root8_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} src/config apps/server)
        target_link_libraries(papacc_config_root8_test PRIVATE papacc_config_root8 papacc_server_run_win32 papacc_config_binding)
        set_target_properties(papacc_config_root8_test PROPERTIES C_STANDARD 90 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        papacc_enable_warnings(papacc_config_root8_test)
        papacc_stage_pst_runtime(papacc_config_root8_test)
        add_test(NAME papacc.config_root8.offline COMMAND papacc_config_root8_test)
        add_executable(papacc_config_root8_v2_test tests/config_root8_v2_test.c apps/server/server_config_root8_data_win32.c)
        target_include_directories(papacc_config_root8_v2_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} src/config src/issuer apps/server)
        target_link_libraries(papacc_config_root8_v2_test PRIVATE papacc_config_root8 papacc_server_run_win32 papacc_config_binding papacc_issuer)
        set_target_properties(papacc_config_root8_v2_test PROPERTIES C_STANDARD 90 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
        papacc_enable_warnings(papacc_config_root8_v2_test)
        papacc_stage_pst_runtime(papacc_config_root8_v2_test)
        add_test(NAME papacc.config_root8.v2_offline COMMAND papacc_config_root8_v2_test)
        if(PAPACC_ENABLE_OFFLINE_D3_WORKER)
            add_executable(papacc_config_root8_v3_test tests/config_root8_v3_test.c apps/server/server_config_root8_data_win32.c)
            target_include_directories(papacc_config_root8_v3_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} src/config src/issuer apps/server)
            target_compile_definitions(papacc_config_root8_v3_test PRIVATE PAPACC_OFFLINE_D3_WORKER=1)
            target_link_libraries(papacc_config_root8_v3_test PRIVATE papacc_config_root8 papacc_server_run_win32 papacc_config_binding papacc_issuer)
            set_target_properties(papacc_config_root8_v3_test PROPERTIES C_STANDARD 90 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
            papacc_enable_warnings(papacc_config_root8_v3_test)
            papacc_stage_pst_runtime(papacc_config_root8_v3_test)
            add_test(NAME papacc.config_root8.v3_offline COMMAND papacc_config_root8_v3_test)
            add_executable(papacc_config_root8_v4_test tests/config_root8_v4_test.c apps/server/server_config_root8_data_win32.c)
            target_include_directories(papacc_config_root8_v4_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} src/config src/issuer apps/server)
            target_compile_definitions(papacc_config_root8_v4_test PRIVATE PAPACC_OFFLINE_D3_WORKER=1)
            target_link_libraries(papacc_config_root8_v4_test PRIVATE papacc_config_root8 papacc_server_run_win32 papacc_config_binding papacc_issuer)
            set_target_properties(papacc_config_root8_v4_test PROPERTIES C_STANDARD 90 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
            papacc_enable_warnings(papacc_config_root8_v4_test)
            papacc_stage_pst_runtime(papacc_config_root8_v4_test)
            add_test(NAME papacc.config_root8.v4_offline COMMAND papacc_config_root8_v4_test)
        endif()
        set_property(SOURCE src/config/config_root8_v2.c src/config/config_root8_v3.c APPEND PROPERTY OBJECT_DEPENDS
            "${CMAKE_CURRENT_SOURCE_DIR}/src/config/config_root8_internal.h;${CMAKE_CURRENT_SOURCE_DIR}/src/config/config_root8_v3.h")
        set_property(SOURCE tests/config_root8_v3_test.c APPEND PROPERTY OBJECT_DEPENDS
            "${CMAKE_CURRENT_SOURCE_DIR}/tests/config_root8_v2_test.c;${CMAKE_CURRENT_SOURCE_DIR}/src/config/config_root8_v3.h")

    endif()
endif()

# Same existing worker, explicitly offline opt-in; never activates root CURRENT.
option(PAPACC_ENABLE_OFFLINE_D3_WORKER "Build offline same-worker D3 update adapter" OFF)
if(PAPACC_ENABLE_OFFLINE_D3_WORKER)
 if(NOT PAPACC_ENABLE_OFFLINE_ROOT8 OR NOT PAPACC_ENABLE_CANONICAL_C8_ROOT OR NOT BUILD_TESTING)
  message(FATAL_ERROR "D3 worker requires existing offline ROOT8/C8 test composition")
 endif()
 target_compile_definitions(papacc_configuration_commit PUBLIC PAPACC_OFFLINE_D3_WORKER=1)
 target_sources(papacc_configuration_commit PRIVATE apps/server/server_config_root8_data_win32.c)
 target_link_libraries(papacc_configuration_commit PUBLIC papacc_config_root8)
 target_compile_definitions(papacc_issuer_root_store_test PRIVATE PAPACC_OFFLINE_D3_WORKER=1)
 target_sources(papacc_issuer_root_store_test PRIVATE apps/server/server_config_root8_data_win32.c)
 target_link_libraries(papacc_issuer_root_store_test PRIVATE papacc_config_root8)
 add_executable(papacc_d3_worker_test tests/d3_worker_win32_test.c tests/issuer_root_preimage_size.c
  apps/server/server_configuration_commit_win32.c apps/server/server_config_root8_data_win32.c)
 target_include_directories(papacc_d3_worker_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} src/config apps/server)
 target_compile_definitions(papacc_d3_worker_test PRIVATE PAPACC_OFFLINE_D3_WORKER=1 PAPACC_ISSUER_STORAGE_COMPONENT_TEST=1)
 target_link_libraries(papacc_d3_worker_test PRIVATE papacc_config_root8 papacc_server_config papacc_issuer bcrypt ktmw32)
 target_compile_options(papacc_d3_worker_test PRIVATE /W4 /WX)
 set_property(SOURCE apps/server/server_configuration_commit_win32.c APPEND PROPERTY OBJECT_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_configuration_d3_worker_win32.inc")
endif()

# Candidate-only config frontend, before runtime acquisition; no second CLI.
option(PAPACC_ENABLE_CONFIG_ADMIN_CLI "Build candidate configuration diagnostics" OFF)
if(PAPACC_ENABLE_CONFIG_ADMIN_CLI)
    if(NOT WIN32 OR NOT PAPACC_ENABLE_CANONICAL_API3_CANDIDATE OR NOT PAPACC_ENABLE_OFFLINE_CONFIG_SOURCE)
        message(FATAL_ERROR "Config CLI requires Windows API3 candidate and existing source parser")
    endif()
    target_sources(papacc_server PRIVATE apps/server/server_config_cli.c)
    target_include_directories(papacc_server PRIVATE src/issuer)
    target_link_libraries(papacc_server PRIVATE papacc_config_source)
    target_compile_definitions(papacc_server PRIVATE PAPACC_ENABLE_CONFIG_ADMIN_CLI=1)
    if(PAPACC_ENABLE_OFFLINE_D3_WORKER)
        target_compile_definitions(papacc_server PRIVATE PAPACC_OFFLINE_D3_WORKER=1)
    endif()
endif()

# Same-worker native namespace primitive regression, isolated diagnostic token bypass.
if(PAPACC_ENABLE_OFFLINE_D3_WORKER)
 target_link_libraries(papacc_configuration_commit PUBLIC advapi32)
 target_link_libraries(papacc_d3_worker_test PRIVATE advapi32)
 target_link_libraries(papacc_issuer_root_store_test PRIVATE advapi32)
 add_executable(papacc_bootstrap_namespace_test tests/bootstrap_namespace_win32_test.c
  apps/server/server_configuration_commit_win32.c apps/server/server_config_root8_data_win32.c)
 target_include_directories(papacc_bootstrap_namespace_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} src/config apps/server)
 target_compile_definitions(papacc_bootstrap_namespace_test PRIVATE PAPACC_OFFLINE_D3_WORKER=1
  PAPACC_ISSUER_STORAGE_COMPONENT_TEST=1 PAPACC_BOOTSTRAP_NATIVE_COMPONENT_TEST=1)
 target_link_libraries(papacc_bootstrap_namespace_test PRIVATE papacc_config_root8 papacc_server_config papacc_issuer bcrypt ktmw32 advapi32)
 target_compile_options(papacc_bootstrap_namespace_test PRIVATE /W4 /WX)
 set_property(SOURCE apps/server/server_configuration_commit_win32.c APPEND PROPERTY OBJECT_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_configuration_bootstrap_win32.inc")
endif()

if(PAPACC_ENABLE_OFFLINE_D3_WORKER)
 add_executable(papacc_bootstrap_d3_commit_test tests/bootstrap_d3_commit_win32_test.c
  apps/server/server_configuration_commit_win32.c apps/server/server_config_root8_data_win32.c)
 target_include_directories(papacc_bootstrap_d3_commit_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} src/config apps/server)
 target_compile_definitions(papacc_bootstrap_d3_commit_test PRIVATE PAPACC_OFFLINE_D3_WORKER=1
  PAPACC_ISSUER_STORAGE_COMPONENT_TEST=1 PAPACC_BOOTSTRAP_NATIVE_COMPONENT_TEST=1)
 target_link_libraries(papacc_bootstrap_d3_commit_test PRIVATE papacc_config_root8 papacc_server_config papacc_issuer bcrypt ktmw32 advapi32)
 target_compile_options(papacc_bootstrap_d3_commit_test PRIVATE /W4 /WX)
 set_property(SOURCE apps/server/server_configuration_commit_win32.c APPEND PROPERTY OBJECT_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_configuration_bootstrap_commit_win32.inc")
endif()

# Public request validation only; no CNG key activation or product linkage.
if(WIN32)
 add_library(papacc_issuer_windows_validation STATIC apps/server/server_issuer_windows_validation.c)
 target_include_directories(papacc_issuer_windows_validation PUBLIC apps/server src/issuer include)
 target_link_libraries(papacc_issuer_windows_validation PUBLIC papacc_issuer crypt32)
 papacc_enable_warnings(papacc_issuer_windows_validation)
 set_source_files_properties(apps/server/server_issuer_windows_validation.c tests/issuer_windows_validation_test.c PROPERTIES OBJECT_DEPENDS
 "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_issuer_windows_validation.h;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_ca_key_win32.h;${CMAKE_CURRENT_SOURCE_DIR}/src/issuer/issuer_core.h")
 if(BUILD_TESTING)
  add_executable(papacc_issuer_windows_validation_test tests/issuer_windows_validation_test.c)
  target_link_libraries(papacc_issuer_windows_validation_test PRIVATE papacc_issuer_windows_validation)
  papacc_enable_warnings(papacc_issuer_windows_validation_test)
  add_test(NAME papacc.issuer.windows_validation COMMAND papacc_issuer_windows_validation_test "${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/security/client-a.der.b64")
 endif()
endif()

if(PAPACC_ENABLE_CANONICAL_C8_ROOT AND PAPACC_ENABLE_OFFLINE_CONFIG_SOURCE AND BUILD_TESTING)
 add_executable(papacc_phase6_memory_census_test tests/phase6_memory_census_test.c)
 target_include_directories(papacc_phase6_memory_census_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} src/config src/issuer apps/server)
 target_link_libraries(papacc_phase6_memory_census_test PRIVATE papacc_c2_root_owner psapi advapi32)
 if(WIN32 AND PAPACC_ENABLE_CONFIG_ADMIN_CLI AND TARGET papacc_server_run_win32)
  target_link_libraries(papacc_phase6_memory_census_test PRIVATE papacc_server_run_win32 papacc_storage_sha256)
  target_compile_definitions(papacc_phase6_memory_census_test PRIVATE PAPACC_CENSUS_UPDATE_K=1)
 endif()
 target_compile_options(papacc_phase6_memory_census_test PRIVATE /W4 /WX)
 papacc_stage_pst_runtime(papacc_phase6_memory_census_test)
 add_test(NAME papacc.phase6.memory_census COMMAND papacc_phase6_memory_census_test)
 add_executable(papacc_phase6_resource_observer_model_test tests/phase6_resource_observer_model_test.c)
 target_include_directories(papacc_phase6_resource_observer_model_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} src/config src/issuer apps/server)
 target_link_libraries(papacc_phase6_resource_observer_model_test PRIVATE papacc_c2_root_owner psapi advapi32)
 target_compile_options(papacc_phase6_resource_observer_model_test PRIVATE /W4 /WX)
 set_source_files_properties(tests/phase6_resource_observer_model_test.c PROPERTIES OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests/phase6_memory_census_test.c")
 add_test(NAME papacc.phase6.resource_observer_model COMMAND papacc_phase6_resource_observer_model_test)
endif()

# Protected signer adapter is an uninstantiated component, not runtime activation.
if(WIN32)
 add_library(papacc_issuer_cng_windows STATIC apps/server/server_issuer_x509_win32.c apps/server/server_issuer_cng_backend_win32.c apps/server/server_ca_key_win32.c)
 target_include_directories(papacc_issuer_cng_windows PUBLIC apps/server src/issuer include)
 target_link_libraries(papacc_issuer_cng_windows PUBLIC papacc_issuer_windows_validation ncrypt advapi32 crypt32)
 papacc_enable_warnings(papacc_issuer_cng_windows)
 set_source_files_properties(apps/server/server_issuer_x509_win32.c apps/server/server_issuer_cng_backend_win32.c tests/issuer_cng_x509_test.c tests/issuer_cng_production_gate_test.c PROPERTIES OBJECT_DEPENDS
 "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_issuer_x509_win32.h;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_ca_key_win32.h;${CMAKE_CURRENT_SOURCE_DIR}/src/issuer/issuer_core.h;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_issuer_x509_win32.c;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_issuer_cng_backend_win32.c;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_issuer_windows_validation.c")
 if(BUILD_TESTING)
  add_executable(papacc_issuer_cng_x509_test tests/issuer_cng_x509_test.c src/issuer/issuer_core.c)
  target_include_directories(papacc_issuer_cng_x509_test PRIVATE apps/server src/issuer include)
  target_compile_definitions(papacc_issuer_cng_x509_test PRIVATE PAPACC_ISSUER_OFFLINE_TEST=1)
  target_link_libraries(papacc_issuer_cng_x509_test PRIVATE crypt32 ncrypt bcrypt)
  papacc_enable_warnings(papacc_issuer_cng_x509_test)
  add_test(NAME papacc.issuer.cng_x509_model COMMAND papacc_issuer_cng_x509_test "${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/security/ca.der.b64" "${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/security/client-a.der.b64")
  add_executable(papacc_issuer_cng_production_gate_test tests/issuer_cng_production_gate_test.c)
  target_link_libraries(papacc_issuer_cng_production_gate_test PRIVATE papacc_issuer_cng_windows)
  papacc_enable_warnings(papacc_issuer_cng_production_gate_test)
  add_test(NAME papacc.issuer.cng_production_gate COMMAND papacc_issuer_cng_production_gate_test)
 endif()
endif()

# Constant profile and shared-credit tests; synthetic, no product authority.
if(BUILD_TESTING AND WIN32)
 foreach(profile selected legacy other)
  add_executable(papacc_ed11_profile_${profile}_test tests/ed11_process_profile_test.c)
  target_include_directories(papacc_ed11_profile_${profile}_test PRIVATE src/core)
  if(profile STREQUAL "selected" AND PAPACC_ENABLE_WINDOWS_3MIB_MANAGED_PROFILE)
   target_compile_definitions(papacc_ed11_profile_${profile}_test PRIVATE ED11_EXPECTED=3145728U)
  else()
   target_compile_definitions(papacc_ed11_profile_${profile}_test PRIVATE ED11_EXPECTED=2097152U ED11_TEST_LEGACY=1)
  endif()
  if(profile STREQUAL "other")
   target_compile_definitions(papacc_ed11_profile_${profile}_test PRIVATE ED11_TEST_OTHER=1)
  endif()
  papacc_enable_warnings(papacc_ed11_profile_${profile}_test)
  add_test(NAME papacc.ed11.profile_${profile} COMMAND papacc_ed11_profile_${profile}_test)
 endforeach()
 foreach(profile selected legacy)
  add_executable(papacc_ed11_credit_${profile}_test tests/ed11_process_credit_test.c)
  target_include_directories(papacc_ed11_credit_${profile}_test PRIVATE include apps/server)
  if(profile STREQUAL "legacy")
   target_compile_definitions(papacc_ed11_credit_${profile}_test PRIVATE ED11_TEST_LEGACY=1)
  endif()
  papacc_enable_warnings(papacc_ed11_credit_${profile}_test)
  add_test(NAME papacc.ed11.credit_${profile} COMMAND papacc_ed11_credit_${profile}_test)
 endforeach()
endif()


# Accepted enrollment DATA components. No sockets/registration/authority.
option(PAPACC_ENABLE_OFFLINE_ENROLLMENT "Build private offline enrollment components only" OFF)
if(PAPACC_ENABLE_OFFLINE_ENROLLMENT)
 add_library(papacc_enrollment STATIC src/provisioning/enrollment.c src/protocol/c2_token_compare.c)
 target_include_directories(papacc_enrollment PUBLIC src/provisioning include)
 set_target_properties(papacc_enrollment PROPERTIES C_STANDARD 90 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
 papacc_enable_warnings(papacc_enrollment)
 add_library(papacc_enrollment_issuer STATIC src/provisioning/enrollment_issuer.c)
 target_link_libraries(papacc_enrollment_issuer PUBLIC papacc_enrollment papacc_issuer)
 set_target_properties(papacc_enrollment_issuer PROPERTIES C_STANDARD 90 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
 papacc_enable_warnings(papacc_enrollment_issuer)
 if(BUILD_TESTING)
  add_executable(papacc_enrollment_issuer_test tests/enrollment_issuer_model_test.c src/issuer/issuer_core.c)
  target_link_libraries(papacc_enrollment_issuer_test PRIVATE papacc_enrollment_issuer)
  target_compile_definitions(papacc_enrollment_issuer_test PRIVATE PAPACC_ISSUER_OFFLINE_TEST=1)
  papacc_enable_warnings(papacc_enrollment_issuer_test)
  set_property(SOURCE tests/enrollment_issuer_model_test.c APPEND PROPERTY OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests/issuer_core_test.c")
  add_test(NAME papacc.enrollment.issuer_composition COMMAND papacc_enrollment_issuer_test)
 endif()
 if(TARGET papacc_issuer_cng_x509_test)
  target_link_libraries(papacc_issuer_cng_x509_test PRIVATE papacc_enrollment_issuer)
 endif()
 if(WIN32)
  add_library(papacc_enrollment_windows STATIC src/provisioning/enrollment_windows.c)
  target_link_libraries(papacc_enrollment_windows PUBLIC papacc_enrollment bcrypt)
  papacc_enable_warnings(papacc_enrollment_windows)
 endif()
 if(BUILD_TESTING)
  add_executable(papacc_enrollment_test tests/enrollment_test.c)
  target_link_libraries(papacc_enrollment_test PRIVATE papacc_enrollment)
  if(WIN32)
   target_link_libraries(papacc_enrollment_test PRIVATE papacc_enrollment_windows)
  endif()
  papacc_enable_warnings(papacc_enrollment_test)
  add_test(NAME papacc.enrollment.data_offline COMMAND papacc_enrollment_test)
 endif()
 if(TARGET papacc_security_composition)
  add_library(papacc_enrollment_pst STATIC src/provisioning/enrollment_pst.c)
  target_link_libraries(papacc_enrollment_pst PUBLIC papacc_enrollment papacc_pst_consumer)
  papacc_enable_warnings(papacc_enrollment_pst)
  if(BUILD_TESTING)
   add_executable(papacc_enrollment_pst_test tests/enrollment_pst_test.c)
   target_link_libraries(papacc_enrollment_pst_test PRIVATE papacc_enrollment_pst papacc_security_composition papacc_pst_consumer)
   papacc_enable_warnings(papacc_enrollment_pst_test)
   papacc_stage_pst_runtime(papacc_enrollment_pst_test)
   add_test(NAME papacc.enrollment.pst_profile_data COMMAND papacc_enrollment_pst_test)
  endif()
 endif()
 set_source_files_properties(src/provisioning/enrollment.c src/provisioning/enrollment_windows.c
  src/provisioning/enrollment_pst.c tests/enrollment_test.c tests/enrollment_pst_test.c PROPERTIES OBJECT_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/provisioning/enrollment.h;${CMAKE_CURRENT_SOURCE_DIR}/src/provisioning/enrollment_pst.h;${CMAKE_CURRENT_SOURCE_DIR}/src/core/ed11_process_profile.h")
endif()

# Isolated native public kind8 DATA component. Never attached to root/runtime.
if(TARGET papacc_config_root8 AND TARGET papacc_issuer_cng_windows)
 add_library(papacc_root8_signer_data STATIC apps/server/server_config_root8_signer_win32.c)
 target_include_directories(papacc_root8_signer_data PUBLIC apps/server src/config src/issuer include)
 target_link_libraries(papacc_root8_signer_data PUBLIC papacc_config_root8 papacc_issuer_cng_windows)
 papacc_enable_warnings(papacc_root8_signer_data)
 if(PAPACC_ENABLE_WINDOWS_3MIB_MANAGED_PROFILE)
  target_compile_definitions(papacc_root8_signer_data PRIVATE PAPACC_ED11_WINDOWS_3MIB=1)
 endif()
 if(BUILD_TESTING)
  add_executable(papacc_root8_kind8_model_test tests/root8_kind8_model_test.c src/issuer/issuer_core.c src/issuer/issuer_codec.c)
  target_include_directories(papacc_root8_kind8_model_test PRIVATE apps/server src/config src/issuer include)
  target_compile_definitions(papacc_root8_kind8_model_test PRIVATE PAPACC_ISSUER_OFFLINE_TEST=1)
  if(PAPACC_ENABLE_WINDOWS_3MIB_MANAGED_PROFILE)
   target_compile_definitions(papacc_root8_kind8_model_test PRIVATE PAPACC_ED11_WINDOWS_3MIB=1)
  endif()
  target_link_libraries(papacc_root8_kind8_model_test PRIVATE crypt32)
  papacc_enable_warnings(papacc_root8_kind8_model_test)
  add_test(NAME papacc.root8.kind8_data_model COMMAND papacc_root8_kind8_model_test "${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/security/ca.der.b64" "${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/security/client-a.der.b64")
  add_executable(papacc_root8_kind8_native_gate_test tests/root8_kind8_model_test.c src/issuer/issuer_core.c src/issuer/issuer_codec.c)
  target_include_directories(papacc_root8_kind8_native_gate_test PRIVATE apps/server src/config src/issuer include)
  target_compile_definitions(papacc_root8_kind8_native_gate_test PRIVATE PAPACC_ISSUER_OFFLINE_TEST=1 PAPACC_KIND8_NATIVE_NEGATIVE=1)
  if(PAPACC_ENABLE_WINDOWS_3MIB_MANAGED_PROFILE)
   target_compile_definitions(papacc_root8_kind8_native_gate_test PRIVATE PAPACC_ED11_WINDOWS_3MIB=1)
  endif()
  target_link_libraries(papacc_root8_kind8_native_gate_test PRIVATE papacc_issuer_cng_windows)
  papacc_enable_warnings(papacc_root8_kind8_native_gate_test)
  add_test(NAME papacc.root8.kind8_native_gate COMMAND papacc_root8_kind8_native_gate_test "${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/security/ca.der.b64" "${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/security/client-a.der.b64")
 endif()
endif()

# Immutable v3 DATA backings in the same canonical slot/pin owner.
if(PAPACC_ENABLE_OFFLINE_D3_WORKER AND PAPACC_ENABLE_WINDOWS_3MIB_MANAGED_PROFILE)
 target_link_libraries(papacc_configuration_commit PUBLIC crypt32)
 foreach(t papacc_issuer_root_store_test papacc_d3_worker_test papacc_bootstrap_d3_commit_test papacc_bootstrap_namespace_test)
  if(TARGET ${t})
   target_link_libraries(${t} PRIVATE crypt32)
  endif()
 endforeach()
 add_executable(papacc_v3_backing_test tests/v3_backing_win32_test.c apps/server/server_config_root8_data_win32.c)
 target_compile_definitions(papacc_v3_backing_test PRIVATE PAPACC_OFFLINE_D3_WORKER=1 PAPACC_V3_BACKING_FIXTURE=1)
 target_include_directories(papacc_v3_backing_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} src/config src/issuer apps/server)
 target_link_libraries(papacc_v3_backing_test PRIVATE papacc_config_root8 papacc_server_run_win32 papacc_config_binding papacc_issuer crypt32)
 papacc_enable_warnings(papacc_v3_backing_test)
 papacc_stage_pst_runtime(papacc_v3_backing_test)
 add_test(NAME papacc.v3.immutable_backings COMMAND papacc_v3_backing_test "${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/security/ca.der.b64")
 add_executable(papacc_v4_backing_test tests/v4_backing_win32_test.c apps/server/server_config_root8_data_win32.c)
 target_compile_definitions(papacc_v4_backing_test PRIVATE PAPACC_OFFLINE_D3_WORKER=1 PAPACC_V3_BACKING_FIXTURE=1 PAPACC_V4_BACKING_FIXTURE=1)
 target_include_directories(papacc_v4_backing_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} src/config src/issuer apps/server)
 target_link_libraries(papacc_v4_backing_test PRIVATE papacc_config_root8 papacc_server_run_win32 papacc_config_binding papacc_issuer crypt32)
 papacc_enable_warnings(papacc_v4_backing_test)
 papacc_stage_pst_runtime(papacc_v4_backing_test)
 add_test(NAME papacc.v4.immutable_backings COMMAND papacc_v4_backing_test "${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/security/ca.der.b64")
 set_property(SOURCE apps/server/server_configuration_commit_win32.c APPEND PROPERTY OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_configuration_v3_backings_win32.inc")
endif()

# Canonical kind8 DATA dispatch under existing protected v3 borrows only.
if(PAPACC_ENABLE_OFFLINE_D3_WORKER AND PAPACC_ENABLE_WINDOWS_3MIB_MANAGED_PROFILE)
 foreach(t papacc_configuration_commit papacc_issuer_root_store_test papacc_d3_worker_test papacc_bootstrap_d3_commit_test papacc_bootstrap_namespace_test papacc_config_root8_v3_test papacc_config_root8_v4_test papacc_v3_backing_test papacc_v4_backing_test)
  if(TARGET ${t})
   target_link_libraries(${t} PRIVATE papacc_root8_signer_data crypt32)
  endif()
 endforeach()
 foreach(mode model native)
  add_executable(papacc_kind8_dispatch_${mode}_test tests/kind8_dispatch_win32_test.c apps/server/server_config_root8_data_win32.c)
  target_compile_definitions(papacc_kind8_dispatch_${mode}_test PRIVATE PAPACC_OFFLINE_D3_WORKER=1 PAPACC_V3_BACKING_FIXTURE=1 PAPACC_KIND8_DISPATCH_FIXTURE=1)
  if(mode STREQUAL "model")
   target_compile_definitions(papacc_kind8_dispatch_${mode}_test PRIVATE PAPACC_DISPATCH_MODEL=1)
  endif()
  target_include_directories(papacc_kind8_dispatch_${mode}_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} src/config src/issuer apps/server)
  target_link_libraries(papacc_kind8_dispatch_${mode}_test PRIVATE papacc_config_root8 papacc_server_run_win32 papacc_config_binding papacc_issuer papacc_root8_signer_data crypt32)
  papacc_enable_warnings(papacc_kind8_dispatch_${mode}_test)
  papacc_stage_pst_runtime(papacc_kind8_dispatch_${mode}_test)
  add_test(NAME papacc.kind8.dispatch_${mode} COMMAND papacc_kind8_dispatch_${mode}_test "${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/security/ca.der.b64")
 endforeach()
endif()

if(PAPACC_ENABLE_OFFLINE_D3_WORKER AND PAPACC_ENABLE_WINDOWS_3MIB_MANAGED_PROFILE)
 add_executable(papacc_v3_current_negative_test tests/v3_current_negative_win32_test.c apps/server/server_config_root8_data_win32.c src/provisioning/shared_structure/trust_unavailable.c)
 target_compile_definitions(papacc_v3_current_negative_test PRIVATE PAPACC_OFFLINE_D3_WORKER=1 PAPACC_V3_BACKING_FIXTURE=1 PAPACC_KIND8_DISPATCH_FIXTURE=1)
 target_include_directories(papacc_v3_current_negative_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} src/config src/issuer src/provisioning/shared_structure apps/server)
 target_link_libraries(papacc_v3_current_negative_test PRIVATE papacc_config_root8 papacc_server_run_win32 papacc_config_binding papacc_issuer papacc_root8_signer_data crypt32)
 papacc_enable_warnings(papacc_v3_current_negative_test)
 papacc_stage_pst_runtime(papacc_v3_current_negative_test)
 add_test(NAME papacc.v3.current_negative COMMAND papacc_v3_current_negative_test "${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/security/ca.der.b64")
endif()

if(PAPACC_ENABLE_OFFLINE_D3_WORKER AND PAPACC_ENABLE_WINDOWS_3MIB_MANAGED_PROFILE)
 add_executable(papacc_v3_transaction_worker_test tests/v3_transaction_worker_win32_test.c apps/server/server_configuration_commit_win32.c apps/server/server_config_root8_data_win32.c)
 target_compile_definitions(papacc_v3_transaction_worker_test PRIVATE PAPACC_V3_BACKING_FIXTURE=1 PAPACC_OFFLINE_D3_WORKER=1 PAPACC_ISSUER_STORAGE_COMPONENT_TEST=1)
 target_include_directories(papacc_v3_transaction_worker_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} src/config src/issuer apps/server)
 target_link_libraries(papacc_v3_transaction_worker_test PRIVATE papacc_config_root8 papacc_server_config papacc_issuer papacc_root8_signer_data bcrypt ktmw32 crypt32 advapi32)
 target_compile_options(papacc_v3_transaction_worker_test PRIVATE /W4 /WX)
endif()

if(PAPACC_ENABLE_OFFLINE_D3_WORKER AND PAPACC_ENABLE_WINDOWS_3MIB_MANAGED_PROFILE)
 add_executable(papacc_v3_update_cohort_test tests/v3_update_cohort_win32_test.c apps/server/server_config_root8_data_win32.c)
 target_compile_definitions(papacc_v3_update_cohort_test PRIVATE PAPACC_V3_BACKING_FIXTURE=1 PAPACC_KIND8_DISPATCH_FIXTURE=1 PAPACC_OFFLINE_D3_WORKER=1)
 target_include_directories(papacc_v3_update_cohort_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} src/config src/issuer apps/server)
 target_link_libraries(papacc_v3_update_cohort_test PRIVATE papacc_config_root8 papacc_server_run_win32 papacc_config_binding papacc_issuer papacc_root8_signer_data crypt32)
 papacc_enable_warnings(papacc_v3_update_cohort_test)
 papacc_stage_pst_runtime(papacc_v3_update_cohort_test)
 add_test(NAME papacc.v3.update_cohort COMMAND papacc_v3_update_cohort_test "${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/security/ca.der.b64")
 set_property(SOURCE apps/server/server_configuration_commit_win32.c APPEND PROPERTY OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_configuration_v3_update_cohort_win32.inc")
endif()

if(TARGET papacc_v3_update_cohort_test)
 set_property(SOURCE apps/server/server_configuration_commit_win32.c APPEND PROPERTY OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_configuration_v3_update_act_win32.inc")
endif()

if(WIN32 AND BUILD_TESTING)
 add_executable(papacc_store_hash_lifetime_test tests/store_hash_lifetime_win32_test.c)
 target_include_directories(papacc_store_hash_lifetime_test PRIVATE apps/server)
 target_link_libraries(papacc_store_hash_lifetime_test PRIVATE bcrypt)
 target_compile_options(papacc_store_hash_lifetime_test PRIVATE /W4 /WX)
 set_target_properties(papacc_store_hash_lifetime_test PROPERTIES C_STANDARD 90 C_EXTENSIONS NO)
 add_test(NAME papacc.store_hash.lifetime COMMAND papacc_store_hash_lifetime_test)
 set_property(SOURCE apps/server/server_configuration_commit_win32.c APPEND PROPERTY OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_store_hash_win32.inc")
endif()

if(TARGET papacc_configuration_commit AND BUILD_TESTING AND PAPACC_ENABLE_OFFLINE_D3_WORKER)
 add_executable(papacc_store_hash_owner_test tests/store_hash_owner_win32_test.c)
 target_link_libraries(papacc_store_hash_owner_test PRIVATE papacc_configuration_commit)
 target_compile_options(papacc_store_hash_owner_test PRIVATE /W4 /WX)
 add_test(NAME papacc.store_hash.owner COMMAND papacc_store_hash_owner_test)
endif()

# Same qualified Accelerator-owned capsule, private storage-only build binding.
if(WIN32 AND TARGET papacc_configuration_commit)
 set(PACC_STORAGE_CAPSULE "${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/storage-hash-capsule")
 add_library(papacc_storage_sha256 STATIC
  ${PACC_STORAGE_CAPSULE}/storage_sha256.c ${PACC_STORAGE_CAPSULE}/memory_ops.c
  ${PACC_STORAGE_CAPSULE}/vendor/bearssl-0.6/src/hash/sha2small.c
  ${PACC_STORAGE_CAPSULE}/vendor/bearssl-0.6/src/codec/dec32be.c
  ${PACC_STORAGE_CAPSULE}/vendor/bearssl-0.6/src/codec/enc32be.c)
 target_include_directories(papacc_storage_sha256 PUBLIC ${PACC_STORAGE_CAPSULE}/vendor/bearssl-0.6/inc
  PRIVATE ${PACC_STORAGE_CAPSULE} ${PACC_STORAGE_CAPSULE}/adapter_include)
 target_compile_options(papacc_storage_sha256 PRIVATE /W4 /WX /Oi- /FI${PACC_STORAGE_CAPSULE}/memory_bind.h)
 set_target_properties(papacc_storage_sha256 PROPERTIES C_STANDARD 90 C_EXTENSIONS NO)
 get_property(pacc_storage_targets DIRECTORY PROPERTY BUILDSYSTEM_TARGETS)
 foreach(pacc_storage_target IN LISTS pacc_storage_targets)
  get_target_property(pacc_storage_sources ${pacc_storage_target} SOURCES)
  if("${pacc_storage_sources}" MATCHES "server_configuration_commit_win32[.]c")
   target_link_libraries(${pacc_storage_target} PRIVATE papacc_storage_sha256)
  endif()
 endforeach()
 set_property(SOURCE apps/server/server_configuration_commit_win32.c APPEND PROPERTY OBJECT_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_store_data_hash_win32.inc")
endif()

if(TARGET papacc_configuration_commit)
 set_property(SOURCE apps/server/server_configuration_commit_win32.c APPEND PROPERTY OBJECT_DEPENDS
 "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_configuration_prepared_storage_win32.inc")
endif()

if(PAPACC_ENABLE_OFFLINE_D3_WORKER AND PAPACC_ENABLE_WINDOWS_3MIB_MANAGED_PROFILE)
 foreach(mode model native)
  add_executable(papacc_prepared_storage_${mode}_test tests/prepared_storage_win32_test.c apps/server/server_config_root8_data_win32.c)
  target_compile_definitions(papacc_prepared_storage_${mode}_test PRIVATE PAPACC_V3_BACKING_FIXTURE=1 PAPACC_V4_BACKING_FIXTURE=1 PAPACC_OFFLINE_D3_WORKER=1)
  if(mode STREQUAL "model")
   target_compile_definitions(papacc_prepared_storage_${mode}_test PRIVATE PAPACC_DISPATCH_MODEL=1)
  endif()
  target_include_directories(papacc_prepared_storage_${mode}_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} src/config src/issuer apps/server)
  target_link_libraries(papacc_prepared_storage_${mode}_test PRIVATE papacc_config_root8 papacc_configuration_commit papacc_root8_signer_data crypt32)
  target_compile_options(papacc_prepared_storage_${mode}_test PRIVATE /W4 /WX)
  add_test(NAME papacc.prepared_storage.${mode} COMMAND papacc_prepared_storage_${mode}_test "${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/security/ca.der.b64")
 endforeach()
endif()

# Isolated v4 packet11 qualification; same canonical native worker, no authority.
if(PAPACC_ENABLE_OFFLINE_D3_WORKER AND PAPACC_ENABLE_WINDOWS_3MIB_MANAGED_PROFILE)
 add_executable(papacc_v4_transaction_worker_test tests/v4_transaction_worker_win32_test.c
  apps/server/server_configuration_commit_win32.c apps/server/server_config_root8_data_win32.c)
 target_compile_definitions(papacc_v4_transaction_worker_test PRIVATE PAPACC_V3_BACKING_FIXTURE=1
  PAPACC_V4_BACKING_FIXTURE=1 PAPACC_OFFLINE_D3_WORKER=1 PAPACC_ISSUER_STORAGE_COMPONENT_TEST=1)
 target_include_directories(papacc_v4_transaction_worker_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} src/config src/issuer apps/server)
 target_link_libraries(papacc_v4_transaction_worker_test PRIVATE papacc_config_root8 papacc_server_config
  papacc_issuer papacc_root8_signer_data papacc_storage_sha256 bcrypt ktmw32 crypt32 advapi32)
 target_compile_options(papacc_v4_transaction_worker_test PRIVATE /W4 /WX)
endif()

# Compile the exact current bootstrap function body with modelled dependencies.
# Extraction is test-only: no copied production implementation or runtime hook.
if(TARGET papacc_config_binding_native_test AND PAPACC_ENABLE_OFFLINE_D3_WORKER)
 set(pacc_bootstrap_source "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_config_source_binding_win32.c")
 set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${pacc_bootstrap_source}")
 file(READ "${pacc_bootstrap_source}" pacc_bootstrap_text)
 string(FIND "${pacc_bootstrap_text}" "PAPACC_RESULT papacc_server_config_initial_bootstrap_commit(" pacc_bootstrap_start)
 if(pacc_bootstrap_start LESS 0)
  message(FATAL_ERROR "Bootstrap scratch qualification: function absent")
 endif()
 string(SUBSTRING "${pacc_bootstrap_text}" ${pacc_bootstrap_start} -1 pacc_bootstrap_tail)
 string(FIND "${pacc_bootstrap_tail}" "\n#endif" pacc_bootstrap_end)
 if(pacc_bootstrap_end LESS 0)
  message(FATAL_ERROR "Bootstrap scratch qualification: function boundary absent")
 endif()
 string(SUBSTRING "${pacc_bootstrap_tail}" 0 ${pacc_bootstrap_end} pacc_bootstrap_body)
 file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/bootstrap_scratch_commit.inc" "${pacc_bootstrap_body}\n")
 add_executable(papacc_bootstrap_scratch_test tests/bootstrap_scratch_win32_test.c)
 target_include_directories(papacc_bootstrap_scratch_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} src/config src/issuer apps/server "${CMAKE_CURRENT_BINARY_DIR}")
 target_compile_definitions(papacc_bootstrap_scratch_test PRIVATE PAPACC_OFFLINE_D3_WORKER=1)
 target_link_libraries(papacc_bootstrap_scratch_test PRIVATE papacc_server_run_win32 papacc_config_root8 papacc_config_binding)
 target_compile_options(papacc_bootstrap_scratch_test PRIVATE /W4 /WX)
 papacc_stage_pst_runtime(papacc_bootstrap_scratch_test)
 add_test(NAME papacc.bootstrap.scratch COMMAND papacc_bootstrap_scratch_test)
endif()

# Personal ceremony independent DATA in the existing owner, no native effects.
if(TARGET papacc_configuration_commit AND PAPACC_ENABLE_OFFLINE_D3_WORKER)
 target_sources(papacc_configuration_commit PRIVATE apps/server/personal_plan_data.c apps/server/server_ca_key_win32.c)
 # This component fixture compiles commit directly rather than borrowing the
 # configuration archive; it must also link the same personal DATA helpers.
 target_sources(papacc_issuer_root_store_test PRIVATE apps/server/personal_plan_data.c apps/server/server_ca_key_win32.c)
 target_link_libraries(papacc_issuer_root_store_test PRIVATE ncrypt advapi32 shell32 ole32 uuid)
 target_link_libraries(papacc_configuration_commit PRIVATE ncrypt advapi32 shell32 ole32 uuid)
 target_link_libraries(papacc_configuration_commit PUBLIC papacc_server_console_win32)
 foreach(scq_console_target papacc_issuer_root_store_test papacc_d3_worker_test papacc_bootstrap_d3_commit_test papacc_bootstrap_namespace_test papacc_config_root8_v3_test papacc_config_root8_v4_test papacc_v3_backing_test papacc_v4_backing_test)
  if(TARGET ${scq_console_target})
   target_link_libraries(${scq_console_target} PRIVATE papacc_server_console_win32)
  endif()
 endforeach()

 target_sources(papacc_configuration_commit PRIVATE apps/server/server_configuration_personal_custody_data_win32.c)
 set_property(SOURCE apps/server/server_configuration_commit_win32.c APPEND PROPERTY OBJECT_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_configuration_personal_data_win32.inc;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/personal_plan_data.h")
 if(BUILD_TESTING)
  add_executable(papacc_personal_plan_data_test tests/personal_plan_data_win32_test.c)
  target_include_directories(papacc_personal_plan_data_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} apps/server src/issuer)
  target_compile_definitions(papacc_personal_plan_data_test PRIVATE PAPACC_OFFLINE_D3_WORKER=1)
  target_link_libraries(papacc_personal_plan_data_test PRIVATE papacc_configuration_commit)
  target_compile_options(papacc_personal_plan_data_test PRIVATE /W4 /WX)
  add_test(NAME papacc.personal_plan.data COMMAND papacc_personal_plan_data_test)
 endif()
endif()

if(TARGET papacc_personal_plan_data_test)
 set_property(SOURCE apps/server/server_configuration_commit_win32.c APPEND PROPERTY OBJECT_DEPENDS
 "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/personal_native_win32.h;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_configuration_personal_native_win32.inc;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_configuration_personal_ca_win32.inc")
 set_property(SOURCE apps/server/server_launcher_owner_win32.c APPEND PROPERTY OBJECT_DEPENDS
 "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/personal_native_win32.h;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_launcher_personal_win32.inc")
 add_executable(papacc_personal_native_test tests/personal_native_win32_test.c)
 target_include_directories(papacc_personal_native_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} apps/server src/issuer)
 target_compile_definitions(papacc_personal_native_test PRIVATE PAPACC_OFFLINE_D3_WORKER=1)
 target_link_libraries(papacc_personal_native_test PRIVATE papacc_configuration_commit)
 target_compile_options(papacc_personal_native_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal_native.readonly COMMAND papacc_personal_native_test)
 add_executable(papacc_personal_token_predicate_test tests/personal_token_predicate_test.c)
 target_include_directories(papacc_personal_token_predicate_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} apps/server)
 target_link_libraries(papacc_personal_token_predicate_test PRIVATE papacc_configuration_commit advapi32)
 target_compile_options(papacc_personal_token_predicate_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal.token_predicates COMMAND papacc_personal_token_predicate_test)
 set_property(SOURCE tests/personal_token_predicate_test.c apps/server/server_configuration_commit_win32.c APPEND PROPERTY OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_personal_token_checks_win32.inc")
 add_executable(papacc_personal_presenter_test tests/personal_presenter_win32_test.c)
 target_include_directories(papacc_personal_presenter_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} apps/server)
 target_link_libraries(papacc_personal_presenter_test PRIVATE papacc_configuration_commit papacc_server_run_win32 bcrypt advapi32)
 target_compile_options(papacc_personal_presenter_test PRIVATE /W4 /WX)
 set_property(SOURCE tests/personal_presenter_win32_test.c APPEND PROPERTY OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_launcher_personal_win32.inc")
 add_test(NAME papacc.personal.presenter_isolated COMMAND papacc_personal_presenter_test "${CMAKE_CURRENT_SOURCE_DIR}/tests/fixtures/presenter-settings.conf")
endif()

# Explicit isolated target effects; NEVER registered as an automatic CTest.
if(TARGET papacc_configuration_commit AND PAPACC_ENABLE_OFFLINE_D3_WORKER AND BUILD_TESTING)
 add_executable(papacc_personal_target_component_test tests/personal_target_component_win32_test.c
  apps/server/server_ca_key_win32.c
  apps/server/server_configuration_commit_win32.c apps/server/server_config_root8_data_win32.c
  apps/server/personal_plan_data.c apps/server/server_configuration_personal_custody_data_win32.c)
 target_include_directories(papacc_personal_target_component_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} apps/server src/config src/issuer)
 target_compile_definitions(papacc_personal_target_component_test PRIVATE PAPACC_OFFLINE_D3_WORKER=1 PAPACC_ISSUER_STORAGE_COMPONENT_TEST=1 PAPACC_BOOTSTRAP_NATIVE_COMPONENT_TEST=1)
 target_link_libraries(papacc_personal_target_component_test PRIVATE papacc_config_root8 papacc_server_config papacc_issuer papacc_storage_sha256 bcrypt ktmw32 advapi32 ncrypt)
 if(PAPACC_ENABLE_WINDOWS_3MIB_MANAGED_PROFILE)
  target_link_libraries(papacc_personal_target_component_test PRIVATE papacc_root8_signer_data crypt32)
 endif()
 target_compile_options(papacc_personal_target_component_test PRIVATE /W4 /WX)
endif()

if(TARGET papacc_personal_plan_data_test)
 add_executable(papacc_personal_effect_plan_test tests/personal_effect_plan_data_test.c)
 target_include_directories(papacc_personal_effect_plan_test PRIVATE apps/server)
 target_link_libraries(papacc_personal_effect_plan_test PRIVATE papacc_configuration_commit)
 target_compile_options(papacc_personal_effect_plan_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal_effect_plan.data COMMAND papacc_personal_effect_plan_test)
 set_property(SOURCE apps/server/personal_plan_data.c APPEND PROPERTY OBJECT_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/personal_effect_plan_data.inc")
endif()

# Same-owner CA borrow association; no automatic native key mutation tests.
if(TARGET papacc_issuer_cng_windows AND PAPACC_ENABLE_OFFLINE_D3_WORKER)
 target_link_libraries(papacc_server_run_win32 PUBLIC papacc_issuer_cng_windows)
 target_compile_definitions(papacc_issuer_cng_windows PRIVATE PAPACC_OFFLINE_D3_WORKER=1)
 target_include_directories(papacc_issuer_cng_windows PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS})
 set_property(SOURCE apps/server/server_issuer_cng_backend_win32.c APPEND PROPERTY OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_configuration_commit_win32.h")
 if(TARGET papacc_issuer_cng_x509_test)
  target_link_libraries(papacc_issuer_cng_x509_test PRIVATE papacc_configuration_commit papacc_server_run_win32)
  add_test(NAME papacc.issuer.native_ephemeral_composition COMMAND papacc_issuer_cng_x509_test unused unused --native-ephemeral)
  target_include_directories(papacc_issuer_cng_x509_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS})
  set_property(SOURCE tests/issuer_cng_x509_test.c APPEND PROPERTY OBJECT_DEPENDS
   "${CMAKE_CURRENT_SOURCE_DIR}/tests/issuer_native_ephemeral_cases.inc;${CMAKE_CURRENT_SOURCE_DIR}/tests/issuer_normal_source_composition_cases.inc;${CMAKE_CURRENT_SOURCE_DIR}/tests/issuer_first_existing_ca_cases.inc;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_configuration_personal_ca_win32.inc")
 set_property(SOURCE apps/server/server_launcher_owner_win32.c APPEND PROPERTY OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_issuer_x509_win32.h")
 endif()
endif()
if(TARGET papacc_personal_plan_data_test)
 add_executable(papacc_ca_key_lifetime_test tests/ca_key_offline_test.c apps/server/server_ca_key_win32.c)
 target_include_directories(papacc_ca_key_lifetime_test PRIVATE apps/server include)
 target_link_libraries(papacc_ca_key_lifetime_test PRIVATE ncrypt advapi32 psapi)
 target_compile_options(papacc_ca_key_lifetime_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal_ca.lifetime COMMAND papacc_ca_key_lifetime_test)
 add_executable(papacc_personal_runtime_identity_test tests/personal_runtime_identity_test.c)
 target_include_directories(papacc_personal_runtime_identity_test PRIVATE apps/server include)
 target_link_libraries(papacc_personal_runtime_identity_test PRIVATE ncrypt advapi32)
 target_compile_options(papacc_personal_runtime_identity_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal.runtime_identity COMMAND papacc_personal_runtime_identity_test)
 add_executable(papacc_scq_machine_fixture_test tests/scq_machine_fixture_test.c)
 target_include_directories(papacc_scq_machine_fixture_test PRIVATE apps/server include)
 target_link_libraries(papacc_scq_machine_fixture_test PRIVATE ncrypt advapi32 psapi)
 target_compile_options(papacc_scq_machine_fixture_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal.scq_machine_fixture COMMAND papacc_scq_machine_fixture_test)
 add_executable(papacc_machine_fixture_source_test tests/machine_fixture_source_test.c)
 target_include_directories(papacc_machine_fixture_source_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} apps/server include src/config src/issuer)
 target_link_libraries(papacc_machine_fixture_source_test PRIVATE papacc_storage_sha256 ncrypt advapi32 psapi)
 target_compile_options(papacc_machine_fixture_source_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal.machine_fixture_source COMMAND papacc_machine_fixture_source_test)
 add_test(NAME papacc.personal.machine_fixture_source_observed_sd COMMAND papacc_machine_fixture_source_test observed-sd)
 add_executable(papacc_scq_machine_read_chain_test tests/scq_machine_read_chain_test.c tests/scq_machine_read_ca_model.c)
 target_include_directories(papacc_scq_machine_read_chain_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} apps/server include src/config src/issuer)
 target_link_libraries(papacc_scq_machine_read_chain_test PRIVATE papacc_storage_sha256 papacc_server_service ncrypt advapi32 psapi bcrypt)
 target_compile_options(papacc_scq_machine_read_chain_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal.scq_machine_read_chain COMMAND papacc_scq_machine_read_chain_test)
 add_test(NAME papacc.personal.scq_machine_read_chain_observed_sd COMMAND papacc_scq_machine_read_chain_test observed-sd)

 add_executable(papacc_scq_read_runtime_test tests/scq_read_runtime_test.c)
 target_include_directories(papacc_scq_read_runtime_test PRIVATE apps/server include)
 target_link_libraries(papacc_scq_read_runtime_test PRIVATE ncrypt advapi32)
 target_compile_options(papacc_scq_read_runtime_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal.scq_read_runtime COMMAND papacc_scq_read_runtime_test)
 add_executable(papacc_scq_read_diagnostic_test tests/scq_read_diagnostic_test.c)
 target_include_directories(papacc_scq_read_diagnostic_test PRIVATE apps/server include)
 target_link_libraries(papacc_scq_read_diagnostic_test PRIVATE ncrypt advapi32)
 target_compile_options(papacc_scq_read_diagnostic_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal.scq_read_diagnostic COMMAND papacc_scq_read_diagnostic_test)
 add_executable(papacc_personal_cng_probe_test tests/personal_cng_probe_test.c)
 target_include_directories(papacc_personal_cng_probe_test PRIVATE apps/server include)
 target_link_libraries(papacc_personal_cng_probe_test PRIVATE ncrypt advapi32)
 target_compile_options(papacc_personal_cng_probe_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal.cng_probe COMMAND papacc_personal_cng_probe_test)
 add_executable(papacc_personal_paths_test tests/personal_install_paths_test.c)
 target_include_directories(papacc_personal_paths_test PRIVATE apps/server include)
 target_link_libraries(papacc_personal_paths_test PRIVATE shell32 ole32 uuid)
 target_compile_options(papacc_personal_paths_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal.install_paths COMMAND papacc_personal_paths_test)
 add_executable(papacc_personal_scq_dispatch_test tests/personal_scq_dispatch_test.c)
 target_include_directories(papacc_personal_scq_dispatch_test PRIVATE apps/server include)
 target_link_libraries(papacc_personal_scq_dispatch_test PRIVATE ncrypt advapi32)
 target_compile_options(papacc_personal_scq_dispatch_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal.scq_dispatch COMMAND papacc_personal_scq_dispatch_test)
 add_executable(papacc_personal_scq_inputs_test tests/personal_scq_inputs_test.c)
 target_include_directories(papacc_personal_scq_inputs_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} apps/server include)
 target_link_libraries(papacc_personal_scq_inputs_test PRIVATE advapi32 bcrypt)
 target_compile_options(papacc_personal_scq_inputs_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal.scq_inputs_metadata COMMAND papacc_personal_scq_inputs_test)
 add_test(NAME papacc.personal.scq_staged_owner_boundary COMMAND papacc_personal_scq_inputs_test --staged-owner-only)
 add_test(NAME papacc.personal.scq_to_image_owner COMMAND papacc_personal_scq_inputs_test --transport-owner)
 add_test(NAME papacc.personal.scq_existing_target COMMAND papacc_personal_scq_inputs_test --existing-target)
 add_executable(papacc_scq_p1_entry_test tests/scq_p1_entry_test.c)
 target_include_directories(papacc_scq_p1_entry_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} apps/server include)
 target_compile_definitions(papacc_scq_p1_entry_test PRIVATE PAPACC_C8_PRIVATE_STORAGE_PROFILE=1 PAPACC_OFFLINE_D3_WORKER=1)
 target_compile_options(papacc_scq_p1_entry_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal.scq_p1_entry COMMAND papacc_scq_p1_entry_test)
 add_executable(papacc_scq_transport_scope_test tests/scq_transport_scope_test.c)
 target_link_libraries(papacc_scq_transport_scope_test PRIVATE advapi32 bcrypt)
 target_compile_options(papacc_scq_transport_scope_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal.scq_transport_scope COMMAND papacc_scq_transport_scope_test)
 add_executable(papacc_scq_transport_continuation_test tests/scq_transport_continuation_test.c)
 target_include_directories(papacc_scq_transport_continuation_test PRIVATE apps/server include)
 target_link_libraries(papacc_scq_transport_continuation_test PRIVATE ncrypt advapi32 bcrypt)
 target_compile_options(papacc_scq_transport_continuation_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal.scq_transport_continuation COMMAND papacc_scq_transport_continuation_test)
 add_executable(papacc_scq_pre_readiness_chain_test tests/scq_pre_readiness_chain_test.c)
 target_include_directories(papacc_scq_pre_readiness_chain_test PRIVATE apps/server include)
 target_link_libraries(papacc_scq_pre_readiness_chain_test PRIVATE papacc_server_service ncrypt advapi32 bcrypt)
 target_compile_options(papacc_scq_pre_readiness_chain_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal.scq_pre_readiness_chain COMMAND papacc_scq_pre_readiness_chain_test)
 add_executable(papacc_scq_native_first_ingress_boundary_test tests/scq_native_first_ingress_boundary_test.c)
 target_include_directories(papacc_scq_native_first_ingress_boundary_test PRIVATE apps/server include)
 target_link_libraries(papacc_scq_native_first_ingress_boundary_test PRIVATE papacc_server_service ncrypt advapi32 bcrypt)
 target_compile_options(papacc_scq_native_first_ingress_boundary_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal.scq_native_first_ingress_boundary COMMAND papacc_scq_native_first_ingress_boundary_test)
 add_executable(papacc_scq_bootstrap_carrier_test tests/scq_bootstrap_carrier_test.c)
 target_include_directories(papacc_scq_bootstrap_carrier_test PRIVATE apps/server include)
 target_link_libraries(papacc_scq_bootstrap_carrier_test PRIVATE papacc_server_service ncrypt advapi32 bcrypt)
 target_compile_options(papacc_scq_bootstrap_carrier_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal.scq_bootstrap_carrier COMMAND papacc_scq_bootstrap_carrier_test)
 add_executable(papacc_scq_start_selection_test tests/scq_start_selection_test.c)
 target_include_directories(papacc_scq_start_selection_test PRIVATE apps/server include)
 target_link_libraries(papacc_scq_start_selection_test PRIVATE papacc_server_service ncrypt advapi32 bcrypt)
 target_compile_options(papacc_scq_start_selection_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal.scq_start_selection COMMAND papacc_scq_start_selection_test)
 add_executable(papacc_scq_query_cold_chain_test tests/scq_query_cold_chain_test.c)
 target_include_directories(papacc_scq_query_cold_chain_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} apps/server include src/config src/issuer)
 target_compile_definitions(papacc_scq_query_cold_chain_test PRIVATE PAPACC_C8_PRIVATE_STORAGE_PROFILE=1)
 target_link_libraries(papacc_scq_query_cold_chain_test PRIVATE papacc_server_service ncrypt advapi32 bcrypt)
 target_compile_options(papacc_scq_query_cold_chain_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal.scq_query_cold_chain COMMAND papacc_scq_query_cold_chain_test)
 add_executable(papacc_scq_failure_ingress_test tests/scq_failure_ingress_test.c)
 target_include_directories(papacc_scq_failure_ingress_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} apps/server include src/config src/issuer)
 target_compile_definitions(papacc_scq_failure_ingress_test PRIVATE PAPACC_C8_PRIVATE_STORAGE_PROFILE=1)
 target_link_libraries(papacc_scq_failure_ingress_test PRIVATE papacc_server_service ncrypt advapi32 bcrypt)
 target_compile_options(papacc_scq_failure_ingress_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal.scq_failure_ingress COMMAND papacc_scq_failure_ingress_test)

 add_executable(papacc_scq_cold_open_test tests/scq_cold_open_test.c)
 target_include_directories(papacc_scq_cold_open_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} apps/server include src/config src/issuer)
 target_compile_definitions(papacc_scq_cold_open_test PRIVATE PAPACC_C8_PRIVATE_STORAGE_PROFILE=1)
 target_link_libraries(papacc_scq_cold_open_test PRIVATE papacc_server_service ncrypt advapi32 bcrypt)
 target_compile_options(papacc_scq_cold_open_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal.scq_cold_open COMMAND papacc_scq_cold_open_test)
 add_executable(papacc_scq_native_sd_test tests/scq_native_sd_test.c)
 target_include_directories(papacc_scq_native_sd_test PRIVATE apps/server)
 target_link_libraries(papacc_scq_native_sd_test PRIVATE advapi32)
 target_compile_options(papacc_scq_native_sd_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal.scq_native_sd COMMAND papacc_scq_native_sd_test)
 add_executable(papacc_scq_transport_continue_wrapper_test tests/scq_transport_continue_wrapper_test.c)
 target_include_directories(papacc_scq_transport_continue_wrapper_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} apps/server include src/config src/issuer)
 target_compile_definitions(papacc_scq_transport_continue_wrapper_test PRIVATE PAPACC_C8_PRIVATE_STORAGE_PROFILE=1)
 target_compile_options(papacc_scq_transport_continue_wrapper_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal.scq_transport_continue_wrapper COMMAND papacc_scq_transport_continue_wrapper_test)
 add_executable(papacc_scq_offer_candidate_test tests/scq_offer_candidate_test.c)
 target_include_directories(papacc_scq_offer_candidate_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} apps/server include src/config src/issuer)
 target_compile_definitions(papacc_scq_offer_candidate_test PRIVATE PAPACC_C8_PRIVATE_STORAGE_PROFILE=1)
 target_compile_options(papacc_scq_offer_candidate_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal.scq_offer_candidate COMMAND papacc_scq_offer_candidate_test)
 add_executable(papacc_scq_originals_candidate_test tests/scq_originals_candidate_test.c)
 target_include_directories(papacc_scq_originals_candidate_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} apps/server include src/config src/issuer)
 target_compile_definitions(papacc_scq_originals_candidate_test PRIVATE PAPACC_C8_PRIVATE_STORAGE_PROFILE=1)
 target_compile_options(papacc_scq_originals_candidate_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal.scq_originals_candidate COMMAND papacc_scq_originals_candidate_test)
 add_executable(papacc_scq_cold_controller_test tests/scq_cold_controller_test.c)
 target_include_directories(papacc_scq_cold_controller_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} apps/server include src/config src/issuer)
 target_compile_definitions(papacc_scq_cold_controller_test PRIVATE PAPACC_C8_PRIVATE_STORAGE_PROFILE=1)
 target_link_libraries(papacc_scq_cold_controller_test PRIVATE papacc_server_service ncrypt advapi32 bcrypt)
 target_compile_options(papacc_scq_cold_controller_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal.scq_cold_controller COMMAND papacc_scq_cold_controller_test)
 add_executable(papacc_scq_start_assurance_test tests/scq_start_assurance_test.c)
 target_include_directories(papacc_scq_start_assurance_test PRIVATE apps/server include)
 target_link_libraries(papacc_scq_start_assurance_test PRIVATE papacc_server_service ncrypt advapi32 bcrypt)
 target_compile_options(papacc_scq_start_assurance_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal.scq_start_assurance COMMAND papacc_scq_start_assurance_test)
 add_executable(papacc_scq_endpoint_act_test tests/scq_endpoint_act_test.c)
 target_include_directories(papacc_scq_endpoint_act_test PRIVATE apps/server include)
 target_link_libraries(papacc_scq_endpoint_act_test PRIVATE papacc_server_service ncrypt advapi32 bcrypt)
 target_compile_options(papacc_scq_endpoint_act_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal.scq_endpoint_act COMMAND papacc_scq_endpoint_act_test)
 add_executable(papacc_scq_first_controller_test tests/scq_first_controller_test.c)
 target_include_directories(papacc_scq_first_controller_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} apps/server include src/config src/issuer)
 target_compile_definitions(papacc_scq_first_controller_test PRIVATE PAPACC_C8_PRIVATE_STORAGE_PROFILE=1)
 target_link_libraries(papacc_scq_first_controller_test PRIVATE papacc_server_service ncrypt advapi32 bcrypt)
 target_compile_options(papacc_scq_first_controller_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal.scq_first_controller COMMAND papacc_scq_first_controller_test)
 add_executable(papacc_scq_native_selection_capture_test tests/scq_native_selection_capture_test.c)
 target_include_directories(papacc_scq_native_selection_capture_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} apps/server include src/config src/issuer)
 target_compile_definitions(papacc_scq_native_selection_capture_test PRIVATE PAPACC_C8_PRIVATE_STORAGE_PROFILE=1)
 target_link_libraries(papacc_scq_native_selection_capture_test PRIVATE papacc_server_service ncrypt advapi32 bcrypt)
 target_compile_options(papacc_scq_native_selection_capture_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal.scq_native_selection_capture COMMAND papacc_scq_native_selection_capture_test)
 add_executable(papacc_scq_bootstrap_capture_test tests/scq_bootstrap_capture_test.c)
 target_include_directories(papacc_scq_bootstrap_capture_test PRIVATE apps/server include)
 target_link_libraries(papacc_scq_bootstrap_capture_test PRIVATE papacc_server_service ncrypt advapi32 bcrypt)
 target_compile_options(papacc_scq_bootstrap_capture_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal.scq_bootstrap_capture COMMAND papacc_scq_bootstrap_capture_test)
 add_executable(papacc_console_original_wait_model_test tests/console_original_wait_model_test.c)
 target_include_directories(papacc_console_original_wait_model_test PRIVATE apps/server include)
 target_compile_options(papacc_console_original_wait_model_test PRIVATE /W4 /WX)
 add_test(NAME papacc.service.console_original_wait_model COMMAND papacc_console_original_wait_model_test)
 add_executable(papacc_scq_recipient_close_test tests/scq_recipient_close_test.c)
 target_include_directories(papacc_scq_recipient_close_test PRIVATE apps/server include)
 target_link_libraries(papacc_scq_recipient_close_test PRIVATE ncrypt advapi32 bcrypt)
 target_compile_options(papacc_scq_recipient_close_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal.scq_recipient_close COMMAND papacc_scq_recipient_close_test)
 set_property(SOURCE apps/server/server_configuration_commit_win32.c tests/scq_recipient_close_test.c APPEND PROPERTY OBJECT_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/personal_scq_recipient_win32.h;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/personal_scq_recipient_win32.inc")
 add_executable(papacc_scq_native_adapter_test tests/scq_native_adapter_test.c)
 target_include_directories(papacc_scq_native_adapter_test PRIVATE apps/server include)
 target_link_libraries(papacc_scq_native_adapter_test PRIVATE ncrypt advapi32 bcrypt)
 target_compile_options(papacc_scq_native_adapter_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal.scq_native_adapter COMMAND papacc_scq_native_adapter_test)
 set_property(SOURCE apps/server/server_configuration_commit_win32.c tests/scq_session_driver_test.c tests/scq_native_adapter_test.c APPEND PROPERTY OBJECT_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/personal_scq_transport_win32.h;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/personal_scq_transport_win32.inc")
 add_executable(papacc_scq_session_driver_test tests/scq_session_driver_test.c)
 target_include_directories(papacc_scq_session_driver_test PRIVATE apps/server include)
 target_link_libraries(papacc_scq_session_driver_test PRIVATE ncrypt advapi32 bcrypt)
 target_compile_options(papacc_scq_session_driver_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal.scq_session_driver COMMAND papacc_scq_session_driver_test)
 set_property(SOURCE apps/server/server_configuration_commit_win32.c tests/scq_session_driver_test.c APPEND PROPERTY OBJECT_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/personal_scq_session_win32.h;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/personal_scq_session_win32.inc")
 add_executable(papacc_scq_admin_source_test tests/scq_admin_source_test.c)
 target_include_directories(papacc_scq_admin_source_test PRIVATE apps/server include)
 target_link_libraries(papacc_scq_admin_source_test PRIVATE ncrypt advapi32)
 target_compile_options(papacc_scq_admin_source_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal.scq_admin_source COMMAND papacc_scq_admin_source_test)
 set_property(SOURCE apps/server/server_configuration_commit_win32.c tests/scq_admin_source_test.c APPEND PROPERTY OBJECT_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/personal_scq_admin_win32.h;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/personal_scq_admin_win32.inc;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/personal_scq_handoff_win32.h;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/personal_scq_handoff_win32.inc;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/personal_confirmation_console_win32.inc")
 add_executable(papacc_scq_private_profile_test tests/scq_private_profile_test.c)
 target_include_directories(papacc_scq_private_profile_test PRIVATE apps/server include)
 target_link_libraries(papacc_scq_private_profile_test PRIVATE advapi32)
 target_compile_options(papacc_scq_private_profile_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal.scq_private_profile COMMAND papacc_scq_private_profile_test)
 set_property(SOURCE apps/server/server_configuration_commit_win32.c tests/scq_private_profile_test.c APPEND PROPERTY OBJECT_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/personal_scq_profile_win32.h;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/personal_scq_profile_win32.inc")
 set_property(SOURCE apps/server/server_configuration_commit_win32.c tests/personal_scq_inputs_test.c APPEND PROPERTY OBJECT_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/personal_scq_inputs_win32.h;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/personal_scq_inputs_win32.inc;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_personal_token_checks_win32.inc")
 set_property(SOURCE apps/server/server_configuration_commit_win32.c tests/personal_install_paths_test.c tests/personal_scq_dispatch_test.c APPEND PROPERTY OBJECT_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/personal_install_paths_win32.h;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/personal_install_paths_win32.inc;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/personal_scq_dispatch_win32.h;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/personal_scq_dispatch_win32.inc")
 set_property(SOURCE apps/server/server_configuration_commit_win32.c tests/personal_cng_probe_test.c APPEND PROPERTY OBJECT_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/personal_cng_probe_win32.h;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/personal_cng_probe_win32.inc")
 set_property(SOURCE apps/server/server_configuration_commit_win32.c tests/personal_runtime_identity_test.c APPEND PROPERTY OBJECT_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/personal_runtime_identity_win32.h;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/personal_runtime_identity_win32.inc")
 set_property(SOURCE apps/server/server_configuration_commit_win32.c tests/personal_ca_binding_win32_test.c APPEND PROPERTY OBJECT_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_configuration_personal_ca_win32.inc;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_issuer_x509_win32.h;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_ca_key_win32.h")
 foreach(binding_variant IN ITEMS native model)
  add_executable(papacc_personal_ca_${binding_variant}_test tests/personal_ca_binding_win32_test.c)
  target_include_directories(papacc_personal_ca_${binding_variant}_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} apps/server)
  target_compile_definitions(papacc_personal_ca_${binding_variant}_test PRIVATE PAPACC_OFFLINE_D3_WORKER=1)
  if(binding_variant STREQUAL "model")
   target_compile_definitions(papacc_personal_ca_${binding_variant}_test PRIVATE PAPACC_PERSONAL_CA_MODEL=1)
  endif()
  target_link_libraries(papacc_personal_ca_${binding_variant}_test PRIVATE papacc_configuration_commit)
  target_compile_options(papacc_personal_ca_${binding_variant}_test PRIVATE /W4 /WX)
  add_test(NAME papacc.personal_ca.${binding_variant} COMMAND papacc_personal_ca_${binding_variant}_test)
 endforeach()
endif()

# Personal SCM bridge: existing candidate executable only; fixed native binding
# unavailable => mutations and productive service startup deny before effects.
option(PAPACC_ENABLE_SINGLE_LAB_ADMISSION "Qualify canonical single diagnostic LAB admission (not production)" OFF)
if(WIN32 AND PAPACC_ENABLE_CONFIG_ADMIN_CLI)
 add_library(papacc_server_service STATIC apps/server/server_service_data.c apps/server/server_service_win32.c)
 target_include_directories(papacc_server_service PUBLIC apps/server PRIVATE include)
 target_link_libraries(papacc_server_service PUBLIC papacc_server_console_win32 advapi32 papacc_storage_sha256 papacc_config_source)
 set_property(SOURCE apps/server/server_service_win32.c tests/service_host_model_test.c APPEND PROPERTY OBJECT_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_service_startup_win32.h;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_service_startup_win32.inc")
 set_target_properties(papacc_server_service PROPERTIES C_STANDARD 90 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
 target_compile_options(papacc_server_service PRIVATE /W4 /WX)
 if(PAPACC_ENABLE_SINGLE_LAB_ADMISSION)
  target_compile_definitions(papacc_server_service PUBLIC PAPACC_SINGLE_LAB_ADMISSION=1 _WIN32_WINNT=0x0603)
 endif()
 target_compile_definitions(papacc_server PRIVATE PAPACC_ENABLE_SERVICE_CLI=1)
 target_compile_definitions(papacc_server_run_win32 PRIVATE PAPACC_ENABLE_SERVICE_CLI=1)
 if(TARGET papacc_configuration_commit)
  target_compile_definitions(papacc_configuration_commit PRIVATE PAPACC_PERSONAL_SCM_HOST_GATE=1)
  target_link_libraries(papacc_configuration_commit PRIVATE papacc_server_service)
 endif()
 target_link_libraries(papacc_server_run_win32 PRIVATE papacc_server_service)
 # Exact same canonical owner supplies closed evidence; both libraries are STATIC.
 target_link_libraries(papacc_server_service PRIVATE papacc_server_run_win32)
 target_link_libraries(papacc_server PRIVATE papacc_server_service)
 # One restricted administrative adapter, linking the SAME canonical owner.
 # No second root implementation, service dispatcher or authority provider.
 # The utility is never a LAB shortcut; protection/provenance remain U2 gates.
 if(NOT PAPACC_ENABLE_SINGLE_LAB_ADMISSION)
  add_executable(papacc_update apps/server/update_main.c)
  target_include_directories(papacc_update PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} apps/server)
  target_link_libraries(papacc_update PRIVATE papacc_server_run_win32)
  target_compile_options(papacc_update PRIVATE /W4 /WX)
  set_target_properties(papacc_update PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
  if(BUILD_TESTING)
   add_executable(papacc_update_entry_test tests/update_entry_test.c)
   target_include_directories(papacc_update_entry_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} apps/server)
   target_compile_options(papacc_update_entry_test PRIVATE /W4 /WX)
   set_target_properties(papacc_update_entry_test PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
   add_test(NAME papacc.personal.update_entry COMMAND papacc_update_entry_test)
  endif()
 endif()
 set_property(SOURCE apps/server/main.c APPEND PROPERTY OBJECT_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/main.c.api3.inc;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_service_win32.h")
 if(BUILD_TESTING)
  # Registration-only isolated LAB: never a product CLI option or runtime grant.
  add_executable(papacc_scm_dependency_acl_readonly tools/qualification/scm-dependency-acl-readonly.c)
  target_link_libraries(papacc_scm_dependency_acl_readonly PRIVATE papacc_server_service)
  target_compile_options(papacc_scm_dependency_acl_readonly PRIVATE /W4 /WX)
  set_target_properties(papacc_scm_dependency_acl_readonly PROPERTIES C_STANDARD 90 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
  add_executable(papacc_scm_registration_lab tools/qualification/scm-registration-lab.c)
  target_link_libraries(papacc_scm_registration_lab PRIVATE papacc_server_service)
  target_compile_options(papacc_scm_registration_lab PRIVATE /W4 /WX)
  set_target_properties(papacc_scm_registration_lab PROPERTIES C_STANDARD 90 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
  add_executable(papacc_scm_runtime_cycle_lab tools/qualification/scm-registration-lab.c)
  target_compile_definitions(papacc_scm_runtime_cycle_lab PRIVATE PAPACC_SCM_CYCLE_LAB=1 _WIN32_WINNT=0x0601 WIN32_LEAN_AND_MEAN=1)
  target_link_libraries(papacc_scm_runtime_cycle_lab PRIVATE papacc_server_service psapi iphlpapi)
  target_compile_options(papacc_scm_runtime_cycle_lab PRIVATE /W4 /WX)
  add_executable(papacc_scm_instrumented_lab tools/qualification/scm-registration-lab.c)
  option(PAPACC_ENABLE_N0_POSITIVE_LAB "Compile the separately approved N0 positive LAB entry (does not authorize execution)" OFF)
  option(PAPACC_ENABLE_N0START03_READONLY_PINS "Explicit reviewed successor03 read-only pins; never authorizes execution" OFF)
  option(PAPACC_ENABLE_N0_PRIVATE_STDOUT "T0-T9 opt-in local noninherited stdout; no operational authority" OFF)
  option(PAPACC_ENABLE_N0START03_PRIVATE_POSITIVE "T10-T14 exact03 positive composition; separate operation approval still required" OFF)
  if(PAPACC_ENABLE_N0START03_PRIVATE_POSITIVE)
    if(NOT PAPACC_ENABLE_N0_PRIVATE_STDOUT OR NOT PAPACC_ENABLE_N0_POSITIVE_LAB OR PAPACC_ENABLE_N0START03_READONLY_PINS OR PAPACC_ENABLE_READONLY_OBSERVER_HANDLE_TRACE OR PAPACC_ENABLE_READONLY_HANDLE_TABLE_TRACE)
      message(FATAL_ERROR "Private positive03 requires private transport + positive entry, no read-only profile/traces")
    endif()
    target_compile_definitions(papacc_scm_instrumented_lab PRIVATE PAPACC_N0START03_PRIVATE_POSITIVE=1)
  endif()
  if(PAPACC_ENABLE_N0_PRIVATE_STDOUT)
    if(NOT PAPACC_ENABLE_N0START03_PRIVATE_POSITIVE AND (NOT PAPACC_ENABLE_N0START03_READONLY_PINS OR PAPACC_ENABLE_N0_POSITIVE_LAB))
      message(FATAL_ERROR "Private transport requires explicit read-only or exact private positive03 composition")
    endif()
    target_sources(papacc_scm_instrumented_lab PRIVATE tools/qualification/scm-n0-private-stdout-native.c)
    target_compile_definitions(papacc_scm_instrumented_lab PRIVATE PAPACC_N0_PRIVATE_STDOUT=1)
    add_library(papacc_n0_private_stdout SHARED tools/qualification/scm-n0-private-stdout-native.c)
    target_compile_definitions(papacc_n0_private_stdout PRIVATE PACC_TRANSPORT_DLL=1 _WIN32_WINNT=0x0603)
    target_compile_options(papacc_n0_private_stdout PRIVATE /W4 /WX)
    target_link_libraries(papacc_n0_private_stdout PRIVATE advapi32 bcrypt)
  endif()
  option(PAPACC_ENABLE_READONLY_OBSERVER_HANDLE_TRACE "Opt-in collector handle samples within the existing successor read-only observer" OFF)
  if(PAPACC_ENABLE_READONLY_OBSERVER_HANDLE_TRACE)
    if(NOT PAPACC_ENABLE_N0START03_READONLY_PINS OR PAPACC_ENABLE_N0_POSITIVE_LAB)
      message(FATAL_ERROR "Observer handle samples require successor read-only pins and forbid positive entry")
    endif()
    target_compile_definitions(papacc_scm_instrumented_lab PRIVATE PAPACC_READONLY_OBSERVER_HANDLE_TRACE=1)
  endif()
  option(PAPACC_ENABLE_READONLY_HANDLE_TABLE_TRACE "Opt-in bounded original-owner handle attribution; no positive authority" OFF)
  if(PAPACC_ENABLE_READONLY_HANDLE_TABLE_TRACE)
    if(NOT PAPACC_ENABLE_N0START03_READONLY_PINS OR PAPACC_ENABLE_N0_POSITIVE_LAB)
      message(FATAL_ERROR "Handle table attribution requires successor read-only pins and forbids positive entry")
    endif()
    target_compile_definitions(papacc_scm_instrumented_lab PRIVATE PAPACC_READONLY_HANDLE_TABLE_TRACE=1)
  endif()
  if(PAPACC_ENABLE_N0START03_READONLY_PINS AND PAPACC_ENABLE_N0_POSITIVE_LAB)
    message(FATAL_ERROR "Successor03 pin profile is read-only; positive route forbidden")
  endif()
  if(PAPACC_ENABLE_N0START03_READONLY_PINS)
    target_compile_definitions(papacc_scm_instrumented_lab PRIVATE PAPACC_N0START03_READONLY_PINS=1)
  endif()
  if(PAPACC_ENABLE_N0_POSITIVE_LAB)
    target_compile_definitions(papacc_scm_instrumented_lab PRIVATE PAPACC_N0_POSITIVE_PINNED_LAB=1)
  else()
    target_compile_definitions(papacc_scm_instrumented_lab PRIVATE PAPACC_N0_READONLY_PINNED_LAB=1)
  endif()
  target_compile_definitions(papacc_scm_instrumented_lab PRIVATE PAPACC_SCM_CYCLE_LAB=1 PAPACC_SCM_INSTRUMENTED_LAB=1 PAPACC_N0_N9_NATIVE_OWNER=1 _WIN32_WINNT=0x0603 WIN32_LEAN_AND_MEAN=1)
  target_link_libraries(papacc_scm_instrumented_lab PRIVATE papacc_server_service psapi iphlpapi wevtapi user32 bcrypt)
  target_compile_options(papacc_scm_instrumented_lab PRIVATE /W4 /WX)
  set_target_properties(papacc_scm_instrumented_lab PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
  foreach(transport_profile default readonly positive)
    add_executable(papacc_private_profile_${transport_profile}_test tests/noninherited_positive_profile_test.c)
    target_compile_options(papacc_private_profile_${transport_profile}_test PRIVATE /W4 /WX)
    target_link_libraries(papacc_private_profile_${transport_profile}_test PRIVATE advapi32)
    if(transport_profile STREQUAL "readonly")
      target_compile_definitions(papacc_private_profile_${transport_profile}_test PRIVATE PAPACC_N0START03_READONLY_PINS=1)
    elseif(transport_profile STREQUAL "positive")
      target_compile_definitions(papacc_private_profile_${transport_profile}_test PRIVATE PAPACC_N0START03_PRIVATE_POSITIVE=1 PAPACC_N0_PRIVATE_STDOUT=1 PAPACC_N0_POSITIVE_PINNED_LAB=1)
    endif()
    add_test(NAME papacc.service.private_profile_${transport_profile} COMMAND papacc_private_profile_${transport_profile}_test)
  endforeach()
  foreach(pin_profile default optin)
    add_executable(papacc_n0start03_pins_${pin_profile}_test tests/service_n0start03_pin_profile_test.c)
    target_compile_options(papacc_n0start03_pins_${pin_profile}_test PRIVATE /W4 /WX)
    target_link_libraries(papacc_n0start03_pins_${pin_profile}_test PRIVATE advapi32)
    if(pin_profile STREQUAL "optin")
      target_compile_definitions(papacc_n0start03_pins_${pin_profile}_test PRIVATE PAPACC_N0START03_READONLY_PINS=1)
    endif()
    add_test(NAME papacc.service.n0start03_pins_${pin_profile} COMMAND papacc_n0start03_pins_${pin_profile}_test)
  endforeach()
  add_executable(papacc_service_cross_admission_test tests/service_cross_admission_win32_test.c)
  target_compile_definitions(papacc_service_cross_admission_test PRIVATE _WIN32_WINNT=0x0603)
  target_compile_options(papacc_service_cross_admission_test PRIVATE /W4 /WX)
  target_link_libraries(papacc_service_cross_admission_test PRIVATE advapi32)
  set_target_properties(papacc_service_cross_admission_test PROPERTIES C_STANDARD 90 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
  add_executable(papacc_service_busy_diagnostic_test tests/service_busy_diagnostic_test.c)
  target_compile_definitions(papacc_service_busy_diagnostic_test PRIVATE PAPACC_SCM_CYCLE_LAB=1 PAPACC_SCM_INSTRUMENTED_LAB=1 PAPACC_N0_N9_NATIVE_OWNER=1 _WIN32_WINNT=0x0603 WIN32_LEAN_AND_MEAN=1)
  target_compile_options(papacc_service_busy_diagnostic_test PRIVATE /W4 /WX)
  target_link_libraries(papacc_service_busy_diagnostic_test PRIVATE papacc_server_service psapi iphlpapi wevtapi wintrust user32 bcrypt)
  set_target_properties(papacc_service_busy_diagnostic_test PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
  add_test(NAME papacc.service.busy_diagnostic COMMAND papacc_service_busy_diagnostic_test)
  if(BUILD_TESTING)
   add_executable(papacc_service_n0_sd_test tests/service_n0_sd_reconciliation_test.c)
   target_compile_options(papacc_service_n0_sd_test PRIVATE /W4 /WX)
   target_compile_definitions(papacc_service_n0_sd_test PRIVATE _WIN32_WINNT=0x0601 WIN32_LEAN_AND_MEAN=1)
   target_link_libraries(papacc_service_n0_sd_test PRIVATE papacc_storage_sha256 advapi32)
   set_target_properties(papacc_service_n0_sd_test PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
   add_test(NAME papacc.service.n0_sd_model COMMAND papacc_service_n0_sd_test --model)
   # Engineering-only creator, excluded from service/admission/product targets.
   add_executable(papacc_n0_root_creator tools/qualification/n0-protected-root-creator.c)
   add_executable(papacc_n0_root_model_test tools/qualification/n0-protected-root-model-test.c)
   foreach(root_target papacc_n0_root_creator papacc_n0_root_model_test)
    target_compile_options(${root_target} PRIVATE /W4 /WX)
    target_compile_definitions(${root_target} PRIVATE _WIN32_WINNT=0x0601)
    target_link_libraries(${root_target} PRIVATE papacc_storage_sha256 advapi32)
    set_target_properties(${root_target} PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
   endforeach()
   add_test(NAME papacc.n0.root_model COMMAND papacc_n0_root_model_test)
   add_test(NAME papacc.n0.root_descriptor COMMAND papacc_n0_root_creator --describe)
  endif()
  add_executable(papacc_service_console_proof_test tests/service_console_proof_test.c)
  target_compile_options(papacc_service_console_proof_test PRIVATE /W4 /WX)
  set_target_properties(papacc_service_console_proof_test PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
  add_test(NAME papacc.service.console_proof COMMAND papacc_service_console_proof_test)

  foreach(bd_kind model source_gate)
   add_executable(papacc_service_bd_${bd_kind}_test tests/service_bd_admission_test.c)
   target_compile_options(papacc_service_bd_${bd_kind}_test PRIVATE /W4 /WX)
   target_compile_definitions(papacc_service_bd_${bd_kind}_test PRIVATE _WIN32_WINNT=0x0603 WIN32_LEAN_AND_MEAN=1)
   if(bd_kind STREQUAL "model")
    target_compile_definitions(papacc_service_bd_${bd_kind}_test PRIVATE PAPACC_SERVICE_BD_MODEL=1 PAPACC_N0_N9_NATIVE_OWNER=1)
   else()
    target_compile_definitions(papacc_service_bd_${bd_kind}_test PRIVATE PS_BD_SOURCE_ONLY=1)
   endif()
   target_link_libraries(papacc_service_bd_${bd_kind}_test PRIVATE advapi32)
   set_target_properties(papacc_service_bd_${bd_kind}_test PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
   add_test(NAME papacc.service.bd_${bd_kind} COMMAND papacc_service_bd_${bd_kind}_test)
  endforeach()
  add_executable(papacc_scm_instrumented_policy_test tests/service_instrumented_policy_test.c)
  target_compile_options(papacc_scm_instrumented_policy_test PRIVATE /W4 /WX)
  set_target_properties(papacc_scm_instrumented_policy_test PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
  add_test(NAME papacc.service.instrumented_policy COMMAND papacc_scm_instrumented_policy_test)
  add_executable(papacc_service_n0_census_content_test tests/service_n0_census_content_test.c)
  target_compile_options(papacc_service_n0_census_content_test PRIVATE /W4 /WX)
  add_test(NAME papacc.service.n0_census_content COMMAND papacc_service_n0_census_content_test)
  add_executable(papacc_scm_cycle_model tests/service_cycle_model_test.c)
  target_compile_definitions(papacc_scm_cycle_model PRIVATE _WIN32_WINNT=0x0601 WIN32_LEAN_AND_MEAN=1)
  target_link_libraries(papacc_scm_cycle_model PRIVATE papacc_server_service psapi iphlpapi)
  target_compile_options(papacc_scm_cycle_model PRIVATE /W4 /WX)
  add_test(NAME papacc.service.cycle_model COMMAND papacc_scm_cycle_model)

  set_target_properties(papacc_scm_runtime_cycle_lab PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
  add_executable(papacc_scm_registration_lab_model tests/service_scm_virtual_model_test.c)
  target_compile_definitions(papacc_scm_registration_lab_model PRIVATE PAPACC_SCM_REGISTRATION_LAB=1)
  target_link_libraries(papacc_scm_registration_lab_model PRIVATE papacc_server_service)
  target_compile_options(papacc_scm_registration_lab_model PRIVATE /W4 /WX)
  set_target_properties(papacc_scm_registration_lab_model PROPERTIES C_STANDARD 90 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
  add_test(NAME papacc.service.registration_lab_model COMMAND papacc_scm_registration_lab_model)
  add_executable(papacc_scm_original_registration_test tests/scm_original_registration_test.c)
  target_include_directories(papacc_scm_original_registration_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} apps/server)
  target_compile_definitions(papacc_scm_original_registration_test PRIVATE PAPACC_OFFLINE_D3_WORKER=1)
  target_link_libraries(papacc_scm_original_registration_test PRIVATE papacc_server_service papacc_server_run_win32 bcrypt advapi32)
  target_compile_options(papacc_scm_original_registration_test PRIVATE /W4 /WX)
  set_target_properties(papacc_scm_original_registration_test PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
  add_test(NAME papacc.service.original_registration COMMAND papacc_scm_original_registration_test)
  add_test(NAME papacc.service.registration_resource_observations COMMAND papacc_scm_original_registration_test --resource-observations)
  add_test(NAME papacc.service.preplan_first_cause COMMAND papacc_scm_original_registration_test --first-cause)
  add_executable(papacc_scm_registration_files_test tests/scm_registration_files_test.c)
  target_include_directories(papacc_scm_registration_files_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} apps/server)
  target_compile_definitions(papacc_scm_registration_files_test PRIVATE PAPACC_OFFLINE_D3_WORKER=1)
  target_compile_options(papacc_scm_registration_files_test PRIVATE /W4 /WX)
  target_link_libraries(papacc_scm_registration_files_test PRIVATE papacc_config_root8)
  set_target_properties(papacc_scm_registration_files_test PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
  add_test(NAME papacc.service.registration_files COMMAND papacc_scm_registration_files_test)
  add_executable(papacc_qualification_staging_test tests/qualification_staging_test.c)
  target_include_directories(papacc_qualification_staging_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} apps/server)
  target_compile_definitions(papacc_qualification_staging_test PRIVATE PAPACC_OFFLINE_D3_WORKER=1)
  target_link_libraries(papacc_qualification_staging_test PRIVATE papacc_server_run_win32 papacc_storage_sha256 advapi32)
  target_compile_options(papacc_qualification_staging_test PRIVATE /W4 /WX)
  set_target_properties(papacc_qualification_staging_test PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
  add_test(NAME papacc.personal.qualification_staging COMMAND papacc_qualification_staging_test)
  add_executable(papacc_staged_binary_update_test tests/staged_binary_update_test.c)
  target_include_directories(papacc_staged_binary_update_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} apps/server)
  target_compile_definitions(papacc_staged_binary_update_test PRIVATE PAPACC_OFFLINE_D3_WORKER=1)
  target_link_libraries(papacc_staged_binary_update_test PRIVATE papacc_server_run_win32 papacc_storage_sha256 advapi32)
  target_compile_options(papacc_staged_binary_update_test PRIVATE /W4 /WX)
  set_target_properties(papacc_staged_binary_update_test PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
  add_test(NAME papacc.personal.staged_binary_update COMMAND papacc_staged_binary_update_test)
  add_test(NAME papacc.personal.utility_preparation COMMAND papacc_staged_binary_update_test --utility-preparation)
  add_test(NAME papacc.personal.utility_replacement COMMAND papacc_staged_binary_update_test --utility-replacement)
  add_test(NAME papacc.personal.staging_resources COMMAND papacc_qualification_staging_test --resources)
  add_test(NAME papacc.personal.anchor_child_guard COMMAND papacc_qualification_staging_test --anchor-child)
  add_executable(papacc_root_chain_metadata_test tests/root_chain_metadata_test.c)
  target_include_directories(papacc_root_chain_metadata_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} apps/server)
  target_compile_definitions(papacc_root_chain_metadata_test PRIVATE PAPACC_OFFLINE_D3_WORKER=1)
  target_link_libraries(papacc_root_chain_metadata_test PRIVATE papacc_server_run_win32 papacc_storage_sha256 advapi32)
  target_compile_options(papacc_root_chain_metadata_test PRIVATE /W4 /WX)
  set_target_properties(papacc_root_chain_metadata_test PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
  add_test(NAME papacc.personal.root_chain_metadata COMMAND papacc_root_chain_metadata_test)
  add_executable(papacc_staging_close_guard_test tests/qualification_staging_close_guard_test.c)
  target_include_directories(papacc_staging_close_guard_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} apps/server)
  target_compile_definitions(papacc_staging_close_guard_test PRIVATE PAPACC_OFFLINE_D3_WORKER=1)
  target_link_libraries(papacc_staging_close_guard_test PRIVATE papacc_server_run_win32)
  target_compile_options(papacc_staging_close_guard_test PRIVATE /W4 /WX)
  add_test(NAME papacc.personal.staging_close_guard COMMAND papacc_staging_close_guard_test)
  foreach(kind lifecycle_data scm_native scm_query_model scm_virtual_model scm_acl_model)
   add_executable(papacc_service_${kind}_test tests/service_${kind}_test.c)
   target_link_libraries(papacc_service_${kind}_test PRIVATE papacc_server_service)
   target_compile_options(papacc_service_${kind}_test PRIVATE /W4 /WX)
   set_target_properties(papacc_service_${kind}_test PROPERTIES C_STANDARD 90 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
   add_test(NAME papacc.service.${kind} COMMAND papacc_service_${kind}_test)
  endforeach()
 endif()
endif()

# Private LAB host shares canonical main/runtime; no positive startup grant.
if(WIN32 AND BUILD_TESTING AND PAPACC_ENABLE_CONFIG_ADMIN_CLI AND PAPACC_ENABLE_CANONICAL_API3_CANDIDATE AND PAPACC_ENABLE_CANONICAL_C8_ROOT)
 get_target_property(lab_host_sources papacc_server SOURCES)
 add_executable(papacc_server_lab_host ${lab_host_sources} apps/server/server_service_win32.c)
 # Direct LAB service object satisfies shared runtime references; the default
 # service object in the static library is not extracted into this executable.
 foreach(prop INCLUDE_DIRECTORIES COMPILE_DEFINITIONS COMPILE_OPTIONS LINK_LIBRARIES)
  get_target_property(lab_host_property papacc_server ${prop})
  if(lab_host_property)
   set_property(TARGET papacc_server_lab_host PROPERTY ${prop} "${lab_host_property}")
  endif()
 endforeach()
 target_compile_definitions(papacc_server_lab_host PRIVATE PAPACC_SCM_REGISTRATION_LAB=1 PAPACC_SCM_RUNTIME_LAB=1)
 option(PAPACC_ENABLE_R0R9_VIRTUAL_USER_LAB "Accepted R0-R9 prospective fixed LAB rule; OFF by default, no operational authority" OFF)
 if(PAPACC_ENABLE_R0R9_VIRTUAL_USER_LAB)
  if(NOT PAPACC_ENABLE_SINGLE_LAB_ADMISSION OR NOT PAPACC_ENABLE_N0START03_PRIVATE_POSITIVE)
   message(FATAL_ERROR "R0-R9 requires existing canonical private LAB admission composition")
  endif()
  target_compile_definitions(papacc_server_lab_host PRIVATE PAPACC_R0R9_VIRTUAL_USER_LAB=1 PAPACC_PSR_TRACE_LAB=1)
 endif()
 target_compile_options(papacc_server_lab_host PRIVATE /W4 /WX)
 target_link_options(papacc_server_lab_host PRIVATE "/MAP:${CMAKE_CURRENT_BINARY_DIR}/papacc_server_lab_host.map")
 set_target_properties(papacc_server_lab_host PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
 papacc_stage_pst_runtime(papacc_server_lab_host)
 add_executable(papacc_service_closed_empty_model tests/service_host_model_test.c)
 target_compile_definitions(papacc_service_closed_empty_model PRIVATE PAPACC_HOST_CANONICAL_EMPTY=1)
 target_link_libraries(papacc_service_closed_empty_model PRIVATE papacc_server_service papacc_server_run_win32)
 target_compile_options(papacc_service_closed_empty_model PRIVATE /W4 /WX)
 add_test(NAME papacc.service.closed_empty_model COMMAND papacc_service_closed_empty_model)
 foreach(profile product lab)
  add_executable(papacc_service_host_${profile}_model tests/service_host_model_test.c)
  if(profile STREQUAL "lab")
   target_compile_definitions(papacc_service_host_${profile}_model PRIVATE PAPACC_SCM_REGISTRATION_LAB=1)
  endif()
  target_link_libraries(papacc_service_host_${profile}_model PRIVATE papacc_server_service)
  target_compile_options(papacc_service_host_${profile}_model PRIVATE /W4 /WX)
  set_target_properties(papacc_service_host_${profile}_model PROPERTIES C_STANDARD 90 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
  add_test(NAME papacc.service.host_${profile}_model COMMAND papacc_service_host_${profile}_model)
 endforeach()
endif()

if(WIN32 AND BUILD_TESTING AND PAPACC_ENABLE_CONFIG_ADMIN_CLI AND PAPACC_ENABLE_CANONICAL_C8_ROOT)
 foreach(kind model native)
  add_executable(papacc_service_runtime_${kind}_test tests/service_runtime_component_test.c)
  target_compile_definitions(papacc_service_runtime_${kind}_test PRIVATE PAPACC_SCM_REGISTRATION_LAB=1 PAPACC_SCM_RUNTIME_LAB=1)
  if(kind STREQUAL "model")
   target_compile_definitions(papacc_service_runtime_${kind}_test PRIVATE PAPACC_SERVICE_RUNTIME_MODEL=1)
  endif()
  target_link_libraries(papacc_service_runtime_${kind}_test PRIVATE papacc_server_run_win32 papacc_server_service)
  target_include_directories(papacc_service_runtime_${kind}_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS})
  target_compile_options(papacc_service_runtime_${kind}_test PRIVATE /W4 /WX)
  set_target_properties(papacc_service_runtime_${kind}_test PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
  add_test(NAME papacc.service.runtime_${kind} COMMAND papacc_service_runtime_${kind}_test)
 endforeach()
 add_executable(papacc_service_runtime_file_model_test tests/service_runtime_file_model_test.c)
 target_compile_definitions(papacc_service_runtime_file_model_test PRIVATE PAPACC_SCM_REGISTRATION_LAB=1 PAPACC_SCM_RUNTIME_LAB=1 PAPACC_SERVICE_RUNTIME_MODEL=1 PAPACC_PSR_TRACE_LAB=1 PAPACC_PSR_FILE_RUNTIME_LAB=1)
 target_link_libraries(papacc_service_runtime_file_model_test PRIVATE papacc_server_run_win32 papacc_server_service user32)
 target_include_directories(papacc_service_runtime_file_model_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS})
 target_compile_options(papacc_service_runtime_file_model_test PRIVATE /W4 /WX)
 set_target_properties(papacc_service_runtime_file_model_test PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
 add_test(NAME papacc.service.runtime_file_model COMMAND papacc_service_runtime_file_model_test)
 add_executable(papacc_service_virtual_user_r0r9_test tests/service_virtual_user_r0r9_test.c)
 target_compile_definitions(papacc_service_virtual_user_r0r9_test PRIVATE PAPACC_SCM_REGISTRATION_LAB=1 PAPACC_SCM_RUNTIME_LAB=1 PAPACC_SERVICE_RUNTIME_MODEL=1 PAPACC_PSR_TRACE_LAB=1 PAPACC_R0R9_VIRTUAL_USER_LAB=1)
 target_link_libraries(papacc_service_virtual_user_r0r9_test PRIVATE papacc_server_run_win32 papacc_server_service user32)
 target_include_directories(papacc_service_virtual_user_r0r9_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS})
 target_compile_options(papacc_service_virtual_user_r0r9_test PRIVATE /W4 /WX)
 set_target_properties(papacc_service_virtual_user_r0r9_test PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
 add_test(NAME papacc.service.virtual_user_r0r9 COMMAND papacc_service_virtual_user_r0r9_test)
 add_executable(papacc_service_virtual_user_r0r9_file_test tests/service_virtual_user_r0r9_file_test.c)
 target_compile_definitions(papacc_service_virtual_user_r0r9_file_test PRIVATE PAPACC_SCM_REGISTRATION_LAB=1 PAPACC_SCM_RUNTIME_LAB=1 PAPACC_SERVICE_RUNTIME_MODEL=1 PAPACC_PSR_TRACE_LAB=1 PAPACC_R0R9_VIRTUAL_USER_LAB=1 PAPACC_PSR_FILE_RUNTIME_LAB=1)
 target_link_libraries(papacc_service_virtual_user_r0r9_file_test PRIVATE papacc_server_run_win32 papacc_server_service user32)
 target_include_directories(papacc_service_virtual_user_r0r9_file_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS})
 target_compile_options(papacc_service_virtual_user_r0r9_file_test PRIVATE /W4 /WX)
 set_target_properties(papacc_service_virtual_user_r0r9_file_test PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
 add_test(NAME papacc.service.virtual_user_r0r9_file COMMAND papacc_service_virtual_user_r0r9_file_test)
 add_executable(papacc_service_runtime_closed_test tests/service_runtime_closed_test.c)
 target_link_libraries(papacc_service_runtime_closed_test PRIVATE papacc_server_run_win32)
 target_include_directories(papacc_service_runtime_closed_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS})
 target_compile_options(papacc_service_runtime_closed_test PRIVATE /W4 /WX)
 set_target_properties(papacc_service_runtime_closed_test PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
 add_test(NAME papacc.service.runtime_closed COMMAND papacc_service_runtime_closed_test)
endif()

# Explicit private include dependencies: localized /showIncludes is not a proof.
set_property(SOURCE
 apps/server/server_service_win32.c
 tests/service_cross_admission_win32_test.c
 tests/service_host_model_test.c
 tests/service_runtime_component_test.c
 tests/service_runtime_file_model_test.c
 tests/service_scm_query_model_test.c
 tests/service_scm_virtual_model_test.c
 tools/qualification/scm-registration-lab.c
 APPEND PROPERTY OBJECT_DEPENDS
 "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_service_virtual_win32.inc;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_service_startup_win32.inc;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_service_startup_win32.h"
 "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_service_win32.c;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_service_admission_win32.inc;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_service_admission_win32.h;${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-runtime-cycle-lab.inc;${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-instrumented-cycle-lab.inc;${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-instrumented-observer.inc;${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-instrumented-policy.h")

set_property(SOURCE apps/server/server_launcher_owner_win32.c APPEND PROPERTY OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_launcher_scm_prepare_win32.inc;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_service_admission_win32.h")
set_property(SOURCE apps/server/server_launcher_owner_win32.c tests/scm_original_registration_test.c APPEND PROPERTY OBJECT_DEPENDS
 "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_scm_registration_resources_win32.inc"
 "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_launcher_scm_registration_win32.inc;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/personal_console_token_win32.inc;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/personal_confirmation_console_win32.inc;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/personal_scm_registration_win32.h")
if(TARGET papacc_server_run_win32)
 target_link_libraries(papacc_server_run_win32 PRIVATE psapi)
endif()
set_property(SOURCE tests/scm_original_registration_test.c APPEND PROPERTY OBJECT_DEPENDS
 "${CMAKE_CURRENT_SOURCE_DIR}/tests/service_scm_virtual_model_test.c;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_service_win32.c;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_service_virtual_win32.inc;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_service_startup_win32.inc")

set_property(SOURCE apps/server/server_service_win32.c tools/qualification/scm-registration-lab.c tests/service_cross_admission_win32_test.c tests/service_busy_diagnostic_test.c APPEND PROPERTY OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-busy-diagnostic.h;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_service_admission_win32.inc;${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-instrumented-observer.inc;${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-instrumented-cycle-lab.inc")

set_property(SOURCE tests/service_busy_diagnostic_test.c tests/service_console_proof_test.c APPEND PROPERTY OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-console-proof.inc;${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-console-proof-model.h")

if(WIN32)
 set_property(SOURCE apps/server/server_service_win32.c tools/qualification/scm-registration-lab.c tests/service_busy_diagnostic_test.c tests/service_bd_admission_test.c tests/service_instrumented_policy_test.c APPEND PROPERTY OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_service_admission_bd_win32.h;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_service_admission_bd_win32.inc")
endif()

# Exact successor binding: localized /showIncludes must not leave stale adapters.
# The qualifier image pin is NOT a runtime-service dependency (no hash cycle).
if(WIN32)
 set_property(SOURCE tools/qualification/scm-registration-lab.c tests/service_busy_diagnostic_test.c tests/service_bd_admission_test.c tests/service_instrumented_policy_test.c
  APPEND PROPERTY OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-n0-context.inc;${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-n0-context-model.h;${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-n0-context-pins.h;${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-successor-image-pin.h")
 set_property(SOURCE apps/server/server_service_win32.c tests/service_runtime_component_test.c tests/service_runtime_file_model_test.c
  APPEND PROPERTY OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_service_runtime_win32.inc;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_service_psr_file_win32.inc")
 set_property(SOURCE tests/service_runtime_file_model_test.c APPEND PROPERTY OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tests/service_runtime_component_test.c;${CMAKE_CURRENT_SOURCE_DIR}/tests/service_runtime_file_fixture.inc")
endif()

if(WIN32)
 set_property(SOURCE tests/service_n0start03_pin_profile_test.c APPEND PROPERTY OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-n0-context-model.h;${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-n0-context-pins.h;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_service_admission_bd_win32.h")
endif()

# A1 private auxiliary owner/includes, including API-simulated caller fixtures.
set_property(SOURCE tools/qualification/scm-registration-lab.c tests/service_busy_diagnostic_test.c
 tests/service_aux_callers_test.c tests/service_aux_legacy_callers_test.c
 APPEND PROPERTY OBJECT_DEPENDS
 "${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-aux-reservation-core.inc;${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-aux-reservation-bridge.inc;${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-registration-lab.c;${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-runtime-cycle-lab.inc;${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-instrumented-cycle-lab.inc;${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-instrumented-observer.inc;${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-n0-context.inc;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_service_admission_bd_win32.inc;${CMAKE_CURRENT_SOURCE_DIR}/tests/service_aux_fixture.inc")

if(WIN32 AND BUILD_TESTING)
 foreach(aux_kind callers legacy_callers)
  add_executable(papacc_service_aux_${aux_kind}_test tests/service_aux_${aux_kind}_test.c)
  target_compile_definitions(papacc_service_aux_${aux_kind}_test PRIVATE _WIN32_WINNT=0x0603 WIN32_LEAN_AND_MEAN=1)
  target_compile_options(papacc_service_aux_${aux_kind}_test PRIVATE /W4 /WX)
  target_link_libraries(papacc_service_aux_${aux_kind}_test PRIVATE papacc_server_service psapi iphlpapi wevtapi wintrust user32 bcrypt)
  set_target_properties(papacc_service_aux_${aux_kind}_test PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
 endforeach()
 add_executable(papacc_service_aux_model_test tests/service_aux_reservation_model_test.c)
 target_compile_options(papacc_service_aux_model_test PRIVATE /W4 /WX)
 target_compile_definitions(papacc_service_aux_model_test PRIVATE A1_SELECTED=$<BOOL:${PAPACC_ENABLE_WINDOWS_3MIB_MANAGED_PROFILE}>)
endif()

set_property(SOURCE tests/service_aux_reservation_model_test.c APPEND PROPERTY OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-aux-reservation-core.inc")

# Scoped self-token lifetime qualification: actual source callers, simulated APIs only.
if(WIN32 AND BUILD_TESTING)
 add_executable(papacc_service_self_token_lifetime_test tests/service_self_token_lifetime_test.c)
 target_compile_definitions(papacc_service_self_token_lifetime_test PRIVATE PAPACC_SCM_CYCLE_LAB=1 PAPACC_SCM_INSTRUMENTED_LAB=1 PAPACC_N0_N9_NATIVE_OWNER=1 _WIN32_WINNT=0x0603 WIN32_LEAN_AND_MEAN=1)
 target_compile_options(papacc_service_self_token_lifetime_test PRIVATE /W4 /WX)
 target_link_libraries(papacc_service_self_token_lifetime_test PRIVATE papacc_server_service psapi iphlpapi wevtapi wintrust user32 bcrypt)
 set_target_properties(papacc_service_self_token_lifetime_test PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
 set_property(SOURCE tests/service_self_token_lifetime_test.c APPEND PROPERTY OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-registration-lab.c;${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-instrumented-cycle-lab.inc;${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-aux-reservation-core.inc;${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-aux-reservation-bridge.inc;${CMAKE_CURRENT_SOURCE_DIR}/docs/integration/premonitor-self-token-lifetime-20261024/applied-branch.UNMODIFIED.inc")
endif()

# Opt-in isolated actual observer/stack-VA test; never invokes qualifier main.
if(WIN32 AND BUILD_TESTING)
 set_property(SOURCE tools/qualification/scm-registration-lab.c APPEND PROPERTY OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-readonly-handle-table.inc")
 set_property(SOURCE tests/service_observer_stack_va_isolated_test.c APPEND PROPERTY OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-readonly-handle-table.inc;${CMAKE_CURRENT_SOURCE_DIR}/tests/service_handle_table_isolated.inc")
 add_executable(papacc_noninherited_transport_test tests/noninherited_transport_test.c)
 target_compile_definitions(papacc_noninherited_transport_test PRIVATE _WIN32_WINNT=0x0603 WIN32_LEAN_AND_MEAN=1)
 target_compile_options(papacc_noninherited_transport_test PRIVATE /W4 /WX)
 target_link_libraries(papacc_noninherited_transport_test PRIVATE advapi32 bcrypt psapi)
 add_executable(papacc_explicit_inheritance_boundary_test tests/explicit_inheritance_boundary_test.c)
 target_compile_definitions(papacc_explicit_inheritance_boundary_test PRIVATE _WIN32_WINNT=0x0603 WIN32_LEAN_AND_MEAN=1)
 target_compile_options(papacc_explicit_inheritance_boundary_test PRIVATE /W4 /WX)
 target_link_libraries(papacc_explicit_inheritance_boundary_test PRIVATE user32)
 add_library(papacc_explicit_inheritance_fixture SHARED tools/qualification/scm-explicit-inheritance-fixture.c)
 target_compile_definitions(papacc_explicit_inheritance_fixture PRIVATE _WIN32_WINNT=0x0600 WIN32_LEAN_AND_MEAN=1)
 target_compile_options(papacc_explicit_inheritance_fixture PRIVATE /W4 /WX)
 set_target_properties(papacc_explicit_inheritance_fixture PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
 add_executable(papacc_service_observer_stack_va_isolated_test tests/service_observer_stack_va_isolated_test.c)
 target_compile_definitions(papacc_service_observer_stack_va_isolated_test PRIVATE PAPACC_READONLY_OBSERVER_HANDLE_TRACE=1 PAPACC_READONLY_HANDLE_TABLE_TRACE=1 PAPACC_N0START03_READONLY_PINS=1 PAPACC_SCM_CYCLE_LAB=1 PAPACC_SCM_INSTRUMENTED_LAB=1 PAPACC_N0_N9_NATIVE_OWNER=1 _WIN32_WINNT=0x0603 WIN32_LEAN_AND_MEAN=1)
 target_compile_options(papacc_service_observer_stack_va_isolated_test PRIVATE /W4 /WX)
 target_link_libraries(papacc_service_observer_stack_va_isolated_test PRIVATE papacc_server_service psapi iphlpapi wevtapi wintrust user32 bcrypt)
 set_target_properties(papacc_service_observer_stack_va_isolated_test PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
 set_property(SOURCE tests/service_observer_stack_va_isolated_test.c APPEND PROPERTY OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-registration-lab.c;${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-instrumented-cycle-lab.inc;${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-instrumented-observer.inc;${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-aux-reservation-core.inc;${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-aux-reservation-bridge.inc")
endif()

if(WIN32 AND MSVC AND BUILD_TESTING)
 set_property(SOURCE tools/qualification/scm-registration-lab.c APPEND PROPERTY OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-n0-private-stdout-adapter.inc;${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-n0-private-stdout-entry.inc;${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-n0-private-stdout-native.h")
endif()

if(WIN32 AND MSVC AND BUILD_TESTING)
 add_executable(papacc_noninherited_transport_integration_test tests/noninherited_transport_integration_test.c)
 target_compile_options(papacc_noninherited_transport_integration_test PRIVATE /W4 /WX)
 target_compile_definitions(papacc_noninherited_transport_integration_test PRIVATE _WIN32_WINNT=0x0603)
 target_link_libraries(papacc_noninherited_transport_integration_test PRIVATE advapi32 bcrypt)
 set_property(SOURCE tests/noninherited_transport_integration_test.c APPEND PROPERTY OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-n0-private-stdout-native.c;${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-n0-private-stdout-native.h")
endif()

if(WIN32 AND MSVC AND BUILD_TESTING)
 set_property(SOURCE tests/noninherited_transport_integration_test.c APPEND PROPERTY OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-n0-private-stdout-adapter.inc")
endif()

# Q4 image pins are generated after freezing the service image. Keep these
# explicit even when localized MSVC /showIncludes dependency discovery fails.
# The service image itself must not depend on its own derived digest.
set_property(SOURCE tools/qualification/scm-registration-lab.c
 APPEND PROPERTY OBJECT_DEPENDS
 "${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-q4-image-pin.h;${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-successor-image-pin.h;${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-lab-attempt-binding.h;${CMAKE_CURRENT_SOURCE_DIR}/tools/qualification/scm-n0-context-pins.h")

# R0-R9 actual caller and prospective private runtime dependencies.
set_property(SOURCE apps/server/server_service_win32.c tests/service_virtual_user_r0r9_test.c
 APPEND PROPERTY OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_service_virtual_user_r0r9_win32.inc;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_service_runtime_win32.inc;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_service_observation_win32.h")

# Diagnostic typed evidence is private to the existing producer/root.
set_property(SOURCE apps/server/server_c8_target_evidence_win32.c apps/server/server_launcher_owner_win32.c tests/target_typed_evidence_test.c
 APPEND PROPERTY OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_c8_target_evidence_win32.h;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/server_c8_resource_root_win32.inc")

# W/L private DATA bridge: actual body with simulated guards, no store/NV effects.
if(WIN32 AND BUILD_TESTING AND TARGET papacc_configuration_commit)
 add_executable(papacc_q4_root_binding_data_test tests/q4_root_binding_data_test.c)
 target_include_directories(papacc_q4_root_binding_data_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS})
 target_link_libraries(papacc_q4_root_binding_data_test PRIVATE papacc_configuration_commit)
 target_compile_options(papacc_q4_root_binding_data_test PRIVATE /W4 /WX)
 set_target_properties(papacc_q4_root_binding_data_test PROPERTIES C_STANDARD 90 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
 add_test(NAME papacc.q4.root_binding_data COMMAND papacc_q4_root_binding_data_test)
endif()

# Accepted L1 carrier DATA, no authority or native execution.
if(TARGET papacc_config_root8_v4_test)
 add_executable(papacc_config_root8_v5_test tests/config_root8_v5_test.c apps/server/server_config_root8_data_win32.c)
 target_include_directories(papacc_config_root8_v5_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} src/config src/issuer apps/server)
 target_compile_definitions(papacc_config_root8_v5_test PRIVATE PAPACC_OFFLINE_D3_WORKER=1)
 target_link_libraries(papacc_config_root8_v5_test PRIVATE papacc_config_root8 papacc_server_run_win32 papacc_config_binding papacc_issuer papacc_root8_signer_data crypt32)
 papacc_enable_warnings(papacc_config_root8_v5_test)
 papacc_stage_pst_runtime(papacc_config_root8_v5_test)
 add_test(NAME papacc.config_root8.v5_offline COMMAND papacc_config_root8_v5_test)
endif()

if(TARGET papacc_v4_transaction_worker_test)
 add_executable(papacc_v5_transaction_worker_test tests/v5_transaction_worker_win32_test.c
  apps/server/server_configuration_commit_win32.c apps/server/server_config_root8_data_win32.c)
 target_compile_definitions(papacc_v5_transaction_worker_test PRIVATE PAPACC_V3_BACKING_FIXTURE=1 PAPACC_V4_BACKING_FIXTURE=1
  PAPACC_V5_BACKING_FIXTURE=1 PAPACC_OFFLINE_D3_WORKER=1 PAPACC_ISSUER_STORAGE_COMPONENT_TEST=1)
 target_include_directories(papacc_v5_transaction_worker_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} src/config src/issuer apps/server)
 target_link_libraries(papacc_v5_transaction_worker_test PRIVATE papacc_config_root8 papacc_server_config
  papacc_issuer papacc_root8_signer_data papacc_storage_sha256 bcrypt ktmw32 crypt32 advapi32)
 target_compile_options(papacc_v5_transaction_worker_test PRIVATE /W4 /WX)
endif()

# The direct same-worker fixtures reuse the canonical personal DATA dependency
# already required by the current worker translation unit; no alternative stub.
foreach(t papacc_v4_transaction_worker_test papacc_v5_transaction_worker_test)
 if(TARGET ${t})
  target_sources(${t} PRIVATE apps/server/personal_plan_data.c)
 endif()
endforeach()

# Scoped modern Windows diagnostic only. Default DENY; no operational CTest.
option(PAPACC_ENABLE_NATIVE_TPM_PROOF_CANDIDATE "Build native TPM mathematical-proof candidate and offline fixtures" OFF)
if(PAPACC_ENABLE_NATIVE_TPM_PROOF_CANDIDATE)
 if(NOT WIN32 OR NOT MSVC OR NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
  message(FATAL_ERROR "Native TPM diagnostic requires modern Windows x64 MSVC; no legacy/backend fallback")
 endif()
 set(tp_native tools/qualification/tpm-native-proof)
 add_library(papacc_tpm_native_core STATIC ${tp_native}/proof_core.c ${tp_native}/json_data.c ${tp_native}/storage20_delivery.c)
 target_include_directories(papacc_tpm_native_core PUBLIC ${tp_native})
 target_link_libraries(papacc_tpm_native_core PUBLIC bcrypt psapi)
 set_target_properties(papacc_tpm_native_core PROPERTIES C_STANDARD 11 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
 target_compile_options(papacc_tpm_native_core PRIVATE /W4 /WX /GS /sdl)
 add_executable(papacc_tpm_native_proof ${tp_native}/native_host.c ${tp_native}/host_platform.c)
 target_link_libraries(papacc_tpm_native_proof PRIVATE papacc_tpm_native_core advapi32 crypt32 wevtapi user32)
 target_compile_options(papacc_tpm_native_proof PRIVATE /W4 /WX /GS /sdl)
 if(BUILD_TESTING AND TARGET papacc_configuration_commit)
  add_library(papacc_storage20_binding STATIC ${tp_native}/storage20_binding.c)
  target_link_libraries(papacc_storage20_binding PUBLIC papacc_tpm_native_core papacc_configuration_commit)
  target_compile_options(papacc_storage20_binding PRIVATE /W4 /WX /GS /sdl)
  add_executable(papacc_storage20_delivery_test tests/storage20_delivery_test.c)
  target_link_libraries(papacc_storage20_delivery_test PRIVATE papacc_tpm_native_core)
  target_compile_options(papacc_storage20_delivery_test PRIVATE /W4 /WX /GS /sdl)
  add_executable(papacc_storage20_root_binding_test tests/storage20_root_binding_test.c)
  target_link_libraries(papacc_storage20_root_binding_test PRIVATE papacc_storage20_binding)
  target_compile_options(papacc_storage20_root_binding_test PRIVATE /W4 /WX /GS /sdl)
  add_test(NAME papacc.storage20.delivery_mock COMMAND papacc_storage20_delivery_test)
  add_test(NAME papacc.storage20.root_binding_mock COMMAND papacc_storage20_root_binding_test)
 endif()
 if(BUILD_TESTING)
  add_executable(papacc_tpm_native_core_test ${tp_native}/proof_test.c)
  target_link_libraries(papacc_tpm_native_core_test PRIVATE papacc_tpm_native_core)
  target_compile_options(papacc_tpm_native_core_test PRIVATE /W4 /WX /GS /sdl)
  add_executable(papacc_tpm_native_platform_test ${tp_native}/platform_test.c)
  target_link_libraries(papacc_tpm_native_platform_test PRIVATE papacc_tpm_native_core advapi32 crypt32 wevtapi user32)
  target_compile_options(papacc_tpm_native_platform_test PRIVATE /W4 /WX /GS /sdl)
  add_executable(papacc_tpm_native_artifact_test ${tp_native}/artifact_test.c)
  target_link_libraries(papacc_tpm_native_artifact_test PRIVATE papacc_tpm_native_core advapi32 crypt32 wevtapi user32)
  target_compile_options(papacc_tpm_native_artifact_test PRIVATE /W4 /WX /GS /sdl)
  add_executable(papacc_tpm_native_admin_fixture ${tp_native}/admin_native_fixture.c)
  target_link_libraries(papacc_tpm_native_admin_fixture PRIVATE papacc_tpm_native_core advapi32 crypt32 wevtapi user32)
  target_compile_options(papacc_tpm_native_admin_fixture PRIVATE /W4 /WX /GS /sdl)
  set_target_properties(papacc_tpm_native_admin_fixture PROPERTIES C_STANDARD 11 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
  add_executable(papacc_tpm_native_e2e_fixture ${tp_native}/host_e2e_fixture.c)
  target_link_libraries(papacc_tpm_native_e2e_fixture PRIVATE papacc_tpm_native_core advapi32 crypt32 wevtapi user32)
  target_compile_options(papacc_tpm_native_e2e_fixture PRIVATE /W4 /WX /GS /sdl)
  add_test(NAME papacc.tpm_native.artifacts_faults COMMAND ${CMAKE_COMMAND}
   -DEXECUTABLE=$<TARGET_FILE:papacc_tpm_native_artifact_test> -DBASE=${CMAKE_CURRENT_BINARY_DIR}/Testing
   -DCERTIFICATE=${CMAKE_CURRENT_SOURCE_DIR}/${tp_native}/fixtures/ek-certificate.PUBLIC.der
   -DMODE=artifacts -P ${CMAKE_CURRENT_SOURCE_DIR}/${tp_native}/run_fixture.cmake)
  option(PAPACC_TPM_NATIVE_HOST_BOUND_TESTS "Run isolated metadata/console fixtures bound to the observed host; never TBS" OFF)
  if(PAPACC_TPM_NATIVE_HOST_BOUND_TESTS)
   add_test(NAME papacc.tpm_native.real_admin_graph COMMAND ${CMAKE_COMMAND}
    -DEXECUTABLE=$<TARGET_FILE:papacc_tpm_native_admin_fixture> -DBASE=${CMAKE_CURRENT_BINARY_DIR}/Testing
    -DMODE=admin -P ${CMAKE_CURRENT_SOURCE_DIR}/${tp_native}/run_fixture.cmake)
   add_test(NAME papacc.tpm_native.role_files_readonly COMMAND papacc_tpm_native_platform_test --role-files-readonly)
   add_test(NAME papacc.tpm_native.metadata_readonly COMMAND papacc_tpm_native_platform_test --current-metadata-readonly)
   add_test(NAME papacc.tpm_native.console_mock_e2e COMMAND ${CMAKE_COMMAND}
    -DEXECUTABLE=$<TARGET_FILE:papacc_tpm_native_e2e_fixture> -DBASE=${CMAKE_CURRENT_BINARY_DIR}/Testing
    -DMODE=console -P ${CMAKE_CURRENT_SOURCE_DIR}/${tp_native}/run_fixture.cmake)
  endif()
  add_test(NAME papacc.tpm_native.protocol_offline COMMAND papacc_tpm_native_core_test ${CMAKE_CURRENT_SOURCE_DIR}/${tp_native}/fixtures/csharp-vectors.bin)
  add_test(NAME papacc.tpm_native.guards_offline COMMAND papacc_tpm_native_platform_test)
  add_test(NAME papacc.tpm_native.selfcheck_no_tpm COMMAND papacc_tpm_native_proof --selfcheck)
  add_test(NAME papacc.tpm_native.default_deny COMMAND papacc_tpm_native_proof)
  set_tests_properties(papacc.tpm_native.default_deny PROPERTIES WILL_FAIL TRUE)
 endif()
endif()

if(PAPACC_ENABLE_NATIVE_TPM_PROOF_CANDIDATE)
 foreach(t papacc_tpm_native_proof papacc_tpm_native_core_test papacc_tpm_native_platform_test papacc_tpm_native_artifact_test papacc_tpm_native_e2e_fixture)
  if(TARGET ${t})
   set_target_properties(${t} PROPERTIES C_STANDARD 11 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
  endif()
 endforeach()
endif()

# Focal take-image qualification: actual native helper, all protected APIs modeled.
if(TARGET papacc_scq_first_controller_test)
 add_executable(papacc_scq_start_take_image_test tests/scq_start_take_image_test.c)
 target_include_directories(papacc_scq_start_take_image_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} apps/server include src/config src/issuer)
 target_compile_definitions(papacc_scq_start_take_image_test PRIVATE PAPACC_C8_PRIVATE_STORAGE_PROFILE=1)
 target_link_libraries(papacc_scq_start_take_image_test PRIVATE papacc_server_service ncrypt advapi32 bcrypt)
 target_compile_options(papacc_scq_start_take_image_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal.scq_start_take_image COMMAND papacc_scq_start_take_image_test)
 set_property(SOURCE apps/server/server_configuration_commit_win32.c tests/scq_start_take_image_test.c APPEND PROPERTY OBJECT_DEPENDS
 "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/personal_scq_start_take_win32.inc;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/personal_scq_resource_failure_win32.inc")
endif()

# Post-Start progress diagnostics: actual bodies, modeled native edges only.
if(TARGET papacc_scq_first_controller_test)
 add_executable(papacc_scq_start_progress_test tests/scq_start_progress_test.c)
 target_include_directories(papacc_scq_start_progress_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} apps/server include src/config src/issuer)
 target_compile_definitions(papacc_scq_start_progress_test PRIVATE PAPACC_C8_PRIVATE_STORAGE_PROFILE=1)
 target_link_libraries(papacc_scq_start_progress_test PRIVATE papacc_server_service ncrypt advapi32 bcrypt)
 target_compile_options(papacc_scq_start_progress_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal.scq_start_progress COMMAND papacc_scq_start_progress_test)
 set_property(SOURCE apps/server/server_configuration_commit_win32.c tests/scq_start_progress_test.c APPEND PROPERTY OBJECT_DEPENDS
 "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/personal_scq_start_progress_win32.inc;${CMAKE_CURRENT_SOURCE_DIR}/apps/server/personal_scq_resource_failure_win32.inc")
 set_property(SOURCE apps/server/server_launcher_owner_win32.c tests/scq_start_progress_test.c APPEND PROPERTY OBJECT_DEPENDS
 "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/personal_scq_start_wait_win32.inc")
endif()

if(TARGET papacc_personal_runtime_identity_test)
 add_executable(papacc_scq_start_runtime_diagnostic_test tests/scq_start_runtime_diagnostic_test.c)
 target_include_directories(papacc_scq_start_runtime_diagnostic_test PRIVATE apps/server include)
 target_link_libraries(papacc_scq_start_runtime_diagnostic_test PRIVATE ncrypt advapi32)
 target_compile_options(papacc_scq_start_runtime_diagnostic_test PRIVATE /W4 /WX)
 add_test(NAME papacc.personal.scq_start_runtime_diagnostic COMMAND papacc_scq_start_runtime_diagnostic_test)
 set_property(SOURCE tests/scq_start_runtime_diagnostic_test.c APPEND PROPERTY OBJECT_DEPENDS
 "${CMAKE_CURRENT_SOURCE_DIR}/apps/server/personal_runtime_identity_win32.inc")
endif()

if(TARGET papacc_personal_runtime_identity_test)
 add_executable(papacc_scq_runtime_token_shape_test tests/scq_runtime_token_shape_test.c)
 target_include_directories(papacc_scq_runtime_token_shape_test PRIVATE apps/server include)
 target_link_libraries(papacc_scq_runtime_token_shape_test PRIVATE ncrypt advapi32)
 target_compile_options(papacc_scq_runtime_token_shape_test PRIVATE /W4 /WX)
 add_test(NAME papacc.scq.runtime_token_shape COMMAND papacc_scq_runtime_token_shape_test)
endif()

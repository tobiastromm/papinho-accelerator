cmake_minimum_required(VERSION 3.16)

project(PapinhoAccelerator VERSION 0.1.0 LANGUAGES C)

if(MSVC AND CMAKE_GENERATOR MATCHES "Ninja")
    set(CMAKE_C_COMPILER_LAUNCHER
        "${CMAKE_CURRENT_SOURCE_DIR}/tools/msvc-ninja-utf8.cmd")
    set(CMAKE_CL_SHOWINCLUDES_PREFIX "Observa??o: incluindo arquivo: ")
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
add_library(papacc_issuer STATIC src/issuer/issuer_core.c src/issuer/issuer_codec.c)
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
    if(WIN32)
    add_executable(papacc_issuer_checkpoint_test tests/issuer_checkpoint_test.c)
    target_link_libraries(papacc_issuer_checkpoint_test PRIVATE papacc_issuer bcrypt)
    set_target_properties(papacc_issuer_checkpoint_test PROPERTIES C_STANDARD 90 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
    papacc_enable_warnings(papacc_issuer_checkpoint_test)
    add_test(NAME papacc.issuer.checkpoint COMMAND papacc_issuer_checkpoint_test)
    endif()
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
 add_executable(papacc_canonical_root_transaction_test tests/canonical_root_transaction_test.c)
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
    add_library(papacc_config_root8 STATIC src/config/config_root8.c src/config/config_root8_v2.c src/config/config_root8_v3.c)
    target_include_directories(papacc_config_root8 PUBLIC src/config include)
    target_link_libraries(papacc_config_root8 PUBLIC papacc_config_lst1 papacc_issuer)
    set_target_properties(papacc_config_root8 PROPERTIES C_STANDARD 90 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
    papacc_enable_warnings(papacc_config_root8)
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
    target_link_libraries(papacc_server PRIVATE papacc_config_source)
    target_compile_definitions(papacc_server PRIVATE PAPACC_ENABLE_CONFIG_ADMIN_CLI=1)
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
 target_link_libraries(papacc_phase6_memory_census_test PRIVATE papacc_c2_root_owner)
 target_compile_options(papacc_phase6_memory_census_test PRIVATE /W4 /WX)
 papacc_stage_pst_runtime(papacc_phase6_memory_census_test)
 add_test(NAME papacc.phase6.memory_census COMMAND papacc_phase6_memory_census_test)
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
  target_link_libraries(papacc_issuer_cng_x509_test PRIVATE crypt32)
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

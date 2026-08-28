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

add_library(papacc_network STATIC
    src/network/bind_selection.c
    src/network/bind_target.c
    src/network/ip_address.c
    src/network/network_endpoint.c
    src/network/network_interface.c
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

add_library(papacc_protocol_framing STATIC
    src/protocol/frame.c
    src/protocol/frame_parser.c
)

target_include_directories(papacc_protocol_framing
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/protocol
)

target_link_libraries(papacc_protocol_framing PUBLIC papacc_core)

set_target_properties(papacc_protocol_framing PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED YES
    C_EXTENSIONS NO
)

papacc_enable_warnings(papacc_protocol_framing)

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

add_library(papacc_control_protocol STATIC
    src/protocol/control_protocol.c
)
target_include_directories(papacc_control_protocol PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/protocol)
target_link_libraries(papacc_control_protocol PUBLIC papacc_protocol_framing)
set_target_properties(papacc_control_protocol PROPERTIES
    C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
papacc_enable_warnings(papacc_control_protocol)

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
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/apps/server
        ${CMAKE_CURRENT_SOURCE_DIR}/src/network
)

target_link_libraries(papacc_server_cli PUBLIC papacc_server_config)

set_target_properties(papacc_server_cli PROPERTIES
    C_STANDARD 99
    C_STANDARD_REQUIRED YES
    C_EXTENSIONS NO
)

papacc_enable_warnings(papacc_server_cli)

if(WIN32)
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
    )
    target_include_directories(papacc_server_io_loop_win32
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/apps/server
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
        papacc_server_acceptor_win32 papacc_control_processor papacc_pal)
    set_target_properties(papacc_server_io_loop_win32 PROPERTIES
        C_STANDARD 99 C_STANDARD_REQUIRED YES C_EXTENSIONS NO)
    papacc_enable_warnings(papacc_server_io_loop_win32)

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
endif()

if(BUILD_TESTING)
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

if(NOT DEFINED PAPACC_SOURCE_ROOT OR NOT DEFINED PAPACC_BINARY_ROOT)
    message(FATAL_ERROR "PAPACC_SOURCE_ROOT and PAPACC_BINARY_ROOT are required")
endif()

set(package_root "${PAPACC_BINARY_ROOT}/papinhopacc-package/PapinhoPACC-1.1.0")
file(MAKE_DIRECTORY "${package_root}/include/papacc/pacc" "${package_root}/src"
    "${package_root}/tests")
file(COPY "${PAPACC_SOURCE_ROOT}/include/papacc/types.h"
    DESTINATION "${package_root}/include/papacc")
file(COPY
    "${PAPACC_SOURCE_ROOT}/include/papacc/pacc/capability_wire.h"
    "${PAPACC_SOURCE_ROOT}/include/papacc/pacc/frame.h"
    "${PAPACC_SOURCE_ROOT}/include/papacc/pacc/frame_parser.h"
    "${PAPACC_SOURCE_ROOT}/include/papacc/pacc/message_types.h"
    "${PAPACC_SOURCE_ROOT}/include/papacc/pacc/control_protocol.h"
    "${PAPACC_SOURCE_ROOT}/include/papacc/pacc/data_association_protocol.h"
    "${PAPACC_SOURCE_ROOT}/include/papacc/pacc/capability_negotiation_protocol.h"
    "${PAPACC_SOURCE_ROOT}/include/papacc/pacc/network_offload_protocol.h"
    "${PAPACC_SOURCE_ROOT}/include/papacc/pacc/version.h"
    DESTINATION "${package_root}/include/papacc/pacc")
file(COPY
    "${PAPACC_SOURCE_ROOT}/src/protocol/frame.c"
    "${PAPACC_SOURCE_ROOT}/src/protocol/frame_parser.c"
    "${PAPACC_SOURCE_ROOT}/src/protocol/control_protocol.c"
    "${PAPACC_SOURCE_ROOT}/src/protocol/data_association_protocol.c"
    "${PAPACC_SOURCE_ROOT}/src/protocol/capability_wire.c"
    "${PAPACC_SOURCE_ROOT}/src/protocol/capability_negotiation_protocol.c"
    "${PAPACC_SOURCE_ROOT}/src/protocol/network_offload_protocol.c"
    DESTINATION "${package_root}/src")
file(COPY
    "${PAPACC_SOURCE_ROOT}/tools/papinhopacc-package/CMakeLists.txt"
    "${PAPACC_SOURCE_ROOT}/tools/papinhopacc-package/README.md"
    DESTINATION "${package_root}")
file(COPY "${PAPACC_SOURCE_ROOT}/tools/papinhopacc-package/consumer_smoke.c"
    DESTINATION "${package_root}/tests")
set(archive "${PAPACC_BINARY_ROOT}/papinhopacc-package/PapinhoPACC-1.1.0.zip")
execute_process(COMMAND "${CMAKE_COMMAND}" -E tar cf "${archive}"
    --format=zip PapinhoPACC-1.1.0
    WORKING_DIRECTORY "${PAPACC_BINARY_ROOT}/papinhopacc-package"
    RESULT_VARIABLE archive_result)
if(NOT archive_result EQUAL 0)
    message(FATAL_ERROR "PapinhoPACC archive creation failed")
endif()
file(SHA256 "${archive}" archive_sha256)
file(WRITE "${archive}.sha256" "${archive_sha256}  PapinhoPACC-1.1.0.zip\n")
message(STATUS "PapinhoPACC source package: ${package_root}")

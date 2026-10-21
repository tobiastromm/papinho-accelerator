#ifndef PAPACC_PACC_VERSION_H
#define PAPACC_PACC_VERSION_H

/* C API version; independent of package release and on-wire version fields. */
#define PAPACC_PACC_API_VERSION_MAJOR 1U
#define PAPACC_PACC_API_VERSION_MINOR 1U
#define PAPACC_PACC_API_VERSION_PATCH 0U

/* External contract generation. No generation field is added to the wire. */
#define PAPACC_PACC_WIRE_CONTRACT_GENERATION 2U

/* Existing, distinct on-wire Envelope and CONTROL version fields. */
#define PAPACC_PACC_WIRE_ENVELOPE_MAJOR 1U
#define PAPACC_PACC_WIRE_ENVELOPE_MINOR 0U
#define PAPACC_PACC_WIRE_CONTROL_MAJOR 1U
#define PAPACC_PACC_WIRE_CONTROL_MINOR 0U

#endif

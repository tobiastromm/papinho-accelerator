#ifndef PAPACC_SERVER_SERVICE_STARTUP_WIN32_H
#define PAPACC_SERVER_SERVICE_STARTUP_WIN32_H
/* Private productive-admission report: closed preparation is a separate
 * original-evidence path, never approval, constructor or authority setter. */
#include <windows.h>
enum { PS_STARTUP_ORIGINAL_UNAVAILABLE=1U,
 PS_STARTUP_ORIGINAL_UNDESIGNATED=PS_STARTUP_ORIGINAL_UNAVAILABLE, /* historical value */
 PS_STARTUP_WRONG_LAB_SCOPE,
 PS_STARTUP_WRONG_THREAD,PS_STARTUP_STOPPED,PS_STARTUP_RETAINED_UNKNOWN,
 PS_STARTUP_POST_RUNNING_CIRCULAR };
enum { PS_STARTUP_IMAGE_METADATA=1U,PS_STARTUP_SCM_METADATA=2U,
 PS_STARTUP_HOST_METADATA=4U,PS_STARTUP_RUNNING_METADATA=8U };
enum { PS_STARTUP_NEEDS_ORIGINAL=1U,PS_STARTUP_NEEDS_INSTALLATION=2U,
 PS_STARTUP_NEEDS_SCOPE=4U,PS_STARTUP_NEEDS_LIFETIME=8U,
 PS_STARTUP_NEEDS_READINESS_EVIDENCE=16U,
 PS_STARTUP_NEEDS_READINESS_CONTRACT=PS_STARTUP_NEEDS_READINESS_EVIDENCE /* historical diagnostic bit */ };
typedef struct PAPACC_SERVICE_STARTUP_SOURCE_REPORT {
 DWORD reason,first_reason,available_metadata,missing_source_fields;
} PAPACC_SERVICE_STARTUP_SOURCE_REPORT;
/* Resolves available native owner observations, NOT startup authorization.
 * Returns DENY until a governed original source is implemented and independently delivered. */
DWORD papacc_server_service_startup_source_status(PAPACC_SERVICE_STARTUP_SOURCE_REPORT *);
/* Diagnostic Start has independent lifecycle authority. These missing terms
 * are not inferred from a stopped-registration receipt or from SCQ DATA.
 * Runtime identity and bilateral readiness are later obligations, never
 * pre-Start PID/token prerequisites. Observation only; no effect or grant. */
enum { PS_DIAGNOSTIC_START_NEEDS_LIFECYCLE_ORIGINAL=1U,
 PS_DIAGNOSTIC_START_NEEDS_CONTROLLER_ORIGINAL=2U,
 PS_DIAGNOSTIC_START_NEEDS_DELIVERY_CONTINUATION=4U,
 PS_DIAGNOSTIC_START_NEEDS_RESOURCE_PLAN=8U };
enum { PS_DIAGNOSTIC_START_FRESH_RUNTIME_IDENTITY=1U,
 PS_DIAGNOSTIC_START_FRESH_BILATERAL=2U,
 PS_DIAGNOSTIC_START_FRESH_CLOSED_READINESS=4U,
 PS_DIAGNOSTIC_START_FRESH_ORIGINAL_CLOSE=8U };
/* Material preconditions of THIS diagnostic Start. No imported staging or
 * stopped-registration risk allowance. Effect evidence is tracked separately
 * so that a future PID/receipt is not required before Start can be admitted. */
enum { PS_START_RESOURCE_PROCESS_ENVELOPE=1U,PS_START_RESOURCE_RPC_TRANSIENT=2U,
 PS_START_RESOURCE_HOST_MARGIN=4U,PS_START_RESOURCE_CLOCK_POLICY=8U,
 PS_START_RESOURCE_RETENTION_ENVELOPE=16U,PS_START_RESOURCE_PARTICIPANT_HANDLES=32U };
enum { PS_START_RESOURCE_EFFECT_RUNTIME=1U,PS_START_RESOURCE_EFFECT_ENDPOINT=2U,
 PS_START_RESOURCE_EFFECT_STOP_CLOSE=4U };
typedef struct PAPACC_SERVICE_DIAGNOSTIC_START_REPORT {
 DWORD reason,missing_pre_start,later_obligations,available_metadata;
 DWORD resource_pre_missing,resource_effect_obligations;
 DWORD initial_receiver_source_unavailable; /* factual build gap, never an input/grant */
 DWORD limited_assurance,limited_eligible; /* G0-G9, not integral provision */
 DWORD original_checked,original_error,resource_checked,resource_error; /* diagnostic results of existing queries only */
 SIZE_T scm_compiled_storage_bytes,carrier_binary_bytes,carrier_text_characters;
 SIZE_T scq_input_storage_bytes,scq_image_chain_storage_bytes;
 SIZE_T controller_plan_storage_bytes; /* one explicit stack object, not total stack/native provision */
 DWORD scq_image_chain_handle_slots; /* controlled slots, not process peak/provision */
} PAPACC_SERVICE_DIAGNOSTIC_START_REPORT;
DWORD papacc_server_service_diagnostic_start_status(PAPACC_SERVICE_DIAGNOSTIC_START_REPORT *);
/* Registration is an earlier, separate action. No runtime PID/token/readiness
 * can exist as a prerequisite of CreateService. These are missing-source
 * diagnostics only; no setter, loader or positive approval constructor. */
enum { PS_REGISTRATION_ORIGINAL_UNAVAILABLE=1U,PS_REGISTRATION_WRONG_LAB_SCOPE,
 PS_REGISTRATION_WRONG_THREAD,PS_REGISTRATION_STOPPED,
 PS_REGISTRATION_RETAINED_UNKNOWN,PS_REGISTRATION_CONSUMED,
 PS_REGISTRATION_INVALID_PROFILE,PS_REGISTRATION_RESOURCES_UNQUALIFIED,PS_REGISTRATION_RESOURCE_OBSERVATION_FAILED,
 PS_REGISTRATION_LIMITED_ELIGIBLE };
enum { PS_REGISTRATION_NEEDS_ORIGINAL=1U,PS_REGISTRATION_NEEDS_TARGET_IMAGE=2U,
 PS_REGISTRATION_NEEDS_EXACT_SCOPE=4U,PS_REGISTRATION_NEEDS_LIVE_ACT=8U,
 PS_REGISTRATION_NEEDS_RESOURCES=16U };
typedef struct PAPACC_SERVICE_REGISTRATION_SOURCE_REPORT {
 DWORD reason,missing_source_fields,selected_profile,image_metadata;
 DWORD resource_observations,resource_error,resource_missing;
 DWORD resource_pre_missing,resource_effect_missing;
} PAPACC_SERVICE_REGISTRATION_SOURCE_REPORT;
DWORD papacc_server_service_registration_source_status(PAPACC_SERVICE_REGISTRATION_SOURCE_REPORT *);
#endif

#ifndef PAPACC_PERSONAL_SCM_REGISTRATION_WIN32_H
#define PAPACC_PERSONAL_SCM_REGISTRATION_WIN32_H
#include <windows.h>
#include <winternl.h>
#include <stdio.h>
/* Source-phase observations, never ceilings, reservations or approval. */
enum { PAPACC_SCM_RESOURCE_ENTRY=1U,PAPACC_SCM_RESOURCE_ORIGINALS,
 PAPACC_SCM_RESOURCE_CONFIRMED,PAPACC_SCM_RESOURCE_PRE_CREATE,
 PAPACC_SCM_RESOURCE_BEFORE_CLOSE,PAPACC_SCM_RESOURCE_CLOSED,PAPACC_SCM_RESOURCE_PRE_CHANGE };
enum { PAPACC_SCM_ASSURANCE_STRICT=0U,PAPACC_SCM_ASSURANCE_LIMITED_R0_R9=1U };
/* First-cause metadata only: never an admission input or object reference. */
enum { SCM_DIAG_POLICY=1U,SCM_DIAG_WIN32,SCM_DIAG_NTSTATUS,SCM_DIAG_RESULT,SCM_DIAG_HELPER_UNKNOWN,SCM_DIAG_SHORT_OUTPUT };
enum { SCM_D_ENTRY=1U,SCM_D_RESERVATION,SCM_D_LAYOUT_PREPARE,SCM_D_TARGET_PATH,
 SCM_D_PARENT_OPEN,SCM_D_PARENT_SD,SCM_D_EXE_OPEN,SCM_D_EXE_SD,
 SCM_D_DEPENDENCY_OPEN,SCM_D_DEPENDENCY_REFERENCE,SCM_D_DEPENDENCY_QUERY,
 SCM_D_DEPENDENCY_ATTRIBUTES,SCM_D_DEPENDENCY_VOLUME,SCM_D_DEPENDENCY_SIZE,
 SCM_D_DEPENDENCY_SD,SCM_D_DEPENDENCY_HASH,SCM_D_DEPENDENCY_ALIAS,
 SCM_D_DEPENDENCY_IDENTITY,SCM_D_DEPENDENCY_SD_CHANGED,SCM_D_DEPENDENCY_HASH_CHANGED,
 SCM_D_ORIGINALS_SAMPLE,SCM_D_RNG_API,SCM_D_RNG_ZERO,SCM_D_OWNER,
 SCM_D_CONSUMPTION_STATE,SCM_D_STOP,SCM_D_SCOPE,SCM_D_RECIPIENT,SCM_D_IMAGE_REFERENCE,
 SCM_D_CAPTURED_IDENTITY,SCM_D_PATH,SCM_D_COMMAND,SCM_D_CAPTURED_DIGEST,
 SCM_D_LAYOUT_CURRENT,SCM_D_NATIVE_CURRENT,SCM_D_RELATIVE_REFERENCE,
 SCM_D_ORIGINAL_QUERY,SCM_D_RELATIVE_QUERY,SCM_D_ORIGINAL_IDENTITY,
 SCM_D_RELATIVE_IDENTITY,SCM_D_IMAGE_ATTRIBUTES,SCM_D_IMAGE_SD,SCM_D_IMAGE_SD_CHANGED,
 SCM_D_IMAGE_HASH,SCM_D_IMAGE_HASH_CHANGED,SCM_D_PACKAGE,SCM_D_PLAN_FIRST_WRITE,
 SCM_D_PLAN_OUTPUT,SCM_D_CONFIRMATION,SCM_D_CONFIRMED_SAMPLE };
enum { PAPACC_SCM_RESOURCE_MISSING_PRE_ENTRY=1U,
 PAPACC_SCM_RESOURCE_MISSING_TRANSITIVE=2U,PAPACC_SCM_RESOURCE_MISSING_HOST_POLICY=4U,
 PAPACC_SCM_RESOURCE_MISSING_RETENTION_ENVELOPE=8U,PAPACC_SCM_RESOURCE_MISSING_ADMIN_COHORT=16U,
 PAPACC_SCM_RESOURCE_MISSING_SCM_ACTIVE=32U,PAPACC_SCM_RESOURCE_MISSING_OBSERVATION=64U,
 PAPACC_SCM_RESOURCE_MISSING_ALL=127U };
/* Diagnostic timing only. No evidence of the future SCM effect is required
 * before its own submission; this split never supplies admission authority. */
#define PAPACC_SCM_RESOURCE_EFFECT_EVIDENCE PAPACC_SCM_RESOURCE_MISSING_SCM_ACTIVE
#define PAPACC_SCM_RESOURCE_PRECONDITIONS (PAPACC_SCM_RESOURCE_MISSING_ALL & ~PAPACC_SCM_RESOURCE_EFFECT_EVIDENCE)
typedef struct PAPACC_SCM_RESOURCE_SAMPLE {
 ULONGLONG frequency,begin,end,private_bytes,working_set,physical_available,commit_available;
 DWORD handles,phase;
} PAPACC_SCM_RESOURCE_SAMPLE;
typedef struct PAPACC_SCM_RESOURCE_OBSERVATION {
 PAPACC_SCM_RESOURCE_SAMPLE first,last;
 ULONGLONG peak_private,peak_working,min_physical,min_commit;
 DWORD samples,attempts,peak_handles,first_error,error_stage;
 FILE *sink; /* same synchronous invocation, nonsecret evidence only */
} PAPACC_SCM_RESOURCE_OBSERVATION;
typedef struct PAPACC_SCM_RESOURCE_REPORT {
 DWORD samples,first_error,missing;
 ULONGLONG peak_private,minimum_physical,minimum_commit,frequency,begin,end;
 DWORD peak_handles;
 DWORD pre_missing,effect_missing;
 DWORD assurance,limited_eligible;
 ULONGLONG controlled_bytes,host_floor;
} PAPACC_SCM_RESOURCE_REPORT;
/* Private same-invocation borrows, never serialized or accepted from IPC. */
typedef struct PAPACC_SERVICE_IMAGE_VIEW {
 HANDLE handle;BY_HANDLE_FILE_INFORMATION identity;const wchar_t *path;
 const unsigned char *sha256;DWORD owner_thread;
} PAPACC_SERVICE_IMAGE_VIEW;
typedef struct PAPACC_SERVICE_REGISTRATION_VIEW {
 PAPACC_SERVICE_IMAGE_VIEW image;const void *recipient;
 const WCHAR *command,*service,*account;DWORD start_type,sid_type;
} PAPACC_SERVICE_REGISTRATION_VIEW;
/* Substate of the EXISTING canonical packet. No independently owned handles,
 * approval loader, reusable grant, new owner or persisted representation. */
typedef struct PAPACC_SCM_DEPENDENCY_REFERENCE {
 HANDLE handle;BY_HANDLE_FILE_INFORMATION identity;BYTE digest[32],acl[32];
} PAPACC_SCM_DEPENDENCY_REFERENCE;
typedef struct PAPACC_SCM_REGISTRATION_ACT {
 struct PAPACC_SCM_REGISTRATION_ACT *self;
 const PAPACC_SERVICE_REGISTRATION_VIEW *original;
 PAPACC_SERVICE_REGISTRATION_VIEW captured;
 ULONGLONG invocation;DWORD thread,phase,first;unsigned consumed;
 BYTE attempt[32],digest[32],image_acl[32];
 WCHAR path[MAX_PATH],command[MAX_PATH+32];
 PAPACC_SCM_RESOURCE_OBSERVATION resources;
 PAPACC_SCM_DEPENDENCY_REFERENCE dependencies[2];
 IO_STATUS_BLOCK original_io; /* durable backing on unexpected pending open */
 unsigned package_captured,dependency_close_attempts;
 unsigned assurance;
 unsigned effect_state[2]; /* 0 absent, 1 submitted, 2 returned; no rearm */
 ULONGLONG effect_begin[2],effect_end[2];
 DWORD effect_result[2];
 DWORD diagnostic_stage,diagnostic_kind,diagnostic_value,diagnostic_object,diagnostic_phase;
 DWORD progress_stage,progress_object;
 DWORD diagnostic_native_stage,diagnostic_native_kind,diagnostic_native_error;
} PAPACC_SCM_REGISTRATION_ACT;
#endif

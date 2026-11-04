#ifndef PAPACC_PERSONAL_NATIVE_WIN32_H
#define PAPACC_PERSONAL_NATIVE_WIN32_H
#include <windows.h>
#include "server_console_win32.h"
#include "personal_runtime_identity_win32.h"
#include "personal_cng_probe_win32.h"
#include "personal_install_paths_win32.h"
#include "personal_scq_dispatch_win32.h"
#include "personal_scq_handoff_win32.h"
#include "personal_scq_inputs_win32.h"
#include "personal_scq_profile_win32.h"
#include "personal_scq_admin_win32.h"
#include "personal_scq_start_assurance_win32.h"
#include "personal_scq_cold_start_win32.h"
#include "personal_scq_session_win32.h"
#include "personal_scq_recipient_win32.h"
#include "personal_scm_registration_win32.h"
#include "personal_scq_read_diagnostic_win32.h"
#include "personal_machine_fixture_win32.h"
#include "personal_qualification_staging_win32.h"
#include "personal_staged_update_win32.h"
/* Same canonical packet owns every handle/allocation through factual close. */
#define PAPACC_PERSONAL_HANDOFF_SUBMITTED 1L
#define PAPACC_PERSONAL_HANDOFF_VERIFIED 2L
#define PAPACC_PERSONAL_HANDOFF_DENIED 3L
#define PAPACC_PERSONAL_HANDOFF_RETAINED 4L
/* First cause only, private diagnostic DATA. No extra query, authority or wire. */
typedef struct PAPACC_SCQ_COLD_DIAGNOSTIC {
 DWORD stage,reference,mapped,native_error;
 DWORD initial_known,initial_handles; /* first existing query; never overwritten */
 DWORD query_attempted,query_succeeded,query_reference,own_count;
 DWORD open_attempted,open_reference,open_status,io_status,returned_handle;
 ULONGLONG io_information; /* initialized IOSB snapshot, not validity on NT failure */
 PAPACC_SCQ_NATIVE_SD_STATUS native_sd;
 DWORD plan_attempted,plan_passed,plan_rejected,unbound_reason,unbound_field,window_known;
 ULONGLONG plan_from,plan_until,plan_now; /* existing final clock, never renewed */
} PAPACC_SCQ_COLD_DIAGNOSTIC;
typedef struct PAPACC_PERSONAL_NATIVE_CONTEXT {
 PAPACC_QUALIFICATION_STAGING staging; /* no NEW_INSTALL/SCQ authority */
 PAPACC_STAGED_UPDATE_STATE staged_update; /* preparation, no staging/SCM grant */
 PAPACC_SCM_REGISTRATION_ACT scm_registration; /* local attributed register act, not SCQ */
 PAPACC_PERSONAL_RUNTIME_IDENTITY runtime_identity; /* identity DATA only */
 PAPACC_MACHINE_FIXTURE_CONTROL machine_fixture; /* TEST_ONLY identity, no grant */
 PAPACC_SCQ_READ_RECIPIENT scq_read_recipient; /* local origin, no transferred pointer */
 PAPACC_SCQ_READ_DIAGNOSTIC scq_read_diagnostic; /* independent P; not enabled by Start */
 PAPACC_PERSONAL_CNG_PROBE runtime_cng_probe; /* diagnostic, no key authority */
 PAPACC_PERSONAL_INSTALL_PATHS install_paths; /* frozen DATA, not object identity */
 PAPACC_PERSONAL_SCQ_DISPATCH scq_dispatch; /* no authority/intent */
 PAPACC_PERSONAL_SCQ_RECEIVER scq_receiver; /* original borrow, never a DATA grant */
 PAPACC_PERSONAL_SCQ_INPUTS scq_inputs; /* reference custody only, never approval */
 PAPACC_SCQ_SESSION_DRIVER scq_session; /* bounded sequence/transport custody, not authority */
 PAPACC_SCQ_RECIPIENT_ORIGIN scq_recipient; /* local QUERY/READ origin, no grant */
 IO_STATUS_BLOCK scq_cold_io;OBJECT_ATTRIBUTES scq_cold_attributes;UNICODE_STRING scq_cold_unicode;
 WCHAR scq_cold_name[MAX_PATH+4];NTSTATUS scq_cold_status;
 HANDLE scq_cold_raw[3];DWORD scq_cold_close; /* initial original references, same packet */
 PAPACC_SCQ_COLD_DIAGNOSTIC scq_cold_diagnostic;
 PAPACC_SCQ_START_ASSURANCE scq_start_assurance; /* G0-G9: no implicit capacity or authority */
 PAPACC_SCQ_ADMIN_SOURCE scq_admin; /* same packet, separate attributed SCQ act */
 PAPACC_SCQ_OWNER_DECISION scq_selection;unsigned scq_selection_captured; /* original-bound candidate, not a grant */
 PS_SCQ_CARRIER scq_carrier;unsigned scq_carrier_captured; /* untrusted discovery DATA only */
 PAPACC_SCQ_PROFILE_DATA scq_profile_data; /* fixed DATA profile; no endpoint or authority */
 /* Private read-only dispatch state; same packet, no authority or child ID. */
 volatile LONG handoff_state;unsigned char handoff_attempt[32];
 char plan_leaf[65]; /* fixed BEFORE presentation; never a child identity */
 TOKEN_STATISTICS token;BYTE sid[SECURITY_MAX_SID_SIZE];DWORD sid_bytes,session;
 BY_HANDLE_FILE_INFORMATION parent,source;
 HANDLE unexpected_thread,check_token;
 HANDLE scm_image; /* read-only image under the held SCM LAB parent */
 PSECURITY_DESCRIPTOR descriptor,check_descriptor;
 LPSTR sid_text;
 DWORD descriptor_bytes,error,close_attempts;
 DWORD diagnostic_first,diagnostic_error,diagnostic_kind; /* first failure only: API=1, predicate=2, NTSTATUS=3; no authority */
 unsigned presenter_phase,presenter_ca; /* receipt only: 1 shown,2 confirmed,3 cancelled,4 UNKNOWN; never authority */
 unsigned char presenter_attempt[32];
 unsigned phase; /*0 empty,1 live,2 retained,3 closed; never authority */
 PAPACC_SERVER_CONSOLE_WIN32 console;
} PAPACC_PERSONAL_NATIVE_CONTEXT;
#endif

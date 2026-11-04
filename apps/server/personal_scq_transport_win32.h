#ifndef PAPACC_PERSONAL_SCQ_TRANSPORT_WIN32_H
#define PAPACC_PERSONAL_SCQ_TRANSPORT_WIN32_H
#include <windows.h>
#include "server_service_data.h"
#define PAPACC_SCQ_CLOSE_BYTES (PAPACC_SCQ_RECEIPT_BYTES+24U)
/* Explicit owner approval62548c75… recorded before this private delta.
 * SCQ1/SQB1/SQR1/SQC1 and public/persisted formats stay byte-identical. */
#define PAPACC_SCQ_BOOTSTRAP_BYTES (PAPACC_SCQ_CLOSE_BYTES+8U)
/* Accepted SQB3/Q6 custody erratum, P1 only. Old sizes remain exact. */
#define PAPACC_SCQ_BOOTSTRAP3_BYTES (PAPACC_SCQ_BOOTSTRAP_BYTES+16U)
#define PAPACC_SCQ_BOOTSTRAP_READ_BYTES (PAPACC_SCQ_BOOTSTRAP3_BYTES+32U) /* private SQK1, K5 only */
#define PAPACC_SCQ_CLOSE3_BYTES (PAPACC_SCQ_CLOSE_BYTES+16U)
/* Private SCQ resource custody in the existing packet. No authority, loader,
 * remote HANDLE-as-authority serialization or implicit operational activation. */
typedef struct PAPACC_SCQ_TRANSPORT {
 const struct PAPACC_SCQ_TRANSPORT *self;const void *owner;
 struct PAPACC_PERSONAL_SCQ_ORIGINAL *original; /* local original backing only */
 DWORD thread,phase,first,operation,offset,total,pending,cancelled,close_attempted,legs,connect_started,applying,close_produced,close_accepted,query_only,close_confirmed;
 HANDLE pipe,event,files[3],delivery_process,remote_files[3],delivered_inputs[3];OVERLAPPED ov;
 ULONGLONG closed_values[3]; /* closed destination-table identifiers, not usable HANDLEs */
 WCHAR name[96];SECURITY_DESCRIPTOR security;BYTE acl[256];
 /* Bootstrap custody only. Candidate bytes/IDs never form an authorization.
  * Same packet/buffer/pipe; originals are promoted only after native checks. */
 const struct PAPACC_SCQ_OWNER_DECISION *capture_registration;
 PS_SCQ_CARRIER capture_carrier;
 HANDLE capture_controller,capture_token,capture_refs[3];DWORD capture_valid,capture_query;
 struct PAPACC_SCQ_START_ASSURANCE *resource_scope; /* same owner, never remote */
 DWORD read_diagnostic;BYTE read_public[32]; /* local typed QUERY metadata, never authority */
 DWORD query_refs3; /* local selected policy, never inferred from frame magic */
 HANDLE remote_query[2]; /* destination IDs, never local handle ownership */
 ULONGLONG closed_query[2]; /* closed original delivery IDs, not custody copies */
 ULONGLONG global_until; /* frozen G3 deadline, authenticated by private SQB2 */
 /* SQF1: retained diagnostic bytes, never overlap bootstrap or admission.
  * 56 bytes + 8 DWORDs = 88 bytes; no additional HANDLE/OVERLAPPED. */
 BYTE failure_frame[56];
 DWORD failure_stage,failure_error,failure_kind,failure_raw;
 DWORD failure_state,failure_pending,failure_secondary,failure_reported;
} PAPACC_SCQ_TRANSPORT;
#endif

#ifndef PAPACC_PERSONAL_SCQ_PROFILE_WIN32_H
#define PAPACC_PERSONAL_SCQ_PROFILE_WIN32_H
#include <windows.h>
#include <winsvc.h>
#define PAPACC_SCQ_PROFILE 2U
#define PAPACC_SCQ_PURPOSE_READ 1U
#define PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY 2U /* TO0-TO9: closed, canonical empty key, no KSP */
#define PAPACC_SCQ_SERVICE_PAPINHO 1U /* PapinhoAccelerator / NT SERVICE\\PapinhoAccelerator only */
#define PAPACC_SCQ_NON_SECRET_SCHEMA 1U
#define PAPACC_SCQ_FILE_READ_RIGHTS 0x00120089UL
#define PAPACC_SCQ_PEER_WORDS 30U
#define PAPACC_SCQ_REF_WORDS 9U
/* Private fixed record, not PACC/ROOT/CURRENT and not a native capability.
 * No local pointer, HANDLE, secret, free text or generic operation field. */
#define PAPACC_SCQ_RECORD_BYTES (36U+160U+112U+2U*(PAPACC_SCQ_PEER_WORDS*4U+100U)+3U*(PAPACC_SCQ_REF_WORDS*4U+64U))
#define PAPACC_SCQ_RECEIPT_BYTES (4U+PAPACC_SCQ_RECORD_BYTES)
typedef struct PAPACC_SCQ_PEER_DATA {
 DWORD pid,creation[2],token_id[2],authentication[2],modified[2];
 DWORD groups,enumerated,restricted,elevated,elevation_type,matches,attributes,sid_bytes,token_type;
 DWORD query_error,wait_status,exit_code;
 DWORD session,logon,administrator_attributes,service_type,service_state,service_sid_type;
 DWORD image[3];BYTE sid[68],image_digest[32];
} PAPACC_SCQ_PEER_DATA;
typedef struct PAPACC_SCQ_REF_DATA {
 DWORD role,rights,volume,high,low,size_low,size_high,attributes,links;
 BYTE digest[32],descriptor[32];
} PAPACC_SCQ_REF_DATA;
typedef struct PAPACC_SCQ_PLAN_DATA {
 DWORD version,profile,purpose,schema,service;
 DWORD valid_from[2],valid_until[2]; /* caller policy DATA; no default lifetime */
 BYTE attempt[32],policy[32],request[32],decision[32],target[32];
 char key[112];
 PAPACC_SCQ_PEER_DATA administrator,runtime;
 PAPACC_SCQ_REF_DATA refs[3];
} PAPACC_SCQ_PLAN_DATA;
typedef struct PAPACC_SCQ_PROFILE_DATA {
 const struct PAPACC_SCQ_PROFILE_DATA *self;
 const void *owner;
 DWORD thread,role,phase,first;
 volatile LONG taken,used,ack;
 DWORD waiting_runtime,pre_readiness,read_diagnostic; /* local mode only; not a persisted grant */
 BYTE record[PAPACC_SCQ_RECORD_BYTES];
} PAPACC_SCQ_PROFILE_DATA;
/* All APIs below are DATA/model transitions only. They cannot create an
 * endpoint, consume an operational approval or activate the KSP/SCM. */
DWORD papacc_scq_plan_encode(const PAPACC_SCQ_PLAN_DATA *,BYTE *,DWORD);
DWORD papacc_scq_plan_decode(const BYTE *,DWORD,PAPACC_SCQ_PLAN_DATA *);
DWORD papacc_scq_profile_prepare_data(PAPACC_SCQ_PROFILE_DATA *,const void *,DWORD,const PAPACC_SCQ_PLAN_DATA *,const PAPACC_SCQ_PEER_DATA *);
DWORD papacc_scq_profile_prepare_closed_data(PAPACC_SCQ_PROFILE_DATA *,const void *,DWORD,const PAPACC_SCQ_PLAN_DATA *,const PAPACC_SCQ_PEER_DATA *);
/* K0 private typed factory; DATA only, no original source or P authority. */
DWORD papacc_scq_profile_prepare_machine_read_data(PAPACC_SCQ_PROFILE_DATA *,const void *,DWORD,const PAPACC_SCQ_PLAN_DATA *,const PAPACC_SCQ_PEER_DATA *);
DWORD papacc_scq_profile_prepare_transport_data(PAPACC_SCQ_PROFILE_DATA *,const void *,DWORD,const PAPACC_SCQ_PLAN_DATA *,const PAPACC_SCQ_PEER_DATA *);
DWORD papacc_scq_profile_take_data(PAPACC_SCQ_PROFILE_DATA *,const void *,const PAPACC_SCQ_PEER_DATA *,const PAPACC_SCQ_PEER_DATA *,const PAPACC_SCQ_REF_DATA[3],volatile LONG *);
DWORD papacc_scq_profile_bind_runtime_data(PAPACC_SCQ_PROFILE_DATA *,const void *,const PAPACC_SCQ_PEER_DATA *,const PAPACC_SCQ_PEER_DATA *,const PAPACC_SCQ_REF_DATA[3],volatile LONG *);
DWORD papacc_scq_profile_offer_data(PAPACC_SCQ_PROFILE_DATA *,const void *,const PAPACC_SCQ_PEER_DATA *,const PAPACC_SCQ_PEER_DATA *,const PAPACC_SCQ_REF_DATA[3],BYTE *,DWORD,volatile LONG *);
DWORD papacc_scq_profile_receive_data(PAPACC_SCQ_PROFILE_DATA *,const void *,const PAPACC_SCQ_PEER_DATA *,const PAPACC_SCQ_PEER_DATA *,const PAPACC_SCQ_REF_DATA[3],const BYTE *,DWORD,volatile LONG *);
DWORD papacc_scq_profile_receipt_data(PAPACC_SCQ_PROFILE_DATA *,const void *,BYTE *,DWORD);
DWORD papacc_scq_profile_ack_data(PAPACC_SCQ_PROFILE_DATA *,const void *,const PAPACC_SCQ_PEER_DATA *,const PAPACC_SCQ_PEER_DATA *,const PAPACC_SCQ_REF_DATA[3],const BYTE *,DWORD,volatile LONG *);
DWORD papacc_scq_profile_close_data(PAPACC_SCQ_PROFILE_DATA *,const void *,int);
#endif

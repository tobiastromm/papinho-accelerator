#include "../../src/core/ed11_process_profile.h"
#ifndef PAPACC_SERVER_C8_RESOURCE_ADMISSION_WIN32_H
#define PAPACC_SERVER_C8_RESOURCE_ADMISSION_WIN32_H
/* Private by-value bookkeeping; never an issuer of measurement or grants. */
#include "papacc/types.h"
#define PAPACC_C8_RESOURCE_SLOTS 64U
#define PAPACC_C8_RESOURCE_LIMIT PAPACC_ED11_PROCESS_LIMIT
#define PAPACC_C8_RESOURCE_METRICS 9U
typedef enum PAPACC_C8_RESOURCE_REASON { PAPACC_C8_RESOURCE_OK=0,
 PAPACC_C8_RESOURCE_UNKNOWN,PAPACC_C8_RESOURCE_STALE,
 PAPACC_C8_RESOURCE_CAPACITY,PAPACC_C8_RESOURCE_STATE_ERROR } PAPACC_C8_RESOURCE_REASON;
typedef enum PAPACC_C8_RESOURCE_STATE { PAPACC_C8_RESOURCE_UNUSED=0,
 PAPACC_C8_RESOURCE_RESERVED=1,PAPACC_C8_RESOURCE_LIVE=2,
 PAPACC_C8_RESOURCE_RETAINED=3,PAPACC_C8_RESOURCE_RELEASED=4 } PAPACC_C8_RESOURCE_STATE;
typedef enum PAPACC_C8_EVIDENCE_CLASS { PAPACC_C8_EVIDENCE_UNKNOWN=0,
 PAPACC_C8_DIRECTLY_MEASURED=1,PAPACC_C8_BOUNDED_BY_CONTRACT=2,
 PAPACC_C8_EMPIRICALLY_QUALIFIED=3 } PAPACC_C8_EVIDENCE_CLASS;
/* Metric indexes: host physical/commit, process private, handles,
 * provider user/Connection, kernel/Connection, observed paged/nonpaged pool,
 * observed sockets. Optional observations remain explicitly UNKNOWN. */
typedef struct PAPACC_C8_RESOURCE_METRIC { PAPACC_U64 value;PAPACC_U32 kind,classification; } PAPACC_C8_RESOURCE_METRIC;
typedef struct PAPACC_C8_TARGET_EVIDENCE {
 PAPACC_U8 target[32],sdk_sha256[32],provenance[32];
 PAPACC_U64 source_id,sampled_ms,expires_ms,profile,baseline,peak,final_value;
 PAPACC_U32 version,api,spi,origin,phase,concurrency,retained,final_class;
 PAPACC_C8_RESOURCE_METRIC metrics[PAPACC_C8_RESOURCE_METRICS];
} PAPACC_C8_TARGET_EVIDENCE;
/* origin:0 absent,1 synthetic offline,2 native raw observation; not approval. */
typedef struct PAPACC_C8_RESOURCE_CONTEXT {
 PAPACC_U64 invocation,owner,connection,generation,revision,epoch,profile,source_id;
 PAPACC_U8 domain[32],target[32],sdk_sha256[32],provenance[32];
 PAPACC_U32 origin,phase,policy_current,connection_state;
 /* connection_state:1 reserved native lifetime,2 live,3 destroyed. */
} PAPACC_C8_RESOURCE_CONTEXT;
typedef struct PAPACC_C8_RESOURCE_KEY {
 PAPACC_U64 invocation,owner,connection,generation,ticket,revision,epoch;
 PAPACC_U8 domain[32];
} PAPACC_C8_RESOURCE_KEY;
/* Acquisition facts are distinct from attempted setup or resource absence. */
typedef enum PAPACC_C8_NATIVE_ACQUISITION { PAPACC_C8_NATIVE_UNKNOWN=0,
 NATIVE_SOCKET_NOT_ACQUIRED=1,NATIVE_SOCKET_ACQUIRED=2,NATIVE_SOCKET_DESTROYED=3 } PAPACC_C8_NATIVE_ACQUISITION;
typedef enum PAPACC_C8_PST_ACQUISITION { PAPACC_C8_PST_UNKNOWN=0,
 PST_CHILD_NOT_CREATED=1,PST_CHILD_CREATED=2,PST_CHILD_DESTROYED=3 } PAPACC_C8_PST_ACQUISITION;
typedef enum PAPACC_C8_BORROW_ACTION { NEW_OPERATIONAL_BORROW=1,
 EXISTING_BORROW_RETURN=2,GOVERNED_CLEANUP_CALLBACK=3 } PAPACC_C8_BORROW_ACTION;
typedef struct PAPACC_C8_RESOURCE_TICKET {
 PAPACC_C8_RESOURCE_KEY key; PAPACC_U64 expires_ms; PAPACC_SIZE managed,user,kernel;
 PAPACC_U32 state,borrowers,native_acquisition,pst_acquisition;
} PAPACC_C8_RESOURCE_TICKET;
typedef struct PAPACC_C8_RESOURCE_TEARDOWN {
 PAPACC_C8_RESOURCE_KEY key;
 PAPACC_RESULT pst_result,native_close_result;
 PAPACC_U32 native_state,pst_state;
 PAPACC_U32 pst_disposition,waitset_removed,transport_destroyed,callbacks_quiescent,leases_quiescent;
} PAPACC_C8_RESOURCE_TEARDOWN;
typedef struct PAPACC_C8_RESOURCE_ADMISSION {
 PAPACC_U64 invocation,revision,epoch,serial,owner;
 PAPACC_U8 domain[32];
 PAPACC_SIZE base_managed,managed_used,user_used,kernel_used;
 PAPACC_SIZE connection_managed,connection_user,connection_kernel;
 PAPACC_SIZE user_limit,kernel_limit,host_physical_credit,host_commit_credit;
 PAPACC_SIZE physical_margin,commit_margin;
 PAPACC_U32 listener_reserved,connection_limit,stopping;
 PAPACC_C8_TARGET_EVIDENCE evidence;
 PAPACC_C8_RESOURCE_TICKET tickets[PAPACC_C8_RESOURCE_SLOTS];
} PAPACC_C8_RESOURCE_ADMISSION;
PAPACC_C8_RESOURCE_REASON papacc_c8_resource_check(const PAPACC_C8_RESOURCE_ADMISSION *,const PAPACC_C8_RESOURCE_CONTEXT *,PAPACC_U64);
PAPACC_C8_RESOURCE_REASON papacc_c8_resource_reserve(PAPACC_C8_RESOURCE_ADMISSION *,const PAPACC_C8_RESOURCE_CONTEXT *,PAPACC_U64,PAPACC_C8_RESOURCE_KEY *);
PAPACC_C8_RESOURCE_REASON papacc_c8_resource_commit(PAPACC_C8_RESOURCE_ADMISSION *,const PAPACC_C8_RESOURCE_CONTEXT *,PAPACC_U64,const PAPACC_C8_RESOURCE_KEY *);
PAPACC_C8_RESOURCE_REASON papacc_c8_resource_rollback(PAPACC_C8_RESOURCE_ADMISSION *,const PAPACC_C8_RESOURCE_CONTEXT *,const PAPACC_C8_RESOURCE_KEY *);
PAPACC_C8_RESOURCE_REASON papacc_c8_resource_retain(PAPACC_C8_RESOURCE_ADMISSION *,const PAPACC_C8_RESOURCE_CONTEXT *,const PAPACC_C8_RESOURCE_KEY *);
PAPACC_C8_RESOURCE_REASON papacc_c8_resource_release(PAPACC_C8_RESOURCE_ADMISSION *,const PAPACC_C8_RESOURCE_CONTEXT *,const PAPACC_C8_RESOURCE_TEARDOWN *);
PAPACC_C8_RESOURCE_REASON papacc_c8_resource_observe_acquisition(PAPACC_C8_RESOURCE_ADMISSION *,const PAPACC_C8_RESOURCE_CONTEXT *,const PAPACC_C8_RESOURCE_KEY *,PAPACC_C8_NATIVE_ACQUISITION,PAPACC_C8_PST_ACQUISITION);
/* Cleanup action consumes an existing borrow; it never registers new work. */
PAPACC_C8_RESOURCE_REASON papacc_c8_resource_borrow(PAPACC_C8_RESOURCE_ADMISSION *,const PAPACC_C8_RESOURCE_CONTEXT *,const PAPACC_C8_RESOURCE_KEY *,PAPACC_C8_BORROW_ACTION,PAPACC_U64);
PAPACC_C8_RESOURCE_REASON papacc_c8_resource_stop(PAPACC_C8_RESOURCE_ADMISSION *,PAPACC_U64,PAPACC_U64);
#endif

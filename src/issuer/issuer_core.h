#ifndef PAPACC_ISSUER_CORE_H
#define PAPACC_ISSUER_CORE_H
#include <papacc/types.h>
/* Private in-memory contracts, NOT a serialized format. No platform types.
 * One serialized durable owner per context; independent contexts use root CAS.
 * Root, backend and output leases outlive busy/retained owner. Zero-init once. */
typedef enum PAPACC_ISSUER_RESULT {
 PAPACC_ISSUER_OK=0,PAPACC_ISSUER_DENIED,PAPACC_ISSUER_ABSENT,
 PAPACC_ISSUER_CONFLICT,PAPACC_ISSUER_UNCERTAIN,PAPACC_ISSUER_RETAINED,
 PAPACC_ISSUER_UNSUPPORTED,PAPACC_ISSUER_STOPPED,PAPACC_ISSUER_BUSY,
 PAPACC_ISSUER_LIMIT,PAPACC_ISSUER_INVALID,PAPACC_ISSUER_CONFIRMATION_REQUIRED
} PAPACC_ISSUER_RESULT;
typedef enum PAPACC_ISSUER_STATE {
 PAPACC_ISSUER_PREPARED=1,PAPACC_ISSUER_SERIAL_RESERVED,PAPACC_ISSUER_SIGN_REQUESTED,
 PAPACC_ISSUER_ISSUED_UNPUBLISHED,PAPACC_ISSUER_PUBLISHED,PAPACC_ISSUER_REVOKED,
 PAPACC_ISSUER_ABANDONED,PAPACC_ISSUER_RECONCILIATION_REQUIRED
} PAPACC_ISSUER_STATE;
enum {PAPACC_ISSUER_CLIENT=1,PAPACC_ISSUER_SERVER,PAPACC_ISSUER_PACKAGE};
enum {PAPACC_ISSUER_PREPARE=1,PAPACC_ISSUER_RESERVE,PAPACC_ISSUER_SIGN,
 PAPACC_ISSUER_PUBLISH,PAPACC_ISSUER_REVOKE,PAPACC_ISSUER_RECOVER,PAPACC_ISSUER_QUERY};
enum {PAPACC_ISSUER_PROOF_CURRENT=1U,PAPACC_ISSUER_PROOF_COMPLETE_CHAIN=2U,
 PAPACC_ISSUER_PROOF_FENCED_HIGH_WATER=4U,PAPACC_ISSUER_PROOF_KEY_BINDING=8U,
 PAPACC_ISSUER_PROOF_PREFIX_HISTORY=16U};
#define PAPACC_ISSUER_ROOT_PROOFS 31U
/* proof bits: CURRENT, verified complete recovery chain, fenced latest
 * high-water, key/generation correspondence, prefix uniqueness/history.
 * Facts supplied ONLY by same administrative root adapter, never config flags. */
typedef struct PAPACC_ISSUER_VIEW {
 PAPACC_U8 root[32],authority[16],generation[16],prefix[8],policy[32],ca_spki_digest[32];
 PAPACC_U64 revision,epoch,high_water,ca_not_after;
 PAPACC_U32 proofs;
} PAPACC_ISSUER_VIEW;
typedef struct PAPACC_ISSUER_REQUEST {
 PAPACC_U8 id[16],device[16],csr_digest[32],key_digest[32],predecessor[16];
 PAPACC_U32 purpose;
 PAPACC_U64 not_before,not_after;
} PAPACC_ISSUER_REQUEST;
typedef struct PAPACC_ISSUER_RECORD {
 PAPACC_ISSUER_REQUEST request;
 PAPACC_U8 authority[16],generation[16],policy[32],serial[16],certificate_digest[32];
 PAPACC_U64 counter,approved_epoch,published_epoch;
 PAPACC_SIZE certificate_size;
 PAPACC_ISSUER_STATE state;
} PAPACC_ISSUER_RECORD;
typedef struct PAPACC_ISSUER_ROOT_PORT {
 /* ctx is existing administrative root, never a second registry.
  * read: verified current root history + exact record; absent is explicit.
  * cas: atomic durable record/cert + high-water update against expected
  * revision/high-water/record; no signing/side effects on CONFLICT.
  * publish=1 additionally requires real root publication/CURRENT/epoch.
  * UNKNOWN durability returns UNCERTAIN. No root adapter => deny. */
 void *ctx;
 PAPACC_ISSUER_RESULT (*read)(void *,const PAPACC_U8[16],PAPACC_ISSUER_VIEW *,PAPACC_ISSUER_RECORD *,int *);
 PAPACC_ISSUER_RESULT (*cas)(void *,const PAPACC_ISSUER_VIEW *,const PAPACC_ISSUER_RECORD *,const PAPACC_ISSUER_RECORD *,PAPACC_U64,const PAPACC_U8 *,PAPACC_SIZE,int);
 PAPACC_ISSUER_RESULT (*copy_certificate)(void *,const PAPACC_U8[16],const PAPACC_U8[32],PAPACC_U8 *,PAPACC_SIZE,PAPACC_SIZE *);
 int test_only;
} PAPACC_ISSUER_ROOT_PORT;
typedef struct PAPACC_ISSUER_BACKEND {
 void *ctx;int test_only;
 PAPACC_ISSUER_RESULT (*authorize)(void *,const PAPACC_ISSUER_VIEW *,const PAPACC_ISSUER_REQUEST *,unsigned);
 PAPACC_ISSUER_RESULT (*utc)(void *,PAPACC_U64 *);
 PAPACC_ISSUER_RESULT (*verify_request)(void *,const PAPACC_ISSUER_RECORD *,const PAPACC_U8 *,PAPACC_SIZE);
 PAPACC_ISSUER_RESULT (*acquire)(void *,const PAPACC_ISSUER_VIEW *,const PAPACC_ISSUER_RECORD *,void **);
 PAPACC_ISSUER_RESULT (*sign)(void *,void *,const PAPACC_ISSUER_RECORD *,const PAPACC_U8 *,PAPACC_SIZE,PAPACC_U8 *,PAPACC_SIZE,PAPACC_SIZE *);
 PAPACC_ISSUER_RESULT (*verify_certificate)(void *,const PAPACC_ISSUER_RECORD *,const PAPACC_U8 *,PAPACC_SIZE,PAPACC_U8[32]);
 PAPACC_ISSUER_RESULT (*release)(void *,void *);
 /* Explicit init/rotation only; not called by issuance or recovery. These
  * ports are unavailable unless separate approved initialization binds them. */
 PAPACC_ISSUER_RESULT (*entropy)(void *,PAPACC_U8 *,PAPACC_SIZE);
 PAPACC_ISSUER_RESULT (*generate_key)(void *,const PAPACC_ISSUER_VIEW *,void **);
} PAPACC_ISSUER_BACKEND;
typedef struct PAPACC_ISSUER_CORE {
 PAPACC_ISSUER_ROOT_PORT root;const PAPACC_ISSUER_BACKEND *backend;
 PAPACC_U8 root_id[32],authority[16],generation[16],prefix[8],ca_spki_digest[32];
 PAPACC_U64 minimum_revision,minimum_counter;
 void *lease;PAPACC_U8 *retained_output;PAPACC_SIZE output_capacity;
 int initialized,busy,stopped,blocked,retained;
} PAPACC_ISSUER_CORE;
/* Pure proposal helper: never authorizes/normalizes a persisted request. */
PAPACC_ISSUER_RESULT papacc_issuer_interval(PAPACC_U64,PAPACC_U64,PAPACC_U64,PAPACC_U64 *);
PAPACC_ISSUER_RESULT papacc_issuer_init(PAPACC_ISSUER_CORE *,const PAPACC_ISSUER_ROOT_PORT *,const PAPACC_ISSUER_BACKEND *);
PAPACC_ISSUER_RESULT papacc_issuer_prepare(PAPACC_ISSUER_CORE *,const PAPACC_ISSUER_REQUEST *);
PAPACC_ISSUER_RESULT papacc_issuer_reserve(PAPACC_ISSUER_CORE *,const PAPACC_U8[16]);
PAPACC_ISSUER_RESULT papacc_issuer_issue(PAPACC_ISSUER_CORE *,const PAPACC_U8[16],const PAPACC_U8 *,PAPACC_SIZE,PAPACC_U8 *,PAPACC_SIZE);
PAPACC_ISSUER_RESULT papacc_issuer_query(PAPACC_ISSUER_CORE *,const PAPACC_U8[16],PAPACC_ISSUER_RECORD *);
PAPACC_ISSUER_RESULT papacc_issuer_transition(PAPACC_ISSUER_CORE *,const PAPACC_U8[16],PAPACC_ISSUER_STATE);
PAPACC_ISSUER_RESULT papacc_issuer_recover(PAPACC_ISSUER_CORE *,const PAPACC_U8[16]);
void papacc_issuer_stop(PAPACC_ISSUER_CORE *);
PAPACC_ISSUER_RESULT papacc_issuer_finish(PAPACC_ISSUER_CORE *);
#endif
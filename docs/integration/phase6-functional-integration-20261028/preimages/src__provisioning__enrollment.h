#ifndef PAPACC_ENROLLMENT_H
#define PAPACC_ENROLLMENT_H
#include <papacc/types.h>
/* Private offline DATA components. No Session, registry, root authority,
 * signature, credential installation or productive listener registration.
 * Caller owner serializes operations and holds immutable disjoint buffers.
 * DATA_ONLY never means trusted, approved, CURRENT, RESOLVED or admitted. */
#define EP_HEADER 16U
#define EP_BODY_MAX 16384U
#define EP_FRAME_MAX 16400U
#define EP_UNKNOWN_BYTES ((PAPACC_SIZE)-1)
typedef enum EP_RESULT { EP_DATA_ONLY=1,EP_MORE=2,EP_INVALID=-1,
 EP_DENIED=-2,EP_LIMIT=-3,EP_STOPPED=-4,EP_EXPIRED=-5,EP_REPLAY=-6 } EP_RESULT;
typedef struct EP_SLICE { const PAPACC_U8 *data; PAPACC_SIZE size; } EP_SLICE;
typedef struct EP_FRAME { unsigned kind,sequence; EP_SLICE payload; } EP_FRAME;
enum { EP_C2S=0,EP_S2C=1 };
EP_RESULT ep_frame_decode(const PAPACC_U8 *,PAPACC_SIZE,unsigned,EP_FRAME *);
EP_RESULT ep_frame_encode(unsigned,unsigned,unsigned,const PAPACC_U8 *,PAPACC_SIZE,PAPACC_U8 *,PAPACC_SIZE,PAPACC_SIZE *);
/* One frame per reader. Returned slice borrows caller RX; do not reset/reuse
 * any storage while a downstream borrower exists, including retained work. */
typedef struct EP_READER {
 PAPACC_U8 *rx;PAPACC_SIZE capacity,have,want;
 PAPACC_U64 first,last;unsigned started,failed,ready,stopped;
} EP_READER;
EP_RESULT ep_reader_push(EP_READER *,const PAPACC_U8 *,PAPACC_SIZE,PAPACC_U64,unsigned,PAPACC_SIZE *,EP_FRAME *);
/* Trace validates DATA order/correlation only. A received APPROVED is not an
 * authority source. Own CSR copy avoids pointers into recycled RX. Native
 * approval/policy/CURRENT/CSR-to-issuer binding remains UNBOUND and DENIED. */
typedef struct EP_TRACE {
 unsigned sequence[2],phase,access,approval,wait_seen,pending,terminal,stopped;
 PAPACC_SIZE bytes[2],csr_size;
 PAPACC_U8 client_nonce[32],server_nonce[32],server_fingerprint[32],request[16],csr[4096];
} EP_TRACE;
EP_RESULT ep_trace_observe(EP_TRACE *,const EP_FRAME *,unsigned);
void ep_trace_stop(EP_TRACE *);
/* Pure code formatting; random7 must come from qualified entropy. Not a RNG. */
EP_RESULT ep_code_format(const PAPACC_U8[7],PAPACC_U8[10]);
EP_RESULT ep_code_parse_local(const PAPACC_U8 *,PAPACC_SIZE,PAPACC_U8[10]);
typedef struct EP_PAIR {
 PAPACC_U8 code[10],request[16],server_nonce[32],client_nonce[32];
 PAPACC_U64 connection,generation,start,last,deadline;
 unsigned initialized,submitted,claimed,invalid,stopped;
} EP_PAIR;
/* Pure owned DATA preparation from random55 = code7 + id16 + nonce32.
 * Collision rejects; never retry/fallback. Existing owner must hold live[0..2]
 * immutable, including invalid-but-retained pairs. No caller boolean is auth. */
EP_RESULT ep_pair_prepare_data(EP_PAIR *,const PAPACC_U8[55],const PAPACC_U8[32],PAPACC_U64,PAPACC_U64,PAPACC_U64,const EP_PAIR *,PAPACC_SIZE);
EP_RESULT ep_pair_claim_data(EP_PAIR *,const PAPACC_U8 *,PAPACC_SIZE,PAPACC_U64,PAPACC_U64,PAPACC_U64);
void ep_pair_stop(EP_PAIR *);
/* Native Windows adapter generates public pairing secret only; no keys/CA.
 * It is not registered at startup or connected to root/CLI. */
#ifdef _WIN32
EP_RESULT ep_pair_generate_windows(EP_PAIR *,const PAPACC_U8[32],PAPACC_U64,PAPACC_U64,PAPACC_U64,const EP_PAIR *,PAPACC_SIZE);
#endif
typedef struct EP_ORIGIN { unsigned family; PAPACC_U8 address[16];PAPACC_U32 scope; } EP_ORIGIN;
typedef struct EP_ORIGIN_SLOT {
 EP_ORIGIN origin;PAPACC_U64 stamps[3];unsigned count,used,holds;
} EP_ORIGIN_SLOT;
typedef struct EP_TICKET { PAPACC_U64 generation;unsigned slot; } EP_TICKET;
typedef struct EP_LIMITER {
 EP_ORIGIN_SLOT origins[16];PAPACC_U64 global[6],last,next,held[2];
 unsigned count,started,stopped,held_slot[2];
} EP_LIMITER;
/* Origin must come from PAL socket endpoint; never remote headers. DATA
 * ledger is subordinate to the existing owner, not credits/identity registry.
 * Release is local accounting only and is called exclusively AFTER factual
 * native closure by future owner integration. Stop never releases a ticket. */
EP_RESULT ep_origin_set(EP_ORIGIN *,unsigned,const PAPACC_U8 *,PAPACC_U32);
EP_RESULT ep_limiter_admit(EP_LIMITER *,const EP_ORIGIN *,PAPACC_U64,EP_TICKET *);
EP_RESULT ep_limiter_release_data(EP_LIMITER *,EP_TICKET);
void ep_limiter_stop(EP_LIMITER *);
/* Checked quote: no allocation/reservation/host or provider qualification. */
EP_RESULT ep_enrollment_quote(PAPACC_SIZE,PAPACC_SIZE,PAPACC_SIZE,PAPACC_SIZE *);
#endif

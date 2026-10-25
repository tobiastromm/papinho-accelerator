#ifndef PAPACC_QUALIFIED_UTC_H
#define PAPACC_QUALIFIED_UTC_H
#include <papacc/types.h>
/* Private, nonserialized DATA calculus for Q0-Q7. Numeric fields, pointers,
 * presence and DATA_OK are NEVER qualification or administrative authority.
 * No producer of qualified observations exists in this implementation.
 * All storage belongs to the caller's existing administrative owner. It must
 * protect referenced owners/pins until factual quiescence, including failure
 * and Stop. These functions neither acquire nor release a borrow/resource.
 * Calls are serialized by that owner. Outputs must be disjoint from inputs. */
#define PAPACC_UTC_DATA_VERSION 1U
/* Presence bits 0..12: context, native, m0, m1, lower, upper,
 * rho_num, rho_den, E, D, read result, quality result, qualified bounds.
 * This is DATA only, not a route for asserting factual results. */
#define PAPACC_UTC_DATA_PRESENT 0x1fffU
#define PAPACC_UTC_NS_PER_SECOND ((PAPACC_U64)1000000000UL)
typedef struct PAPACC_UTC_CONTEXT {
 const void *owner,*backend,*source,*pin;
 PAPACC_U8 root[32],candidate[32];
 PAPACC_U64 process_lifetime,invocation,revision,qualification,continuity,boundary;
} PAPACC_UTC_CONTEXT;
typedef struct PAPACC_UTC_OBSERVATION {
 PAPACC_U32 version,present;
 PAPACC_RESULT read_result,quality_result;
 PAPACC_UTC_CONTEXT context;
 PAPACC_U64 native_utc,m0,m1,lower,upper,rho_num,rho_den,pal_error,deadline;
} PAPACC_UTC_OBSERVATION;
typedef enum PAPACC_UTC_DATA_RESULT {
 PAPACC_UTC_DATA_OK=0,PAPACC_UTC_DATA_INVALID,PAPACC_UTC_DATA_UNKNOWN,
 PAPACC_UTC_DATA_LIMIT,PAPACC_UTC_DATA_EXPIRED,PAPACC_UTC_DATA_DISCONTINUITY,
 PAPACC_UTC_DATA_STOPPED
} PAPACC_UTC_DATA_RESULT;
typedef struct PAPACC_UTC_HISTORY {
 PAPACC_UTC_OBSERVATION last;
 PAPACC_U64 last_monotonic;
 unsigned has_last,invalid,stopped;
} PAPACC_UTC_HISTORY;
/* Zero-initialize once per governed operation, never reset after failure to
 * retry until a favorable UTC. Restart cannot restore authority/history.
 * Failure invalidates applicability but preserves last/context and ownership.
 * Source/context changes require explicit requalification and a new operation. */
PAPACC_UTC_DATA_RESULT papacc_utc_data_propagate(const PAPACC_UTC_OBSERVATION *,const PAPACC_UTC_CONTEXT *,PAPACC_U64,PAPACC_U64 *,PAPACC_U64 *);
PAPACC_UTC_DATA_RESULT papacc_utc_data_observe(PAPACC_UTC_HISTORY *,const PAPACC_UTC_OBSERVATION *);
PAPACC_UTC_DATA_RESULT papacc_utc_data_query(PAPACC_UTC_HISTORY *,const PAPACC_UTC_CONTEXT *,PAPACC_U64,PAPACC_U64 *,PAPACC_U64 *);
void papacc_utc_data_invalidate(PAPACC_UTC_HISTORY *);
void papacc_utc_data_stop(PAPACC_UTC_HISTORY *);
PAPACC_UTC_DATA_RESULT papacc_utc_data_within(PAPACC_U64,PAPACC_U64,PAPACC_U64,PAPACC_U64);
PAPACC_UTC_DATA_RESULT papacc_utc_data_filetime(PAPACC_U64,PAPACC_U64 *);
struct PAPACC_ISSUER_BACKEND;
/* Existing scalar backend.utc and PAL do not expose Q2 provenance, finite E,
 * rate or continuity. Always NOT_SUPPORTED, without reading UTC, issuing I/O,
 * writing output, or treating backend/test callbacks as evidence. */
PAPACC_RESULT papacc_issuer_qualified_utc_read(const struct PAPACC_ISSUER_BACKEND *,PAPACC_UTC_OBSERVATION *);
#endif

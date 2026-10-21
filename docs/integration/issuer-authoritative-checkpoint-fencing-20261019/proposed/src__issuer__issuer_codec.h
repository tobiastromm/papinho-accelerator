#ifndef PAPACC_ISSUER_CODEC_H
#define PAPACC_ISSUER_CODEC_H
#include "issuer_core.h"
#define PAPACC_ISSUER_IMAGE_BYTES 8192U
#define PAPACC_ISSUER_IMAGE_HEADER 256U
#define PAPACC_ISSUER_EVENT_FIXED 356U
/* Private format1 inside ROOT06. Hash function is SHA256 in real adapters;
 * test hashes cannot establish native integrity/authority. No proof flags or
 * operational epoch are reconstructed from this image. */
typedef int (*PAPACC_ISSUER_HASH)(void *,const PAPACC_U8 *,PAPACC_SIZE,PAPACC_U8[32]);
PAPACC_ISSUER_RESULT papacc_issuer_image_init(PAPACC_U8 *,const PAPACC_ISSUER_VIEW *,const PAPACC_U8[32],PAPACC_ISSUER_HASH,void *);
PAPACC_ISSUER_RESULT papacc_issuer_image_validate(const PAPACC_U8 *,PAPACC_ISSUER_VIEW *,PAPACC_ISSUER_HASH,void *);
PAPACC_ISSUER_RESULT papacc_issuer_image_find(const PAPACC_U8 *,const PAPACC_U8[16],PAPACC_ISSUER_RECORD *,PAPACC_ISSUER_HASH,void *);
PAPACC_ISSUER_RESULT papacc_issuer_image_append(PAPACC_U8 *,const PAPACC_ISSUER_RECORD *,const PAPACC_U8 *,PAPACC_SIZE,PAPACC_ISSUER_HASH,void *);
PAPACC_ISSUER_RESULT papacc_issuer_image_certificate(const PAPACC_U8 *,const PAPACC_U8[16],PAPACC_U8 *,PAPACC_SIZE,PAPACC_SIZE *,PAPACC_ISSUER_HASH,void *);

/* Data-only recovery assessment of the EXISTING version1 ROOT06 issuer image.
 * Inputs are immutable borrowed complete snapshots for this synchronous call.
 * Output must be disjoint from inputs; caller serializes Stop and owns leases.
 * No owner-thread/native latency claim or active-root invocation is added.
 * Retained baseline is a comparison floor, NOT independent proof of latest state.
 * No signer/backend call, allocation, lock, ownership transfer or authority bit.
 * All outcomes deny authoritative recovery, including matching valid images. */
typedef enum PAPACC_ISSUER_CHECKPOINT_STATE {
 PAPACC_ISSUER_CHECKPOINT_INVALID=1,
 PAPACC_ISSUER_CHECKPOINT_INCOMPLETE,
 PAPACC_ISSUER_CHECKPOINT_ROLLBACK,
 PAPACC_ISSUER_CHECKPOINT_DIVERGENT,
 PAPACC_ISSUER_CHECKPOINT_STOPPED,
 PAPACC_ISSUER_CHECKPOINT_DATA_VALID_RECOVERY_DENIED
} PAPACC_ISSUER_CHECKPOINT_STATE;
typedef struct PAPACC_ISSUER_CHECKPOINT_REPORT {
 unsigned version;
 PAPACC_ISSUER_CHECKPOINT_STATE state;
 PAPACC_ISSUER_VIEW data;
 PAPACC_U32 events;
 /* No CURRENT/A/P reconstructed from issuer image; no proof flags granted. */
} PAPACC_ISSUER_CHECKPOINT_REPORT;
PAPACC_ISSUER_RESULT papacc_issuer_checkpoint_assess(
 const PAPACC_U8 *,PAPACC_SIZE,const PAPACC_U8 *,PAPACC_SIZE,
 PAPACC_ISSUER_HASH,void *,int,PAPACC_ISSUER_CHECKPOINT_REPORT *);
#endif
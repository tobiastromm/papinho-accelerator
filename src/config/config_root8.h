#ifndef PAPACC_CONFIG_ROOT8_H
#define PAPACC_CONFIG_ROOT8_H
#include "config_lst1.h"
#define PC_ROOT8_MAX 25792U
#define PC_ROOT8_MODEL_BYTES 25760U
#define PC_ROOT8_MIN 2592U
/* Exact no-carrier maximum: 1056 + 1536*4 + 2208. */
#define PC_ROOT8_NO_ISSUER_MAX 9408U
#define PC_ROOT8_NO_ISSUER_FIELDS 9376U
/* Complete owned explicit byte fields, no native struct dump/AST/authority.
 * Header/records/components/checkpoint follow accepted D3; unused tail zero.
 * Exclusive synchronous inputs/output, no hash/component callback reentry or
 * retained borrow. No call from event loop/Stop. Metadata valid only on OK. */
typedef struct PC_ROOT8_MODEL { PAPACC_U8 fields[PC_ROOT8_MODEL_BYTES]; } PC_ROOT8_MODEL;
typedef enum PC_ROOT8_RESULT { PC_ROOT8_OK=0,PC_ROOT8_INVALID,
 PC_ROOT8_UNSUPPORTED,PC_ROOT8_HASH_FAILED,PC_ROOT8_INCOMPLETE } PC_ROOT8_RESULT;
/* Existing platform codec adapter verifies full BODY05 + descriptor/root match
 * (kind5), or full historical ROOT06 issuer carrier (kind6). Mandatory, no
 * fallback. Return data validity only; no CURRENT/actor/native identity proof.
 * For kind5 p=BODY1024, descriptor=512; kind6 p=carrier16384, descriptor=NULL.
 * Header is immutable512; ctx lifetime/disjointness belongs to caller. */
typedef PC_ROOT8_RESULT (*PC_ROOT8_COMPONENT)(void *,unsigned,const PAPACC_U8 *,
 PAPACC_SIZE,const PAPACC_U8 *,const PAPACC_U8 *);
PC_ROOT8_RESULT pc_root8_encode(const PC_ROOT8_MODEL *,PAPACC_U8 *,PAPACC_SIZE,
 PAPACC_SIZE *,PC_LST1_HASH,void *,PC_ROOT8_COMPONENT,void *);
/* Data-only bounded encoder for the currently supported no-carrier profile.
 * Caller owns exactly fields_capacity bytes (header through CP, zero tail),
 * disjoint encoded output and metadata. Same lifetime/callback/Stop contract.
 * Carrier flags/length return UNSUPPORTED, never truncate/fallback. No view or
 * allocation escapes. A retained input/output cannot be reused by this API. */
PC_ROOT8_RESULT pc_root8_encode_no_issuer(const PAPACC_U8 *,PAPACC_SIZE,
 PAPACC_U8 *,PAPACC_SIZE,PAPACC_SIZE *,PC_LST1_HASH,void *,PC_ROOT8_COMPONENT,void *);
/* Owned bounded decode; output capacity is no-carrier fields maximum9376.
 * Complete is structural only. Zero output after safe precondition checks;
 * no borrowed view, carrier always UNSUPPORTED, all other checks unchanged. */
/* Same no-carrier integrity/shape checks without an owned output copy.
 * Read-only synchronous validation only, never CURRENT/authority. Input and
 * callback contexts remain immutable/alive throughout; no view escapes. */
PC_ROOT8_RESULT pc_root8_validate_no_issuer(const PAPACC_U8 *,PAPACC_SIZE,
 PC_LST1_HASH,void *,PC_ROOT8_COMPONENT,void *);
PC_ROOT8_RESULT pc_root8_decode_no_issuer(const PAPACC_U8 *,PAPACC_SIZE,
 PAPACC_U8 *,PAPACC_SIZE,int *,PC_LST1_HASH,void *,PC_ROOT8_COMPONENT,void *);
PC_ROOT8_RESULT pc_root8_decode(const PAPACC_U8 *,PAPACC_SIZE,PC_ROOT8_MODEL *,
 int *,PC_LST1_HASH,void *,PC_ROOT8_COMPONENT,void *);
/* Data-only comparison to a complete prior image. No CAS/native authority;
 * caller still needs factual CURRENT, actor, identity, pin, budget and Stop.
 * Missing base -> INCOMPLETE; lower/equal conflicting history -> INVALID. */
PC_ROOT8_RESULT pc_root8_base_link(const PAPACC_U8 *,PAPACC_SIZE,
 const PAPACC_U8 *,PAPACC_SIZE,PC_LST1_HASH,void *,PC_ROOT8_COMPONENT,void *);
#endif

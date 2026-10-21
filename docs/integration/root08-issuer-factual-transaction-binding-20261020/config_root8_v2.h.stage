#ifndef PAPACC_CONFIG_ROOT8_V2_H
#define PAPACC_CONFIG_ROOT8_V2_H
#include "config_root8.h"
#define PC_ROOT8_V2_MIN 20512U
#define PC_ROOT8_V2_MAX 28960U
#define PC_ROOT8_V2_FIELDS_MAX 28928U
#define PC_SNT1_MAX 3168U
/* Private portable DATA codec, not CURRENT/resolution/authorization.
 * No allocation/store/thread/owner. Caller serializes input/output lifetime,
 * Stop and callbacks; all data/metadata outputs disjoint from all inputs.
 * Hash/component callbacks are synchronous, nonreentrant, cannot retain/mutate.
 * Existing component kinds5/6 validate BODY05/full historical carrier.
 * Kind7 additionally validates carrier against one SNI record in desc (768):
 * exact root/issuer generation/CA SPKI; stage2/3 exact existing SERVER request,
 * CSR/full-SPKI and conditional result digests/state. Never native authority.
 * Existing native carrier adapter stays UNSUPPORTED. A test callback is not
 * a product-configurable authority and cannot make a source RESOLVED.
 */
PC_ROOT8_RESULT pc_root8_v2_validate(const PAPACC_U8 *,PAPACC_SIZE,
 PC_LST1_HASH,void *,PC_ROOT8_COMPONENT,void *);
PC_ROOT8_RESULT pc_root8_v2_encode(const PAPACC_U8 *,PAPACC_SIZE,PAPACC_U8 *,PAPACC_SIZE,
 PAPACC_SIZE *,PC_LST1_HASH,void *,PC_ROOT8_COMPONENT,void *);
PC_ROOT8_RESULT pc_root8_v2_decode(const PAPACC_U8 *,PAPACC_SIZE,PAPACC_U8 *,PAPACC_SIZE,
 PAPACC_SIZE *,int *,PC_LST1_HASH,void *,PC_ROOT8_COMPONENT,void *);
/* Fragment encoders require the COMPLETE containing immutable prefix/header/
 * collection as context. They do not create independent storage or authority.
 * Input is full component including trailer slot; output seals SHA256 only.
 * CP2 requires supplied content digest already matching the prefix; it never
 * fabricates publication R/A/P/I. ZERO512 checkpoint preserves absence.
 * All remain offline-only; callers retain buffers on pending/uncertain use. */
PC_ROOT8_RESULT pc_root8_v2_snt_encode(const PAPACC_U8 *,PAPACC_SIZE,
 PAPACC_U8 *,PAPACC_SIZE,PAPACC_SIZE *,PC_LST1_HASH,void *);
PC_ROOT8_RESULT pc_root8_v2_cp_encode(const PAPACC_U8 *,PAPACC_SIZE,
 PAPACC_U8 *,PAPACC_SIZE,PAPACC_SIZE *,PC_LST1_HASH,void *);
/* Complete serialized v2->v2 predecessor relation ONLY, not CURRENT or source
 * approval. Caller retains both complete immutable inputs; callbacks cannot
 * mutate/reenter/retain them. Requires nonzero valid predecessor CP2, exact
 * base collection and full issuer event-prefix/high-water continuity. v1
 * adoption needs its separate qualified transition and returns INCOMPLETE.
 * Changed approval rows return INCOMPLETE pending actual source/history proof;
 * identical rows do not themselves establish their current authorization. */
PC_ROOT8_RESULT pc_root8_v2_base_link(const PAPACC_U8 *,PAPACC_SIZE,
 const PAPACC_U8 *,PAPACC_SIZE,PC_LST1_HASH,void *,PC_ROOT8_COMPONENT,void *);
#endif

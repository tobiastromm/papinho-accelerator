#ifndef PAPACC_PERSONAL_PLAN_DATA_H
#define PAPACC_PERSONAL_PLAN_DATA_H
/* Private DATA sequencing, never an authority/eligibility proof. Embedded in
 * the existing canonical owner. No platform types, allocation or callbacks. */
#include <stddef.h>
#define PAPACC_PERSONAL_PLAN_MAX 8192U
#define PAPACC_PERSONAL_DATA_CAPTURED 1U
#define PAPACC_PERSONAL_DATA_PRESENTED 2U
#define PAPACC_PERSONAL_DATA_CONFIRMED 3U
#define PAPACC_PERSONAL_DATA_CONSUMED 4U
#define PAPACC_PERSONAL_DATA_INVALID 5U
#define PAPACC_PERSONAL_DATA_FINISHED 6U
typedef struct PAPACC_PERSONAL_PLAN_DATA {
 unsigned phase,consumed,effectful;size_t bytes;
 unsigned char attempt[32],plan[PAPACC_PERSONAL_PLAN_MAX];
} PAPACC_PERSONAL_PLAN_DATA;
/* Input nonce is untrusted DATA here: qualified native CSPRNG/identity/target
 * and publication are deliberately NOT supplied by this module. */
int papacc_personal_data_capture(PAPACC_PERSONAL_PLAN_DATA *,const unsigned char *,const unsigned char *,size_t);
int papacc_personal_data_present(PAPACC_PERSONAL_PLAN_DATA *);
int papacc_personal_data_confirm(PAPACC_PERSONAL_PLAN_DATA *,const unsigned char *,const unsigned char *,size_t);
int papacc_personal_data_consume(PAPACC_PERSONAL_PLAN_DATA *,const unsigned char *,const unsigned char *,size_t);
void papacc_personal_data_invalidate(PAPACC_PERSONAL_PLAN_DATA *);
/* Caller must prove no worker/borrow before invoking retire. No timeout. */
int papacc_personal_data_retire(PAPACC_PERSONAL_PLAN_DATA *);
/* Private presentation DATA, not a wire, grant or factual identity source.
 * All identities are supplied observations/requests; adapter proves provenance. */
typedef struct PAPACC_PERSONAL_EFFECT_SPEC {
 const unsigned char *operator_id,*parent_id,*leaf,*authority,*generation,*service_id,*configuration;
 size_t operator_bytes,leaf_bytes,service_bytes,configuration_bytes;
} PAPACC_PERSONAL_EFFECT_SPEC;
size_t papacc_personal_effect_build(const PAPACC_PERSONAL_EFFECT_SPEC *,unsigned char *,size_t);
/* Exact DATA association only; never validates native custody. */
int papacc_personal_effect_matches(const unsigned char *,size_t,const PAPACC_PERSONAL_EFFECT_SPEC *);
int papacc_personal_effect_source(const unsigned char *,size_t,const unsigned char **,size_t *);
/* Upgrade is allowed ONLY before presentation; never after F10 confirmation. */
int papacc_personal_effect_upgrade(PAPACC_PERSONAL_PLAN_DATA *,const unsigned char *,size_t);
int papacc_personal_effect_confirm(PAPACC_PERSONAL_PLAN_DATA *,const unsigned char *,const unsigned char *,size_t,const char *,size_t);
int papacc_personal_effect_consume(PAPACC_PERSONAL_PLAN_DATA *,const unsigned char *,const unsigned char *,size_t);
#endif

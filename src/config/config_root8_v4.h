#ifndef PAPACC_CONFIG_ROOT8_V4_H
#define PAPACC_CONFIG_ROOT8_V4_H
#include "config_root8_v3.h"
#define PC_ROOT8_V4_MIN 20704U
#define PC_ROOT8_V4_MAX 31840U
#define PC_ROOT8_V4_FIELDS_MAX 31808U
#define PC_TSC1_MAX 1248U
/* Private synchronous DATA codecs; no allocation, retained refs, I/O, root,
 * pin, CURRENT or temporal authority. Caller protects immutable inputs and
 * disjoint outputs/backend during each call; same component kinds5..8.
 * New private kind9 receives p=row384, n=384, desc=row, header=checked v4;
 * it must validate bare Ed25519 key DATA using an existing qualified backend.
 * Missing adapter => INCOMPLETE/UNSUPPORTED, never implicit key validity.
 * Models cannot qualify a native key or source. UNKNOWN/REVOKED are DATA.
 * No v4 integration into worker/backings/positive binding is implied. */
PC_ROOT8_RESULT pc_root8_v4_validate(const PAPACC_U8 *,PAPACC_SIZE,PC_LST1_HASH,void *,PC_ROOT8_COMPONENT,void *);
PC_ROOT8_RESULT pc_root8_v4_encode(const PAPACC_U8 *,PAPACC_SIZE,PAPACC_U8 *,PAPACC_SIZE,PAPACC_SIZE *,PC_LST1_HASH,void *,PC_ROOT8_COMPONENT,void *);
PC_ROOT8_RESULT pc_root8_v4_decode(const PAPACC_U8 *,PAPACC_SIZE,PAPACC_U8 *,PAPACC_SIZE,PAPACC_SIZE *,int *,PC_LST1_HASH,void *,PC_ROOT8_COMPONENT,void *);
PC_ROOT8_RESULT pc_root8_v4_snt_encode(const PAPACC_U8 *,PAPACC_SIZE,PAPACC_U8 *,PAPACC_SIZE,PAPACC_SIZE *,PC_LST1_HASH,void *);
PC_ROOT8_RESULT pc_root8_v4_pdg_encode(const PAPACC_U8 *,PAPACC_SIZE,PAPACC_U8 *,PAPACC_SIZE,PAPACC_SIZE *,PC_LST1_HASH,void *);
PC_ROOT8_RESULT pc_root8_v4_tsc_encode(const PAPACC_U8 *,PAPACC_SIZE,PAPACC_U8 *,PAPACC_SIZE,PAPACC_SIZE *,PC_LST1_HASH,void *);
PC_ROOT8_RESULT pc_root8_v4_cp_encode(const PAPACC_U8 *,PAPACC_SIZE,PAPACC_U8 *,PAPACC_SIZE,PAPACC_SIZE *,PC_LST1_HASH,void *);
/* Complete v3/v4 predecessor DATA only. Changed sources => INCOMPLETE.
 * Never resolves approval, currentness, pins or CAS. No historic fallback. */
PC_ROOT8_RESULT pc_root8_v4_base_link(const PAPACC_U8 *,PAPACC_SIZE,const PAPACC_U8 *,PAPACC_SIZE,PC_LST1_HASH,void *,PC_ROOT8_COMPONENT,void *);
#endif

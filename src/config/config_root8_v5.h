#ifndef PAPACC_CONFIG_ROOT8_V5_H
#define PAPACC_CONFIG_ROOT8_V5_H
#include "config_root8_v4.h"
#define PC_ROOT8_V5_MIN 21216U
#define PC_ROOT8_V5_MAX 32352U
#define PC_ROOT8_V5_FIELDS_MAX 32320U
/* See accepted L1; DATA only, never publication/admission. */
PC_ROOT8_RESULT pc_root8_v5_key_data(void *,unsigned,const PAPACC_U8 *,PAPACC_SIZE,const PAPACC_U8 *,const PAPACC_U8 *);
PC_ROOT8_RESULT pc_root8_q4_data(const PAPACC_U8 *,PAPACC_SIZE,PC_LST1_HASH,void *);
/* Private synchronous carrier5 DATA; kind10 validates complete frozen Q4
 * manifests through the existing owner. Missing adapter is INCOMPLETE. */
PC_ROOT8_RESULT pc_root8_v5_validate(const PAPACC_U8 *,PAPACC_SIZE,PC_LST1_HASH,void *,PC_ROOT8_COMPONENT,void *);
PC_ROOT8_RESULT pc_root8_v5_encode(const PAPACC_U8 *,PAPACC_SIZE,PAPACC_U8 *,PAPACC_SIZE,PAPACC_SIZE *,PC_LST1_HASH,void *,PC_ROOT8_COMPONENT,void *);
PC_ROOT8_RESULT pc_root8_v5_decode(const PAPACC_U8 *,PAPACC_SIZE,PAPACC_U8 *,PAPACC_SIZE,PAPACC_SIZE *,int *,PC_LST1_HASH,void *,PC_ROOT8_COMPONENT,void *);
PC_ROOT8_RESULT pc_root8_v5_snt_encode(const PAPACC_U8 *,PAPACC_SIZE,PAPACC_U8 *,PAPACC_SIZE,PAPACC_SIZE *,PC_LST1_HASH,void *);
PC_ROOT8_RESULT pc_root8_v5_pdg_encode(const PAPACC_U8 *,PAPACC_SIZE,PAPACC_U8 *,PAPACC_SIZE,PAPACC_SIZE *,PC_LST1_HASH,void *);
PC_ROOT8_RESULT pc_root8_v5_tsc_encode(const PAPACC_U8 *,PAPACC_SIZE,PAPACC_U8 *,PAPACC_SIZE,PAPACC_SIZE *,PC_LST1_HASH,void *);
PC_ROOT8_RESULT pc_root8_v5_cp_encode(const PAPACC_U8 *,PAPACC_SIZE,PAPACC_U8 *,PAPACC_SIZE,PAPACC_SIZE *,PC_LST1_HASH,void *);
/* Complete v4/v5 predecessor DATA only. Changed sources => INCOMPLETE.
 * Never resolves approval, currentness, pins or CAS. No historic fallback. */
PC_ROOT8_RESULT pc_root8_v5_base_link(const PAPACC_U8 *,PAPACC_SIZE,const PAPACC_U8 *,PAPACC_SIZE,PC_LST1_HASH,void *,PC_ROOT8_COMPONENT,void *);
#endif

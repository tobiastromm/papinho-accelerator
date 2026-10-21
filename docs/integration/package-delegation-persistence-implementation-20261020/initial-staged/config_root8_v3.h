#ifndef PAPACC_CONFIG_ROOT8_V3_H
#define PAPACC_CONFIG_ROOT8_V3_H
#include "config_root8_v2.h"
#define PC_ROOT8_V3_MIN 20608U
#define PC_ROOT8_V3_MAX 30592U
#define PC_ROOT8_V3_FIELDS_MAX 30560U
#define PC_PDG1_MAX 1632U
/* Offline DATA codec only. No CURRENT, grants, trust, store, pins or epoch.
 * Caller serializes immutable inputs and disjoint outputs through return;
 * hash/component callbacks are synchronous, nonreentrant, do not retain.
 * Existing component kinds5/6/7 unchanged. Kind8 requires vetted public signer
 * certificate/profile DATA validation: p=full carrier16384, desc=PDG row384,
 * header=complete checked outer header. Missing adapter is INCOMPLETE.
 * A fixture callback never establishes delegation authority or Browser trust.
 * Never invoke synchronous hashing/native validation on owner Stop path. */
PC_ROOT8_RESULT pc_root8_v3_validate(const PAPACC_U8 *,PAPACC_SIZE,PC_LST1_HASH,void *,PC_ROOT8_COMPONENT,void *);
PC_ROOT8_RESULT pc_root8_v3_encode(const PAPACC_U8 *,PAPACC_SIZE,PAPACC_U8 *,PAPACC_SIZE,PAPACC_SIZE *,PC_LST1_HASH,void *,PC_ROOT8_COMPONENT,void *);
PC_ROOT8_RESULT pc_root8_v3_decode(const PAPACC_U8 *,PAPACC_SIZE,PAPACC_U8 *,PAPACC_SIZE,PAPACC_SIZE *,int *,PC_LST1_HASH,void *,PC_ROOT8_COMPONENT,void *);
/* Seal one fragment from complete containing prefix context; never publish
 * approval/status or invent CP3. CP input must already contain factual values.
 * Zero CP remains absent. Caller retains all borrowed backing through return. */
PC_ROOT8_RESULT pc_root8_v3_snt_encode(const PAPACC_U8 *,PAPACC_SIZE,PAPACC_U8 *,PAPACC_SIZE,PAPACC_SIZE *,PC_LST1_HASH,void *);
PC_ROOT8_RESULT pc_root8_v3_pdg_encode(const PAPACC_U8 *,PAPACC_SIZE,PAPACC_U8 *,PAPACC_SIZE,PAPACC_SIZE *,PC_LST1_HASH,void *);
PC_ROOT8_RESULT pc_root8_v3_cp_encode(const PAPACC_U8 *,PAPACC_SIZE,PAPACC_U8 *,PAPACC_SIZE,PAPACC_SIZE *,PC_LST1_HASH,void *);
/* Full v2/v3 predecessor DATA relation, never actual CAS/current/history proof.
 * Changed SNT/delegation rows return INCOMPLETE pending source approval history.
 * Even OK grants no authority; D0 is structural absence, never trusted signer. */
PC_ROOT8_RESULT pc_root8_v3_base_link(const PAPACC_U8 *,PAPACC_SIZE,const PAPACC_U8 *,PAPACC_SIZE,PC_LST1_HASH,void *,PC_ROOT8_COMPONENT,void *);
#endif

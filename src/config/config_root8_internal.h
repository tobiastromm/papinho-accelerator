#ifndef PAPACC_CONFIG_ROOT8_INTERNAL_H
#define PAPACC_CONFIG_ROOT8_INTERNAL_H
#include "config_root8.h"
/* Private shared DATA routines. Caller MUST first validate the exact version's
 * complete header/ranges/offsets/counts. No allocation, mutation, retained
 * borrow or authority; same synchronous nonreentrant component contract. */
PC_ROOT8_RESULT pc_root8_snt_data(const PAPACC_U8 *,int,PC_LST1_HASH,void *);
PC_ROOT8_RESULT pc_root8_components_data(const PAPACC_U8 *,PC_LST1_HASH,void *,PC_ROOT8_COMPONENT,void *);
/* Shared historical PDG semantic/issuer validation. The caller must already
 * validate its exact containing v3/v4 header/ranges. No root reinterpretation. */
PC_ROOT8_RESULT pc_root8_pdg_data(const PAPACC_U8 *,int,PC_LST1_HASH,void *);
PC_ROOT8_RESULT pc_root8_pdg_records_data(const PAPACC_U8 *,PC_LST1_HASH,void *,PC_ROOT8_COMPONENT,void *);
#endif

#ifndef PAPACC_ROOT8_DATA_WIN32_H
#define PAPACC_ROOT8_DATA_WIN32_H
#include "config_root8.h"
/* Existing BODY05 data decoder only. No root pin, loader or native identity
 * proof returned. ROOT06 issuer carrier remains unsupported until its separate
 * origin/adapter gate is qualified. This library never calls commit/publish. */
PC_ROOT8_RESULT papacc_root8_body05_data(void *,unsigned,const PAPACC_U8 *,
 PAPACC_SIZE,const PAPACC_U8 *,const PAPACC_U8 *);
#include "server_configuration_commit_win32.h"
/* Borrowed immutable carrier floor and caller workspace, DATA only. The real
 * root owner must separately supply CURRENT, pins, source leases and budgets.
 * No configuration can select this callback as an authority. */
typedef struct PAPACC_ROOT8_CARRIER_DATA {
 PAPACC_CONFIGURATION_COMMIT *owner;
 PAPACC_CONFIG_VIEW *workspace;
 const PAPACC_U8 *floor;
 PAPACC_SIZE floor_bytes;
#if defined(PAPACC_ED11_WINDOWS_3MIB) && defined(PAPACC_OFFLINE_D3_WORKER)
 /* Private opt-in kind8 DATA context v1. Zero initialize the WHOLE context.
  * Same held canonical pin, no ownership transfer; caller retains it and all
  * exclusive disjoint arenas through synchronous return. No Stop-path calls.
  * CA decode arena is distinct from signer arena/cert scratch. None escapes.
  * reserved is a caller lower-bound precondition, NOT factual credit. */
 unsigned v3_version;
 PAPACC_CONFIG_PIN v3_pin;
 PAPACC_ISSUER_WINDOWS_WORKSPACE *v3_ca_workspace,*v3_signer_workspace;
 PAPACC_U8 *v3_certificate;PAPACC_SIZE v3_certificate_capacity,v3_reserved;
#endif
} PAPACC_ROOT8_CARRIER_DATA;
PC_ROOT8_RESULT papacc_root8_issuer_carrier_data(void *,unsigned,const PAPACC_U8 *,
 PAPACC_SIZE,const PAPACC_U8 *,const PAPACC_U8 *);
#endif

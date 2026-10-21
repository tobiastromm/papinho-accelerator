#ifndef PAPACC_ROOT8_SIGNER_DATA_WIN32_H
#define PAPACC_ROOT8_SIGNER_DATA_WIN32_H
#include "config_root8_v3.h"
#include "issuer_codec.h"
#include "server_issuer_x509_win32.h"
/* PRIVATE synchronous kind8 DATA adapter, no factual observation or source.
 * Caller owns immutable complete carrier/row/header/CA and ALL deep CA backing,
 * disjoint context/workspace/certificate scratch until return. Serialize Stop;
 * never call from a Stop path. No retained pointers, allocation, I/O or handles.
 * hash is the existing codec's SHA256 port, not an authority callback.
 * All buffers/CA backing/frames/coexistence need prior caller accounting.
 * already_reserved is a lower-bound guard, not proof of reserved credit.
 */
typedef struct PAPACC_ROOT8_SIGNER_DATA {
 const CERT_INFO *ca;
 PAPACC_ISSUER_WINDOWS_WORKSPACE *workspace;
 PAPACC_U8 *certificate;PAPACC_SIZE certificate_capacity;
 PC_LST1_HASH hash;void *hash_context;
 PAPACC_SIZE already_reserved;
} PAPACC_ROOT8_SIGNER_DATA;
/* ONLY kind8, version3. Full codec still validates BODY/LST/SNT/issuer/history
 * through its existing component composition. This result is DATA only:
 * no UTC/status freshness, CURRENT/pin/provenance, delegation grant or TRUST.
 * Missing protected CA/workspace => INCOMPLETE. Historical adapters unchanged.
 */
PC_ROOT8_RESULT papacc_root8_signer_data(void *,unsigned,const PAPACC_U8 *,
 PAPACC_SIZE,const PAPACC_U8 *,const PAPACC_U8 *);
#endif

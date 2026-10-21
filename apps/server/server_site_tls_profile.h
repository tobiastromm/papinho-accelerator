#ifndef PAPACC_SERVER_SITE_TLS_PROFILE_H
#define PAPACC_SERVER_SITE_TLS_PROFILE_H
#include "papacc/types.h"
/* One administrative profile per current configuration. No dynamic registry. */
typedef struct PAPACC_SITE_TLS_CATALOG {
 PAPACC_U32 present,version,reference,trust_system,hostname_required,sni_required,tls13_only,revoked;
 PAPACC_U8 identity[16],profile_identity[16],administrative_origin[16];
 PAPACC_U64 revision,profile_revision,not_before_utc_ms,expires_utc_ms;
 PAPACC_U64 max_bytes,max_duration_ms;
 PAPACC_U32 max_concurrent,alpn_length;
 PAPACC_U8 alpn[64]; /* exact vector of [length, opaque protocol], no wildcard */
} PAPACC_SITE_TLS_CATALOG;
#define PAPACC_SITE_TLS_CATALOG_INITIALIZER {0}
PAPACC_RESULT papacc_site_tls_catalog_validate(const PAPACC_SITE_TLS_CATALOG *);
PAPACC_RESULT papacc_site_tls_alpn_allowed(const PAPACC_SITE_TLS_CATALOG *,const PAPACC_U8 *,PAPACC_SIZE);
PAPACC_RESULT papacc_site_tls_catalog_select(const PAPACC_SITE_TLS_CATALOG *,PAPACC_U32,PAPACC_U64,PAPACC_U64,PAPACC_SITE_TLS_CATALOG *);
#endif

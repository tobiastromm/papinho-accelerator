#ifndef PAPACC_ENROLLMENT_PST_H
#define PAPACC_ENROLLMENT_PST_H
#include "enrollment.h"
#include "papinho_secure_transport.h"
/* Owned candidate metadata only, no PST handles acquired. Existing credential
 * owner keeps its lease and runtime alive through prepare/use/retention.
 * No root/model/codec factory can publish this profile yet: native listener
 * integration remains DENY. Not an alias of SECURE_PRINCIPAL/Site TLS. */
typedef struct EP_TLS_PROFILE {
 char provider[32];PAPACC_U8 alpn[15];PST_ALPN_PROTOCOL protocol;
 PST_CONNECTION_CONFIG config;unsigned initialized;
} EP_TLS_PROFILE;
EP_RESULT ep_tls_profile_prepare(EP_TLS_PROFILE *,pst_credentials *,const char *,PAPACC_SIZE);
EP_RESULT ep_tls_selected_data(const PAPACC_U8 *,PAPACC_SIZE,pst_u32);
#endif

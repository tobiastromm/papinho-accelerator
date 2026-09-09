#ifndef PAPACC_SECURITY_COMPOSITION_H
#define PAPACC_SECURITY_COMPOSITION_H

#include "papacc/types.h"
#include "papinho_secure_transport.h"

#define PAPACC_SECURITY_PROVIDER_ID_CAPACITY 32U
#define PAPACC_SECURITY_ALPN_PAPACC_1 "papacc/1"
#define PAPACC_SECURITY_ALPN_PAPACC_1_SIZE 8U

typedef enum PAPACC_SECURITY_COMPOSITION_STATE {
    PAPACC_SECURITY_COMPOSITION_UNINITIALIZED = 0,
    PAPACC_SECURITY_COMPOSITION_READY = 1,
    PAPACC_SECURITY_COMPOSITION_CLOSED = 2
} PAPACC_SECURITY_COMPOSITION_STATE;

typedef struct PAPACC_SECURITY_DER_ITEM {
    const PAPACC_U8 *data;
    PAPACC_SIZE size;
} PAPACC_SECURITY_DER_ITEM;

typedef PAPACC_RESULT (*PAPACC_SECURITY_PROVIDER_BOOTSTRAP_FN)(void *context);

typedef struct PAPACC_SECURITY_COMPOSITION_INPUTS {
    const PAPACC_SECURITY_DER_ITEM *local_certificate_chain;
    PAPACC_SIZE local_certificate_count;
    const PAPACC_U8 *local_private_key_pkcs8_der;
    PAPACC_SIZE local_private_key_pkcs8_der_size;
    const PAPACC_SECURITY_DER_ITEM *peer_trust_anchors;
    PAPACC_SIZE peer_trust_anchor_count;
    const PAPACC_U8 *secure_principal_alpn;
    PAPACC_SIZE secure_principal_alpn_size;
    const char *provider_id;
    PAPACC_SECURITY_PROVIDER_BOOTSTRAP_FN provider_bootstrap;
    void *provider_bootstrap_context;
} PAPACC_SECURITY_COMPOSITION_INPUTS;

typedef struct PAPACC_SECURITY_COMPOSITION {
    PAPACC_SECURITY_COMPOSITION_STATE state;
    pst_runtime *runtime;
    pst_credentials *local_credentials;
    pst_trust *peer_trust;
    char provider_id[PAPACC_SECURITY_PROVIDER_ID_CAPACITY];
    pst_u8 secure_principal_alpn[PAPACC_SECURITY_ALPN_PAPACC_1_SIZE];
    PST_ALPN_PROTOCOL secure_principal_alpn_protocol;
} PAPACC_SECURITY_COMPOSITION;

#define PAPACC_SECURITY_COMPOSITION_INITIALIZER \
    { PAPACC_SECURITY_COMPOSITION_UNINITIALIZED, NULL, NULL, NULL, \
      { 0 }, { 0 }, { NULL, 0U } }

PAPACC_RESULT papacc_security_composition_init(
    PAPACC_SECURITY_COMPOSITION *composition,
    const PAPACC_SECURITY_COMPOSITION_INPUTS *inputs);

void papacc_security_composition_release(
    PAPACC_SECURITY_COMPOSITION *composition);

PAPACC_BOOL papacc_security_composition_is_ready(
    const PAPACC_SECURITY_COMPOSITION *composition);

PAPACC_U32 papacc_security_secure_principal_required_capabilities(void);

PAPACC_RESULT papacc_security_composition_build_secure_principal_config(
    const PAPACC_SECURITY_COMPOSITION *composition,
    PST_CONNECTION_CONFIG *connection_config);

#endif

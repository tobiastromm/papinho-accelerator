#ifndef PAPACC_PST_OUTBOUND_TLS_H
#define PAPACC_PST_OUTBOUND_TLS_H

#include <winsock2.h>
#include "papacc/types.h"
#include "papinho_secure_transport.h"
#include "pst_secure_processor.h"

#define PAPACC_OUTBOUND_TLS_NAME_CAPACITY 256U
#define PAPACC_OUTBOUND_TLS_ALPN_CAPACITY 255U

typedef struct PAPACC_OUTBOUND_TLS_POLICY {
    pst_runtime *runtime;
    pst_trust *trust;
    const char *provider_id;
    const PAPACC_U8 *alpn;
    PAPACC_SIZE alpn_size;
    PAPACC_BOOL alpn_required;
} PAPACC_OUTBOUND_TLS_POLICY;

typedef struct PAPACC_OUTBOUND_TLS {
    PAPACC_PST_SECURE_PROCESSOR processor;
    char expected_peer_name[PAPACC_OUTBOUND_TLS_NAME_CAPACITY];
    PAPACC_U8 alpn_value[PAPACC_OUTBOUND_TLS_ALPN_CAPACITY];
    PST_ALPN_PROTOCOL alpn_protocol;
    PAPACC_BOOL established;
    PAPACC_BOOL clean_peer_close;
    PAPACC_BOOL truncated;
#ifdef PAPACC_ENABLE_PST_TEST_CORRELATION
    char test_local_endpoint[96];
    char test_remote_endpoint[96];
    pst_connection *test_correlation_connection;
    PAPACC_U32 test_correlation_token_high;
    PAPACC_U32 test_correlation_token_low;
    PAPACC_U32 test_correlation_association_attempts;
    PAPACC_U32 test_correlation_association_successes;
    PAPACC_U32 test_correlation_waitset_reregistrations;
    PAPACC_BOOL test_correlation_associated;
#endif
} PAPACC_OUTBOUND_TLS;

#define PAPACC_OUTBOUND_TLS_INITIALIZER { \
    PAPACC_PST_SECURE_PROCESSOR_INITIALIZER,{0},{0},{NULL,0U}, \
    PAPACC_FALSE,PAPACC_FALSE,PAPACC_FALSE }

PAPACC_RESULT papacc_outbound_tls_init(PAPACC_OUTBOUND_TLS *,
    const PAPACC_OUTBOUND_TLS_POLICY *,const char *,PAPACC_U64);
#ifdef PAPACC_ENABLE_PST_TEST_CORRELATION
PAPACC_RESULT papacc_outbound_tls_test_capture_endpoints(
    PAPACC_OUTBOUND_TLS *,SOCKET);
#endif
PAPACC_RESULT papacc_outbound_tls_attach_socket(PAPACC_OUTBOUND_TLS *,SOCKET);
PAPACC_RESULT papacc_outbound_tls_handshake_once(PAPACC_OUTBOUND_TLS *,
    PAPACC_BOOL *);
PAPACC_RESULT papacc_outbound_tls_read(void *,PAPACC_U8 *,PAPACC_SIZE,
    PAPACC_SIZE *,PAPACC_BOOL *);
PAPACC_RESULT papacc_outbound_tls_write(void *,const PAPACC_U8 *,PAPACC_SIZE,
    PAPACC_SIZE *);
PAPACC_RESULT papacc_outbound_tls_half_close(void *);
PAPACC_RESULT papacc_outbound_tls_interest(PAPACC_OUTBOUND_TLS *,
    PAPACC_BOOL *,PAPACC_BOOL *);
PAPACC_RESULT papacc_outbound_tls_check_deadline(PAPACC_OUTBOUND_TLS *,
    PAPACC_U64,PAPACC_BOOL *);
PAPACC_RESULT papacc_outbound_tls_shutdown_once(PAPACC_OUTBOUND_TLS *,
    PAPACC_BOOL *);
void papacc_outbound_tls_abort(PAPACC_OUTBOUND_TLS *);
void papacc_outbound_tls_close(void *);

#endif

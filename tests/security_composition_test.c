#include <string.h>

#include "security_composition.h"
#include "pst_provider_bootstrap_win32.h"

#define CHECK(condition, code) do { if (!(condition)) return (code); } while (0)

typedef struct TEST_LOG_SINK {
    PAPACC_SIZE count;
    PAPACC_LOG_RECORD record;
} TEST_LOG_SINK;
static PAPACC_LOGGER *test_logger;

static void test_log_sink(void *context, const PAPACC_LOG_RECORD *record)
{
    TEST_LOG_SINK *sink = (TEST_LOG_SINK *)context;
    (void)record;
    ++sink->count;
    sink->record = *record;
}

static PAPACC_RESULT failing_bootstrap(void *context)
{
    (void)context;
    return PAPACC_RESULT_INTERNAL_ERROR;
}

static void initialize_inputs(PAPACC_SECURITY_COMPOSITION_INPUTS *inputs,
    PAPACC_SECURITY_DER_ITEM *certificate,
    PAPACC_SECURITY_DER_ITEM *trust_anchor,
    const PAPACC_U8 *private_key)
{
    static const PAPACC_U8 certificate_der[] = { 0x30U, 0x01U, 0x00U };
    static const PAPACC_U8 trust_der[] = { 0x30U, 0x01U, 0x01U };
    certificate->data = certificate_der;
    certificate->size = sizeof(certificate_der);
    trust_anchor->data = trust_der;
    trust_anchor->size = sizeof(trust_der);
    memset(inputs, 0, sizeof(*inputs));
    inputs->local_certificate_chain = certificate;
    inputs->local_certificate_count = 1U;
    inputs->local_private_key_pkcs8_der = private_key;
    inputs->local_private_key_pkcs8_der_size = 3U;
    inputs->peer_trust_anchors = trust_anchor;
    inputs->peer_trust_anchor_count = 1U;
    inputs->secure_principal_alpn =
        (const PAPACC_U8 *)PAPACC_SECURITY_ALPN_PAPACC_1;
    inputs->secure_principal_alpn_size =
        PAPACC_SECURITY_ALPN_PAPACC_1_SIZE;
    inputs->provider_id = "openssl";
    inputs->provider_bootstrap = papacc_pst_provider_bootstrap_win32;
    inputs->logger = test_logger;
}

int main(void)
{
    static const PAPACC_U8 private_key[] = { 0x30U, 0x01U, 0x02U };
    PAPACC_SECURITY_COMPOSITION composition =
        PAPACC_SECURITY_COMPOSITION_INITIALIZER;
    PAPACC_SECURITY_COMPOSITION_INPUTS inputs;
    PAPACC_SECURITY_DER_ITEM certificate;
    PAPACC_SECURITY_DER_ITEM trust_anchor;
    PST_CONNECTION_CONFIG config;
    PST_PROVIDER_INFO provider_info;
    PST_RUNTIME_INFO runtime_info;
    PAPACC_U32 required;
    pst_size index;
    int openssl_found = 0;
    TEST_LOG_SINK sink = { 0U };
    PAPACC_LOGGER logger;

    CHECK(papacc_logger_init(&logger, test_log_sink, &sink,
        PAPACC_LOG_INFO) == PAPACC_RESULT_OK, 34);
    test_logger = &logger;

    CHECK(!papacc_security_composition_is_ready(&composition), 1);
    CHECK(papacc_security_composition_init(NULL, NULL) ==
        PAPACC_RESULT_INVALID_ARGUMENT, 2);
    initialize_inputs(&inputs, &certificate, &trust_anchor, private_key);

    inputs.local_certificate_chain = NULL;
    CHECK(papacc_security_composition_init(&composition, &inputs) ==
        PAPACC_RESULT_INVALID_ARGUMENT, 3);
    CHECK(composition.state == PAPACC_SECURITY_COMPOSITION_UNINITIALIZED, 4);
    initialize_inputs(&inputs, &certificate, &trust_anchor, private_key);
    inputs.local_private_key_pkcs8_der = NULL;
    CHECK(papacc_security_composition_init(&composition, &inputs) ==
        PAPACC_RESULT_INVALID_ARGUMENT, 5);
    initialize_inputs(&inputs, &certificate, &trust_anchor, private_key);
    inputs.peer_trust_anchors = NULL;
    CHECK(papacc_security_composition_init(&composition, &inputs) ==
        PAPACC_RESULT_INVALID_ARGUMENT, 6);
    initialize_inputs(&inputs, &certificate, &trust_anchor, private_key);
    inputs.provider_id = "";
    CHECK(papacc_security_composition_init(&composition, &inputs) ==
        PAPACC_RESULT_INVALID_ARGUMENT, 7);

    initialize_inputs(&inputs, &certificate, &trust_anchor, private_key);
    inputs.secure_principal_alpn = (const PAPACC_U8 *)"wrong/1";
    inputs.secure_principal_alpn_size = 7U;
    CHECK(papacc_security_composition_init(&composition, &inputs) ==
        PAPACC_RESULT_INVALID_ARGUMENT, 31);

    initialize_inputs(&inputs, &certificate, &trust_anchor, private_key);
    inputs.provider_id = "provider-that-does-not-exist";
    CHECK(papacc_security_composition_init(&composition, &inputs) ==
        PAPACC_RESULT_NOT_SUPPORTED, 29);
    CHECK(composition.state == PAPACC_SECURITY_COMPOSITION_UNINITIALIZED &&
        composition.runtime == NULL && composition.local_credentials == NULL &&
        composition.peer_trust == NULL, 30);

    initialize_inputs(&inputs, &certificate, &trust_anchor, private_key);
    inputs.provider_bootstrap = failing_bootstrap;
    CHECK(papacc_security_composition_init(&composition, &inputs) ==
        PAPACC_RESULT_INTERNAL_ERROR, 8);
    CHECK(composition.state == PAPACC_SECURITY_COMPOSITION_UNINITIALIZED &&
        composition.runtime == NULL && composition.local_credentials == NULL &&
        composition.peer_trust == NULL, 9);

    initialize_inputs(&inputs, &certificate, &trust_anchor, private_key);
    sink.count = 0U;
    CHECK(papacc_security_composition_init(&composition, &inputs) ==
        PAPACC_RESULT_OK, 10);
    CHECK(sink.count != 0U && sink.record.event_id ==
        PAPACC_LOG_EVENT_SECURE_TRANSPORT_RUNTIME_READY &&
        sink.record.level == PAPACC_LOG_INFO, 35);
    CHECK(papacc_security_composition_is_ready(&composition), 11);
    CHECK(papacc_security_composition_init(&composition, &inputs) ==
        PAPACC_RESULT_INVALID_STATE, 12);

    required = papacc_security_secure_principal_required_capabilities();
    CHECK(required == (PST_CAP_TLS_1_3 | PST_CAP_ROLE_SERVER |
        PST_CAP_LOCAL_IDENTITY | PST_CAP_PEER_CERT_AUTH |
        PST_CAP_ALPN_SERVER | PST_CAP_CUSTOM_TRUST | PST_CAP_PEER_INFO |
        PST_CAP_NONBLOCKING | PST_CAP_BACKEND_WAIT |
        PST_CAP_GRACEFUL_SHUTDOWN), 13);
    CHECK((required & (PST_CAP_PEER_CERT_OPTIONAL | PST_CAP_PEER_NAME_VERIFY |
        PST_CAP_SYSTEM_TRUST)) == 0U, 14);

    CHECK(papacc_security_composition_build_secure_principal_config(
        &composition, &config) == PAPACC_RESULT_OK, 15);
    CHECK(config.role == PST_CONNECTION_ROLE_SERVER, 16);
    CHECK(config.provider_selection.mode == PST_BACKEND_SELECTION_EXACT &&
        strcmp(config.provider_selection.exact_provider_id, "openssl") == 0 &&
        config.provider_selection.required_capabilities == required, 17);
    CHECK(config.local_identity.credentials == composition.local_credentials,
        18);
    CHECK(config.peer_authentication.certificate_mode ==
        PST_PEER_CERTIFICATE_REQUIRED &&
        config.peer_authentication.trust == composition.peer_trust &&
        config.peer_authentication.expected_peer_name == NULL, 19);
    CHECK(config.tls.minimum_version == PST_TLS_VERSION_1_3 &&
        config.tls.maximum_version == PST_TLS_VERSION_1_3 &&
        config.tls.resumption == PST_FEATURE_DISABLED &&
        config.tls.early_data == PST_FEATURE_DISABLED, 20);
    CHECK(config.alpn.mode == PST_FEATURE_REQUIRED &&
        config.alpn.protocol_count == 1U &&
        config.alpn.protocols[0].size == PAPACC_SECURITY_ALPN_PAPACC_1_SIZE &&
        memcmp(config.alpn.protocols[0].data,
            PAPACC_SECURITY_ALPN_PAPACC_1,
            PAPACC_SECURITY_ALPN_PAPACC_1_SIZE) == 0, 21);

    memset(&runtime_info, 0, sizeof(runtime_info));
    runtime_info.struct_size = (pst_u32)sizeof(runtime_info);
    runtime_info.api_version = PST_API_VERSION;
    CHECK(pst_runtime_get_info(composition.runtime, &runtime_info) ==
        PST_RESULT_OK, 22);
    for (index = 0U; index < runtime_info.provider_count; ++index) {
        memset(&provider_info, 0, sizeof(provider_info));
        provider_info.struct_size = (pst_u32)sizeof(provider_info);
        provider_info.api_version = PST_API_VERSION;
        CHECK(pst_runtime_get_provider_info(composition.runtime, index,
            &provider_info) == PST_RESULT_OK, 23);
        if (strcmp(provider_info.provider_id, "openssl") == 0) {
            openssl_found = 1;
            CHECK(provider_info.available != 0U &&
                (required & ~provider_info.server_capabilities) == 0U, 24);
        }
    }
    CHECK(openssl_found, 25);

    papacc_security_composition_release(&composition);
    CHECK(composition.state == PAPACC_SECURITY_COMPOSITION_CLOSED &&
        !papacc_security_composition_is_ready(&composition) &&
        composition.runtime == NULL && composition.local_credentials == NULL &&
        composition.peer_trust == NULL, 26);
    papacc_security_composition_release(&composition);
    CHECK(composition.state == PAPACC_SECURITY_COMPOSITION_CLOSED, 27);
    CHECK(papacc_security_composition_build_secure_principal_config(
        &composition, &config) == PAPACC_RESULT_INVALID_STATE, 28);

    composition = (PAPACC_SECURITY_COMPOSITION)
        PAPACC_SECURITY_COMPOSITION_INITIALIZER;
    CHECK(papacc_logger_init(&logger, test_log_sink, &sink,
        PAPACC_LOG_LEVEL_OFF) == PAPACC_RESULT_OK, 36);
    sink.count = 0U;
    initialize_inputs(&inputs, &certificate, &trust_anchor, private_key);
    CHECK(papacc_security_composition_init(&composition, &inputs) ==
        PAPACC_RESULT_OK, 32);
    CHECK(sink.count == 0U, 37);
    papacc_security_composition_release(&composition);
    CHECK(composition.state == PAPACC_SECURITY_COMPOSITION_CLOSED, 33);
    return 0;
}

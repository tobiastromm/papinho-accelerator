#include "security_composition.h"
#include "pst_log_adapter.h"

#include <stdlib.h>
#include <string.h>

static PAPACC_RESULT papacc_security_map_pst_result(PST_RESULT result)
{
    if (result == PST_RESULT_OK) return PAPACC_RESULT_OK;
    if (result == PST_RESULT_INVALID_ARGUMENT)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (result == PST_RESULT_INVALID_STATE)
        return PAPACC_RESULT_INVALID_STATE;
    if (result == PST_RESULT_UNSUPPORTED)
        return PAPACC_RESULT_NOT_SUPPORTED;
    if (result == PST_RESULT_OUT_OF_MEMORY)
        return PAPACC_RESULT_OUT_OF_MEMORY;
    return PAPACC_RESULT_INTERNAL_ERROR;
}

static int papacc_security_der_items_valid(
    const PAPACC_SECURITY_DER_ITEM *items, PAPACC_SIZE count)
{
    PAPACC_SIZE index;
    if (items == NULL || count == 0U) return 0;
    for (index = 0U; index < count; ++index) {
        if (items[index].data == NULL || items[index].size == 0U) return 0;
    }
    return 1;
}

static PST_RESULT papacc_security_provider_is_eligible(
    pst_runtime *runtime, const char *provider_id, PAPACC_U32 required)
{
    PST_RUNTIME_INFO runtime_info;
    PST_PROVIDER_INFO provider_info;
    pst_size index;

    memset(&runtime_info, 0, sizeof(runtime_info));
    runtime_info.struct_size = (pst_u32)sizeof(runtime_info);
    runtime_info.api_version = PST_API_VERSION;
    if (pst_runtime_get_info(runtime, &runtime_info) != PST_RESULT_OK)
        return PST_RESULT_INVALID_STATE;
    for (index = 0U; index < runtime_info.provider_count; ++index) {
        memset(&provider_info, 0, sizeof(provider_info));
        provider_info.struct_size = (pst_u32)sizeof(provider_info);
        provider_info.api_version = PST_API_VERSION;
        if (pst_runtime_get_provider_info(runtime, index, &provider_info) !=
            PST_RESULT_OK)
            return PST_RESULT_INVALID_STATE;
        if (strcmp(provider_info.provider_id, provider_id) == 0) {
            if (provider_info.available == 0U ||
                (required & ~provider_info.server_capabilities) != 0U)
                return PST_RESULT_UNSUPPORTED;
            return PST_RESULT_OK;
        }
    }
    return PST_RESULT_UNSUPPORTED;
}

PAPACC_U32 papacc_security_secure_principal_required_capabilities(void)
{
    return PST_CAP_TLS_1_3 | PST_CAP_ROLE_SERVER |
        PST_CAP_LOCAL_IDENTITY | PST_CAP_PEER_CERT_AUTH |
        PST_CAP_ALPN_SERVER | PST_CAP_CUSTOM_TRUST | PST_CAP_PEER_INFO |
        PST_CAP_NONBLOCKING | PST_CAP_BACKEND_WAIT;
}

PAPACC_RESULT papacc_security_composition_init(
    PAPACC_SECURITY_COMPOSITION *composition,
    const PAPACC_SECURITY_COMPOSITION_INPUTS *inputs)
{
    PAPACC_SECURITY_COMPOSITION pending =
        PAPACC_SECURITY_COMPOSITION_INITIALIZER;
    PST_RUNTIME_OPTIONS runtime_options;
    PST_CREDENTIAL_SOURCE credential_source;
    PST_TRUST_SOURCE trust_source;
    PST_LOG_CONFIG log_config;
    PST_DER_ITEM *certificate_chain = NULL;
    PST_DER_ITEM *trust_anchors = NULL;
    PST_RESULT pst_result;
    PAPACC_RESULT result;
    PAPACC_SIZE index;
    size_t provider_id_size;

    if (composition == NULL || inputs == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (composition->state != PAPACC_SECURITY_COMPOSITION_UNINITIALIZED)
        return PAPACC_RESULT_INVALID_STATE;
    if (!papacc_security_der_items_valid(inputs->local_certificate_chain,
            inputs->local_certificate_count) ||
        inputs->local_private_key_pkcs8_der == NULL ||
        inputs->local_private_key_pkcs8_der_size == 0U ||
        !papacc_security_der_items_valid(inputs->peer_trust_anchors,
            inputs->peer_trust_anchor_count) ||
        inputs->secure_principal_alpn == NULL ||
        inputs->secure_principal_alpn_size !=
            PAPACC_SECURITY_ALPN_PAPACC_1_SIZE ||
        memcmp(inputs->secure_principal_alpn,
            PAPACC_SECURITY_ALPN_PAPACC_1,
            PAPACC_SECURITY_ALPN_PAPACC_1_SIZE) != 0 ||
        inputs->provider_id == NULL || inputs->provider_id[0] == '\0' ||
        inputs->provider_bootstrap == NULL || inputs->logger == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    provider_id_size = strlen(inputs->provider_id);
    if (provider_id_size >= PAPACC_SECURITY_PROVIDER_ID_CAPACITY)
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    if (inputs->local_certificate_count >
            ((PAPACC_SIZE)-1) / sizeof(*certificate_chain) ||
        inputs->peer_trust_anchor_count >
            ((PAPACC_SIZE)-1) / sizeof(*trust_anchors))
        return PAPACC_RESULT_LIMIT_EXCEEDED;

    result = inputs->provider_bootstrap(inputs->provider_bootstrap_context);
    if (result != PAPACC_RESULT_OK) return result;

    pending.log_adapter = (PAPACC_PST_LOG_ADAPTER *)calloc(
        1U, sizeof(*pending.log_adapter));
    if (pending.log_adapter == NULL) return PAPACC_RESULT_OUT_OF_MEMORY;
    *pending.log_adapter = (PAPACC_PST_LOG_ADAPTER)
        PAPACC_PST_LOG_ADAPTER_INITIALIZER;
    result = papacc_pst_log_adapter_init(pending.log_adapter, inputs->logger);
    if (result != PAPACC_RESULT_OK) {
        free(pending.log_adapter);
        return result;
    }
    result = papacc_pst_log_adapter_make_config(pending.log_adapter,
        &log_config);
    if (result != PAPACC_RESULT_OK) {
        papacc_pst_log_adapter_release(pending.log_adapter);
        free(pending.log_adapter);
        return result;
    }

    memset(&runtime_options, 0, sizeof(runtime_options));
    runtime_options.struct_size = (pst_u32)sizeof(runtime_options);
    runtime_options.api_version = PST_API_VERSION;
    pst_result = pst_runtime_create_with_logging(&runtime_options, &log_config,
        &pending.runtime, NULL);
    if (pst_result != PST_RESULT_OK) goto fail;

    certificate_chain = (PST_DER_ITEM *)calloc(
        inputs->local_certificate_count, sizeof(*certificate_chain));
    trust_anchors = (PST_DER_ITEM *)calloc(
        inputs->peer_trust_anchor_count, sizeof(*trust_anchors));
    if (certificate_chain == NULL || trust_anchors == NULL) {
        pst_result = PST_RESULT_OUT_OF_MEMORY;
        goto fail;
    }
    for (index = 0U; index < inputs->local_certificate_count; ++index) {
        certificate_chain[index].data =
            inputs->local_certificate_chain[index].data;
        certificate_chain[index].size =
            inputs->local_certificate_chain[index].size;
    }
    for (index = 0U; index < inputs->peer_trust_anchor_count; ++index) {
        trust_anchors[index].data = inputs->peer_trust_anchors[index].data;
        trust_anchors[index].size = inputs->peer_trust_anchors[index].size;
    }

    memset(&credential_source, 0, sizeof(credential_source));
    credential_source.struct_size = (pst_u32)sizeof(credential_source);
    credential_source.api_version = PST_API_VERSION;
    credential_source.kind = PST_CREDENTIAL_SOURCE_CERT_CHAIN_DER_PKCS8_DER;
    credential_source.certificate_chain = certificate_chain;
    credential_source.certificate_count = inputs->local_certificate_count;
    credential_source.private_key_der = inputs->local_private_key_pkcs8_der;
    credential_source.private_key_der_size =
        inputs->local_private_key_pkcs8_der_size;
    pst_result = pst_credentials_create(&credential_source,
        &pending.local_credentials);
    if (pst_result != PST_RESULT_OK) goto fail;

    memset(&trust_source, 0, sizeof(trust_source));
    trust_source.struct_size = (pst_u32)sizeof(trust_source);
    trust_source.api_version = PST_API_VERSION;
    trust_source.kind = PST_TRUST_SOURCE_CUSTOM_CA_DER;
    trust_source.anchors = trust_anchors;
    trust_source.anchor_count = inputs->peer_trust_anchor_count;
    pst_result = pst_trust_create(&trust_source, &pending.peer_trust);
    if (pst_result != PST_RESULT_OK) goto fail;

    pst_result = papacc_security_provider_is_eligible(pending.runtime,
        inputs->provider_id,
        papacc_security_secure_principal_required_capabilities());
    if (pst_result != PST_RESULT_OK) goto fail;

    memcpy(pending.provider_id, inputs->provider_id, provider_id_size + 1U);
    memcpy(pending.secure_principal_alpn, inputs->secure_principal_alpn,
        PAPACC_SECURITY_ALPN_PAPACC_1_SIZE);
    pending.state = PAPACC_SECURITY_COMPOSITION_READY;
    free(certificate_chain);
    free(trust_anchors);
    *composition = pending;
    composition->secure_principal_alpn_protocol.data =
        composition->secure_principal_alpn;
    composition->secure_principal_alpn_protocol.size =
        PAPACC_SECURITY_ALPN_PAPACC_1_SIZE;
    return PAPACC_RESULT_OK;

fail:
    free(certificate_chain);
    free(trust_anchors);
    if (pending.peer_trust != NULL) pst_trust_release(pending.peer_trust);
    if (pending.local_credentials != NULL)
        pst_credentials_release(pending.local_credentials);
    if (pending.runtime != NULL) pst_runtime_release(pending.runtime);
    if (pending.log_adapter != NULL) {
        papacc_pst_log_adapter_release(pending.log_adapter);
        free(pending.log_adapter);
    }
    return papacc_security_map_pst_result(pst_result);
}

void papacc_security_composition_release(
    PAPACC_SECURITY_COMPOSITION *composition)
{
    if (composition == NULL ||
        composition->state == PAPACC_SECURITY_COMPOSITION_CLOSED)
        return;
    if (composition->peer_trust != NULL)
        pst_trust_release(composition->peer_trust);
    if (composition->local_credentials != NULL)
        pst_credentials_release(composition->local_credentials);
    if (composition->runtime != NULL) pst_runtime_release(composition->runtime);
    if (composition->log_adapter != NULL) {
        papacc_pst_log_adapter_release(composition->log_adapter);
        free(composition->log_adapter);
    }
    composition->runtime = NULL;
    composition->local_credentials = NULL;
    composition->peer_trust = NULL;
    composition->log_adapter = NULL;
    memset(composition->provider_id, 0, sizeof(composition->provider_id));
    memset(composition->secure_principal_alpn, 0,
        sizeof(composition->secure_principal_alpn));
    composition->secure_principal_alpn_protocol.data = NULL;
    composition->secure_principal_alpn_protocol.size = 0U;
    composition->state = PAPACC_SECURITY_COMPOSITION_CLOSED;
}

PAPACC_BOOL papacc_security_composition_is_ready(
    const PAPACC_SECURITY_COMPOSITION *composition)
{
    return composition != NULL &&
        composition->state == PAPACC_SECURITY_COMPOSITION_READY ?
        PAPACC_TRUE : PAPACC_FALSE;
}

PAPACC_RESULT papacc_security_composition_build_secure_principal_config(
    const PAPACC_SECURITY_COMPOSITION *composition,
    PST_CONNECTION_CONFIG *connection_config)
{
    if (composition == NULL || connection_config == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (composition->state != PAPACC_SECURITY_COMPOSITION_READY)
        return PAPACC_RESULT_INVALID_STATE;

    memset(connection_config, 0, sizeof(*connection_config));
    connection_config->struct_size = (pst_u32)sizeof(*connection_config);
    connection_config->api_version = PST_API_VERSION;
    connection_config->role = PST_CONNECTION_ROLE_SERVER;
    connection_config->provider_selection.struct_size =
        (pst_u32)sizeof(connection_config->provider_selection);
    connection_config->provider_selection.api_version = PST_API_VERSION;
    connection_config->provider_selection.mode = PST_BACKEND_SELECTION_EXACT;
    connection_config->provider_selection.exact_provider_id =
        composition->provider_id;
    connection_config->provider_selection.required_capabilities =
        papacc_security_secure_principal_required_capabilities();
    connection_config->local_identity.struct_size =
        (pst_u32)sizeof(connection_config->local_identity);
    connection_config->local_identity.api_version = PST_API_VERSION;
    connection_config->local_identity.credentials =
        composition->local_credentials;
    connection_config->peer_authentication.struct_size =
        (pst_u32)sizeof(connection_config->peer_authentication);
    connection_config->peer_authentication.api_version = PST_API_VERSION;
    connection_config->peer_authentication.certificate_mode =
        PST_PEER_CERTIFICATE_REQUIRED;
    connection_config->peer_authentication.trust = composition->peer_trust;
    connection_config->tls.struct_size =
        (pst_u32)sizeof(connection_config->tls);
    connection_config->tls.api_version = PST_API_VERSION;
    connection_config->tls.minimum_version = PST_TLS_VERSION_1_3;
    connection_config->tls.maximum_version = PST_TLS_VERSION_1_3;
    connection_config->tls.resumption = PST_FEATURE_DISABLED;
    connection_config->tls.early_data = PST_FEATURE_DISABLED;
    connection_config->tls.require_graceful_shutdown = PST_FEATURE_REQUIRED;
    connection_config->alpn.struct_size =
        (pst_u32)sizeof(connection_config->alpn);
    connection_config->alpn.api_version = PST_API_VERSION;
    connection_config->alpn.mode = PST_FEATURE_REQUIRED;
    connection_config->alpn.protocols =
        &composition->secure_principal_alpn_protocol;
    connection_config->alpn.protocol_count = 1U;
    return PAPACC_RESULT_OK;
}

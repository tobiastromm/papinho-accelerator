#include "server_listener_configuration.h"

#include <string.h>

#define CHECK(condition, code) do { if (!(condition)) return (code); } while (0)

static PAPACC_RESULT resolve_principal(void *context,
    const PAPACC_PEER_EVIDENCE *evidence,
    PAPACC_PRINCIPAL_RESOLUTION *resolution, PAPACC_PRINCIPAL *principal)
{
    (void)context;
    (void)evidence;
    *resolution = PAPACC_PRINCIPAL_RESOLVED;
    principal->valid = PAPACC_TRUE;
    return PAPACC_RESULT_OK;
}

static PAPACC_RESULT authorize(void *context,
    const PAPACC_PRINCIPAL *principal, PAPACC_AUTHORIZATION_ACTION action,
    PAPACC_AUTHORIZATION_DECISION *decision)
{
    (void)context;
    (void)principal;
    (void)action;
    *decision = PAPACC_AUTHORIZATION_ALLOW;
    return PAPACC_RESULT_OK;
}

static void release_snapshot(void *context)
{
    PAPACC_U32 *calls = (PAPACC_U32 *)context;
    ++*calls;
}

static PAPACC_RESULT unresolved_configuration(void *context,
    const PAPACC_SECURITY_CONFIGURATION_REF *reference,
    PAPACC_SECURITY_CONFIGURATION_RESOLUTION *resolution,
    PAPACC_RESOLVED_SERVER_SECURITY_CONFIGURATION *configuration)
{
    (void)context;
    (void)reference;
    (void)configuration;
    *resolution = PAPACC_SECURITY_CONFIGURATION_NOT_FOUND;
    return PAPACC_RESULT_OK;
}

static PAPACC_RESULT invalid_configuration(void *context,
    const PAPACC_SECURITY_CONFIGURATION_REF *reference,
    PAPACC_SECURITY_CONFIGURATION_RESOLUTION *resolution,
    PAPACC_RESOLVED_SERVER_SECURITY_CONFIGURATION *configuration)
{
    (void)context;
    (void)reference;
    (void)configuration;
    *resolution = PAPACC_SECURITY_CONFIGURATION_INVALID;
    return PAPACC_RESULT_OK;
}

static PAPACC_RESULT resolver_error(void *context,
    const PAPACC_SECURITY_CONFIGURATION_REF *reference,
    PAPACC_SECURITY_CONFIGURATION_RESOLUTION *resolution,
    PAPACC_RESOLVED_SERVER_SECURITY_CONFIGURATION *configuration)
{
    (void)context;
    (void)reference;
    (void)resolution;
    (void)configuration;
    return PAPACC_RESULT_INTERNAL_ERROR;
}

static PAPACC_RESULT resolved_invalid_composition(void *context,
    const PAPACC_SECURITY_CONFIGURATION_REF *reference,
    PAPACC_SECURITY_CONFIGURATION_RESOLUTION *resolution,
    PAPACC_RESOLVED_SERVER_SECURITY_CONFIGURATION *configuration)
{
    (void)reference;
    *configuration =
        *(const PAPACC_RESOLVED_SERVER_SECURITY_CONFIGURATION *)context;
    *resolution = PAPACC_SECURITY_CONFIGURATION_RESOLVED;
    return PAPACC_RESULT_OK;
}

int main(void)
{
    static const PAPACC_U8 reference_value[] = "server-main";
    static const PAPACC_U8 other_value[] = "server-other";
    static const PAPACC_U8 byte = 1U;
    PAPACC_SECURITY_DER_ITEM item = { &byte, 1U };
    PAPACC_SECURITY_CONFIGURATION_REF reference =
        PAPACC_SECURITY_CONFIGURATION_REF_INITIALIZER;
    PAPACC_SECURITY_CONFIGURATION_REF same =
        PAPACC_SECURITY_CONFIGURATION_REF_INITIALIZER;
    PAPACC_SECURITY_CONFIGURATION_REF other =
        PAPACC_SECURITY_CONFIGURATION_REF_INITIALIZER;
    PAPACC_SERVER_LISTENER_CONFIGURATION listeners[2];
    PAPACC_SERVER_LISTENER_CONFIGURATION_SET set =
        PAPACC_SERVER_LISTENER_CONFIGURATION_SET_INITIALIZER;
    PAPACC_RESOLVED_SERVER_SECURITY_CONFIGURATION resolved =
        PAPACC_RESOLVED_SERVER_SECURITY_CONFIGURATION_INITIALIZER;
    PAPACC_RESOLVED_SERVER_SECURITY_CONFIGURATION bad =
        PAPACC_RESOLVED_SERVER_SECURITY_CONFIGURATION_INITIALIZER;
    PAPACC_U32 releases = 0U;
    PAPACC_SECURITY_COMPOSITION composition =
        PAPACC_SECURITY_COMPOSITION_INITIALIZER;
    PAPACC_SECURITY_CONFIGURATION_RESOLUTION resolution =
        PAPACC_SECURITY_CONFIGURATION_RESOLVER_ERROR;

    CHECK(papacc_security_configuration_ref_set(&reference, reference_value,
        sizeof(reference_value) - 1U) == PAPACC_RESULT_OK, 1);
    CHECK(papacc_security_configuration_ref_set(&same, reference_value,
        sizeof(reference_value) - 1U) == PAPACC_RESULT_OK, 2);
    CHECK(papacc_security_configuration_ref_set(&other, other_value,
        sizeof(other_value) - 1U) == PAPACC_RESULT_OK, 3);
    CHECK(papacc_security_configuration_ref_equal(&reference, &same) ==
        PAPACC_TRUE, 4);
    CHECK(papacc_security_configuration_ref_equal(&reference, &other) ==
        PAPACC_FALSE, 5);

    listeners[0] = (PAPACC_SERVER_LISTENER_CONFIGURATION)
        PAPACC_SERVER_LISTENER_CONFIGURATION_INITIALIZER;
    listeners[0].bind_selection.mode = PAPACC_BIND_SELECTION_ALL_INTERFACES;
    listeners[0].port = 4433U;
    listeners[0].transport_profile =
        PAPACC_LISTENER_TRANSPORT_PROFILE_SECURE_PRINCIPAL;
    listeners[0].security_configuration_ref = reference;
    CHECK(papacc_server_listener_configuration_validate(&listeners[0]) ==
        PAPACC_RESULT_OK, 6);

    listeners[1] = (PAPACC_SERVER_LISTENER_CONFIGURATION)
        PAPACC_SERVER_LISTENER_CONFIGURATION_INITIALIZER;
    listeners[1].bind_selection.mode = PAPACC_BIND_SELECTION_ALL_INTERFACES;
    listeners[1].port = 4434U;
    listeners[1].transport_profile =
        PAPACC_LISTENER_TRANSPORT_PROFILE_LEGACY_ENDPOINT;
    CHECK(papacc_server_listener_configuration_validate(&listeners[1]) ==
        PAPACC_RESULT_OK, 7);
    listeners[1].security_configuration_ref = reference;
    CHECK(papacc_server_listener_configuration_validate(&listeners[1]) ==
        PAPACC_RESULT_INVALID_STATE, 8);
    listeners[1].security_configuration_ref =
        (PAPACC_SECURITY_CONFIGURATION_REF)
            PAPACC_SECURITY_CONFIGURATION_REF_INITIALIZER;

    set.listeners = listeners;
    set.count = 2U;
    CHECK(papacc_server_listener_configuration_set_validate(&set) ==
        PAPACC_RESULT_OK, 9);
    listeners[1].port = listeners[0].port;
    CHECK(papacc_server_listener_configuration_set_validate(&set) ==
        PAPACC_RESULT_INVALID_STATE, 10);

    resolved.valid = PAPACC_TRUE;
    resolved.composition_inputs.local_certificate_chain = &item;
    resolved.composition_inputs.local_certificate_count = 1U;
    resolved.composition_inputs.local_private_key_pkcs8_der = &byte;
    resolved.composition_inputs.local_private_key_pkcs8_der_size = 1U;
    resolved.composition_inputs.peer_trust_anchors = &item;
    resolved.composition_inputs.peer_trust_anchor_count = 1U;
    resolved.principal_resolve = resolve_principal;
    resolved.authorize = authorize;
    resolved.release = release_snapshot;
    resolved.release_context = &releases;
    CHECK(papacc_resolved_server_security_configuration_validate(&resolved) ==
        PAPACC_RESULT_OK, 11);
    papacc_resolved_server_security_configuration_release(&resolved);
    CHECK(releases == 1U && resolved.valid == PAPACC_FALSE, 12);
    papacc_resolved_server_security_configuration_release(&resolved);
    CHECK(releases == 1U, 13);
    CHECK(papacc_server_security_configuration_resolve_and_compose(
        &reference, unresolved_configuration, NULL, &resolution, &resolved,
        &composition) == PAPACC_RESULT_INVALID_STATE, 14);
    CHECK(resolution == PAPACC_SECURITY_CONFIGURATION_NOT_FOUND &&
        resolved.valid == PAPACC_FALSE &&
        composition.state == PAPACC_SECURITY_COMPOSITION_UNINITIALIZED, 15);
    CHECK(papacc_server_security_configuration_resolve_and_compose(
        &reference, invalid_configuration, NULL, &resolution, &resolved,
        &composition) == PAPACC_RESULT_INVALID_STATE, 16);
    CHECK(resolution == PAPACC_SECURITY_CONFIGURATION_INVALID &&
        resolved.valid == PAPACC_FALSE &&
        composition.state == PAPACC_SECURITY_COMPOSITION_UNINITIALIZED, 17);
    CHECK(papacc_server_security_configuration_resolve_and_compose(
        &reference, resolver_error, NULL, &resolution, &resolved,
        &composition) == PAPACC_RESULT_INTERNAL_ERROR, 18);
    CHECK(resolved.valid == PAPACC_FALSE &&
        composition.state == PAPACC_SECURITY_COMPOSITION_UNINITIALIZED, 19);
    bad.valid = PAPACC_TRUE;
    bad.composition_inputs.local_certificate_chain = &item;
    bad.composition_inputs.local_certificate_count = 1U;
    bad.composition_inputs.local_private_key_pkcs8_der = &byte;
    bad.composition_inputs.local_private_key_pkcs8_der_size = 1U;
    bad.composition_inputs.peer_trust_anchors = &item;
    bad.composition_inputs.peer_trust_anchor_count = 1U;
    bad.principal_resolve = resolve_principal;
    bad.authorize = authorize;
    CHECK(papacc_server_security_configuration_resolve_and_compose(
        &reference, resolved_invalid_composition, &bad, &resolution,
        &resolved, &composition) != PAPACC_RESULT_OK, 20);
    CHECK(resolution == PAPACC_SECURITY_CONFIGURATION_RESOLVED &&
        resolved.valid == PAPACC_FALSE &&
        composition.state == PAPACC_SECURITY_COMPOSITION_UNINITIALIZED, 21);
    return 0;
}

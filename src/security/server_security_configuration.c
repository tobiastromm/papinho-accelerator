#include "server_security_configuration.h"

#include <string.h>

PAPACC_RESULT papacc_security_configuration_ref_set(
    PAPACC_SECURITY_CONFIGURATION_REF *reference,
    const PAPACC_U8 *value, PAPACC_SIZE length)
{
    if (reference == NULL || value == NULL || length == 0U ||
        length > PAPACC_SECURITY_CONFIGURATION_REF_CAPACITY)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    *reference = (PAPACC_SECURITY_CONFIGURATION_REF)
        PAPACC_SECURITY_CONFIGURATION_REF_INITIALIZER;
    memcpy(reference->value, value, length);
    reference->length = length;
    reference->valid = PAPACC_TRUE;
    return PAPACC_RESULT_OK;
}

PAPACC_BOOL papacc_security_configuration_ref_equal(
    const PAPACC_SECURITY_CONFIGURATION_REF *left,
    const PAPACC_SECURITY_CONFIGURATION_REF *right)
{
    if (left == NULL || right == NULL || left->valid != PAPACC_TRUE ||
        right->valid != PAPACC_TRUE || left->length == 0U ||
        left->length != right->length)
        return PAPACC_FALSE;
    return memcmp(left->value, right->value, left->length) == 0
        ? PAPACC_TRUE : PAPACC_FALSE;
}

PAPACC_RESULT papacc_resolved_server_security_configuration_validate(
    const PAPACC_RESOLVED_SERVER_SECURITY_CONFIGURATION *configuration)
{
    if (configuration == NULL) return PAPACC_RESULT_INVALID_ARGUMENT;
    if (configuration->valid != PAPACC_TRUE ||
        configuration->principal_resolve == NULL ||
        configuration->authorize == NULL ||
        configuration->composition_inputs.local_certificate_chain == NULL ||
        configuration->composition_inputs.local_certificate_count == 0U ||
        configuration->composition_inputs.local_private_key_pkcs8_der == NULL ||
        configuration->composition_inputs.local_private_key_pkcs8_der_size == 0U ||
        configuration->composition_inputs.peer_trust_anchors == NULL ||
        configuration->composition_inputs.peer_trust_anchor_count == 0U)
        return PAPACC_RESULT_INVALID_STATE;
    return PAPACC_RESULT_OK;
}

void papacc_resolved_server_security_configuration_release(
    PAPACC_RESOLVED_SERVER_SECURITY_CONFIGURATION *configuration)
{
    PAPACC_RESOLVED_SECURITY_RELEASE_FN release;
    void *context;
    if (configuration == NULL) return;
    release = configuration->release;
    context = configuration->release_context;
    *configuration = (PAPACC_RESOLVED_SERVER_SECURITY_CONFIGURATION)
        PAPACC_RESOLVED_SERVER_SECURITY_CONFIGURATION_INITIALIZER;
    if (release != NULL) release(context);
}

PAPACC_RESULT papacc_server_security_configuration_resolve_and_compose(
    const PAPACC_SECURITY_CONFIGURATION_REF *reference,
    PAPACC_SECURITY_CONFIGURATION_RESOLVE_FN resolve, void *resolve_context,
    PAPACC_SECURITY_CONFIGURATION_RESOLUTION *resolution,
    PAPACC_RESOLVED_SERVER_SECURITY_CONFIGURATION *configuration,
    PAPACC_SECURITY_COMPOSITION *composition)
{
    PAPACC_RESULT result;
    if (reference == NULL || resolve == NULL || resolution == NULL ||
        configuration == NULL || composition == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    *resolution = PAPACC_SECURITY_CONFIGURATION_RESOLVER_ERROR;
    if (reference->valid != PAPACC_TRUE || reference->length == 0U ||
        reference->length > PAPACC_SECURITY_CONFIGURATION_REF_CAPACITY)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (configuration->valid == PAPACC_TRUE ||
        composition->state != PAPACC_SECURITY_COMPOSITION_UNINITIALIZED)
        return PAPACC_RESULT_INVALID_STATE;
    result = resolve(resolve_context, reference, resolution, configuration);
    if (result != PAPACC_RESULT_OK) {
        papacc_resolved_server_security_configuration_release(configuration);
        return result;
    }
    if (*resolution != PAPACC_SECURITY_CONFIGURATION_RESOLVED ||
        papacc_resolved_server_security_configuration_validate(configuration) !=
            PAPACC_RESULT_OK) {
        papacc_resolved_server_security_configuration_release(configuration);
        return PAPACC_RESULT_INVALID_STATE;
    }
    result = papacc_security_composition_init(composition,
        &configuration->composition_inputs);
    if (result != PAPACC_RESULT_OK) {
        papacc_resolved_server_security_configuration_release(configuration);
        return result;
    }
    return PAPACC_RESULT_OK;
}

#include "server_listener_configuration.h"

static PAPACC_BOOL papacc_bind_selection_same(
    const PAPACC_PERSISTENT_BIND_SELECTION *left,
    const PAPACC_PERSISTENT_BIND_SELECTION *right)
{
    PAPACC_SIZE index;
    if (left->mode != right->mode ||
        left->interface_persistent_id_count !=
            right->interface_persistent_id_count)
        return PAPACC_FALSE;
    for (index = 0U; index < left->interface_persistent_id_count; ++index) {
        if (papacc_network_interface_persistent_id_equal(
                &left->interface_persistent_ids[index],
                &right->interface_persistent_ids[index]) != PAPACC_TRUE)
            return PAPACC_FALSE;
    }
    return PAPACC_TRUE;
}

PAPACC_RESULT papacc_server_listener_configuration_validate(
    const PAPACC_SERVER_LISTENER_CONFIGURATION *listener)
{
    PAPACC_RESULT result;
    if (listener == NULL) return PAPACC_RESULT_INVALID_ARGUMENT;
    result = papacc_persistent_bind_selection_validate(
        &listener->bind_selection);
    if (result != PAPACC_RESULT_OK) return result;
    if (listener->port == 0U) return PAPACC_RESULT_INVALID_STATE;
    if (listener->transport_profile ==
            PAPACC_LISTENER_TRANSPORT_PROFILE_SECURE_PRINCIPAL) {
        if (listener->security_configuration_ref.valid != PAPACC_TRUE ||
            listener->security_configuration_ref.length == 0U ||
            listener->security_configuration_ref.length >
                PAPACC_SECURITY_CONFIGURATION_REF_CAPACITY)
            return PAPACC_RESULT_INVALID_STATE;
        return PAPACC_RESULT_OK;
    }
    if (listener->transport_profile ==
            PAPACC_LISTENER_TRANSPORT_PROFILE_LEGACY_ENDPOINT) {
        if (listener->security_configuration_ref.valid != PAPACC_FALSE ||
            listener->security_configuration_ref.length != 0U)
            return PAPACC_RESULT_INVALID_STATE;
        return PAPACC_RESULT_OK;
    }
    return PAPACC_RESULT_INVALID_STATE;
}

PAPACC_RESULT papacc_server_listener_configuration_set_validate(
    const PAPACC_SERVER_LISTENER_CONFIGURATION_SET *configuration)
{
    PAPACC_SIZE left, right;
    PAPACC_RESULT result;
    if (configuration == NULL) return PAPACC_RESULT_INVALID_ARGUMENT;
    if (configuration->listeners == NULL || configuration->count == 0U)
        return PAPACC_RESULT_INVALID_STATE;
    for (left = 0U; left < configuration->count; ++left) {
        result = papacc_server_listener_configuration_validate(
            &configuration->listeners[left]);
        if (result != PAPACC_RESULT_OK) return result;
        for (right = 0U; right < left; ++right) {
            if (configuration->listeners[left].port ==
                    configuration->listeners[right].port &&
                papacc_bind_selection_same(
                    &configuration->listeners[left].bind_selection,
                    &configuration->listeners[right].bind_selection) ==
                        PAPACC_TRUE)
                return PAPACC_RESULT_INVALID_STATE;
        }
    }
    return PAPACC_RESULT_OK;
}

#include "server_config.h"

PAPACC_RESULT papacc_server_config_validate(
    const PAPACC_SERVER_CONFIG *config)
{
    PAPACC_RESULT result;

    if (config == NULL) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (config->allow_network_egress != PAPACC_FALSE &&
        config->allow_network_egress != PAPACC_TRUE) {
        return PAPACC_RESULT_INVALID_ARGUMENT;
    }
    if (config->listener_profile !=
            PAPACC_SERVER_LISTENER_PROFILE_LEGACY_ENDPOINT &&
        config->listener_profile !=
            PAPACC_SERVER_LISTENER_PROFILE_SECURE_PRINCIPAL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (config->listener_profile ==
            PAPACC_SERVER_LISTENER_PROFILE_SECURE_PRINCIPAL) {
        if (config->security_configuration_ref_valid != PAPACC_TRUE ||
            config->security_configuration_ref_length == 0U ||
            config->security_configuration_ref_length >
                PAPACC_SERVER_SECURITY_CONFIGURATION_REF_CAPACITY)
            return PAPACC_RESULT_INVALID_STATE;
    } else if (config->security_configuration_ref_valid != PAPACC_FALSE ||
        config->security_configuration_ref_length != 0U)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (config->allow_network_egress == PAPACC_TRUE) {
        if (config->listener_profile !=
                PAPACC_SERVER_LISTENER_PROFILE_SECURE_PRINCIPAL ||
            config->egress_policy_ref_valid != PAPACC_TRUE ||
            config->egress_profile_ref_valid != PAPACC_TRUE ||
            config->outbound_tls_profile_ref_valid != PAPACC_TRUE ||
            config->egress_policy_ref_length == 0U ||
            config->egress_profile_ref_length == 0U ||
            config->outbound_tls_profile_ref_length == 0U ||
            config->egress_policy_ref_length >
                PAPACC_SERVER_EGRESS_CONFIGURATION_REF_CAPACITY ||
            config->egress_profile_ref_length >
                PAPACC_SERVER_EGRESS_CONFIGURATION_REF_CAPACITY ||
            config->outbound_tls_profile_ref_length >
                PAPACC_SERVER_EGRESS_CONFIGURATION_REF_CAPACITY)
            return PAPACC_RESULT_INVALID_STATE;
    } else if (config->egress_policy_ref_valid != PAPACC_FALSE ||
        config->egress_profile_ref_valid != PAPACC_FALSE ||
        config->outbound_tls_profile_ref_valid != PAPACC_FALSE ||
        config->egress_policy_ref_length != 0U ||
        config->egress_profile_ref_length != 0U ||
        config->outbound_tls_profile_ref_length != 0U)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    result = papacc_persistent_bind_selection_validate(
        &config->bind_selection);
    if (result != PAPACC_RESULT_OK) {
        return result;
    }
    if (config->control_port == 0) {
        return PAPACC_RESULT_INVALID_STATE;
    }

    return PAPACC_RESULT_OK;
}

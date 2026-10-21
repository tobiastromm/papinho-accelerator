#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "server_egress_source_win32.h"
#include "server_outbound_tls_source_win32.h"

static void clear_environment(void)
{
    _putenv_s("PAPACC_EGRESS_POLICY_REF", "");
    _putenv_s("PAPACC_EGRESS_PROFILE_REF", "");
    _putenv_s("PAPACC_EGRESS_RULE_0_HOST", "");
    _putenv_s("PAPACC_EGRESS_RULE_0_IPV4", "");
    _putenv_s("PAPACC_EGRESS_RULE_0_PORT", "");
}

static void configure(PAPACC_SERVER_CONFIG *config)
{
    *config = (PAPACC_SERVER_CONFIG)PAPACC_SERVER_CONFIG_INITIALIZER;
    config->control_port = 4433U;
    config->bind_selection.mode = PAPACC_BIND_SELECTION_ALL_INTERFACES;
    config->listener_profile = PAPACC_SERVER_LISTENER_PROFILE_SECURE_PRINCIPAL;
    config->security_configuration_ref_valid = PAPACC_TRUE;
    config->security_configuration_ref_length = 3U;
    memcpy(config->security_configuration_ref, "lab", 3U);
    config->allow_network_egress = PAPACC_TRUE;
    config->egress_policy_ref_valid = PAPACC_TRUE;
    config->egress_policy_ref_length = 10U;
    memcpy(config->egress_policy_ref, "lab-policy", 10U);
    config->egress_profile_ref_valid = PAPACC_TRUE;
    config->egress_profile_ref_length = 13U;
    memcpy(config->egress_profile_ref, "default-route", 13U);
    config->outbound_tls_profile_ref_valid = PAPACC_TRUE;
    config->outbound_tls_profile_ref_length = 12U;
    memcpy(config->outbound_tls_profile_ref, "system-tls13", 12U);
}

int main(int argc,char **argv)
{
    PAPACC_SERVER_CONFIG config;
    PAPACC_SERVER_EGRESS_SOURCE_WIN32 source =
        PAPACC_SERVER_EGRESS_SOURCE_WIN32_INITIALIZER;
    PAPACC_NETWORK_ENDPOINT candidates[2];
    PAPACC_NETWORK_ENDPOINT denied = PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    PAPACC_SIZE count = 0U;
    PAPACC_RESOLVED_OUTBOUND_TLS_PROFILE tls_profile =
        PAPACC_RESOLVED_OUTBOUND_TLS_PROFILE_INITIALIZER;
    char ca_path[1024];

    if(argc!=2)return 10;

    configure(&config);
    clear_environment();
    if (papacc_server_egress_environment_resolve(&config, &source) !=
            PAPACC_RESULT_INVALID_STATE)
        return 1;
    _putenv_s("PAPACC_EGRESS_POLICY_REF", "wrong-policy");
    _putenv_s("PAPACC_EGRESS_PROFILE_REF", "default-route");
    if (papacc_server_egress_environment_resolve(&config, &source) !=
            PAPACC_RESULT_INVALID_STATE)
        return 2;
    _putenv_s("PAPACC_EGRESS_POLICY_REF", "lab-policy");
    _putenv_s("PAPACC_EGRESS_PROFILE_REF", "unknown-profile");
    config.egress_profile_ref_length = 15U;
    memcpy(config.egress_profile_ref, "unknown-profile", 15U);
    if (papacc_server_egress_environment_resolve(&config, &source) !=
            PAPACC_RESULT_INVALID_STATE)
        return 3;
    config.egress_profile_ref_length = 13U;
    memcpy(config.egress_profile_ref, "default-route", 13U);
    _putenv_s("PAPACC_EGRESS_PROFILE_REF", "default-route");
    _putenv_s("PAPACC_EGRESS_RULE_0_HOST", "lab.example");
    _putenv_s("PAPACC_EGRESS_RULE_0_IPV4", "127.0.0.1");
    _putenv_s("PAPACC_EGRESS_RULE_0_PORT", "8443");
    if (papacc_server_egress_environment_resolve(&config, &source) !=
            PAPACC_RESULT_OK)
        return 4;
    if (source.policy.host_policy(source.policy.context, "lab.example", 11U,
            8443U) != PAPACC_EGRESS_ALLOW ||
        source.policy.host_policy(source.policy.context, "lab.example", 11U,
            443U) != PAPACC_EGRESS_DENY)
        return 5;
    if (source.resolver.resolve(source.resolver.context, "lab.example", 11U,
            8443U, candidates, 2U, &count) != PAPACC_RESULT_OK || count != 1U)
        return 6;
    denied = candidates[0];
    denied.address.bytes[3] = 2U;
    if (source.policy.endpoint_policy(source.policy.context, &candidates[0],
            PAPACC_EGRESS_SENSITIVE_LOOPBACK) != PAPACC_EGRESS_ALLOW ||
        source.policy.endpoint_policy(source.policy.context, &denied,
            PAPACC_EGRESS_SENSITIVE_LOOPBACK) != PAPACC_EGRESS_DENY)
        return 7;
    _putenv_s("PAPACC_OUTBOUND_TLS_PROFILE_REF", "wrong");
    if(papacc_server_outbound_tls_environment_resolve(&config,&tls_profile)!=
            PAPACC_RESULT_INVALID_STATE)return 8;
    _putenv_s("PAPACC_OUTBOUND_TLS_PROFILE_REF", "system-tls13");
    if(papacc_server_outbound_tls_environment_resolve(&config,&tls_profile)!=
            PAPACC_RESULT_OK||tls_profile.trust_kind!=
            PAPACC_OUTBOUND_TLS_TRUST_SYSTEM)return 9;
    papacc_resolved_outbound_tls_profile_release(&tls_profile);
    memcpy(config.outbound_tls_profile_ref,"lab-private-ca",14U);
    config.outbound_tls_profile_ref_length=14U;
    _putenv_s("PAPACC_OUTBOUND_TLS_PROFILE_REF", "lab-private-ca");
    if(sprintf_s(ca_path,sizeof(ca_path),"%s\\ca.der.b64",argv[1])<0||
        _putenv_s("PAPACC_OUTBOUND_TLS_CA_DER_B64",ca_path)!=0||
        papacc_server_outbound_tls_environment_resolve(&config,&tls_profile)!=
            PAPACC_RESULT_OK||tls_profile.trust_kind!=
            PAPACC_OUTBOUND_TLS_TRUST_CUSTOM_CA_DER||
        tls_profile.custom_ca_der_size==0U)return 11;
    papacc_resolved_outbound_tls_profile_release(&tls_profile);
    _putenv_s("PAPACC_OUTBOUND_TLS_PROFILE_REF", "");
    _putenv_s("PAPACC_OUTBOUND_TLS_CA_DER_B64", "");
    clear_environment();
    return 0;
}

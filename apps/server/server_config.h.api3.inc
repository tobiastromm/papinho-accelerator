#ifndef PAPACC_SERVER_CONFIG_H
#define PAPACC_SERVER_CONFIG_H

#include "persistent_bind_selection.h"
#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
#include "server_policy_model.h"
#include "server_site_tls_profile.h"
#define PAPACC_SERVER_POLICY_INITIALIZER , PAPACC_ADMIN_POLICY_INITIALIZER, PAPACC_SITE_TLS_CATALOG_INITIALIZER
#else
#define PAPACC_SERVER_POLICY_INITIALIZER
#endif

#define PAPACC_SERVER_SECURITY_CONFIGURATION_REF_CAPACITY 64U
#define PAPACC_SERVER_EGRESS_CONFIGURATION_REF_CAPACITY 64U

typedef enum PAPACC_SERVER_LISTENER_PROFILE {
    PAPACC_SERVER_LISTENER_PROFILE_LEGACY_ENDPOINT = 0,
    PAPACC_SERVER_LISTENER_PROFILE_SECURE_PRINCIPAL = 1
} PAPACC_SERVER_LISTENER_PROFILE;

typedef struct PAPACC_SERVER_CONFIG {
    PAPACC_U16 control_port;
    PAPACC_BOOL allow_network_egress;
    PAPACC_PERSISTENT_BIND_SELECTION bind_selection;
    PAPACC_SERVER_LISTENER_PROFILE listener_profile;
    PAPACC_BOOL security_configuration_ref_valid;
    PAPACC_SIZE security_configuration_ref_length;
    PAPACC_U8 security_configuration_ref[
        PAPACC_SERVER_SECURITY_CONFIGURATION_REF_CAPACITY];
    PAPACC_BOOL egress_policy_ref_valid;
    PAPACC_SIZE egress_policy_ref_length;
    PAPACC_U8 egress_policy_ref[PAPACC_SERVER_EGRESS_CONFIGURATION_REF_CAPACITY];
    PAPACC_BOOL egress_profile_ref_valid;
    PAPACC_SIZE egress_profile_ref_length;
    PAPACC_U8 egress_profile_ref[PAPACC_SERVER_EGRESS_CONFIGURATION_REF_CAPACITY];
    PAPACC_BOOL outbound_tls_profile_ref_valid;
    PAPACC_SIZE outbound_tls_profile_ref_length;
    PAPACC_U8 outbound_tls_profile_ref[
        PAPACC_SERVER_EGRESS_CONFIGURATION_REF_CAPACITY];
#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
    PAPACC_ADMIN_POLICY policy;
    PAPACC_SITE_TLS_CATALOG site_tls;
#endif
} PAPACC_SERVER_CONFIG;

/*
 * Port zero means unspecified, not an official or usable control port.
 * Network egress is denied and bind intent is UNSPECIFIED by default. These
 * initialization defaults are safe and deterministic, but intentionally not
 * operationally valid until the application supplies both decisions.
 *
 * The model is configuration-source agnostic: CLI, GUI, files, Registry,
 * firmware, tests, or programmatic APIs may construct the same structure.
 * It owns no heap and is copyable by value. For SELECTED_INTERFACES, copying
 * the config copies only the persistent-ID view; the source/caller owns that
 * array and keeps it alive while the config is consulted. The config never
 * modifies, reallocates, or frees it.
 */
#define PAPACC_SERVER_CONFIG_INITIALIZER \
    { 0, PAPACC_FALSE, PAPACC_PERSISTENT_BIND_SELECTION_INITIALIZER, \
      PAPACC_SERVER_LISTENER_PROFILE_LEGACY_ENDPOINT, PAPACC_FALSE, 0U, { 0 }, \
      PAPACC_FALSE, 0U, { 0 }, PAPACC_FALSE, 0U, { 0 }, \
      PAPACC_FALSE, 0U, { 0 } PAPACC_SERVER_POLICY_INITIALIZER }

/*
 * Validates the in-memory model independently of any future configuration
 * source and without consulting discovery state. Validation order is boolean
 * representation, persistent bind selection, then operational port. Result
 * categories from persistent selection validation are preserved.
 */
PAPACC_RESULT papacc_server_config_validate(
    const PAPACC_SERVER_CONFIG *config);

#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
PAPACC_RESULT papacc_server_policy_validate(const PAPACC_SERVER_CONFIG *);
/* Pure rule decision, NOT authentication, grant or current-policy authority.
 * Caller must still enforce all accepted security/current/epoch/resource gates. */
PAPACC_RESULT papacc_server_policy_evaluate(const PAPACC_SERVER_CONFIG *,
 const PAPACC_ADMIN_POLICY_CONTEXT *,const PAPACC_CAPABILITY_KEY *,
 PAPACC_U32 *,PAPACC_SIZE *);
#endif

#endif

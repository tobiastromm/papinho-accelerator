#ifndef PAPACC_SERVER_SECURITY_SOURCE_WIN32_H
#define PAPACC_SERVER_SECURITY_SOURCE_WIN32_H

#include "server_security_configuration.h"

/*
 * Lab-only application Configuration Source.  The reference is matched
 * against PAPACC_SECURITY_CONFIGURATION_REF and the credential locations are
 * read from PAPACC_SERVER_CERTIFICATE_DER_B64,
 * PAPACC_SERVER_PRIVATE_KEY_PKCS8_DER_B64 and PAPACC_CLIENT_CA_DER_B64.
 * PAPACC_ALLOWED_CLIENT_CERT_SHA256 contains the single enrolled client
 * certificate fingerprint as 64 hexadecimal characters.
 * Paths and secret material never become part of PAPACC_SERVER_CONFIG.
 */
PAPACC_RESULT papacc_server_security_environment_resolve(
    void *context, const PAPACC_SECURITY_CONFIGURATION_REF *reference,
    PAPACC_SECURITY_CONFIGURATION_RESOLUTION *resolution,
    PAPACC_RESOLVED_SERVER_SECURITY_CONFIGURATION *configuration);

/* Private source-lease states, NOT configuration commit, provider readiness
 * or authentication. Only zeroed durable storage may enter EMPTY. */
typedef enum PAPACC_SECURITY_SOURCE_STATE {
    PAPACC_SECURITY_SOURCE_EMPTY = 0,
    PAPACC_SECURITY_SOURCE_PREPARING,
    PAPACC_SECURITY_SOURCE_PREPARED,
    PAPACC_SECURITY_SOURCE_COMMITTED,
    PAPACC_SECURITY_SOURCE_PUBLISHED,
    PAPACC_SECURITY_SOURCE_REJECTED,
    PAPACC_SECURITY_SOURCE_RETAINED,
    PAPACC_SECURITY_SOURCE_CLOSED
} PAPACC_SECURITY_SOURCE_STATE;
typedef struct PAPACC_SECURITY_SOURCE_TRANSACTION {
    PAPACC_RESOLVED_SERVER_SECURITY_CONFIGURATION configuration;
    PAPACC_SECURITY_CONFIGURATION_REF reference;
    PAPACC_LOGGER logger;
    PAPACC_SIZE other_reserved, credit, used, peak;
    PAPACC_U32 owner_thread;
    PAPACC_SECURITY_SOURCE_STATE state;
    PAPACC_RESULT failure;
    PAPACC_U64 next_ticket, tickets[8];
    PAPACC_SIZE borrowers;
    void *held_storage, *retained_staging;
    PAPACC_SIZE retained_staging_capacity;
    PAPACC_BOOL close_uncertain;
#ifdef PAPACC_SECURITY_SOURCE_OFFLINE_TEST
    PAPACC_SIZE fail_allocation_at, allocation_attempts;
#endif
} PAPACC_SECURITY_SOURCE_TRANSACTION;
/* other_reserved prices all other simultaneous owners, stacks and overhead.
 * Add sizeof(transaction) and its material credit before effects. Opaque
 * provider/kernel memory stays UNKNOWN. A borrowed view is read-only. */
PAPACC_RESULT papacc_server_security_source_prepare(
    PAPACC_SECURITY_SOURCE_TRANSACTION *,const PAPACC_SECURITY_CONFIGURATION_REF *,
    const PAPACC_LOGGER *,PAPACC_SIZE,PAPACC_SIZE);
PAPACC_RESULT papacc_server_security_source_validate(const PAPACC_SECURITY_SOURCE_TRANSACTION *);
PAPACC_RESULT papacc_server_security_source_commit(PAPACC_SECURITY_SOURCE_TRANSACTION *);
PAPACC_RESULT papacc_server_security_source_publish(PAPACC_SECURITY_SOURCE_TRANSACTION *);
PAPACC_RESULT papacc_server_security_source_borrow(PAPACC_SECURITY_SOURCE_TRANSACTION *,
    PAPACC_U64 *,const PAPACC_RESOLVED_SERVER_SECURITY_CONFIGURATION **);
PAPACC_RESULT papacc_server_security_source_unborrow(PAPACC_SECURITY_SOURCE_TRANSACTION *,PAPACC_U64);
PAPACC_RESULT papacc_server_security_source_rollback(PAPACC_SECURITY_SOURCE_TRANSACTION *);
PAPACC_RESULT papacc_server_security_source_finish(PAPACC_SECURITY_SOURCE_TRANSACTION *,PAPACC_BOOL);
PAPACC_RESULT papacc_server_security_source_retain(PAPACC_SECURITY_SOURCE_TRANSACTION *);
#endif

#ifndef PAPACC_SERVER_CONFIGURATION_COMMIT_WIN32_H
#define PAPACC_SERVER_CONFIGURATION_COMMIT_WIN32_H
/* Private modern-Windows diagnostic component; not a configuration service.
 * Zeroed durable owner storage required. No network, PST or secret storage. */
#include <windows.h>
#include "server_config.h"
#define PAPACC_CONFIG_RECORD_BYTES 512U
#define PAPACC_CONFIG_STORE_BYTES 8192U
#define PAPACC_CONFIG_SLOTS 3U
#define PAPACC_CONFIG_IDS 8U
typedef struct PAPACC_CONFIG_VIEW {
    PAPACC_SERVER_CONFIG config;
    PAPACC_NETWORK_INTERFACE_PERSISTENT_ID ids[PAPACC_CONFIG_IDS];
    unsigned char domain[32];
    PAPACC_U64 revision, predecessor, policy_epoch;
} PAPACC_CONFIG_VIEW;
typedef struct PAPACC_CONFIG_SLOT {
    PAPACC_CONFIG_VIEW view;
    unsigned long readers;
    PAPACC_U64 serial;
    int retained;
} PAPACC_CONFIG_SLOT;
typedef struct PAPACC_CONFIG_PIN {
    const void *issuer;
    unsigned index;
    PAPACC_U64 serial, ticket;
} PAPACC_CONFIG_PIN;
typedef struct PAPACC_CONFIGURATION_COMMIT {
    CRITICAL_SECTION lock;
    PAPACC_CONFIG_SLOT slots[PAPACC_CONFIG_SLOTS];
    PAPACC_CONFIG_PIN tickets[32];
    PAPACC_U64 next_ticket;
    unsigned char committed[PAPACC_CONFIG_STORE_BYTES];
    char path[MAX_PATH];
    unsigned char domain[32];
    HANDLE retained_file, retained_transaction;
    DWORD writer;
    unsigned current, pending;
    PAPACC_U64 next_serial;
    int initialized, prepared, committed_pending, published, stopped, unresolved;
    int store02, recovery_selected;
#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
    int recovery_guard; /* Exclusive native read lease, not Model E destruction. */
#endif
} PAPACC_CONFIGURATION_COMMIT;
/* init only creates an in-memory diagnostic domain; CREATE_NEW at first commit
 * prevents replacing any existing installation. No recovery authorization. */
PAPACC_RESULT papacc_configuration_commit_init(PAPACC_CONFIGURATION_COMMIT *,const char *);
PAPACC_RESULT papacc_configuration_prepare(PAPACC_CONFIGURATION_COMMIT *,
    const PAPACC_SERVER_CONFIG *,PAPACC_U64,PAPACC_U64);
PAPACC_RESULT papacc_configuration_commit(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_publish(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_abort(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_pin(PAPACC_CONFIGURATION_COMMIT *,PAPACC_CONFIG_PIN *,PAPACC_CONFIG_VIEW *);
PAPACC_RESULT papacc_configuration_unpin(PAPACC_CONFIGURATION_COMMIT *,const PAPACC_CONFIG_PIN *,int);
PAPACC_BOOL papacc_configuration_current(PAPACC_CONFIGURATION_COMMIT *,const PAPACC_CONFIG_VIEW *);
PAPACC_RESULT papacc_configuration_stop(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_finish(PAPACC_CONFIGURATION_COMMIT *);
/* Integrity inspection cannot authorize recovery. Output never contains a
 * usable view. Even a valid record returns NOT_SUPPORTED without continuity. */
PAPACC_RESULT papacc_configuration_recovery_inspect(PAPACC_CONFIGURATION_COMMIT *,const char *);
/* Opt-in protocol0.2 full server-model profile; no operational activation. */
PAPACC_RESULT papacc_configuration_commit_init02(PAPACC_CONFIGURATION_COMMIT *,const char *);
PAPACC_RESULT papacc_configuration_recovery_init02(PAPACC_CONFIGURATION_COMMIT *,const char *);
/* Query-only candidate, NOT a published/current configuration or new grant. */
PAPACC_RESULT papacc_configuration_recovered_copy(PAPACC_CONFIGURATION_COMMIT *,PAPACC_CONFIG_VIEW *);
#ifdef PAPACC_C8_PRIVATE_STORAGE_PROFILE
/* BODY0.3 is a COMPONENT store, not the application's normative commit.
 * No caller epoch accepted; zero records unavailable epoch, never a grant. */
PAPACC_RESULT papacc_configuration_commit_init03(PAPACC_CONFIGURATION_COMMIT *,const char *);
PAPACC_RESULT papacc_configuration_recovery_init03(PAPACC_CONFIGURATION_COMMIT *,const char *);
PAPACC_RESULT papacc_configuration_prepare03(PAPACC_CONFIGURATION_COMMIT *,const PAPACC_SERVER_CONFIG *,PAPACC_U64);
/* Private isolated preserved-local-custody evidence. Integrity/file ID alone
 * do not establish freshness outside ADR0046's explicitly supervised scope. */
typedef struct PAPACC_NORMAL_RESTART_WITNESS {
 unsigned char domain[32], image_sha256[32];
 PAPACC_U64 revision, predecessor, volume, file_index;
} PAPACC_NORMAL_RESTART_WITNESS;
PAPACC_RESULT papacc_configuration_recovery_guard03(PAPACC_CONFIGURATION_COMMIT *,const PAPACC_NORMAL_RESTART_WITNESS *);
PAPACC_RESULT papacc_configuration_recovery_validate03(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_recovery_publish03(PAPACC_CONFIGURATION_COMMIT *);
/* Explicit BODY0.5 catalog profile; no old-format promotion. */
PAPACC_RESULT papacc_configuration_commit_init05(PAPACC_CONFIGURATION_COMMIT *,const char *);
PAPACC_RESULT papacc_configuration_prepare05(PAPACC_CONFIGURATION_COMMIT *,const PAPACC_SERVER_CONFIG *,PAPACC_U64);
/* Explicit typed0.4 profile;0.2/0.3 entry points remain strict. */
PAPACC_RESULT papacc_configuration_commit_init04(PAPACC_CONFIGURATION_COMMIT *,const char *);
PAPACC_RESULT papacc_configuration_recovery_init04(PAPACC_CONFIGURATION_COMMIT *,const char *);
PAPACC_RESULT papacc_configuration_prepare04(PAPACC_CONFIGURATION_COMMIT *,const PAPACC_SERVER_CONFIG *,PAPACC_U64);
PAPACC_RESULT papacc_configuration_recovery_guard04(PAPACC_CONFIGURATION_COMMIT *,const PAPACC_NORMAL_RESTART_WITNESS *);
PAPACC_RESULT papacc_configuration_recovery_validate04(PAPACC_CONFIGURATION_COMMIT *);
PAPACC_RESULT papacc_configuration_recovery_publish04(PAPACC_CONFIGURATION_COMMIT *);
#endif
#endif

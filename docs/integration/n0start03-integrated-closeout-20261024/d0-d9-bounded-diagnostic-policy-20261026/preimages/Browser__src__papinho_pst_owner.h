#ifndef PAPINHO_PST_OWNER_H
#define PAPINHO_PST_OWNER_H
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <winsock.h>
#include <papinho_secure_transport.h>

/* Browser-private owner; not a cross-consumer helper or public ABI. */
typedef struct papinho_pst_owner papinho_pst_owner;
typedef struct papinho_pst_owner_registry {
    CRITICAL_SECTION lock;
    papinho_pst_owner *head;
    int admission_closed;
} papinho_pst_owner_registry;

typedef struct papinho_pst_owner_dependencies {
    pst_runtime *runtime;
    pst_credentials *credentials;
    pst_trust *trust;
    void *credential_lease;
    /* Context must be durable, not an initiating stack address. The callback
     * finalizes this owner's dependency lease, not other owners' resources. */
    void *context;
    /* Must only drop the owner's group lease, as in D. A fallible resource
     * destructor cannot be substituted for this void callback. */
    void (*finalize)(void *);
} papinho_pst_owner_dependencies;

typedef struct papinho_pst_owner_api {
    PST_RESULT (PST_CALL *release)(pst_connection *, PST_RELEASE_INFO *);
    PST_RESULT (PST_CALL *remove)(pst_wait_set *, pst_connection *);
} papinho_pst_owner_api;

struct papinho_pst_owner {
    papinho_pst_owner *next;
    papinho_pst_owner_registry *registry;
    pst_connection *connection;
    pst_wait_set *wait_set;
    papinho_pst_owner_dependencies dependencies;
    papinho_pst_owner_api api;
    DWORD thread_id;
    unsigned long identity, generation, role;
    int closing, finalized, terminal_retained, winsock_owned, release_eligible;
    unsigned long release_attempts, finalizations;
    PST_RELEASE_INFO release_info;
    PST_RESULT preclose_result;
    int quiescence_confirmed, winsock_cleanup_attempted, winsock_cleanup_result, winsock_cleanup_error;
    int terminal_cause, network_close_error; /* Browser facts, not PST disposition */
};

void papinho_pst_owner_registry_init(papinho_pst_owner_registry *);
int papinho_pst_owner_registry_dispose(papinho_pst_owner_registry *);
/* Success transfers the connection and one dependency lease. Failure transfers
 * nothing. NULL api selects the byte-exact normal SDK; non-NULL is harness-only. */
papinho_pst_owner *papinho_pst_owner_create(papinho_pst_owner_registry *,
    pst_connection *, unsigned long, unsigned long, unsigned long,
    const papinho_pst_owner_dependencies *, const papinho_pst_owner_api *);
int papinho_pst_owner_callback_allowed(papinho_pst_owner *, unsigned long, unsigned long);
/* Caller proves callback/worker quiescence on the owner thread before close.
 * No scheduler or join proof is supplied by this isolated preparation. */
int papinho_pst_owner_close(papinho_pst_owner *, unsigned long, unsigned long);
int papinho_pst_owner_remove_membership(papinho_pst_owner *);
/* Called only after all callback sources referencing this owner have quiesced.
 * Registry identity allocation must not reuse this owner's token. */
int papinho_pst_owner_reclaim(papinho_pst_owner *);
#endif

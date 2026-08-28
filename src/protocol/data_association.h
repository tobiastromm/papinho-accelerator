#ifndef PAPACC_DATA_ASSOCIATION_H
#define PAPACC_DATA_ASSOCIATION_H

#include "channel.h"
#include "data_association_protocol.h"

#define PAPACC_DATA_ASSOCIATION_GENERATION_ATTEMPTS 32U

typedef PAPACC_RESULT (*PAPACC_DATA_TICKET_GENERATE_FN)(
    void *context, PAPACC_DATA_ASSOCIATION_TICKET *out_ticket);

typedef struct PAPACC_DATA_ASSOCIATION_ENTRY {
    PAPACC_BOOL in_use;
    PAPACC_U64 session_instance_id;
    PAPACC_DATA_ASSOCIATION_TICKET ticket;
    PAPACC_U64 deadline_ns;
} PAPACC_DATA_ASSOCIATION_ENTRY;

#define PAPACC_DATA_ASSOCIATION_ENTRY_INITIALIZER \
    { PAPACC_FALSE, 0, PAPACC_DATA_ASSOCIATION_TICKET_INITIALIZER, 0 }

typedef struct PAPACC_DATA_ASSOCIATION_MANAGER {
    PAPACC_DATA_ASSOCIATION_ENTRY *storage;
    PAPACC_SIZE capacity;
    PAPACC_SIZE count;
    PAPACC_SESSION_MANAGER *session_manager;
    PAPACC_CHANNEL_MANAGER *channel_manager;
    PAPACC_DATA_TICKET_GENERATE_FN generate_fn;
    void *generate_context;
    PAPACC_U64 ticket_lifetime_ns;
    PAPACC_BOOL initialized;
} PAPACC_DATA_ASSOCIATION_MANAGER;

#define PAPACC_DATA_ASSOCIATION_MANAGER_INITIALIZER \
    { NULL, 0, 0, NULL, NULL, NULL, NULL, 0, PAPACC_FALSE }

PAPACC_RESULT papacc_data_association_manager_init(
    PAPACC_DATA_ASSOCIATION_MANAGER *manager,
    PAPACC_DATA_ASSOCIATION_ENTRY *storage, PAPACC_SIZE capacity,
    PAPACC_SESSION_MANAGER *session_manager,
    PAPACC_CHANNEL_MANAGER *channel_manager,
    PAPACC_DATA_TICKET_GENERATE_FN generate_fn, void *generate_context,
    PAPACC_U64 ticket_lifetime_ns);

PAPACC_RESULT papacc_data_association_manager_issue(
    PAPACC_DATA_ASSOCIATION_MANAGER *manager, PAPACC_U64 session_instance_id,
    PAPACC_U64 now_ns, PAPACC_DATA_ASSOCIATION_TICKET *out_ticket,
    PAPACC_U64 *out_deadline_ns);
PAPACC_RESULT papacc_data_association_manager_consume(
    PAPACC_DATA_ASSOCIATION_MANAGER *manager,
    const PAPACC_DATA_ASSOCIATION_TICKET *ticket, PAPACC_U64 now_ns,
    PAPACC_U64 *out_session_instance_id);
PAPACC_RESULT papacc_data_association_manager_invalidate_session(
    PAPACC_DATA_ASSOCIATION_MANAGER *manager, PAPACC_U64 session_instance_id);
PAPACC_RESULT papacc_data_association_manager_expire(
    PAPACC_DATA_ASSOCIATION_MANAGER *manager, PAPACC_U64 now_ns,
    PAPACC_SIZE *out_expired_count);
void papacc_data_association_manager_shutdown(
    PAPACC_DATA_ASSOCIATION_MANAGER *manager);

#endif

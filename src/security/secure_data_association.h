#ifndef PAPACC_SECURE_DATA_ASSOCIATION_H
#define PAPACC_SECURE_DATA_ASSOCIATION_H

#include "authentication_authorization.h"
#include "data_association.h"

typedef PAPACC_RESULT (*PAPACC_SESSION_SECURITY_LOOKUP_FN)(
    void *context, PAPACC_U64 session_instance_id,
    const PAPACC_SESSION_SECURITY_CONTEXT **out_security_context);

typedef enum PAPACC_SECURE_DATA_OUTCOME {
    PAPACC_SECURE_DATA_OUTCOME_UNSPECIFIED = 0,
    PAPACC_SECURE_DATA_OUTCOME_ACCEPTED,
    PAPACC_SECURE_DATA_OUTCOME_STRUCTURAL_TICKET_INVALID,
    PAPACC_SECURE_DATA_OUTCOME_DATA_CONTEXT_INVALID,
    PAPACC_SECURE_DATA_OUTCOME_SESSION_CONTEXT_INVALID,
    PAPACC_SECURE_DATA_OUTCOME_PRINCIPAL_MISMATCH,
    PAPACC_SECURE_DATA_OUTCOME_AUTHORIZATION_DENIED,
    PAPACC_SECURE_DATA_OUTCOME_AUTHORIZATION_ERROR,
    PAPACC_SECURE_DATA_OUTCOME_COMMIT_CONFLICT,
    PAPACC_SECURE_DATA_OUTCOME_CHANNEL_BIND_FAILED
} PAPACC_SECURE_DATA_OUTCOME;

PAPACC_RESULT papacc_secure_data_ticket_issue(
    PAPACC_DATA_ASSOCIATION_MANAGER *association_manager,
    PAPACC_U64 session_instance_id,
    const PAPACC_SESSION_SECURITY_CONTEXT *session_security_context,
    PAPACC_AUTHORIZATION_FN authorize, void *authorization_context,
    PAPACC_U64 now_ns, PAPACC_DATA_ASSOCIATION_TICKET *out_ticket,
    PAPACC_U64 *out_deadline_ns, PAPACC_SECURE_DATA_OUTCOME *out_outcome);

PAPACC_RESULT papacc_secure_data_attach(
    PAPACC_DATA_ASSOCIATION_MANAGER *association_manager,
    PAPACC_CHANNEL_MANAGER *channel_manager,
    const PAPACC_DATA_ASSOCIATION_TICKET *ticket, PAPACC_U64 now_ns,
    PAPACC_U64 data_connection_instance_id,
    const PAPACC_CONNECTION_SECURITY_CONTEXT *data_security_context,
    PAPACC_SESSION_SECURITY_LOOKUP_FN lookup_session_security,
    void *session_security_context,
    PAPACC_AUTHORIZATION_FN authorize, void *authorization_context,
    PAPACC_U64 *out_session_instance_id,
    PAPACC_U64 *out_channel_instance_id,
    PAPACC_SECURE_DATA_OUTCOME *out_outcome);

#endif

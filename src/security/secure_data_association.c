#include "secure_data_association.h"

static PAPACC_BOOL papacc_session_security_valid(
    const PAPACC_SESSION_SECURITY_CONTEXT *context)
{
    return context != NULL && context->published == PAPACC_TRUE &&
        context->principal.valid == PAPACC_TRUE ? PAPACC_TRUE : PAPACC_FALSE;
}

static PAPACC_BOOL papacc_connection_security_valid(
    const PAPACC_CONNECTION_SECURITY_CONTEXT *context)
{
    return context != NULL &&
        context->state == PAPACC_SECURITY_CONTEXT_AUTHORIZED &&
        context->principal.valid == PAPACC_TRUE ? PAPACC_TRUE : PAPACC_FALSE;
}

static PAPACC_RESULT papacc_secure_data_authorize(
    const PAPACC_PRINCIPAL *principal, PAPACC_AUTHORIZATION_FN authorize,
    void *authorization_context, PAPACC_SECURE_DATA_OUTCOME *out_outcome)
{
    PAPACC_AUTHORIZATION_DECISION decision = PAPACC_AUTHORIZATION_DENY;
    PAPACC_RESULT result;
    if (authorize == NULL) {
        *out_outcome = PAPACC_SECURE_DATA_OUTCOME_AUTHORIZATION_ERROR;
        return PAPACC_RESULT_OK;
    }
    result = authorize(authorization_context, principal,
        PAPACC_AUTHORIZATION_ASSOCIATE_DATA, &decision);
    if (result != PAPACC_RESULT_OK || decision == PAPACC_AUTHORIZATION_ERROR) {
        *out_outcome = PAPACC_SECURE_DATA_OUTCOME_AUTHORIZATION_ERROR;
        return PAPACC_RESULT_OK;
    }
    if (decision != PAPACC_AUTHORIZATION_ALLOW) {
        *out_outcome = PAPACC_SECURE_DATA_OUTCOME_AUTHORIZATION_DENIED;
        return PAPACC_RESULT_OK;
    }
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_secure_data_ticket_issue(
    PAPACC_DATA_ASSOCIATION_MANAGER *association_manager,
    PAPACC_U64 session_instance_id,
    const PAPACC_SESSION_SECURITY_CONTEXT *session_security_context,
    PAPACC_AUTHORIZATION_FN authorize, void *authorization_context,
    PAPACC_U64 now_ns, PAPACC_DATA_ASSOCIATION_TICKET *out_ticket,
    PAPACC_U64 *out_deadline_ns, PAPACC_SECURE_DATA_OUTCOME *out_outcome)
{
    PAPACC_RESULT result;
    if (out_ticket != NULL) *out_ticket = (PAPACC_DATA_ASSOCIATION_TICKET)
        PAPACC_DATA_ASSOCIATION_TICKET_INITIALIZER;
    if (out_deadline_ns != NULL) *out_deadline_ns = 0;
    if (out_outcome != NULL)
        *out_outcome = PAPACC_SECURE_DATA_OUTCOME_UNSPECIFIED;
    if (association_manager == NULL || session_instance_id == 0 ||
        out_ticket == NULL || out_deadline_ns == NULL || out_outcome == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (papacc_session_security_valid(session_security_context) != PAPACC_TRUE) {
        *out_outcome = PAPACC_SECURE_DATA_OUTCOME_SESSION_CONTEXT_INVALID;
        return PAPACC_RESULT_OK;
    }
    result = papacc_secure_data_authorize(&session_security_context->principal,
        authorize, authorization_context, out_outcome);
    if (result != PAPACC_RESULT_OK ||
        *out_outcome != PAPACC_SECURE_DATA_OUTCOME_UNSPECIFIED) return result;
    result = papacc_data_association_manager_issue(association_manager,
        session_instance_id, now_ns, out_ticket, out_deadline_ns);
    if (result != PAPACC_RESULT_OK) {
        *out_outcome = PAPACC_SECURE_DATA_OUTCOME_STRUCTURAL_TICKET_INVALID;
        return result;
    }
    *out_outcome = PAPACC_SECURE_DATA_OUTCOME_ACCEPTED;
    return PAPACC_RESULT_OK;
}

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
    PAPACC_SECURE_DATA_OUTCOME *out_outcome)
{
    const PAPACC_SESSION_SECURITY_CONTEXT *control_security = NULL;
    PAPACC_CHANNEL *channel = NULL;
    PAPACC_U64 session_id = 0;
    PAPACC_RESULT result;
    if (out_session_instance_id != NULL) *out_session_instance_id = 0;
    if (out_channel_instance_id != NULL) *out_channel_instance_id = 0;
    if (out_outcome != NULL)
        *out_outcome = PAPACC_SECURE_DATA_OUTCOME_UNSPECIFIED;
    if (association_manager == NULL || channel_manager == NULL ||
        ticket == NULL || data_connection_instance_id == 0 ||
        out_session_instance_id == NULL || out_channel_instance_id == NULL ||
        out_outcome == NULL) return PAPACC_RESULT_INVALID_ARGUMENT;
    result = papacc_data_association_manager_inspect(
        association_manager, ticket, now_ns, &session_id);
    if (result != PAPACC_RESULT_OK) {
        *out_outcome = PAPACC_SECURE_DATA_OUTCOME_STRUCTURAL_TICKET_INVALID;
        return PAPACC_RESULT_OK;
    }
    if (papacc_connection_security_valid(data_security_context) != PAPACC_TRUE) {
        *out_outcome = PAPACC_SECURE_DATA_OUTCOME_DATA_CONTEXT_INVALID;
        return PAPACC_RESULT_OK;
    }
    if (lookup_session_security == NULL ||
        lookup_session_security(session_security_context, session_id,
            &control_security) != PAPACC_RESULT_OK ||
        papacc_session_security_valid(control_security) != PAPACC_TRUE) {
        *out_outcome = PAPACC_SECURE_DATA_OUTCOME_SESSION_CONTEXT_INVALID;
        return PAPACC_RESULT_OK;
    }
    if (papacc_principal_equal(&data_security_context->principal,
            &control_security->principal) != PAPACC_TRUE) {
        *out_outcome = PAPACC_SECURE_DATA_OUTCOME_PRINCIPAL_MISMATCH;
        return PAPACC_RESULT_OK;
    }
    result = papacc_secure_data_authorize(&data_security_context->principal,
        authorize, authorization_context, out_outcome);
    if (result != PAPACC_RESULT_OK ||
        *out_outcome != PAPACC_SECURE_DATA_OUTCOME_UNSPECIFIED) return result;
    control_security = NULL;
    if (lookup_session_security(session_security_context, session_id,
            &control_security) != PAPACC_RESULT_OK ||
        papacc_session_security_valid(control_security) != PAPACC_TRUE ||
        papacc_principal_equal(&data_security_context->principal,
            &control_security->principal) != PAPACC_TRUE) {
        *out_outcome = PAPACC_SECURE_DATA_OUTCOME_COMMIT_CONFLICT;
        return PAPACC_RESULT_OK;
    }
    result = papacc_data_association_manager_commit(
        association_manager, ticket, session_id, now_ns);
    if (result != PAPACC_RESULT_OK) {
        *out_outcome = PAPACC_SECURE_DATA_OUTCOME_COMMIT_CONFLICT;
        return PAPACC_RESULT_OK;
    }
    result = papacc_channel_manager_bind(channel_manager, session_id,
        data_connection_instance_id, PAPACC_CHANNEL_ROLE_DATA, &channel);
    if (result != PAPACC_RESULT_OK) {
        *out_outcome = PAPACC_SECURE_DATA_OUTCOME_CHANNEL_BIND_FAILED;
        return result;
    }
    *out_session_instance_id = session_id;
    *out_channel_instance_id = channel->channel_instance_id;
    *out_outcome = PAPACC_SECURE_DATA_OUTCOME_ACCEPTED;
    return PAPACC_RESULT_OK;
}

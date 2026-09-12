#include "authentication_authorization.h"

#include <string.h>

PAPACC_BOOL papacc_principal_equal(const PAPACC_PRINCIPAL *a,
    const PAPACC_PRINCIPAL *b)
{
    if (a == NULL || b == NULL || !a->valid || !b->valid) return PAPACC_FALSE;
    return memcmp(a->value, b->value, PAPACC_PRINCIPAL_VALUE_SIZE) == 0 ?
        PAPACC_TRUE : PAPACC_FALSE;
}

PAPACC_RESULT papacc_authenticate_and_authorize(
    PAPACC_CONNECTION_SECURITY_CONTEXT *context,
    const PAPACC_PEER_EVIDENCE *evidence,
    PAPACC_PRINCIPAL_RESOLVE_FN resolver, void *resolver_context,
    PAPACC_AUTHORIZATION_FN authorize, void *authorization_context,
    PAPACC_AUTHORIZATION_ACTION action, PAPACC_PRINCIPAL_RESOLUTION *resolution,
    PAPACC_AUTHORIZATION_DECISION *decision)
{
    PAPACC_PRINCIPAL temporary = PAPACC_PRINCIPAL_INITIALIZER;
    PAPACC_RESULT result;
    if (context == NULL || evidence == NULL || resolution == NULL ||
        decision == NULL) return PAPACC_RESULT_INVALID_ARGUMENT;
    *resolution = PAPACC_PRINCIPAL_RESOLVER_ERROR;
    *decision = PAPACC_AUTHORIZATION_DENY;
    context->state = PAPACC_SECURITY_CONTEXT_AUTHENTICATING;
    context->principal = (PAPACC_PRINCIPAL)PAPACC_PRINCIPAL_INITIALIZER;
    if (!evidence->certificate_present || !evidence->chain_validated ||
        !evidence->peer_authenticated || !evidence->certificate_sha256_valid ||
        resolver == NULL || authorize == NULL) goto denied;
    result = resolver(resolver_context, evidence, resolution, &temporary);
    if (result != PAPACC_RESULT_OK || *resolution != PAPACC_PRINCIPAL_RESOLVED ||
        !temporary.valid) goto denied;
    result = authorize(authorization_context, &temporary, action, decision);
    if (result != PAPACC_RESULT_OK || *decision != PAPACC_AUTHORIZATION_ALLOW)
        goto denied;
    context->principal = temporary;
    context->state = PAPACC_SECURITY_CONTEXT_AUTHORIZED;
    return PAPACC_RESULT_OK;
denied:
    context->state = PAPACC_SECURITY_CONTEXT_FAILED;
    context->principal = (PAPACC_PRINCIPAL)PAPACC_PRINCIPAL_INITIALIZER;
    return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_session_security_context_publish(
    PAPACC_SESSION_SECURITY_CONTEXT *session,
    const PAPACC_CONNECTION_SECURITY_CONTEXT *connection)
{
    if (session == NULL || connection == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if (connection->state != PAPACC_SECURITY_CONTEXT_AUTHORIZED ||
        !connection->principal.valid) return PAPACC_RESULT_INVALID_STATE;
    session->principal = connection->principal;
    session->published = PAPACC_TRUE;
    return PAPACC_RESULT_OK;
}

void papacc_connection_security_context_release(PAPACC_CONNECTION_SECURITY_CONTEXT *context)
{
    if (context != NULL) *context = (PAPACC_CONNECTION_SECURITY_CONTEXT)
        PAPACC_CONNECTION_SECURITY_CONTEXT_INITIALIZER;
}

void papacc_session_security_context_release(PAPACC_SESSION_SECURITY_CONTEXT *context)
{
    if (context != NULL) *context = (PAPACC_SESSION_SECURITY_CONTEXT)
        PAPACC_SESSION_SECURITY_CONTEXT_INITIALIZER;
}

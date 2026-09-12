#include "pst_peer_evidence.h"

#include <string.h>

PAPACC_RESULT papacc_pst_peer_evidence_extract(
    const PAPACC_PST_SECURE_PROCESSOR *processor,
    PAPACC_PEER_EVIDENCE *evidence)
{
    pst_peer_info *peer = NULL;
    PST_PEER_INFO_SUMMARY summary;
    PST_RESULT result;
    if (processor == NULL || evidence == NULL)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    *evidence = (PAPACC_PEER_EVIDENCE)PAPACC_PEER_EVIDENCE_INITIALIZER;
    if (processor->state != PAPACC_PST_SECURE_PROCESSOR_ESTABLISHED ||
        processor->connection == NULL) return PAPACC_RESULT_INVALID_STATE;
    result = pst_connection_get_peer_info(processor->connection, &peer);
    if (result != PST_RESULT_OK) return PAPACC_RESULT_INTERNAL_ERROR;
    memset(&summary, 0, sizeof(summary));
    summary.struct_size = (pst_u32)sizeof(summary);
    summary.api_version = PST_API_VERSION;
    result = pst_peer_info_get_summary(peer, &summary);
    if (result == PST_RESULT_OK && summary.local_role == PST_CONNECTION_ROLE_SERVER &&
        summary.certificate_present == PST_KNOWN_TRUE &&
        summary.chain_validated == PST_KNOWN_TRUE &&
        summary.peer_authenticated == PST_KNOWN_TRUE &&
        summary.certificate_sha256_size == PAPACC_CREDENTIAL_SHA256_SIZE) {
        evidence->certificate_present = PAPACC_TRUE;
        evidence->chain_validated = PAPACC_TRUE;
        evidence->peer_authenticated = PAPACC_TRUE;
        evidence->certificate_sha256_valid = PAPACC_TRUE;
        memcpy(evidence->certificate_sha256, summary.certificate_sha256,
            PAPACC_CREDENTIAL_SHA256_SIZE);
    }
    pst_peer_info_release(peer);
    return result == PST_RESULT_OK ? PAPACC_RESULT_OK : PAPACC_RESULT_INTERNAL_ERROR;
}

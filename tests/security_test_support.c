#include "security_test_support.h"

#include <windows.h>
#include <wincrypt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const PST_ALPN_PROTOCOL papacc_test_alpn = {
    (const pst_u8 *)"papacc/1", 8U
};

int papacc_test_security_load_blob(const char *name, PAPACC_TEST_BLOB *blob)
{
    char path[1024];
    FILE *file = NULL;
    long length;
    char *encoded;
    DWORD needed = 0U;
    DWORD written;
    if (sprintf_s(path, sizeof(path), "%s/%s.b64", PAPACC_TEST_PKI_DIR,
            name) < 0 || fopen_s(&file, path, "rb") != 0 || file == NULL)
        return 0;
    if (fseek(file, 0, SEEK_END) != 0 || (length = ftell(file)) <= 0 ||
        fseek(file, 0, SEEK_SET) != 0) {
        fclose(file);
        return 0;
    }
    encoded = (char *)malloc((size_t)length + 1U);
    if (encoded == NULL || fread(encoded, 1U, (size_t)length, file) !=
            (size_t)length) {
        free(encoded);
        fclose(file);
        return 0;
    }
    fclose(file);
    encoded[length] = '\0';
    if (!CryptStringToBinaryA(encoded, (DWORD)length, CRYPT_STRING_BASE64,
            NULL, &needed, NULL, NULL)) {
        free(encoded);
        return 0;
    }
    blob->data = (PAPACC_U8 *)malloc(needed);
    if (blob->data == NULL) {
        free(encoded);
        return 0;
    }
    written = needed;
    if (!CryptStringToBinaryA(encoded, (DWORD)length, CRYPT_STRING_BASE64,
            blob->data, &written, NULL, NULL)) {
        free(encoded);
        free(blob->data);
        blob->data = NULL;
        return 0;
    }
    free(encoded);
    blob->size = written;
    return 1;
}

int papacc_test_security_load_identity(const char *certificate_name,
    const char *key_name, PAPACC_TEST_IDENTITY *identity)
{
    PST_DER_ITEM certificate;
    PST_CREDENTIAL_SOURCE source;
    if (!papacc_test_security_load_blob(certificate_name, &identity->certificate) ||
        !papacc_test_security_load_blob(key_name, &identity->private_key)) return 0;
    certificate.data = identity->certificate.data;
    certificate.size = identity->certificate.size;
    memset(&source, 0, sizeof(source));
    source.struct_size = sizeof(source);
    source.api_version = PST_API_VERSION;
    source.kind = PST_CREDENTIAL_SOURCE_CERT_CHAIN_DER_PKCS8_DER;
    source.certificate_chain = &certificate;
    source.certificate_count = 1U;
    source.private_key_der = identity->private_key.data;
    source.private_key_der_size = identity->private_key.size;
    return pst_credentials_create(&source, &identity->credentials) ==
        PST_RESULT_OK;
}

void papacc_test_security_identity_release(PAPACC_TEST_IDENTITY *identity)
{
    if (identity->credentials != NULL)
        pst_credentials_release(identity->credentials);
    free(identity->certificate.data);
    free(identity->private_key.data);
    memset(identity, 0, sizeof(*identity));
}

int papacc_test_security_fixture_init(PAPACC_TEST_SECURITY_FIXTURE *fixture)
{
    PST_DER_ITEM anchor;
    PST_TRUST_SOURCE source;
    if (fixture == NULL) return 0;
    *fixture = (PAPACC_TEST_SECURITY_FIXTURE)
        PAPACC_TEST_SECURITY_FIXTURE_INITIALIZER;
    if (!papacc_test_security_load_blob("ca.der", &fixture->ca) ||
        !papacc_test_security_load_identity("server.der", "server.pk8",
            &fixture->server) ||
        !papacc_test_security_load_identity("client-a.der", "client-a.pk8",
            &fixture->client_a) ||
        !papacc_test_security_load_identity("client-b.der", "client-b.pk8",
            &fixture->client_b) ||
        !papacc_test_security_load_identity("client-c.der", "client-c.pk8",
            &fixture->client_c)) goto fail;
    anchor.data = fixture->ca.data;
    anchor.size = fixture->ca.size;
    memset(&source, 0, sizeof(source));
    source.struct_size = sizeof(source);
    source.api_version = PST_API_VERSION;
    source.kind = PST_TRUST_SOURCE_CUSTOM_CA_DER;
    source.anchors = &anchor;
    source.anchor_count = 1U;
    if (pst_trust_create(&source, &fixture->trust) != PST_RESULT_OK) goto fail;
    return 1;
fail:
    papacc_test_security_fixture_release(fixture);
    return 0;
}

void papacc_test_security_fixture_release(PAPACC_TEST_SECURITY_FIXTURE *fixture)
{
    if (fixture == NULL) return;
    if (fixture->trust != NULL) pst_trust_release(fixture->trust);
    papacc_test_security_identity_release(&fixture->client_c);
    papacc_test_security_identity_release(&fixture->client_b);
    papacc_test_security_identity_release(&fixture->client_a);
    papacc_test_security_identity_release(&fixture->server);
    free(fixture->ca.data);
    *fixture = (PAPACC_TEST_SECURITY_FIXTURE)
        PAPACC_TEST_SECURITY_FIXTURE_INITIALIZER;
}

PAPACC_U32 papacc_test_pst_client_capabilities(void)
{
    return PST_CAP_TLS_1_3 | PST_CAP_ROLE_CLIENT |
        PST_CAP_LOCAL_IDENTITY | PST_CAP_PEER_CERT_AUTH |
        PST_CAP_ALPN_CLIENT | PST_CAP_CUSTOM_TRUST |
        PST_CAP_PEER_NAME_VERIFY | PST_CAP_NONBLOCKING |
        PST_CAP_BACKEND_WAIT | PST_CAP_GRACEFUL_SHUTDOWN;
}

void papacc_test_pst_client_config(PST_CONNECTION_CONFIG *configuration,
    pst_credentials *identity, pst_trust *trust)
{
    static const char host[] = "papacc-test-server";
    memset(configuration, 0, sizeof(*configuration));
    configuration->struct_size = sizeof(*configuration);
    configuration->api_version = PST_API_VERSION;
    configuration->role = PST_CONNECTION_ROLE_CLIENT;
    configuration->provider_selection.struct_size =
        sizeof(configuration->provider_selection);
    configuration->provider_selection.api_version = PST_API_VERSION;
    configuration->provider_selection.mode = PST_BACKEND_SELECTION_EXACT;
    configuration->provider_selection.exact_provider_id = "openssl";
    configuration->provider_selection.required_capabilities =
        papacc_test_pst_client_capabilities();
    configuration->local_identity.struct_size =
        sizeof(configuration->local_identity);
    configuration->local_identity.api_version = PST_API_VERSION;
    configuration->local_identity.credentials = identity;
    configuration->peer_authentication.struct_size =
        sizeof(configuration->peer_authentication);
    configuration->peer_authentication.api_version = PST_API_VERSION;
    configuration->peer_authentication.certificate_mode =
        PST_PEER_CERTIFICATE_REQUIRED;
    configuration->peer_authentication.trust = trust;
    configuration->peer_authentication.expected_peer_name = host;
    configuration->peer_authentication.expected_peer_name_size =
        sizeof(host) - 1U;
    configuration->tls.struct_size = sizeof(configuration->tls);
    configuration->tls.api_version = PST_API_VERSION;
    configuration->tls.minimum_version = PST_TLS_VERSION_1_3;
    configuration->tls.maximum_version = PST_TLS_VERSION_1_3;
    configuration->tls.resumption = PST_FEATURE_DISABLED;
    configuration->tls.early_data = PST_FEATURE_DISABLED;
    configuration->tls.require_graceful_shutdown = PST_FEATURE_REQUIRED;
    configuration->alpn.struct_size = sizeof(configuration->alpn);
    configuration->alpn.api_version = PST_API_VERSION;
    configuration->alpn.mode = PST_FEATURE_REQUIRED;
    configuration->alpn.protocols = &papacc_test_alpn;
    configuration->alpn.protocol_count = 1U;
    configuration->server_name_indication_mode = PST_SNI_MODE_COMPAT;
}

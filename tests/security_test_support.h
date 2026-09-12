#ifndef PAPACC_SECURITY_TEST_SUPPORT_H
#define PAPACC_SECURITY_TEST_SUPPORT_H

#include "papacc/types.h"
#include "papinho_secure_transport.h"

typedef struct PAPACC_TEST_BLOB {
    PAPACC_U8 *data;
    PAPACC_SIZE size;
} PAPACC_TEST_BLOB;

typedef struct PAPACC_TEST_IDENTITY {
    PAPACC_TEST_BLOB certificate;
    PAPACC_TEST_BLOB private_key;
    pst_credentials *credentials;
} PAPACC_TEST_IDENTITY;

typedef struct PAPACC_TEST_SECURITY_FIXTURE {
    PAPACC_TEST_BLOB ca;
    PAPACC_TEST_IDENTITY server;
    PAPACC_TEST_IDENTITY client_a;
    PAPACC_TEST_IDENTITY client_b;
    PAPACC_TEST_IDENTITY client_c;
    pst_trust *trust;
} PAPACC_TEST_SECURITY_FIXTURE;

#define PAPACC_TEST_SECURITY_FIXTURE_INITIALIZER \
    { { NULL, 0U }, { { NULL, 0U }, { NULL, 0U }, NULL }, \
      { { NULL, 0U }, { NULL, 0U }, NULL }, \
      { { NULL, 0U }, { NULL, 0U }, NULL }, \
      { { NULL, 0U }, { NULL, 0U }, NULL }, NULL }

int papacc_test_security_fixture_init(PAPACC_TEST_SECURITY_FIXTURE *fixture);
void papacc_test_security_fixture_release(PAPACC_TEST_SECURITY_FIXTURE *fixture);
int papacc_test_security_load_blob(const char *name, PAPACC_TEST_BLOB *blob);
int papacc_test_security_load_identity(const char *certificate_name,
    const char *key_name, PAPACC_TEST_IDENTITY *identity);
void papacc_test_security_identity_release(PAPACC_TEST_IDENTITY *identity);
PAPACC_U32 papacc_test_pst_client_capabilities(void);
void papacc_test_pst_client_config(PST_CONNECTION_CONFIG *configuration,
    pst_credentials *identity, pst_trust *trust);

#endif

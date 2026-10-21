#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "server_security_source_win32.h"
#include <stdio.h>
#include <string.h>
static PAPACC_SECURITY_SOURCE_TRANSACTION normal,rollback,retained,rejected;
static unsigned checks;
#define CHECK(x) do{++checks;if(!(x)){printf("FAIL line=%d\n",__LINE__);return 1;}}while(0)
int main(void)
{
 PAPACC_LOGGER logger={NULL,NULL,PAPACC_LOG_LEVEL_OFF};
 PAPACC_SECURITY_CONFIGURATION_REF ref=PAPACC_SECURITY_CONFIGURATION_REF_INITIALIZER;
 const PAPACC_RESOLVED_SERVER_SECURITY_CONFIGURATION *view=NULL;
 PAPACC_U64 ticket=0U,stale;PAPACC_SIZE held;
 CHECK(SetEnvironmentVariableA("PAPACC_SECURITY_CONFIGURATION_REF","offline-source")!=0);
 CHECK(SetEnvironmentVariableA("PAPACC_SERVER_CERTIFICATE_DER_B64","tests/fixtures/security/server.der.b64")!=0);
 CHECK(SetEnvironmentVariableA("PAPACC_SERVER_PRIVATE_KEY_PKCS8_DER_B64","tests/fixtures/security/server.pk8.b64")!=0);
 CHECK(SetEnvironmentVariableA("PAPACC_CLIENT_CA_DER_B64","tests/fixtures/security/ca.der.b64")!=0);
 CHECK(SetEnvironmentVariableA("PAPACC_ALLOWED_CLIENT_CERT_SHA256","0000000000000000000000000000000000000000000000000000000000000000")!=0);
 CHECK(papacc_security_configuration_ref_set(&ref,(const PAPACC_U8 *)"offline-source",14U)==PAPACC_RESULT_OK);
 CHECK(papacc_server_security_source_prepare(&rejected,&ref,&logger,2097152U,32768U)==PAPACC_RESULT_LIMIT_EXCEEDED);
 CHECK(!rejected.held_storage&&rejected.state==PAPACC_SECURITY_SOURCE_EMPTY);
 CHECK(papacc_server_security_source_prepare(&rejected,&ref,&logger,0U,1U)!=PAPACC_RESULT_OK);
 CHECK(!rejected.held_storage&&!rejected.used);
 CHECK(papacc_server_security_source_prepare(&normal,&ref,&logger,0U,32768U)==PAPACC_RESULT_OK);
 CHECK(normal.state==PAPACC_SECURITY_SOURCE_PREPARED&&normal.used&&normal.used<=normal.credit);
 CHECK(papacc_server_security_source_validate(&normal)==PAPACC_RESULT_OK);
 CHECK(papacc_server_security_source_borrow(&normal,&ticket,&view)==PAPACC_RESULT_INVALID_STATE&&!ticket&&!view);
 /* Material-local commit/publication, NOT CONFIG_COMMITTED or policy CURRENT. */
 CHECK(papacc_server_security_source_commit(&normal)==PAPACC_RESULT_OK);
 CHECK(papacc_server_security_source_publish(&normal)==PAPACC_RESULT_OK);
 CHECK(papacc_server_security_source_borrow(&normal,&ticket,&view)==PAPACC_RESULT_OK);
 CHECK(view==&normal.configuration&&normal.borrowers==1U&&view->valid);
 CHECK(view->principal_resolve_context==view->release_context&&view->authorization_context==NULL&&view->composition_inputs.logger==&normal.logger);
 held=normal.used;stale=ticket;
 CHECK(papacc_server_security_source_finish(&normal,PAPACC_TRUE)==PAPACC_RESULT_INVALID_STATE&&normal.used==held);
 CHECK(papacc_server_security_source_unborrow(&normal,ticket)==PAPACC_RESULT_OK);
 CHECK(papacc_server_security_source_unborrow(&normal,stale)==PAPACC_RESULT_INVALID_STATE);
 CHECK(papacc_server_security_source_finish(&normal,PAPACC_FALSE)==PAPACC_RESULT_INVALID_STATE&&normal.used==held);
 CHECK(papacc_server_security_source_finish(&normal,PAPACC_TRUE)==PAPACC_RESULT_OK);
 CHECK(normal.state==PAPACC_SECURITY_SOURCE_CLOSED&&!normal.used&&!normal.held_storage);
 CHECK(papacc_server_security_source_finish(&normal,PAPACC_TRUE)==PAPACC_RESULT_INVALID_STATE);
 CHECK(papacc_server_security_source_prepare(&normal,&ref,&logger,0U,32768U)==PAPACC_RESULT_INVALID_STATE);
 CHECK(papacc_server_security_source_prepare(&rollback,&ref,&logger,0U,32768U)==PAPACC_RESULT_OK);
 CHECK(papacc_server_security_source_rollback(&rollback)==PAPACC_RESULT_OK);
 CHECK(!rollback.used&&!rollback.held_storage&&rollback.state==PAPACC_SECURITY_SOURCE_CLOSED);
 CHECK(papacc_server_security_source_prepare(&retained,&ref,&logger,0U,32768U)==PAPACC_RESULT_OK);
 CHECK(papacc_server_security_source_commit(&retained)==PAPACC_RESULT_OK);
 CHECK(papacc_server_security_source_publish(&retained)==PAPACC_RESULT_OK);
 CHECK(papacc_server_security_source_borrow(&retained,&ticket,&view)==PAPACC_RESULT_OK);
 held=retained.used;
 CHECK(papacc_server_security_source_retain(&retained)==PAPACC_RESULT_OK);
 CHECK(papacc_server_security_source_borrow(&retained,&stale,&view)==PAPACC_RESULT_INVALID_STATE);
 CHECK(papacc_server_security_source_unborrow(&retained,ticket)==PAPACC_RESULT_OK);
 CHECK(papacc_server_security_source_finish(&retained,PAPACC_TRUE)==PAPACC_RESULT_INVALID_STATE&&retained.used==held);
 CHECK(papacc_server_security_source_rollback(&retained)==PAPACC_RESULT_INVALID_STATE);
 printf("PASS checks=%u source_size=%llu credit=32768 peak=%llu retained=%llu\n",checks,
 (unsigned long long)sizeof(normal),(unsigned long long)normal.peak,(unsigned long long)retained.used);
 return 0;
}
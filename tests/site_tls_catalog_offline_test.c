#include "server_configuration_body05_win32.h"
#include <stdio.h>
#include <string.h>
static PAPACC_CONFIG_VIEW view,decoded;
static PAPACC_CONFIG_BODY04_VIEW old;
static unsigned char bytes[1024],saved[1024];
#define CHECK(x) do{++checks;if(!(x)){printf("FAIL line=%d %s\n",__LINE__,#x);return 1;}}while(0)
int main(void)
{
 unsigned checks=0;PAPACC_SITE_TLS_CATALOG c,out;PAPACC_SERVER_CONFIG *config=&view.config;
 memset(&view,0,sizeof(view));config->control_port=4433;config->listener_profile=1;
 config->security_configuration_ref_valid=1;config->security_configuration_ref_length=1;config->security_configuration_ref[0]='x';
 config->bind_selection.mode=PAPACC_BIND_SELECTION_ALL_INTERFACES;
 config->policy.present=1;config->policy.representation=PAPACC_ADMIN_REPRESENTATION_04;
 config->policy.identity[0]=1;config->policy.profile=1;config->policy.listener_port=4433;
 c=(PAPACC_SITE_TLS_CATALOG)PAPACC_SITE_TLS_CATALOG_INITIALIZER;
 c.present=c.version=c.trust_system=c.hostname_required=c.sni_required=c.tls13_only=1;
 c.identity[0]=1;c.profile_identity[0]=2;c.administrative_origin[0]=3;c.revision=c.profile_revision=1;
 c.reference=7;c.not_before_utc_ms=1000;c.expires_utc_ms=2000;c.max_bytes=4096;c.max_duration_ms=1000;c.max_concurrent=1;
 c.alpn_length=3;c.alpn[0]=2;c.alpn[1]='h';c.alpn[2]='2';
 CHECK(papacc_site_tls_catalog_validate(&c)==PAPACC_RESULT_OK);
 CHECK(papacc_site_tls_catalog_select(&c,7,1,1500,&out)==PAPACC_RESULT_OK&&!memcmp(&out,&c,sizeof(c)));
 CHECK(papacc_site_tls_catalog_select(&c,8,1,1500,&out)!=PAPACC_RESULT_OK&&!out.present);
 CHECK(papacc_site_tls_catalog_select(&c,7,2,1500,&out)!=PAPACC_RESULT_OK);
 CHECK(papacc_site_tls_catalog_select(&c,7,1,999,&out)!=PAPACC_RESULT_OK);
 CHECK(papacc_site_tls_catalog_select(&c,7,1,2000,&out)!=PAPACC_RESULT_OK);
 c.revoked=1;CHECK(papacc_site_tls_catalog_select(&c,7,1,1500,&out)!=PAPACC_RESULT_OK);c.revoked=0;
 c.trust_system=2;CHECK(papacc_site_tls_catalog_validate(&c)!=PAPACC_RESULT_OK);c.trust_system=1;
 c.hostname_required=0;CHECK(papacc_site_tls_catalog_validate(&c)!=PAPACC_RESULT_OK);c.hostname_required=1;
 c.sni_required=0;CHECK(papacc_site_tls_catalog_validate(&c)!=PAPACC_RESULT_OK);c.sni_required=1;
 c.tls13_only=0;CHECK(papacc_site_tls_catalog_validate(&c)!=PAPACC_RESULT_OK);c.tls13_only=1;
 c.max_concurrent=3;CHECK(papacc_site_tls_catalog_validate(&c)!=PAPACC_RESULT_OK);c.max_concurrent=1;
 c.alpn_length=65;CHECK(papacc_site_tls_catalog_validate(&c)!=PAPACC_RESULT_OK);c.alpn_length=3;
 c.version=2;CHECK(papacc_site_tls_catalog_validate(&c)!=PAPACC_RESULT_OK);c.version=1;
 out=(PAPACC_SITE_TLS_CATALOG)PAPACC_SITE_TLS_CATALOG_INITIALIZER;
 CHECK(papacc_site_tls_catalog_select(&out,0,1,1500,&out)==PAPACC_RESULT_INVALID_ARGUMENT);
 {PAPACC_SITE_TLS_CATALOG absent=PAPACC_SITE_TLS_CATALOG_INITIALIZER;CHECK(papacc_site_tls_catalog_select(&absent,0,1,1500,&out)!=PAPACC_RESULT_OK);}
 CHECK(papacc_site_tls_alpn_allowed(&c,c.alpn,c.alpn_length)==PAPACC_RESULT_OK);
 {const PAPACC_U8 bad[]={2U,'h','3'};
 CHECK(papacc_site_tls_alpn_allowed(&c,bad,sizeof(bad))!=PAPACC_RESULT_OK);}
 CHECK(papacc_site_tls_alpn_allowed(&c,c.alpn,1U)!=PAPACC_RESULT_OK);
 CHECK(papacc_site_tls_alpn_allowed(&c,NULL,0U)!=PAPACC_RESULT_OK);
 CHECK(papacc_site_tls_alpn_allowed(&c,c.alpn,257U)!=PAPACC_RESULT_OK);
 config->site_tls=c;view.domain[0]=1;view.revision=1;
 CHECK(papacc_server_config_validate(config)==PAPACC_RESULT_OK);
 CHECK(papacc_configuration_body05_encode(&view,bytes,1024)==PAPACC_RESULT_OK);
 memcpy(saved,bytes,1024);
 CHECK(papacc_configuration_body05_decode(bytes,1024,&decoded)==PAPACC_RESULT_OK&&!memcmp(&decoded.config.site_tls,&c,sizeof(c)));
 CHECK(papacc_configuration_body04_decode(bytes,1024,&old)!=PAPACC_RESULT_OK);
 bytes[850]^=1;CHECK(papacc_configuration_body05_decode(bytes,1024,&decoded)!=PAPACC_RESULT_OK);memcpy(bytes,saved,1024);
 CHECK(papacc_configuration_body05_decode(bytes,1023,&decoded)!=PAPACC_RESULT_OK);
 old.base=view;old.base.config.site_tls=(PAPACC_SITE_TLS_CATALOG)PAPACC_SITE_TLS_CATALOG_INITIALIZER;
 CHECK(papacc_configuration_body04_encode(&old,bytes,1024)==PAPACC_RESULT_OK);
 CHECK(papacc_configuration_body05_decode(bytes,1024,&decoded)!=PAPACC_RESULT_OK);
 CHECK(papacc_configuration_body04_decode(bytes,1024,&old)==PAPACC_RESULT_OK&&!old.base.config.site_tls.present);
 CHECK(papacc_configuration_body05_encode(&old.base,bytes,1024)!=PAPACC_RESULT_OK);
 puts("PASS_OFFLINE_SYNTHETIC_CATALOG_CODEC NOT_OPERATIONAL_AUTHORITY");
 printf("CHECKS=%u CATALOG_SIZE=%llu\n",checks,(unsigned long long)sizeof(c));return 0;
}

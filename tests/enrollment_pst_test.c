/* Config projection only; no PST create/attach/handshake or socket. */
#include "enrollment_pst.h"
#include "security_composition.h"
#include <stdio.h>
#include <string.h>
static unsigned checks;
#define CHECK(x) do{checks++;if(!(x)){printf("FAIL line=%u\n",(unsigned)__LINE__);return 1;}}while(0)
int main(void){
 EP_TLS_PROFILE p;PAPACC_SECURITY_COMPOSITION op;PST_CONNECTION_CONFIG c;unsigned char sentinel;
 memset(&p,0,sizeof(p));memset(&op,0,sizeof(op));
 CHECK(ep_tls_profile_prepare(&p,NULL,"openssl",7)==EP_INVALID&&!p.initialized);
 CHECK(ep_tls_profile_prepare(&p,(pst_credentials *)&sentinel,"openssl",7)==EP_DATA_ONLY);
 CHECK(p.config.role==PST_CONNECTION_ROLE_SERVER&&p.config.peer_authentication.certificate_mode==PST_PEER_CERTIFICATE_DISABLED&&p.config.peer_authentication.trust==NULL);
 CHECK(p.config.tls.minimum_version==PST_TLS_VERSION_1_3&&p.config.tls.maximum_version==PST_TLS_VERSION_1_3&&p.config.tls.early_data==PST_FEATURE_DISABLED&&p.config.tls.resumption==PST_FEATURE_DISABLED);
 CHECK(p.config.alpn.mode==PST_FEATURE_REQUIRED&&p.config.alpn.protocol_count==1&&p.protocol.data==p.alpn&&p.protocol.size==15&&!memcmp(p.alpn,"papacc-enroll/1",15));
 CHECK(ep_tls_profile_prepare(&p,(pst_credentials *)&sentinel,"openssl",7)==EP_INVALID);
 CHECK(ep_tls_selected_data(p.alpn,15,PST_TLS_VERSION_1_3)==EP_DATA_ONLY);CHECK(ep_tls_selected_data((const PAPACC_U8 *)"papacc/1",8,PST_TLS_VERSION_1_3)==EP_DENIED);CHECK(ep_tls_selected_data(p.alpn,15,PST_TLS_VERSION_1_2)==EP_DENIED);
 op.state=PAPACC_SECURITY_COMPOSITION_READY;op.local_credentials=(pst_credentials *)&sentinel;memcpy(op.provider_id,"openssl",8);memcpy(op.secure_principal_alpn,"papacc/1",8);op.secure_principal_alpn_protocol.data=op.secure_principal_alpn;op.secure_principal_alpn_protocol.size=8;
 CHECK(papacc_security_composition_build_secure_principal_config(&op,&c)==PAPACC_RESULT_OK);
 CHECK(c.peer_authentication.certificate_mode==PST_PEER_CERTIFICATE_REQUIRED&&c.tls.minimum_version==PST_TLS_VERSION_1_3&&c.tls.maximum_version==PST_TLS_VERSION_1_3);
 CHECK(c.alpn.protocol_count==1&&c.alpn.protocols->size==8&&!memcmp(c.alpn.protocols->data,"papacc/1",8));
 CHECK(c.provider_selection.required_capabilities&PST_CAP_PEER_CERT_AUTH);
 printf("PASS %u PROFILE_DATA assertions size=%zu; fake opaque credential is never dereferenced; no native TLS proof\n",checks,sizeof(p));return 0;
}

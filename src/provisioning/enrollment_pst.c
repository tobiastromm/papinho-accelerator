#include "enrollment_pst.h"
#include <string.h>
EP_RESULT ep_tls_profile_prepare(EP_TLS_PROFILE *p,pst_credentials *credential,const char *provider,PAPACC_SIZE n){
 PST_CONNECTION_CONFIG *c;PAPACC_SIZE i,a=(PAPACC_SIZE)p,b=(PAPACC_SIZE)provider;
 if(!p||p->initialized||!credential||!provider||!n||n>=sizeof(p->provider)||n>(PAPACC_SIZE)-1-b)return EP_INVALID;
 if(a<=b?b-a<sizeof(*p):a-b<n)return EP_INVALID;
 for(i=0;i<n;i++)if(provider[i]<33||provider[i]>126)return EP_INVALID;
 memset(p,0,sizeof(*p));memcpy(p->provider,provider,n);memcpy(p->alpn,"papacc-enroll/1",15);p->protocol.data=p->alpn;p->protocol.size=15;c=&p->config;
 c->struct_size=(pst_u32)sizeof(*c);c->api_version=PST_API_VERSION;c->role=PST_CONNECTION_ROLE_SERVER;
 c->provider_selection.struct_size=(pst_u32)sizeof(c->provider_selection);c->provider_selection.api_version=PST_API_VERSION;c->provider_selection.mode=PST_BACKEND_SELECTION_EXACT;c->provider_selection.exact_provider_id=p->provider;
 c->provider_selection.required_capabilities=PST_CAP_ROLE_SERVER|PST_CAP_TLS_1_3|PST_CAP_LOCAL_IDENTITY|PST_CAP_ALPN_SERVER|PST_CAP_NONBLOCKING|PST_CAP_BACKEND_WAIT|PST_CAP_GRACEFUL_SHUTDOWN;
 c->local_identity.struct_size=(pst_u32)sizeof(c->local_identity);c->local_identity.api_version=PST_API_VERSION;c->local_identity.credentials=credential;
 c->peer_authentication.struct_size=(pst_u32)sizeof(c->peer_authentication);c->peer_authentication.api_version=PST_API_VERSION;c->peer_authentication.certificate_mode=PST_PEER_CERTIFICATE_DISABLED;
 c->tls.struct_size=(pst_u32)sizeof(c->tls);c->tls.api_version=PST_API_VERSION;c->tls.minimum_version=PST_TLS_VERSION_1_3;c->tls.maximum_version=PST_TLS_VERSION_1_3;c->tls.resumption=PST_FEATURE_DISABLED;c->tls.early_data=PST_FEATURE_DISABLED;c->tls.require_graceful_shutdown=PST_FEATURE_REQUIRED;
 c->alpn.struct_size=(pst_u32)sizeof(c->alpn);c->alpn.api_version=PST_API_VERSION;c->alpn.mode=PST_FEATURE_REQUIRED;c->alpn.protocols=&p->protocol;c->alpn.protocol_count=1;p->initialized=1;return EP_DATA_ONLY;
}
EP_RESULT ep_tls_selected_data(const PAPACC_U8 *p,PAPACC_SIZE n,pst_u32 version){
 if(!p||n!=15U||version!=PST_TLS_VERSION_1_3||memcmp(p,"papacc-enroll/1",15))return EP_DENIED;
 return EP_DATA_ONLY; /* only ALPN/version data, NOT a handshake/trust fact */
}

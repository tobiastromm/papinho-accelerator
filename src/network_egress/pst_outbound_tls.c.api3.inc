#include "pst_socket_attachment_win32.h"
#include "pst_outbound_tls.h"
#include "papinho_secure_transport_win32.h"
#include <string.h>
#ifdef PAPACC_ENABLE_PST_TEST_CORRELATION
#include <stdio.h>
#include <ws2tcpip.h>
#endif

#ifdef PAPACC_ENABLE_PST_NEED_WRITE_GATE
/* Private fixture configuration, absent from normal builds. No lifecycle override. */
static pst_runtime *def_fixture_runtime;
static pst_credentials *def_fixture_credentials;
void papacc_def_test_only_client_identity(pst_runtime *runtime, pst_credentials *credentials)
{
    def_fixture_runtime=runtime;
    def_fixture_credentials=credentials;
}
#endif

static PAPACC_RESULT verify_peer(PAPACC_OUTBOUND_TLS *tls)
{
    pst_peer_info *peer=NULL;PST_PEER_INFO_SUMMARY summary;PST_RESULT r;
    PAPACC_U8 selected[PAPACC_OUTBOUND_TLS_ALPN_CAPACITY];pst_size size=0U;
    memset(&summary,0,sizeof(summary));summary.struct_size=sizeof(summary);
    summary.api_version=PST_API_VERSION;
    r=pst_connection_get_peer_info(tls->processor.connection,&peer);
    if(r==PST_RESULT_OK)r=pst_peer_info_get_summary(peer,&summary);
    if(r==PST_RESULT_OK&&(summary.tls_version!=PST_TLS_VERSION_1_3||
        summary.certificate_present!=PST_KNOWN_TRUE||
        summary.chain_validated!=PST_KNOWN_TRUE||
        summary.peer_name_validated!=PST_KNOWN_TRUE||
        summary.peer_authenticated!=PST_KNOWN_TRUE))r=PST_RESULT_POLICY_VIOLATION;
    if(r==PST_RESULT_OK&&tls->alpn_protocol.size!=0U){
        r=pst_connection_get_negotiated_alpn(tls->processor.connection,
            selected,sizeof(selected),&size);
        if(r==PST_RESULT_OK&&(size!=tls->alpn_protocol.size||
            memcmp(selected,tls->alpn_value,size)!=0))r=PST_RESULT_POLICY_VIOLATION;
    }
    if(peer!=NULL)pst_peer_info_release(peer);
    if(r!=PST_RESULT_OK){tls->processor.source_result=r;
        tls->processor.state=PAPACC_PST_SECURE_PROCESSOR_FAILED;
        return PAPACC_RESULT_INTERNAL_ERROR;}
    tls->established=PAPACC_TRUE;return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_outbound_tls_init(PAPACC_OUTBOUND_TLS *tls,
    const PAPACC_OUTBOUND_TLS_POLICY *p,const char *expected_peer_name,
    PAPACC_U64 deadline)
{
    PST_CONNECTION_CONFIG c;PAPACC_SIZE n;PAPACC_U32 caps;
    if(tls==NULL||p==NULL||p->runtime==NULL||p->trust==NULL||
        p->provider_id==NULL||expected_peer_name==NULL||deadline==0U||
        p->alpn_size>PAPACC_OUTBOUND_TLS_ALPN_CAPACITY||
        (p->alpn_size!=0U&&p->alpn==NULL))return PAPACC_RESULT_INVALID_ARGUMENT;
    n=strlen(expected_peer_name);if(n==0U||n>=sizeof(tls->expected_peer_name))
        return PAPACC_RESULT_INVALID_ARGUMENT;
    if(tls->unconsumed_transport!=NULL||tls->processor.connection!=NULL||
       tls->processor.state==PAPACC_PST_SECURE_PROCESSOR_RETAINED_TERMINAL)
        return PAPACC_RESULT_INVALID_STATE;
    *tls=(PAPACC_OUTBOUND_TLS)PAPACC_OUTBOUND_TLS_INITIALIZER;
    memcpy(tls->expected_peer_name,expected_peer_name,n+1U);
    if(p->alpn_size!=0U){memcpy(tls->alpn_value,p->alpn,p->alpn_size);
        tls->alpn_protocol.data=tls->alpn_value;
        tls->alpn_protocol.size=p->alpn_size;}
    memset(&c,0,sizeof(c));c.struct_size=sizeof(c);c.api_version=PST_API_VERSION;
    c.role=PST_CONNECTION_ROLE_CLIENT;
    c.provider_selection.struct_size=sizeof(c.provider_selection);
    c.provider_selection.api_version=PST_API_VERSION;
    c.provider_selection.mode=PST_BACKEND_SELECTION_EXACT;
    c.provider_selection.exact_provider_id=p->provider_id;
    caps=PST_CAP_TLS_1_3|PST_CAP_ROLE_CLIENT|PST_CAP_PEER_CERT_AUTH|
        PST_CAP_CUSTOM_TRUST|PST_CAP_PEER_NAME_VERIFY|PST_CAP_PEER_INFO|
        PST_CAP_NONBLOCKING|PST_CAP_BACKEND_WAIT|PST_CAP_GRACEFUL_SHUTDOWN;
    if(p->alpn_size!=0U)caps|=PST_CAP_ALPN_CLIENT;
    c.provider_selection.required_capabilities=caps;
    c.local_identity.struct_size=sizeof(c.local_identity);
    c.local_identity.api_version=PST_API_VERSION;
#ifdef PAPACC_ENABLE_PST_NEED_WRITE_GATE
    if(p->runtime==def_fixture_runtime && def_fixture_credentials!=NULL){
        c.local_identity.credentials=def_fixture_credentials;
        c.provider_selection.required_capabilities|=PST_CAP_LOCAL_IDENTITY;
    }
#endif
    c.peer_authentication.struct_size=sizeof(c.peer_authentication);
    c.peer_authentication.api_version=PST_API_VERSION;
    c.peer_authentication.certificate_mode=PST_PEER_CERTIFICATE_REQUIRED;
    c.peer_authentication.trust=p->trust;
    c.peer_authentication.expected_peer_name=tls->expected_peer_name;
    c.peer_authentication.expected_peer_name_size=n;
    c.tls.struct_size=sizeof(c.tls);c.tls.api_version=PST_API_VERSION;
    c.tls.minimum_version=PST_TLS_VERSION_1_3;
    c.tls.maximum_version=PST_TLS_VERSION_1_3;
    c.tls.resumption=PST_FEATURE_DISABLED;c.tls.early_data=PST_FEATURE_DISABLED;
    c.tls.require_graceful_shutdown=PST_FEATURE_REQUIRED;
    c.alpn.struct_size=sizeof(c.alpn);c.alpn.api_version=PST_API_VERSION;
    c.alpn.mode=p->alpn_size==0U?PST_FEATURE_DISABLED:
        (p->alpn_required?PST_FEATURE_REQUIRED:PST_FEATURE_OPTIONAL);
    c.alpn.protocols=p->alpn_size==0U?NULL:&tls->alpn_protocol;
    c.alpn.protocol_count=p->alpn_size==0U?0U:1U;
    c.server_name_indication_mode=PST_SNI_MODE_COMPAT;
    return papacc_pst_secure_processor_init(&tls->processor,p->runtime,&c,deadline);
}

#ifdef PAPACC_ENABLE_PST_TEST_CORRELATION
static PAPACC_RESULT papacc_outbound_tls_test_format_endpoint(
    SOCKET s,int peer,char *out,PAPACC_SIZE capacity)
{
    struct sockaddr_storage address;int address_size=(int)sizeof(address);
    char address_text[INET6_ADDRSTRLEN];USHORT port;int written;
    int result=peer?getpeername(s,(struct sockaddr *)&address,&address_size):
        getsockname(s,(struct sockaddr *)&address,&address_size);
    if(result!=0)return PAPACC_RESULT_INTERNAL_ERROR;
    if(address.ss_family==AF_INET){
        const struct sockaddr_in *value=(const struct sockaddr_in *)&address;
        if(InetNtopA(AF_INET,(void *)&value->sin_addr,address_text,
                sizeof(address_text))==NULL)return PAPACC_RESULT_INTERNAL_ERROR;
        port=ntohs(value->sin_port);
        written=sprintf_s(out,capacity,"%s:%u",address_text,(unsigned)port);
    }else if(address.ss_family==AF_INET6){
        const struct sockaddr_in6 *value=(const struct sockaddr_in6 *)&address;
        if(InetNtopA(AF_INET6,(void *)&value->sin6_addr,address_text,
                sizeof(address_text))==NULL)return PAPACC_RESULT_INTERNAL_ERROR;
        port=ntohs(value->sin6_port);
        written=sprintf_s(out,capacity,"[%s]:%u",address_text,(unsigned)port);
    }else return PAPACC_RESULT_NOT_SUPPORTED;
    return written>0&&(PAPACC_SIZE)written<capacity?
        PAPACC_RESULT_OK:PAPACC_RESULT_LIMIT_EXCEEDED;
}
PAPACC_RESULT papacc_outbound_tls_test_capture_endpoints(
    PAPACC_OUTBOUND_TLS *tls,SOCKET s)
{
    PAPACC_RESULT result;
    if(tls==NULL||s==INVALID_SOCKET)return PAPACC_RESULT_INVALID_ARGUMENT;
    result=papacc_outbound_tls_test_format_endpoint(s,0,
        tls->test_local_endpoint,sizeof(tls->test_local_endpoint));
    if(result!=PAPACC_RESULT_OK)return result;
    return papacc_outbound_tls_test_format_endpoint(s,1,
        tls->test_remote_endpoint,sizeof(tls->test_remote_endpoint));
}
#endif

PAPACC_RESULT papacc_outbound_tls_attach_socket(PAPACC_OUTBOUND_TLS *tls,SOCKET s,PAPACC_BOOL *consumed)
{
 PAPACC_BOOL accepted=PAPACC_FALSE;PAPACC_RESULT result;
 if(consumed==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;
 *consumed=PAPACC_FALSE;
 if(tls==NULL||s==INVALID_SOCKET)return PAPACC_RESULT_INVALID_ARGUMENT;
 result=papacc_pst_socket_attach(&tls->processor,&s,&tls->unconsumed_transport,&accepted);
 /* consumed here means caller socket ownership, not PST attach consumption. */
 if(s==INVALID_SOCKET)*consumed=PAPACC_TRUE;
 return result;
}
PAPACC_RESULT papacc_outbound_tls_handshake_once(PAPACC_OUTBOUND_TLS*t,
    PAPACC_BOOL*done){PAPACC_PST_SECURE_STEP s;PAPACC_RESULT r;if(done!=NULL)*done=PAPACC_FALSE;
 if(t==NULL||done==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;
 r=papacc_pst_secure_processor_handshake_once(&t->processor,&s);
 if(r!=PAPACC_RESULT_OK||s==PAPACC_PST_SECURE_STEP_FAILED||s==PAPACC_PST_SECURE_STEP_CLOSED)return PAPACC_RESULT_INTERNAL_ERROR;
 if(s==PAPACC_PST_SECURE_STEP_COMPLETE){r=verify_peer(t);if(r==PAPACC_RESULT_OK)*done=PAPACC_TRUE;}return r;}
PAPACC_RESULT papacc_outbound_tls_read(void*c,PAPACC_U8*b,PAPACC_SIZE z,PAPACC_SIZE*n,PAPACC_BOOL*eof)
{PAPACC_OUTBOUND_TLS*t=(PAPACC_OUTBOUND_TLS*)c;PAPACC_PST_SECURE_STEP s;PAPACC_RESULT r;if(eof!=NULL)*eof=PAPACC_FALSE;
 r=papacc_pst_secure_processor_read_once(&t->processor,b,z,n,&s);if(r!=PAPACC_RESULT_OK)return r;
 if(s==PAPACC_PST_SECURE_STEP_FAILED){if(t->processor.source_result==
      PST_RESULT_TRUNCATED)t->truncated=PAPACC_TRUE;
  return PAPACC_RESULT_INTERNAL_ERROR;}
 if(s==PAPACC_PST_SECURE_STEP_CLOSED){t->clean_peer_close=PAPACC_TRUE;
  *eof=PAPACC_TRUE;}return PAPACC_RESULT_OK;}
PAPACC_RESULT papacc_outbound_tls_write(void*c,const PAPACC_U8*b,PAPACC_SIZE z,PAPACC_SIZE*n)
{PAPACC_PST_SECURE_STEP s;PAPACC_RESULT r=papacc_pst_secure_processor_write_once(&((PAPACC_OUTBOUND_TLS*)c)->processor,b,z,n,&s);return r==PAPACC_RESULT_OK&&s!=PAPACC_PST_SECURE_STEP_FAILED&&s!=PAPACC_PST_SECURE_STEP_CLOSED?PAPACC_RESULT_OK:PAPACC_RESULT_INTERNAL_ERROR;}
PAPACC_RESULT papacc_outbound_tls_half_close(void*c){(void)c;return PAPACC_RESULT_NOT_SUPPORTED;}
PAPACC_RESULT papacc_outbound_tls_interest(PAPACC_OUTBOUND_TLS*t,
    PAPACC_BOOL*r,PAPACC_BOOL*w){if(t==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;
 return papacc_pst_secure_processor_interest(&t->processor,r,w);}
PAPACC_RESULT papacc_outbound_tls_check_deadline(PAPACC_OUTBOUND_TLS*t,
    PAPACC_U64 now,PAPACC_BOOL*expired){if(t==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;
 return papacc_pst_secure_processor_check_deadline(&t->processor,now,expired);}
PAPACC_RESULT papacc_outbound_tls_shutdown_once(PAPACC_OUTBOUND_TLS*t,
    PAPACC_BOOL*done){PAPACC_PST_SECURE_STEP s;PAPACC_RESULT r;
 if(done!=NULL)*done=PAPACC_FALSE;if(t==NULL||done==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;
 r=papacc_pst_secure_processor_shutdown_once(&t->processor,&s);
 if(r!=PAPACC_RESULT_OK)return r;if(s==PAPACC_PST_SECURE_STEP_FAILED)
  return PAPACC_RESULT_INTERNAL_ERROR;if(s==PAPACC_PST_SECURE_STEP_COMPLETE||
  s==PAPACC_PST_SECURE_STEP_CLOSED)*done=PAPACC_TRUE;return PAPACC_RESULT_OK;}
PAPACC_RESULT papacc_outbound_tls_try_close(void *context, PAPACC_BOOL *closed)
{
    PAPACC_OUTBOUND_TLS *tls = (PAPACC_OUTBOUND_TLS *)context;
    PAPACC_RESULT result;
    if (tls == NULL || closed == NULL) return PAPACC_RESULT_INVALID_ARGUMENT;
    *closed = PAPACC_FALSE;
    if(tls->unconsumed_transport!=NULL)return PAPACC_RESULT_INVALID_STATE;
    if(tls->processor.connection!=NULL&&tls->close_authorize!=NULL){
        result=tls->close_authorize(tls->close_authority_context);
        if(result!=PAPACC_RESULT_OK)return result;
    }
    result = papacc_pst_secure_processor_release(&tls->processor);
    if (result != PAPACC_RESULT_OK || tls->processor.connection != NULL)
        return result == PAPACC_RESULT_OK ? PAPACC_RESULT_INTERNAL_ERROR : result;
    tls->established = PAPACC_FALSE;
    *closed = PAPACC_TRUE;
    return PAPACC_RESULT_OK;
}
void papacc_outbound_tls_abort(PAPACC_OUTBOUND_TLS *tls)
{
    PAPACC_BOOL closed;
    (void)papacc_outbound_tls_try_close(tls, &closed);
}
void papacc_outbound_tls_close(void *context)
{
    papacc_outbound_tls_abort((PAPACC_OUTBOUND_TLS *)context);
}

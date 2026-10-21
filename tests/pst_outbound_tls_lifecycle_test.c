#include "pst_outbound_tls.h"
#include "network_stream.h"
#include "security_test_support.h"
#include "papinho_secure_transport_win32.h"
#include <string.h>

#define CHECK(x,n) do { if (!(x)) { result=(n); goto cleanup; } } while (0)
#define ALPN "papacc/1"

typedef struct TLS_PAIR {
    PAPACC_OUTBOUND_TLS client;
    pst_connection *server;
    SOCKET client_socket;
    SOCKET server_socket_alias;
    int close_count;
    int shutdown_count;
} TLS_PAIR;

typedef struct PLAIN_IO {
    PAPACC_U8 bytes[16];
    PAPACC_SIZE size;
    int close_count;
} PLAIN_IO;

static int socket_pair(SOCKET *client, SOCKET *server)
{
    SOCKET listener=INVALID_SOCKET;
    struct sockaddr_in address;
    int address_size=(int)sizeof(address);
    u_long nonblocking=1UL;
    *client=INVALID_SOCKET;*server=INVALID_SOCKET;
    listener=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(listener==INVALID_SOCKET)return 0;
    memset(&address,0,sizeof(address));address.sin_family=AF_INET;
    address.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
    if(bind(listener,(struct sockaddr*)&address,sizeof(address))!=0||
       listen(listener,1)!=0||getsockname(listener,(struct sockaddr*)&address,
       &address_size)!=0)goto fail;
    *client=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(*client==INVALID_SOCKET||connect(*client,(struct sockaddr*)&address,
       sizeof(address))!=0)goto fail;
    *server=accept(listener,NULL,NULL);
    if(*server==INVALID_SOCKET)goto fail;
    closesocket(listener);listener=INVALID_SOCKET;
    if(ioctlsocket(*client,FIONBIO,&nonblocking)!=0||
       ioctlsocket(*server,FIONBIO,&nonblocking)!=0)goto fail;
    return 1;
fail:
    if(listener!=INVALID_SOCKET)closesocket(listener);
    if(*client!=INVALID_SOCKET){closesocket(*client);*client=INVALID_SOCKET;}
    if(*server!=INVALID_SOCKET){closesocket(*server);*server=INVALID_SOCKET;}
    return 0;
}

static void server_config(PST_CONNECTION_CONFIG *configuration,
    pst_credentials *credentials)
{
    static const PST_ALPN_PROTOCOL alpn={
        (const pst_u8*)ALPN,sizeof(ALPN)-1U};
    memset(configuration,0,sizeof(*configuration));
    configuration->struct_size=sizeof(*configuration);
    configuration->api_version=PST_API_VERSION;
    configuration->role=PST_CONNECTION_ROLE_SERVER;
    configuration->provider_selection.struct_size=
        sizeof(configuration->provider_selection);
    configuration->provider_selection.api_version=PST_API_VERSION;
    configuration->provider_selection.mode=PST_BACKEND_SELECTION_EXACT;
    configuration->provider_selection.exact_provider_id="openssl";
    configuration->provider_selection.required_capabilities=
        PST_CAP_TLS_1_3|PST_CAP_ROLE_SERVER|PST_CAP_LOCAL_IDENTITY|
        PST_CAP_ALPN_SERVER|PST_CAP_NONBLOCKING|PST_CAP_BACKEND_WAIT|
        PST_CAP_GRACEFUL_SHUTDOWN;
    configuration->local_identity.struct_size=
        sizeof(configuration->local_identity);
    configuration->local_identity.api_version=PST_API_VERSION;
    configuration->local_identity.credentials=credentials;
    configuration->peer_authentication.struct_size=
        sizeof(configuration->peer_authentication);
    configuration->peer_authentication.api_version=PST_API_VERSION;
    configuration->peer_authentication.certificate_mode=
        PST_PEER_CERTIFICATE_DISABLED;
    configuration->tls.struct_size=sizeof(configuration->tls);
    configuration->tls.api_version=PST_API_VERSION;
    configuration->tls.minimum_version=PST_TLS_VERSION_1_3;
    configuration->tls.maximum_version=PST_TLS_VERSION_1_3;
    configuration->tls.resumption=PST_FEATURE_DISABLED;
    configuration->tls.early_data=PST_FEATURE_DISABLED;
    configuration->tls.require_graceful_shutdown=PST_FEATURE_REQUIRED;
    configuration->alpn.struct_size=sizeof(configuration->alpn);
    configuration->alpn.api_version=PST_API_VERSION;
    configuration->alpn.mode=PST_FEATURE_REQUIRED;
    configuration->alpn.protocols=&alpn;
    configuration->alpn.protocol_count=1U;
}

static int establish_pair(TLS_PAIR *pair_value,pst_runtime *runtime,
    pst_trust *trust,pst_credentials *credentials)
{
    PAPACC_OUTBOUND_TLS_POLICY policy;
    PST_CONNECTION_CONFIG configuration;
    pst_transport *transport=NULL;
    pst_u32 accepted=0U,operation=PST_OPERATION_NEED_READ;
    PST_RESULT source=PST_RESULT_OK;
    PAPACC_BOOL complete=PAPACC_FALSE;
    SOCKET client=INVALID_SOCKET,server=INVALID_SOCKET;
    int index;
    *pair_value=(TLS_PAIR){PAPACC_OUTBOUND_TLS_INITIALIZER,NULL,
        INVALID_SOCKET,INVALID_SOCKET,0,0};
    if(!socket_pair(&client,&server))return 0;
    server_config(&configuration,credentials);
    if(pst_connection_create(runtime,&configuration,&pair_value->server)!=
            PST_RESULT_OK||
       pst_win32_socket_transport_create(server,&transport)!=PST_RESULT_OK||
       pst_connection_attach(pair_value->server,transport,
            PST_OWNERSHIP_TRANSFERRED,&accepted)!=PST_RESULT_OK||!accepted)
        goto fail;
    pair_value->server_socket_alias=server;transport=NULL;
    memset(&policy,0,sizeof(policy));policy.runtime=runtime;policy.trust=trust;
    policy.provider_id="openssl";policy.alpn=(const PAPACC_U8*)ALPN;
    policy.alpn_size=sizeof(ALPN)-1U;policy.alpn_required=PAPACC_TRUE;
    if(papacc_outbound_tls_init(&pair_value->client,&policy,
            "papacc-test-server",~(PAPACC_U64)0)!=PAPACC_RESULT_OK||
       papacc_outbound_tls_attach_socket(&pair_value->client,client)!=
            PAPACC_RESULT_OK)goto fail;
    pair_value->client_socket=client;client=INVALID_SOCKET;
    for(index=0;index<10000&&!complete;++index){
        if(papacc_outbound_tls_handshake_once(&pair_value->client,&complete)!=
                PAPACC_RESULT_OK||
           pst_connection_handshake(pair_value->server,&operation,&source)!=
                PST_RESULT_OK||operation==PST_OPERATION_FAILED)goto fail;
    }
    return complete==PAPACC_TRUE;
fail:
    if(transport!=NULL)pst_transport_release(transport);
    if(client!=INVALID_SOCKET)closesocket(client);
    return 0;
}

static PAPACC_RESULT tls_read(void *context,PAPACC_U8 *bytes,PAPACC_SIZE size,
    PAPACC_SIZE *read_size,PAPACC_BOOL *eof)
{return papacc_outbound_tls_read(&((TLS_PAIR*)context)->client,bytes,size,
    read_size,eof);}
static PAPACC_RESULT tls_write(void *context,const PAPACC_U8 *bytes,
    PAPACC_SIZE size,PAPACC_SIZE *written)
{return papacc_outbound_tls_write(&((TLS_PAIR*)context)->client,bytes,size,
    written);}
static PAPACC_RESULT tls_half(void *context)
{return papacc_outbound_tls_half_close(&((TLS_PAIR*)context)->client);}
static PAPACC_RESULT tls_shutdown(void *context,PAPACC_BOOL *done)
{TLS_PAIR *pair_value=(TLS_PAIR*)context;++pair_value->shutdown_count;
 return papacc_outbound_tls_shutdown_once(&pair_value->client,done);}
static void tls_close(void *context)
{TLS_PAIR *pair_value=(TLS_PAIR*)context;++pair_value->close_count;
 papacc_outbound_tls_close(&pair_value->client);}

static PAPACC_RESULT plain_read(void *context,PAPACC_U8 *bytes,PAPACC_SIZE size,
    PAPACC_SIZE *read_size,PAPACC_BOOL *eof)
{(void)context;(void)bytes;(void)size;*read_size=0U;*eof=PAPACC_FALSE;
 return PAPACC_RESULT_OK;}
static PAPACC_RESULT plain_write(void *context,const PAPACC_U8 *bytes,
    PAPACC_SIZE size,PAPACC_SIZE *written)
{PLAIN_IO *plain=(PLAIN_IO*)context;PAPACC_SIZE amount=size;
 if(amount>sizeof(plain->bytes)-plain->size)amount=sizeof(plain->bytes)-plain->size;
 memcpy(plain->bytes+plain->size,bytes,amount);plain->size+=amount;
 *written=amount;return PAPACC_RESULT_OK;}
static PAPACC_RESULT plain_half(void *context){(void)context;return PAPACC_RESULT_OK;}
static void plain_close(void *context){++((PLAIN_IO*)context)->close_count;}

static int server_read_exact(pst_connection *connection,PAPACC_U8 expected)
{
    int index;for(index=0;index<10000;++index){PST_IO_RESULT io;
        PAPACC_U8 value=0U;memset(&io,0,sizeof(io));
        if(pst_connection_read(connection,&value,1U,&io)!=PST_RESULT_OK||
           io.operation==PST_OPERATION_FAILED)return 0;
        if(io.bytes_transferred==1U)return value==expected;
    }return 0;
}

static int server_write_byte(pst_connection *connection,PAPACC_U8 value)
{
    int index;for(index=0;index<10000;++index){PST_IO_RESULT io;
        memset(&io,0,sizeof(io));
        if(pst_connection_write(connection,&value,1U,&io)!=PST_RESULT_OK||
           io.operation==PST_OPERATION_FAILED)return 0;
        if(io.bytes_transferred==1U)return 1;
    }return 0;
}

static void release_pair(TLS_PAIR *pair_value)
{
    papacc_outbound_tls_abort(&pair_value->client);
    if(pair_value->server!=NULL)pst_connection_release(pair_value->server);
    pair_value->server=NULL;pair_value->server_socket_alias=INVALID_SOCKET;
}

int main(void)
{
    WSADATA winsock;
    PST_RUNTIME_OPTIONS options;
    PAPACC_TEST_SECURITY_FIXTURE fixture=PAPACC_TEST_SECURITY_FIXTURE_INITIALIZER;
    pst_runtime *runtime=NULL;
    TLS_PAIR a,b,c,d;
    PLAIN_IO plain={0};
    PAPACC_NETWORK_STREAM records[4],*sa=NULL,*sb=NULL,*sc=NULL,*sd=NULL,*chosen;
    PAPACC_U8 tx[512],rx[512],busy[120],value;
    PAPACC_NETWORK_STREAM_MANAGER manager=PAPACC_NETWORK_STREAM_MANAGER_INITIALIZER;
    PAPACC_NETWORK_STREAM_IO aio={&a,tls_read,tls_write,tls_half,tls_shutdown,tls_close};
    PAPACC_NETWORK_STREAM_IO bio={&b,tls_read,tls_write,tls_half,tls_shutdown,tls_close};
    PAPACC_NETWORK_STREAM_IO cio={&c,tls_read,tls_write,tls_half,tls_shutdown,tls_close};
    PAPACC_NETWORK_STREAM_IO dio={&d,tls_read,tls_write,tls_half,tls_shutdown,tls_close};
    PAPACC_NETWORK_STREAM_IO pio={&plain,plain_read,plain_write,plain_half,NULL,plain_close};
    PAPACC_SIZE index,size;
    const PAPACC_U8 *peek;
    int result=0;
    memset(&a,0,sizeof(a));memset(&b,0,sizeof(b));
    memset(&c,0,sizeof(c));memset(&d,0,sizeof(d));
    CHECK(WSAStartup(MAKEWORD(2,2),&winsock)==0,1);
    CHECK(pst_win32_register_builtin_providers()==PST_RESULT_OK,2);
    memset(&options,0,sizeof(options));options.struct_size=sizeof(options);
    options.api_version=PST_API_VERSION;
    CHECK(pst_runtime_create(&options,&runtime)==PST_RESULT_OK,3);
    CHECK(papacc_test_security_fixture_init(&fixture),4);
    CHECK(establish_pair(&a,runtime,fixture.trust,fixture.server.credentials),5);
    CHECK(establish_pair(&b,runtime,fixture.trust,fixture.server.credentials),6);
    CHECK(establish_pair(&c,runtime,fixture.trust,fixture.server.credentials),7);
    CHECK(establish_pair(&d,runtime,fixture.trust,fixture.server.credentials),8);
    CHECK(papacc_network_stream_manager_init(&manager,records,tx,rx,4U,3U,
        128U)==PAPACC_RESULT_OK,9);
    CHECK(papacc_network_stream_reserve(&manager,1U,11U,
        PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED,&sa)==PAPACC_RESULT_OK&&
        papacc_network_stream_publish(&manager,sa,&aio)==PAPACC_RESULT_OK,10);
    CHECK(papacc_network_stream_reserve(&manager,1U,12U,
        PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED,&sb)==PAPACC_RESULT_OK&&
        papacc_network_stream_publish(&manager,sb,&bio)==PAPACC_RESULT_OK,11);
    CHECK(papacc_network_stream_reserve(&manager,2U,21U,
        PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED,&sc)==PAPACC_RESULT_OK&&
        papacc_network_stream_publish(&manager,sc,&cio)==PAPACC_RESULT_OK,12);
    value=0xA1U;CHECK(papacc_network_stream_enqueue_client(sa,&value,1U)==PAPACC_RESULT_OK,13);
    value=0xB2U;CHECK(papacc_network_stream_enqueue_client(sb,&value,1U)==PAPACC_RESULT_OK,14);
    value=0xC3U;CHECK(papacc_network_stream_enqueue_client(sc,&value,1U)==PAPACC_RESULT_OK,15);
    for(index=0U;index<12U;++index)CHECK(papacc_network_stream_progress_one_at(
        &manager,16U,(PAPACC_U64)index,&chosen)==PAPACC_RESULT_OK,16);
    CHECK(server_read_exact(a.server,0xA1U)&&server_read_exact(b.server,0xB2U)&&
        server_read_exact(c.server,0xC3U),17);
    memset(busy,0xA5,sizeof(busy));value=0xB6U;
    CHECK(papacc_network_stream_enqueue_client(sa,busy,sizeof(busy))==PAPACC_RESULT_OK&&
        papacc_network_stream_enqueue_client(sb,&value,1U)==PAPACC_RESULT_OK,18);
    for(index=0U;index<8U;++index)CHECK(papacc_network_stream_progress_one_at(
        &manager,16U,12U+(PAPACC_U64)index,&chosen)==PAPACC_RESULT_OK,19);
    CHECK(sb->to_remote_size==0U&&sa->to_remote_size>0U,20);
    for(index=0U;index<40U&&sa->to_remote_size>0U;++index)
        CHECK(papacc_network_stream_progress_one_at(&manager,16U,
            16U+(PAPACC_U64)index,&chosen)==PAPACC_RESULT_OK,20);
    CHECK(sa->to_remote_size==0U,20);
    CHECK(server_write_byte(a.server,0x1AU)&&server_write_byte(b.server,0x2BU)&&
        server_write_byte(c.server,0x3CU),21);
    for(index=0U;index<12U;++index)CHECK(papacc_network_stream_progress_one_at(
        &manager,16U,20U+(PAPACC_U64)index,&chosen)==PAPACC_RESULT_OK,19);
    CHECK(papacc_network_stream_peek_client(sa,&peek,&size)==PAPACC_RESULT_OK&&
        size==1U&&peek[0]==0x1AU,20);
    CHECK(papacc_network_stream_peek_client(sb,&peek,&size)==PAPACC_RESULT_OK&&
        size==1U&&peek[0]==0x2BU,21);
    CHECK(papacc_network_stream_peek_client(sc,&peek,&size)==PAPACC_RESULT_OK&&
        size==1U&&peek[0]==0x3CU,22);
    /* A raw peer EOF is terminal only for A; B and the other Session survive. */
    CHECK(shutdown(a.server_socket_alias,SD_BOTH)==0,23);
    for(index=0U;index<10000U&&
        papacc_network_stream_find_channel(&manager,11U)!=NULL;++index){
        PAPACC_RESULT progress=papacc_network_stream_progress_one_at(
            &manager,16U,40U+(PAPACC_U64)index,&chosen);
        CHECK(progress==PAPACC_RESULT_OK||
            progress==PAPACC_RESULT_INTERNAL_ERROR,24);
    }
    CHECK(a.close_count==1&&manager.count==2U&&
        papacc_network_stream_find_channel(&manager,12U)==sb&&
        papacc_network_stream_find_channel(&manager,21U)==sc,25);
    release_pair(&a);
    CHECK(establish_pair(&a,runtime,fixture.trust,fixture.server.credentials),26);
    CHECK(papacc_network_stream_reserve(&manager,1U,11U,
        PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED,&sa)==PAPACC_RESULT_OK&&
        papacc_network_stream_publish(&manager,sa,&aio)==PAPACC_RESULT_OK,27);
    CHECK(papacc_network_stream_close_channel(&manager,11U)==PAPACC_RESULT_OK&&
        a.close_count==1&&manager.count==2U&&
        papacc_network_stream_find_channel(&manager,12U)==sb,28);
    CHECK(papacc_network_stream_close_session(&manager,1U)==PAPACC_RESULT_OK&&
        b.close_count==1&&manager.count==1U&&
        papacc_network_stream_find_channel(&manager,21U)==sc,24);
    CHECK(papacc_network_stream_reserve(&manager,3U,31U,
        PAPACC_NETWORK_TRANSPORT_TCP_PLAIN,&sd)==PAPACC_RESULT_OK&&
        papacc_network_stream_publish(&manager,sd,&pio)==PAPACC_RESULT_OK,25);
    value=0xD4U;CHECK(papacc_network_stream_enqueue_client(sd,&value,1U)==PAPACC_RESULT_OK,26);
    value=0xC4U;CHECK(papacc_network_stream_enqueue_client(sc,&value,1U)==PAPACC_RESULT_OK,27);
    for(index=0U;index<6U;++index)CHECK(papacc_network_stream_progress_one_at(
        &manager,16U,40U+(PAPACC_U64)index,&chosen)==PAPACC_RESULT_OK,28);
    CHECK(plain.size==1U&&plain.bytes[0]==0xD4U&&server_read_exact(c.server,0xC4U),29);
    CHECK(papacc_network_stream_request_close(sc,1000U)==PAPACC_RESULT_OK,31);
    {
        pst_u32 operation=PST_OPERATION_NEED_READ;PST_RESULT source=PST_RESULT_OK;
        for(index=0U;index<10000U&&
            papacc_network_stream_find_channel(&manager,21U)!=NULL;++index){
            CHECK(papacc_network_stream_progress_one_at(&manager,16U,
                100U+(PAPACC_U64)index,&chosen)==PAPACC_RESULT_OK,32);
            if(operation!=PST_OPERATION_COMPLETE&&operation!=PST_OPERATION_FAILED)
                CHECK(pst_connection_shutdown(c.server,&operation,&source)==PST_RESULT_OK,33);
        }
        CHECK(manager.count==2U&&c.shutdown_count>0&&c.close_count==0&&
            operation==PST_OPERATION_COMPLETE&&
            papacc_network_stream_find_channel(&manager,21U)==sc&&
            sc->remote_half_closed==PAPACC_TRUE&&
            sc->remote_half_close_signalled==PAPACC_TRUE&&
            papacc_network_stream_find_channel(&manager,31U)==sd,34);
        CHECK(papacc_network_stream_close_channel(&manager,21U)==
            PAPACC_RESULT_OK&&c.close_count==1&&manager.count==1U,34);
    }
    CHECK(papacc_network_stream_close_channel(&manager,31U)==PAPACC_RESULT_OK&&
        plain.close_count==1&&manager.count==0U,35);
    CHECK(papacc_network_stream_reserve(&manager,4U,41U,
        PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED,&sd)==PAPACC_RESULT_OK&&
        sd==records&&papacc_network_stream_publish(&manager,sd,&dio)==PAPACC_RESULT_OK,36);
    value=0xE5U;CHECK(papacc_network_stream_enqueue_client(sd,&value,1U)==PAPACC_RESULT_OK,36);
    for(index=0U;index<4U;++index)CHECK(papacc_network_stream_progress_one_at(
        &manager,16U,200U+(PAPACC_U64)index,&chosen)==PAPACC_RESULT_OK,37);
    CHECK(server_read_exact(d.server,0xE5U),38);
    papacc_network_stream_manager_shutdown(&manager);
    CHECK(d.shutdown_count==1&&d.close_count==1,39);
cleanup:
    if(manager.initialized==PAPACC_TRUE)papacc_network_stream_manager_shutdown(&manager);
    release_pair(&d);release_pair(&c);release_pair(&b);release_pair(&a);
    papacc_test_security_fixture_release(&fixture);
    if(runtime!=NULL)pst_runtime_release(runtime);
    WSACleanup();return result;
}

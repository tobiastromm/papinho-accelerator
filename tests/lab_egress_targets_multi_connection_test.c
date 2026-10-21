#include "security_test_support.h"
#include "pst_provider_bootstrap_win32.h"
#include "papinho_secure_transport_win32.h"

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>

static const char request[] =
    "GET / HTTP/1.1\r\nHost: papacc-test-server\r\nConnection: close\r\n\r\n";

static int file_exists(const char *path)
{
    DWORD attributes=GetFileAttributesA(path);
    return attributes!=INVALID_FILE_ATTRIBUTES&&
        (attributes&FILE_ATTRIBUTE_DIRECTORY)==0U;
}

static unsigned short reserve_port(void)
{
    SOCKET value=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    struct sockaddr_in address;int size=sizeof(address);unsigned short port=0U;
    if(value==INVALID_SOCKET)return 0U;memset(&address,0,sizeof(address));
    address.sin_family=AF_INET;address.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
    if(bind(value,(const struct sockaddr *)&address,sizeof(address))==0&&
       getsockname(value,(struct sockaddr *)&address,&size)==0)
        port=ntohs(address.sin_port);
    closesocket(value);return port;
}

static SOCKET connect_loopback(unsigned short port)
{
    SOCKET value=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);struct sockaddr_in a;
    if(value==INVALID_SOCKET)return INVALID_SOCKET;memset(&a,0,sizeof(a));
    a.sin_family=AF_INET;a.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
    a.sin_port=htons(port);
    if(connect(value,(const struct sockaddr *)&a,sizeof(a))==SOCKET_ERROR){
        closesocket(value);return INVALID_SOCKET;}
    return value;
}

static int contains(const char *bytes,size_t size,const char *needle)
{
    size_t n=strlen(needle),i;if(n>size)return 0;
    for(i=0U;i+n<=size;++i)if(memcmp(bytes+i,needle,n)==0)return 1;
    return 0;
}

static int plain_request(unsigned short port)
{
    SOCKET s=connect_loopback(port);char response[4096];size_t used=0U;int n;
    if(s==INVALID_SOCKET)return 0;
    if(send(s,request,(int)(sizeof(request)-1U),0)!=(int)(sizeof(request)-1U)){
        closesocket(s);return 0;}
    while(used<sizeof(response)&&(n=recv(s,response+used,
            (int)(sizeof(response)-used),0))>0)used+=(size_t)n;
    closesocket(s);return contains(response,used,"HTTP/1.1 200 OK")&&
        contains(response,used,"papacc-tcp-plain-ok\n");
}

static void client_config(PST_CONNECTION_CONFIG *c,pst_trust *trust)
{
    static const char name[]="papacc-test-server";memset(c,0,sizeof(*c));
    c->struct_size=sizeof(*c);c->api_version=PST_API_VERSION;
    c->role=PST_CONNECTION_ROLE_CLIENT;
    c->provider_selection.struct_size=sizeof(c->provider_selection);
    c->provider_selection.api_version=PST_API_VERSION;
    c->provider_selection.mode=PST_BACKEND_SELECTION_EXACT;
    c->provider_selection.exact_provider_id="openssl";
    c->provider_selection.required_capabilities=PST_CAP_TLS_1_3|
        PST_CAP_ROLE_CLIENT|PST_CAP_PEER_CERT_AUTH|PST_CAP_CUSTOM_TRUST|
        PST_CAP_PEER_NAME_VERIFY|PST_CAP_PEER_INFO|PST_CAP_NONBLOCKING|PST_CAP_BACKEND_WAIT|
        PST_CAP_GRACEFUL_SHUTDOWN;
    c->local_identity.struct_size=sizeof(c->local_identity);
    c->local_identity.api_version=PST_API_VERSION;
    c->peer_authentication.struct_size=sizeof(c->peer_authentication);
    c->peer_authentication.api_version=PST_API_VERSION;
    c->peer_authentication.certificate_mode=PST_PEER_CERTIFICATE_REQUIRED;
    c->peer_authentication.trust=trust;
    c->peer_authentication.expected_peer_name=name;
    c->peer_authentication.expected_peer_name_size=sizeof(name)-1U;
    c->tls.struct_size=sizeof(c->tls);c->tls.api_version=PST_API_VERSION;
    c->tls.minimum_version=PST_TLS_VERSION_1_3;
    c->tls.maximum_version=PST_TLS_VERSION_1_3;
    c->tls.resumption=PST_FEATURE_DISABLED;c->tls.early_data=PST_FEATURE_DISABLED;
    c->tls.require_graceful_shutdown=PST_FEATURE_REQUIRED;
    c->alpn.struct_size=sizeof(c->alpn);c->alpn.api_version=PST_API_VERSION;
    c->alpn.mode=PST_FEATURE_DISABLED;c->server_name_indication_mode=PST_SNI_MODE_COMPAT;
}

static int tls_request(pst_runtime *runtime,pst_trust *trust,unsigned short port)
{
    SOCKET socket_value=connect_loopback(port);pst_transport *transport=NULL;
    pst_connection *connection=NULL;PST_CONNECTION_CONFIG config;
    pst_u32 accepted=0U,operation=PST_OPERATION_NEED_READ;PST_RESULT source;
    char response[4096];size_t used=0U,offset=0U;int i,ok=0;u_long nonblocking=1UL;
    if(socket_value==INVALID_SOCKET)return 0;client_config(&config,trust);
    if(ioctlsocket(socket_value,FIONBIO,&nonblocking)==SOCKET_ERROR||
       pst_connection_create(runtime,&config,&connection)!=PST_RESULT_OK||
       pst_win32_socket_transport_create((pst_size)socket_value,&transport)!=PST_RESULT_OK||
       pst_connection_attach(connection,transport,PST_OWNERSHIP_TRANSFERRED,
            &accepted)!=PST_RESULT_OK||accepted==0U)goto done;
    socket_value=INVALID_SOCKET;transport=NULL;
    for(i=0;i<5000&&operation!=PST_OPERATION_COMPLETE;++i){PST_WAIT_RESULT wait;
        if(pst_connection_handshake(connection,&operation,&source)!=PST_RESULT_OK||
           operation==PST_OPERATION_FAILED||operation==PST_OPERATION_CLOSED)goto done;
        if(operation!=PST_OPERATION_COMPLETE&&
           pst_connection_wait(connection,1000U,&wait)!=PST_RESULT_OK)goto done;}
    if(operation!=PST_OPERATION_COMPLETE)goto done;
    while(offset<sizeof(request)-1U){PST_IO_RESULT io;PST_WAIT_RESULT wait;
        if(pst_connection_write(connection,(const pst_u8 *)request+offset,
             sizeof(request)-1U-offset,&io)!=PST_RESULT_OK||
           io.operation==PST_OPERATION_FAILED||io.operation==PST_OPERATION_CLOSED)goto done;
        offset+=io.bytes_transferred;if(offset<sizeof(request)-1U&&
           pst_connection_wait(connection,1000U,&wait)!=PST_RESULT_OK)goto done;}
    while(used<sizeof(response)){PST_IO_RESULT io;PST_WAIT_RESULT wait;
        if(pst_connection_read(connection,(pst_u8 *)response+used,
             sizeof(response)-used,&io)!=PST_RESULT_OK||io.operation==PST_OPERATION_FAILED)
            goto done;
        used+=io.bytes_transferred;
        if(contains(response,used,"papacc-mode-b-ok\n"))break;
        if(io.operation==PST_OPERATION_CLOSED)break;
        if(pst_connection_wait(connection,1000U,&wait)!=PST_RESULT_OK)goto done;}
    ok=contains(response,used,"HTTP/1.1 200 OK")&&
        contains(response,used,"papacc-mode-b-ok\n");
    operation=PST_OPERATION_NEED_READ;
    for(i=0;i<5000&&operation!=PST_OPERATION_COMPLETE&&
        operation!=PST_OPERATION_CLOSED;++i){PST_WAIT_RESULT wait;
        if(pst_connection_shutdown(connection,&operation,&source)!=PST_RESULT_OK||
           operation==PST_OPERATION_FAILED)break;
        if(operation!=PST_OPERATION_COMPLETE&&operation!=PST_OPERATION_CLOSED)
            (void)pst_connection_wait(connection,1000U,&wait);}
done:
    if(connection!=NULL)pst_connection_release(connection);
    if(transport!=NULL)pst_transport_release(transport);
    if(socket_value!=INVALID_SOCKET)closesocket(socket_value);return ok;
}

int main(int argc,char **argv)
{
    WSADATA winsock;PAPACC_TEST_SECURITY_FIXTURE fixture=PAPACC_TEST_SECURITY_FIXTURE_INITIALIZER;
    PST_RUNTIME_OPTIONS options;pst_runtime *runtime=NULL;PROCESS_INFORMATION pi;
    STARTUPINFOA si;HANDLE stop=NULL;char command[4096],ready[MAX_PATH*2],temp[MAX_PATH];
    const char *stop_name="Local\\PapaccLabTargetsMultiStop";unsigned short hp,tp;
    DWORD exit_code=1U,attempt;int i,result=1,stage=0;
    if(argc!=2||WSAStartup(MAKEWORD(2,2),&winsock)!=0)return 2;
    hp=reserve_port();tp=reserve_port();if(hp==0U||tp==0U||hp==tp){stage=1;goto done;}
    if(GetTempPathA(sizeof(temp),temp)==0U||sprintf_s(ready,sizeof(ready),
       "%spapacc-lab-targets-multi.ready",temp)<0){stage=2;goto done;}DeleteFileA(ready);
    stop=CreateEventA(NULL,TRUE,FALSE,stop_name);if(stop==NULL){stage=3;goto done;}
    memset(&si,0,sizeof(si));memset(&pi,0,sizeof(pi));si.cb=sizeof(si);
    if(sprintf_s(command,sizeof(command),"\"%s\" --http-port %u --https-port %u "
       "--ready-file \"%s\" --stop-event \"%s\"",argv[1],(unsigned)hp,
       (unsigned)tp,ready,stop_name)<0||!CreateProcessA(NULL,command,NULL,NULL,FALSE,
       CREATE_NO_WINDOW,NULL,NULL,&si,&pi)){stage=4;goto done;}
    for(attempt=0U;attempt<200U&&!file_exists(ready);++attempt)Sleep(25U);
    if(!file_exists(ready)||papacc_pst_provider_bootstrap_win32(NULL)!=PAPACC_RESULT_OK||
       !papacc_test_security_fixture_init(&fixture)){stage=5;goto stop_process;}
    memset(&options,0,sizeof(options));options.struct_size=sizeof(options);
    options.api_version=PST_API_VERSION;if(pst_runtime_create(&options,&runtime)!=PST_RESULT_OK){stage=6;goto stop_process;}
    for(i=0;i<3;++i)if(!plain_request(hp)){stage=10+i;goto stop_process;}
    for(i=0;i<3;++i)if(!tls_request(runtime,fixture.trust,tp)){stage=20+i;goto stop_process;}
    if(!SetEvent(stop)||WaitForSingleObject(pi.hProcess,15000U)!=WAIT_OBJECT_0||
       !GetExitCodeProcess(pi.hProcess,&exit_code)||exit_code!=0U){stage=30;goto stop_process;}
    result=0;goto done_process;
stop_process:
    SetEvent(stop);if(pi.hProcess!=NULL&&WaitForSingleObject(pi.hProcess,15000U)!=WAIT_OBJECT_0)
        TerminateProcess(pi.hProcess,99U);
done_process:
    if(pi.hThread!=NULL)CloseHandle(pi.hThread);if(pi.hProcess!=NULL)CloseHandle(pi.hProcess);
done:
    if(runtime!=NULL)pst_runtime_release(runtime);papacc_test_security_fixture_release(&fixture);
    if(stop!=NULL)CloseHandle(stop);DeleteFileA(ready);WSACleanup();
    if(result==0)puts("STANDALONE_HTTP_THREE_CONNECTIONS=PASS\nSTANDALONE_HTTPS_THREE_CONNECTIONS=PASS\nLAB_TARGETS_CLEAN_STOP=PASS");
    else fprintf(stderr,"LAB_TARGET_MULTI_FAILURE stage=%d process_exit=%lu\n",
        stage,(unsigned long)exit_code);
    return result;
}

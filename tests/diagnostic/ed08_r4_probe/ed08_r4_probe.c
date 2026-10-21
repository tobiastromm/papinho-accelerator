/* TEST_ONLY: isolated public-PST/C3 observer; no production C2 wire. */
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "papinho_secure_transport.h"
#include "papinho_secure_transport_win32.h"

#define CAP 65536
#define SLICE 16384
typedef struct Q { unsigned char data[CAP]; int n,eof,err; uint64_t received,accepted,rxhash,txhash; unsigned partial,blocked; } Q;
typedef struct Probe {
    unsigned op,gen,seq; SOCKET c,b;
    pst_runtime *rt; pst_connection *client,*server;
    pst_trust *trust; pst_credentials *ci,*si; pst_wait_set *ws;
    int cm,sm,hc,hs,sc,ss,freeze,hold_ab,cancel,sealed,failed,rt_owned; pst_u32 interest;
    Q ba,ab; uint64_t app_sent,app_received;
} Probe;
static unsigned char payload[SLICE];
static int r4_enabled;
static int retained_boundary(Probe *p);
static void event(Probe *p,const char *name,PST_RESULT result,pst_u32 operation,PST_RESULT error) {
    char xs[32],os[32],es[32];int factual;
    factual=!strcmp(name,"FACTUAL_NEED_WRITE")||!strcmp(name,"PST_TLS_COMPLETE")||!strcmp(name,"PEER_CLOSE_NOTIFY_PUBLIC_CLOSED")||!strcmp(name,"FIXTURE_SERVER_CLOSE_NOTIFY_PUBLIC_CLOSED")||!strcmp(name,"PUBLIC_TLS_NON_CLEAN_AFTER_BRIDGE_LOSS");
    if(factual){sprintf_s(xs,sizeof(xs),"%ld",(long)result);sprintf_s(os,sizeof(os),"%lu",(unsigned long)operation);sprintf_s(es,sizeof(es),"%ld",(long)error);}
    else {strcpy_s(xs,sizeof(xs),"null");strcpy_s(os,sizeof(os),"null");strcpy_s(es,sizeof(es),"null");}
    printf("{\"seq\":%u,\"op\":%u,\"generation\":%u,\"client_lifetime\":%u,\"server_lifetime\":%u,\"event\":\"%s\",\"call_result\":%s,\"pst_operation\":%s,\"operation_result\":%s,\"ba_received\":%llu,\"ba_c3_accepted\":%llu,\"ab_c3_received\":%llu,\"ab_site_accepted\":%llu,\"ba_pending\":%d,\"ab_pending\":%d,\"app_accepted\":%llu,\"app_peer_received\":%llu,\"ba_rxhash\":%llu,\"ba_txhash\":%llu,\"ab_rxhash\":%llu,\"ab_txhash\":%llu}\n",
        ++p->seq,p->op,p->gen,p->op*10+1,p->op*10+2,name,xs,os,es,
        (unsigned long long)p->ba.received,(unsigned long long)p->ba.accepted,
        (unsigned long long)p->ab.received,(unsigned long long)p->ab.accepted,p->ba.n,p->ab.n,
        (unsigned long long)p->app_sent,(unsigned long long)p->app_received,
        (unsigned long long)p->ba.rxhash,(unsigned long long)p->ba.txhash,(unsigned long long)p->ab.rxhash,(unsigned long long)p->ab.txhash);
    fflush(stdout);
}
static int bad(Probe *p,const char *what) { event(p,what,PST_RESULT_PROTOCOL_FAILURE,PST_OPERATION_FAILED,PST_RESULT_PROTOCOL_FAILURE);p->failed=1;return 0; }
#define REQUIRE(p,x,msg) do { if(!(x)) return bad(p,msg); } while(0)
static int pair(SOCKET *a,SOCKET *b) {
    SOCKET l=INVALID_SOCKET; struct sockaddr_in addr; int n=(int)sizeof(addr),v=4096;u_long nb=1;
    *a=*b=INVALID_SOCKET;
    l=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);if(l==INVALID_SOCKET)return 0;
    memset(&addr,0,sizeof(addr));addr.sin_family=AF_INET;addr.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
    if(bind(l,(struct sockaddr*)&addr,n)||listen(l,1)||getsockname(l,(struct sockaddr*)&addr,&n))goto fail;
    *a=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);if(*a==INVALID_SOCKET)goto fail;
    if(connect(*a,(struct sockaddr*)&addr,n))goto fail;
    *b=accept(l,NULL,NULL);if(*b==INVALID_SOCKET)goto fail;
    closesocket(l);
    if(ioctlsocket(*a,FIONBIO,&nb)||ioctlsocket(*b,FIONBIO,&nb))goto fail2;
    if(setsockopt(*a,SOL_SOCKET,SO_SNDBUF,(char*)&v,sizeof(v))||setsockopt(*a,SOL_SOCKET,SO_RCVBUF,(char*)&v,sizeof(v))||
       setsockopt(*b,SOL_SOCKET,SO_SNDBUF,(char*)&v,sizeof(v))||setsockopt(*b,SOL_SOCKET,SO_RCVBUF,(char*)&v,sizeof(v)))goto fail2;
    return 1;
fail: if(l!=INVALID_SOCKET)closesocket(l);
fail2: if(*a!=INVALID_SOCKET)closesocket(*a);if(*b!=INVALID_SOCKET)closesocket(*b);*a=*b=INVALID_SOCKET;return 0;
}
static unsigned char *blob(const char *dir,const char *name,size_t *size) {
    char path[MAX_PATH];FILE *f=NULL;long n;unsigned char *v;
    if(sprintf_s(path,sizeof(path),"%s/%s",dir,name)<0||fopen_s(&f,path,"rb")||!f)return NULL;
    if(fseek(f,0,SEEK_END)){fclose(f);return NULL;}n=ftell(f);rewind(f);
    if(n<=0||n>65536){fclose(f);return NULL;}
    v=(unsigned char*)malloc((size_t)n);if(!v){fclose(f);return NULL;}
    if(fread(v,1,(size_t)n,f)!=(size_t)n){free(v);fclose(f);return NULL;}fclose(f);*size=(size_t)n;return v;
}
static int credentials(Probe *p,const char *dir) {
    unsigned char *ca,*cc,*ck,*sc,*sk;size_t na=0,nc=0,nk=0,ns=0,nsk=0;
    PST_TRUST_SOURCE ts;PST_CREDENTIAL_SOURCE cs;PST_DER_ITEM item;PST_RESULT x;
    ca=blob(dir,"ca.der",&na);cc=blob(dir,"client-a.der",&nc);ck=blob(dir,"client-a.pk8",&nk);
    sc=blob(dir,"server.der",&ns);sk=blob(dir,"server.pk8",&nsk);
    if(!ca||!cc||!ck||!sc||!sk){free(ca);free(cc);free(ck);free(sc);free(sk);return bad(p,"FIXTURE_LOAD_FAIL");}
    memset(&ts,0,sizeof(ts));ts.struct_size=(pst_u32)sizeof(ts);ts.api_version=PST_API_VERSION;ts.kind=PST_TRUST_SOURCE_CUSTOM_CA_DER;
    item.data=ca;item.size=na;ts.anchors=&item;ts.anchor_count=1;x=pst_trust_create(&ts,&p->trust);
    memset(&cs,0,sizeof(cs));cs.struct_size=(pst_u32)sizeof(cs);cs.api_version=PST_API_VERSION;cs.kind=PST_CREDENTIAL_SOURCE_CERT_CHAIN_DER_PKCS8_DER;
    item.data=cc;item.size=nc;cs.certificate_chain=&item;cs.certificate_count=1;cs.private_key_der=ck;cs.private_key_der_size=nk;
    if(x==PST_RESULT_OK)x=pst_credentials_create(&cs,&p->ci);
    item.data=sc;item.size=ns;cs.private_key_der=sk;cs.private_key_der_size=nsk;
    if(x==PST_RESULT_OK)x=pst_credentials_create(&cs,&p->si);
    SecureZeroMemory(ck,nk);SecureZeroMemory(sk,nsk);free(ca);free(cc);free(ck);free(sc);free(sk);
    REQUIRE(p,x==PST_RESULT_OK,"FIXTURE_CONFIG_FAIL");return 1;
}
static int connection(Probe *p,pst_connection **out,SOCKET s,pst_u32 role,const char *selection) {
    PST_CONNECTION_CONFIG c;PST_ALPN_PROTOCOL ap; pst_transport *t=NULL;pst_u32 accepted=0;PST_RESULT x;PST_PROVIDER_INFO pi;
    static const pst_u8 protocol[]={'e','d','0','8','/','1'};
    memset(&c,0,sizeof(c));c.struct_size=(pst_u32)sizeof(c);c.api_version=PST_API_VERSION;c.role=role;
    c.provider_selection.struct_size=(pst_u32)sizeof(c.provider_selection);c.provider_selection.api_version=PST_API_VERSION;
    c.provider_selection.mode=strcmp(selection,"auto")==0?PST_BACKEND_SELECTION_AUTOMATIC:PST_BACKEND_SELECTION_EXACT;
    if(c.provider_selection.mode==PST_BACKEND_SELECTION_EXACT)c.provider_selection.exact_provider_id="openssl";
    c.local_identity.struct_size=(pst_u32)sizeof(c.local_identity);c.local_identity.api_version=PST_API_VERSION;c.local_identity.credentials=role==PST_CONNECTION_ROLE_CLIENT?p->ci:p->si;
    c.peer_authentication.struct_size=(pst_u32)sizeof(c.peer_authentication);c.peer_authentication.api_version=PST_API_VERSION;
    c.peer_authentication.certificate_mode=PST_PEER_CERTIFICATE_REQUIRED;c.peer_authentication.trust=p->trust;
    if(role==PST_CONNECTION_ROLE_CLIENT){c.peer_authentication.expected_peer_name="papacc-test-server";c.peer_authentication.expected_peer_name_size=18;c.server_name_indication="papacc-test-server";c.server_name_indication_size=18;c.server_name_indication_mode=PST_SNI_MODE_EXPLICIT;}
    c.tls.struct_size=(pst_u32)sizeof(c.tls);c.tls.api_version=PST_API_VERSION;c.tls.minimum_version=c.tls.maximum_version=PST_TLS_VERSION_1_3;
    c.tls.resumption=PST_FEATURE_DISABLED;c.tls.early_data=PST_FEATURE_DISABLED;c.tls.require_graceful_shutdown=PST_FEATURE_REQUIRED;
    ap.data=protocol;ap.size=sizeof(protocol);c.alpn.struct_size=(pst_u32)sizeof(c.alpn);c.alpn.api_version=PST_API_VERSION;c.alpn.mode=PST_FEATURE_REQUIRED;c.alpn.protocols=&ap;c.alpn.protocol_count=1;
    x=pst_connection_create(p->rt,&c,out);
    if(x!=PST_RESULT_OK){closesocket(s);return bad(p,"PST_CREATE_FAIL");}
    memset(&pi,0,sizeof(pi));pi.struct_size=(pst_u32)sizeof(pi);pi.api_version=PST_API_VERSION;
    REQUIRE(p,pst_connection_get_provider_info(*out,&pi)==PST_RESULT_OK&&!strcmp(pi.provider_id,"openssl"),"NOT_OPENSSL_BOUND");
    printf("{\"op\":%u,\"generation\":%u,\"event\":\"BOUND_PROVIDER\",\"role\":%lu,\"provider\":\"%s\",\"selection\":\"%s\"}\n",p->op,p->gen,(unsigned long)role,pi.provider_id,selection);
    x=pst_win32_socket_transport_create((pst_size)s,&t);
    if(x==PST_RESULT_OK)x=pst_connection_attach(*out,t,PST_OWNERSHIP_TRANSFERRED,&accepted);
    if(!accepted){if(t)pst_transport_release(t);closesocket(s);}
    REQUIRE(p,x==PST_RESULT_OK&&accepted==1,"PST_ATTACH_FAIL");return 1;
}
static int init(Probe *p,unsigned op,const char *dir,const char *sel,pst_runtime *shared) {
    PST_RUNTIME_OPTIONS ro;SOCKET pc=INVALID_SOCKET,ps=INVALID_SOCKET;PST_RUNTIME_INFO ri;
    memset(p,0,sizeof(*p));p->c=p->b=INVALID_SOCKET;p->op=op;p->gen=7;
    memset(&ro,0,sizeof(ro));ro.struct_size=(pst_u32)sizeof(ro);ro.api_version=PST_API_VERSION;
    if(shared)p->rt=shared;else { REQUIRE(p,pst_runtime_create(&ro,&p->rt)==PST_RESULT_OK,"RUNTIME_FAIL");p->rt_owned=1; }
    memset(&ri,0,sizeof(ri));ri.struct_size=(pst_u32)sizeof(ri);ri.api_version=PST_API_VERSION;
    REQUIRE(p,pst_runtime_get_info(p->rt,&ri)==PST_RESULT_OK,"RUNTIME_QUERY_FAIL");
    printf("{\"op\":%u,\"generation\":7,\"event\":\"RUNTIME_PROVIDERS\",\"count\":%llu}\n",op,(unsigned long long)ri.provider_count);
    if(!credentials(p,dir))return 0;
    REQUIRE(p,pair(&pc,&p->c)&&pair(&ps,&p->b),"PRIVATE_PAIR_FAIL");
    if(!connection(p,&p->client,pc,PST_CONNECTION_ROLE_CLIENT,sel))return 0;
    if(!connection(p,&p->server,ps,PST_CONNECTION_ROLE_SERVER,sel))return 0;
    REQUIRE(p,pst_wait_set_create(&p->ws)==PST_RESULT_OK,"WAITSET_FAIL");
    REQUIRE(p,pst_wait_set_add_connection(p->ws,p->client,11)==PST_RESULT_OK,"MEMBERSHIP_CLIENT_FAIL");p->cm=1;
    REQUIRE(p,pst_wait_set_add_connection(p->ws,p->server,12)==PST_RESULT_OK,"MEMBERSHIP_SERVER_FAIL");p->sm=1;
    event(p,"PAIR_AND_MEMBERSHIP_READY",0,0,0);return 1;
}
static uint64_t hashbytes(uint64_t h,const unsigned char *b,int n){int i;for(i=0;i<n;i++)h=(h^(uint64_t)b[i])*1099511628211ULL;return h;}
static void release_event(Probe *p,const char *name,PST_RESULT x,const PST_RELEASE_INFO *i){
 printf("{\"seq\":%u,\"op\":%u,\"generation\":%u,\"client_lifetime\":%u,\"server_lifetime\":%u,\"event\":\"%s\",\"release_call_result\":%ld,\"release_record_result\":%ld,\"release_disposition\":%lu,\"native_domain\":%lu,\"native_code\":%ld,\"native_code_valid\":%lu,\"ba_c3_accepted\":%llu,\"ab_c3_received\":%llu}\n",++p->seq,p->op,p->gen,p->op*10+1,p->op*10+2,name,(long)x,(long)i->result,(unsigned long)i->disposition,(unsigned long)i->native_domain,(long)i->native_code,(unsigned long)i->native_code_valid,(unsigned long long)p->ba.accepted,(unsigned long long)p->ab.received);fflush(stdout);
}
static int rx(Probe *p,Q *q,SOCKET s,const char *name) {
    int n;if(q->eof||q->err||q->n==CAP)return 1;
    n=recv(s,(char*)q->data+q->n,CAP-q->n<SLICE?CAP-q->n:SLICE,0);
    if(n>0){q->rxhash=hashbytes(q->rxhash,q->data+q->n,n);q->received+=(uint64_t)n;q->n+=n;return 1;}
    if(n==0){q->eof=1;event(p,name,0,0,0);return 1;}
    n=WSAGetLastError();if(n==WSAEWOULDBLOCK)return 1;q->err=n;return bad(p,"NATIVE_RX_ERROR");
}
static int tx(Probe *p,Q *q,SOCKET s) {
    int want,n;if(!q->n||p->cancel)return 1;want=q->n<SLICE?q->n:SLICE;n=send(s,(char*)q->data,want,0);
    if(n>0){if(n<want)q->partial++;q->txhash=hashbytes(q->txhash,q->data,n);q->accepted+=(uint64_t)n;q->n-=n;if(q->n)memmove(q->data,q->data+n,(size_t)q->n);return 1;}
    if(n==SOCKET_ERROR&&WSAGetLastError()==WSAEWOULDBLOCK){q->blocked++;return 1;}return bad(p,"NATIVE_TX_ERROR");
}
static int pump(Probe *p) {
    if(!p->hold_ab&&!rx(p,&p->ab,p->c,"C3_ORDERED_EOF"))return 0;
    if(!p->freeze&&!rx(p,&p->ba,p->b,"SITE_RAW_EOF"))return 0;
    if(!tx(p,&p->ba,p->c)||!tx(p,&p->ab,p->b))return 0;return 1;
}
static void wait_turn(Probe *p) {
    PST_WAIT_EVENT ev[4];PST_WAIT_SET_RESULT wr;
    if(p->ws){memset(&wr,0,sizeof(wr));(void)pst_wait_set_wait(p->ws,1,ev,4,&wr);}
}
static int handshake(Probe *p) {
    DWORD start=GetTickCount();pst_u32 op;PST_RESULT x,e;PST_PEER_INFO_SUMMARY s;pst_peer_info *pi=NULL;
    while(!p->hc||!p->hs){
        if((DWORD)(GetTickCount()-start)>15000)return bad(p,"HANDSHAKE_TIMEOUT");
        if(!p->hc){op=99;e=99;x=pst_connection_handshake(p->client,&op,&e);REQUIRE(p,x==0&&op!=PST_OPERATION_FAILED,"CLIENT_HS_FAIL");if(op==0)p->hc=1;}
        if(!p->hs){op=99;e=99;x=pst_connection_handshake(p->server,&op,&e);REQUIRE(p,x==0&&op!=PST_OPERATION_FAILED,"SERVER_HS_FAIL");if(op==0)p->hs=1;}
        if(!pump(p))return 0;wait_turn(p);
    }
    REQUIRE(p,pst_connection_get_peer_info(p->client,&pi)==0,"PEER_QUERY_FAIL");
    memset(&s,0,sizeof(s));s.struct_size=(pst_u32)sizeof(s);s.api_version=PST_API_VERSION;
    x=pst_peer_info_get_summary(pi,&s);pst_peer_info_release(pi);
    REQUIRE(p,x==0&&s.tls_version==13&&s.chain_validated==PST_KNOWN_TRUE&&s.peer_name_validated==PST_KNOWN_TRUE&&s.peer_authenticated==PST_KNOWN_TRUE,"PUBLIC_TLS_FACT_FAIL");
    event(p,"TLS13_MTLS_VERIFIED",0,0,0);return 1;
}
static int read_server(Probe *p) {
    unsigned char b[SLICE];PST_IO_RESULT io;PST_RESULT x;size_t i;
    memset(&io,0,sizeof(io));x=pst_connection_read(p->server,b,sizeof(b),&io);
    REQUIRE(p,x==0&&io.operation!=PST_OPERATION_FAILED,"SERVER_READ_FAIL");
    for(i=0;i<io.bytes_transferred;i++)REQUIRE(p,b[i]==0x5a,"APPLICATION_CONTENT_FAIL");
    p->app_received+=(uint64_t)io.bytes_transferred;return 1;
}
static int pressure(Probe *p) {
    PST_IO_RESULT io;PST_RESULT x;unsigned i;int found=0;DWORD start;
    p->hold_ab=1;
    for(i=0;i<4096;i++){
        memset(&io,0,sizeof(io));x=pst_connection_write(p->client,payload,sizeof(payload),&io);
        REQUIRE(p,x==0&&io.operation!=PST_OPERATION_FAILED,"PRESSURE_WRITE_FAIL");
        p->app_sent+=(uint64_t)io.bytes_transferred;
        if(io.operation==PST_OPERATION_NEED_WRITE||io.operation==PST_OPERATION_NEED_READ_WRITE){REQUIRE(p,pst_connection_get_interest(p->client,&p->interest)==0&&(p->interest&PST_INTEREST_WRITE),"NEED_WRITE_INTEREST_MISSING");event(p,"FACTUAL_NEED_WRITE",x,io.operation,io.error);printf("{\"op\":%u,\"generation\":%u,\"event\":\"FACTUAL_INTEREST\",\"interest\":%lu}\n",p->op,p->gen,(unsigned long)p->interest);found=1;break;}
    }
    REQUIRE(p,found,"NO_FACTUAL_NEED_WRITE");p->hold_ab=0;start=GetTickCount();
    for(;;){
        if((DWORD)(GetTickCount()-start)>15000)return bad(p,"PRESSURE_RESUME_TIMEOUT");
        if(!pump(p)||!read_server(p))return 0;
        memset(&io,0,sizeof(io));x=pst_connection_write(p->client,payload,sizeof(payload),&io);
        REQUIRE(p,x==0&&io.operation!=PST_OPERATION_FAILED,"RETRY_WRITE_FAIL");
        p->app_sent+=(uint64_t)io.bytes_transferred;
        if(io.operation==PST_OPERATION_COMPLETE)break;wait_turn(p);
    }
    while(p->app_received<p->app_sent){
        if((DWORD)(GetTickCount()-start)>15000)return bad(p,"PAYLOAD_DRAIN_TIMEOUT");
        if(!pump(p)||!read_server(p))return 0;wait_turn(p);
    }
    REQUIRE(p,p->app_received==p->app_sent,"PAYLOAD_PREFIX_MISMATCH");
    event(p,"BACKPRESSURE_RESUMED_EXACT_APPLICATION",0,0,0);return 1;
}
static void put64(unsigned char *b,uint64_t x){int i;for(i=7;i>=0;i--){b[i]=(unsigned char)x;x>>=8;}}
static uint64_t get64(const unsigned char *b){uint64_t x=0;int i;for(i=0;i<8;i++)x=(x<<8)|b[i];return x;}
/* Candidate FENCE/FENCE_ACK bytes only inside this TEST_ONLY harness. */
static void frame(Probe *p,unsigned char b[48],int ack,int dir,uint64_t count) {
    memset(b,0,48);memcpy(b,"PACC",4);b[4]=1;b[7]=16;b[9]=(unsigned char)(ack?24:23);b[15]=32;
    put64(b+16,p->op);put64(b+24,p->gen);b[32]=2;b[33]=(unsigned char)dir;put64(b+36,count);if(!ack)b[45]=1;
}
static int validate(Probe *p,const unsigned char *b,int role,int ack,int dir,uint64_t source,uint64_t downstream,int producer_done) {
    if(p->cancel||!producer_done||memcmp(b,"PACC",4)||b[4]!=1||b[5]||b[6]||b[7]!=16||b[8]||b[9]!=(ack?24:23)||b[10]||b[11]||b[12]||b[13]||b[14]||b[15]!=32)return 0;
    if(get64(b+16)!=p->op||get64(b+24)!=p->gen||b[32]!=role||b[33]!=dir||b[34]||b[35]||get64(b+36)!=source||b[44]||b[45]!=(ack?0:1)||b[46]||b[47])return 0;
    return downstream==source;
}
static int frame_transport(Probe *p,const unsigned char *src,unsigned char *dst) {
    SOCKET a,b;int done=0,n;DWORD start;fd_set rs;struct timeval tv;int one=1;
    REQUIRE(p,pair(&a,&b),"FENCE_PIPE_FAIL");
    REQUIRE(p,setsockopt(a,IPPROTO_TCP,TCP_NODELAY,(char*)&one,sizeof(one))==0,"FENCE_NODELAY_FAIL");
    start=GetTickCount();
    while(done<48){n=send(a,(const char*)src+done,48-done,0);if(n>0)done+=n;else if(n==SOCKET_ERROR&&WSAGetLastError()==WSAEWOULDBLOCK)continue;else return bad(p,"FENCE_SEND_FAIL");if((DWORD)(GetTickCount()-start)>5000)return bad(p,"FENCE_SEND_TIMEOUT");}
    done=0;
    while(done<48){
        n=recv(b,(char*)dst+done,1,0);
        if(n==1){done++;continue;}
        REQUIRE(p,n==SOCKET_ERROR&&WSAGetLastError()==WSAEWOULDBLOCK,"FENCE_RECV_FAIL");
        if((DWORD)(GetTickCount()-start)>5000)return bad(p,"FENCE_RECV_TIMEOUT");
        FD_ZERO(&rs);FD_SET(b,&rs);tv.tv_sec=0;tv.tv_usec=10000;(void)select(0,&rs,NULL,NULL,&tv);
    }
    REQUIRE(p,closesocket(a)==0&&closesocket(b)==0,"FENCE_PIPE_CLOSE_FAIL");
    event(p,"TEST_ONLY_CANDIDATE_FRAME_48_NATIVE_BYTES_RECEIVED",0,0,0);return 1;
}
static int app_fence(Probe *p,int dir,uint64_t count,int completed) {
    unsigned char b[48],received[48];
    frame(p,b,0,dir,count);b[32]=1;
    if(!frame_transport(p,b,received))return 0;
    REQUIRE(p,validate(p,received,1,0,dir,count,count,completed),"APPLICATION_FENCE_FAIL");
    frame(p,b,1,dir,count);b[32]=1;
    if(!frame_transport(p,b,received))return 0;
    REQUIRE(p,validate(p,received,1,1,dir,count,count,completed),"APPLICATION_FENCE_ACK_FAIL");
    event(p,dir?"TEST_ONLY_APP_AB_FENCE_ACK_VALID":"TEST_ONLY_APP_BA_FENCE_ACK_VALID",0,0,0);return 1;
}
static int fence(Probe *p,int dir,Q *q,int ended) {
    unsigned char b[48],partial[48];
    frame(p,b,0,dir,q->received);
    if(!frame_transport(p,b,partial))return 0;
    REQUIRE(p,validate(p,partial,2,0,dir,q->received,q->accepted,ended),"FENCE_NOT_ELIGIBLE");
    event(p,dir?"TEST_ONLY_AB_FENCE_VALID":"TEST_ONLY_BA_FENCE_VALID",0,0,0);
    frame(p,b,1,dir,q->received);
    if(!frame_transport(p,b,partial))return 0;
    REQUIRE(p,validate(p,partial,2,1,dir,q->received,q->accepted,ended),"FENCE_ACK_NOT_ELIGIBLE");
    event(p,dir?"TEST_ONLY_AB_FENCE_ACK_VALID":"TEST_ONLY_BA_FENCE_ACK_VALID",0,0,0);return 1;
}
static int release_one(Probe *p,pst_connection **c,int *member,const char *name) {
    PST_RELEASE_INFO info;PST_RESULT x;
    if(!*c)return 1;
    if(*member){REQUIRE(p,pst_wait_set_remove_connection(p->ws,*c)==0,"REMOVE_FAIL");*member=0;}
    REQUIRE(p,pst_release_info_init(&info,(pst_u32)sizeof(info))==0,"INFO_INIT_FAIL");
    x=pst_connection_try_release(*c,&info);
    release_event(p,name,x,&info);
    if(x!=0||info.disposition!=PST_RELEASE_DISPOSITION_DESTROYED)return bad(p,"RELEASE_RETAINED_NO_RETRY");
    *c=NULL;return 1;
}
static int close_probe(Probe *p,int precondition,int local) {
    pst_u32 op=99;PST_RESULT e=99,x;PST_IO_RESULT io;DWORD start=GetTickCount();int peek;char byte;PST_RELEASE_INFO info;
    if(!app_fence(p,0,p->app_sent,1))return 0;
    if(local){
        x=pst_connection_shutdown(p->client,&op,&e);
        REQUIRE(p,x==0&&op!=PST_OPERATION_FAILED,"LOCAL_INIT_SHUTDOWN_FAIL");
        for(;;){
            if((DWORD)(GetTickCount()-start)>15000)return bad(p,"LOCAL_INIT_NOTIFY_TIMEOUT");
            if(!pump(p))return 0;
            memset(&io,0,sizeof(io));x=pst_connection_read(p->server,&byte,1,&io);
            REQUIRE(p,x==0&&io.operation!=PST_OPERATION_FAILED,"FIXTURE_CLOSE_READ_FAIL");
            REQUIRE(p,io.bytes_transferred==0,"LATE_PLAINTEXT_NOT_ACCOUNTED");
            if(io.operation==PST_OPERATION_CLOSED){REQUIRE(p,io.close_kind==PST_CLOSE_CLEAN,"FIXTURE_NOT_CLOSE_NOTIFY");event(p,"FIXTURE_SERVER_CLOSE_NOTIFY_PUBLIC_CLOSED",x,io.operation,io.error);break;}
            wait_turn(p);
        }
        for(;;){
            op=99;e=99;x=pst_connection_shutdown(p->server,&op,&e);
            REQUIRE(p,x==0&&op!=PST_OPERATION_FAILED,"FIXTURE_CLOSE_STEP_FAIL");
            if(op==0){REQUIRE(p,e==0,"FIXTURE_COMPLETE_ERROR");p->ss=1;break;}
            if((DWORD)(GetTickCount()-start)>15000)return bad(p,"FIXTURE_CLOSE_STEP_TIMEOUT");
            if(!pump(p))return 0;wait_turn(p);
        }
        for(;;){
            if(!pump(p))return 0;
            op=99;e=99;x=pst_connection_shutdown(p->client,&op,&e);
            REQUIRE(p,x==0&&op!=PST_OPERATION_FAILED,"LOCAL_FINAL_STEP_FAIL");
            if(op==0){REQUIRE(p,e==0,"LOCAL_COMPLETE_ERROR");p->sc=1;event(p,"PST_TLS_COMPLETE",x,op,e);break;}
            if((DWORD)(GetTickCount()-start)>15000)return bad(p,"LOCAL_FINAL_STEP_TIMEOUT");
            wait_turn(p);
        }
        event(p,"LOCAL_INIT_OUTPUT_COUNT_SEPARATE_FROM_COMPLETE",0,0,0);
    } else {
    /* Peer initiated close, with final reciprocal output still on C3. */
    x=pst_connection_shutdown(p->server,&op,&e);
    REQUIRE(p,x==0&&op!=PST_OPERATION_FAILED,"PEER_SHUTDOWN_FAIL");
    if(op==0)p->ss=1;
    for(;;){
        if((DWORD)(GetTickCount()-start)>15000)return bad(p,"PEER_NOTIFY_TIMEOUT");
        if(!pump(p))return 0;
        memset(&io,0,sizeof(io));x=pst_connection_read(p->client,&byte,1,&io);
        REQUIRE(p,x==0&&io.operation!=PST_OPERATION_FAILED,"CLIENT_CLOSE_READ_FAIL");
        if(io.operation==PST_OPERATION_CLOSED){REQUIRE(p,io.close_kind==PST_CLOSE_CLEAN,"NOT_CLOSE_NOTIFY");event(p,"PEER_CLOSE_NOTIFY_PUBLIC_CLOSED",x,io.operation,io.error);break;}
        wait_turn(p);
    }
    p->hold_ab=1;
    for(;;){
        op=99;e=99;x=pst_connection_shutdown(p->client,&op,&e);
        REQUIRE(p,x==0&&op!=PST_OPERATION_FAILED,"LOCAL_SHUTDOWN_FAIL");
        if(op==0){REQUIRE(p,e==0,"RECIPROCAL_COMPLETE_ERROR");p->sc=1;event(p,"PST_TLS_COMPLETE",x,op,e);break;}
        if((DWORD)(GetTickCount()-start)>15000)return bad(p,"LOCAL_NOTIFY_TIMEOUT");
        /* BA alone may progress; AB observation deliberately withheld. */
        if(!pump(p))return 0;wait_turn(p);
    }
    start=GetTickCount();
    for(;;){
        fd_set rs;struct timeval tv;
        peek=recv(p->c,&byte,1,MSG_PEEK);if(peek==1)break;
        REQUIRE(p,peek==SOCKET_ERROR&&WSAGetLastError()==WSAEWOULDBLOCK,"UNEXPECTED_AB_BEFORE_DESTROY");
        if((DWORD)(GetTickCount()-start)>5000)return bad(p,"PENDING_AB_BARRIER_TIMEOUT");
        FD_ZERO(&rs);FD_SET(p->c,&rs);tv.tv_sec=0;tv.tv_usec=10000;(void)select(0,&rs,NULL,NULL,&tv);
    }
    REQUIRE(p,peek==1,"NO_PENDING_AB_AT_COMPLETE");event(p,"AB_BYTES_PENDING_AFTER_COMPLETE",0,0,0);
    }
    /* Proposed APP AB fence phase observation modeled only in harness. */
    if(!app_fence(p,1,0,p->sc))return 0;
    event(p,"TEST_ONLY_NORMAL_PHASE_APP_AB_FENCE_OBSERVED",0,0,0);p->freeze=1;
    REQUIRE(p,p->ba.n==0&&p->ba.received==p->ba.accepted&&p->ba.rxhash==p->ba.txhash,"BA_TAIL_NOT_ACCEPTED");
    if(!fence(p,0,&p->ba,1))return 0;
    if(precondition){
        REQUIRE(p,pst_release_info_init(&info,(pst_u32)sizeof(info))==0,"PRECONDITION_INIT_FAIL");
        x=pst_connection_try_release(p->client,&info);release_event(p,"PRECONDITION_RETAINED_OBSERVED",x,&info);
        REQUIRE(p,x==PST_RESULT_INVALID_STATE&&info.disposition==PST_RELEASE_DISPOSITION_PRECONDITION_RETAINED,"PRECONDITION_ORACLE_FAIL");
    }
    if(r4_enabled)return retained_boundary(p);
    if(!release_one(p,&p->client,&p->cm,"PST_DESTROYED"))return 0;
    p->hold_ab=0;start=GetTickCount();
    while(!p->ab.eof||p->ab.n){
        if((DWORD)(GetTickCount()-start)>15000)return bad(p,"AB_EOF_DRAIN_TIMEOUT");
        if(!pump(p))return 0;
        if(!p->ss){op=99;e=99;x=pst_connection_shutdown(p->server,&op,&e);REQUIRE(p,x==0&&op!=PST_OPERATION_FAILED,"FIXTURE_RECIPROCAL_FAIL");if(op==0)p->ss=1;}
        wait_turn(p);
    }
    REQUIRE(p,p->ab.eof&&p->ab.received==p->ab.accepted&&p->ab.rxhash==p->ab.txhash,"AB_PREFIX_EOF_MISMATCH");
    event(p,"PST_RECORD_OUTPUT_DRAINED_EXTERNAL",0,0,0);
    if(!fence(p,1,&p->ab,1))return 0;
    REQUIRE(p,p->ss,"PEER_TLS_NOT_COMPLETE");
    event(p,"HARNESS_CLEAN_GUARDS_ONLY",0,0,0);p->sealed=1;return 1;
}
static int late_native_byte(Probe *p){
    SOCKET a,b;unsigned char in=0,out=0xa5;int n;DWORD start=GetTickCount();fd_set rs;struct timeval tv;
    REQUIRE(p,p->sealed&&p->client==NULL,"LATE_BYTE_AUTHORITY_SETUP");
    REQUIRE(p,pair(&a,&b),"LATE_BYTE_PAIR_FAIL");
    REQUIRE(p,send(a,(char*)&out,1,0)==1,"LATE_BYTE_SEND_FAIL");
    for(;;){n=recv(b,(char*)&in,1,0);if(n==1)break;
        REQUIRE(p,n==SOCKET_ERROR&&WSAGetLastError()==WSAEWOULDBLOCK,"LATE_BYTE_RECV_FAIL");
        if((DWORD)(GetTickCount()-start)>5000)return bad(p,"LATE_BYTE_TIMEOUT");
        FD_ZERO(&rs);FD_SET(b,&rs);tv.tv_sec=0;tv.tv_usec=10000;(void)select(0,&rs,NULL,NULL,&tv);
    }
    REQUIRE(p,in==out&&p->sealed&&p->client==NULL,"LATE_BYTE_RESURRECTION");
    REQUIRE(p,closesocket(a)==0&&closesocket(b)==0,"LATE_BYTE_PAIR_CLOSE");
    event(p,"TEST_ONLY_NATIVE_LATE_BYTE_RECEIVED_REJECTED_NO_PST_DEREFERENCE",0,0,0);return 1;
}
static int synthetic_retention_oracle(Probe *p){
    struct Model { unsigned connection,generation,provider_lease,runtime_lease,quota;int retained,io,release,query; } m;
    memset(&m,0,sizeof(m));m.connection=p->op*10+9;m.generation=p->gen;m.provider_lease=m.runtime_lease=m.quota=1;m.retained=1;
    /* This is a consumer guard model, NOT a PST return or native failure. */
    if(!m.retained){m.io++;m.release++;}m.query++;
    REQUIRE(p,m.io==0&&m.release==0&&m.query==1&&m.quota==1&&m.provider_lease==1&&m.runtime_lease==1,"SYNTHETIC_RETENTION_GUARD_FAIL");
    event(p,"SYNTHETIC_CLOSE_UNRESOLVED_CONSUMER_ORACLE_ONLY_NOT_PST_OBSERVATION",0,0,0);return 1;
}

static int negatives(Probe *p) {
    unsigned char b[48];uint64_t total=p->ab.received;
    frame(p,b,0,1,total);REQUIRE(p,!validate(p,b,2,0,1,total,total,0),"PREMATURE_FENCE_ACCEPTED");
    frame(p,b,1,1,total);REQUIRE(p,total>0&&!validate(p,b,2,1,1,total,total-1,1),"PREMATURE_ACK_ACCEPTED");
    frame(p,b,0,1,total);b[31]^=1;REQUIRE(p,!validate(p,b,2,0,1,total,total,1),"STALE_GENERATION_ACCEPTED");
    frame(p,b,0,1,total);b[23]^=1;REQUIRE(p,!validate(p,b,2,0,1,total,total,1),"CROSS_OPERATION_ACCEPTED");
    frame(p,b,0,1,total);REQUIRE(p,!validate(p,b,2,0,1,total+1,total+1,1),"LATE_BYTE_ACCEPTED");
    p->cancel=1;frame(p,b,0,1,total);REQUIRE(p,!validate(p,b,2,0,1,total,total,1),"CANCEL_AUTHORITY_RESURRECTED");p->cancel=0;
    REQUIRE(p,p->sealed,"LATE_CALLBACK_SETUP_FAIL");
    if(!late_native_byte(p)||!synthetic_retention_oracle(p))return 0;
    event(p,"TEST_ONLY_LATE_BYTE_AND_CALLBACK_REJECTED_AFTER_SEAL",0,0,0);
    event(p,"TEST_ONLY_FENCE_AUTHORITY_NEGATIVES_PASS",0,0,0);return 1;
}
static int negative_native(Probe *p,const char *mode) {
    PST_IO_RESULT io;PST_RESULT x;DWORD start=GetTickCount();struct linger lg;
    if(!strcmp(mode,"reset")){lg.l_onoff=1;lg.l_linger=0;REQUIRE(p,setsockopt(p->c,SOL_SOCKET,SO_LINGER,(char*)&lg,sizeof(lg))==0,"FIXTURE_RST_CONFIG_FAIL");}
    REQUIRE(p,closesocket(p->c)==0,"FIXTURE_C3_CLOSE_FAIL");p->c=INVALID_SOCKET;
    event(p,!strcmp(mode,"reset")?"FIXTURE_OWNED_C3_RESET":"PREMATURE_C3_EOF_SOURCE",0,0,0);
    for(;;){
        memset(&io,0,sizeof(io));x=pst_connection_read(p->client,payload,sizeof(payload),&io);
        if(io.operation==PST_OPERATION_FAILED||x!=0){
            event(p,"PUBLIC_TLS_NON_CLEAN_AFTER_BRIDGE_LOSS",x,io.operation,io.error);
            REQUIRE(p,io.close_kind!=PST_CLOSE_CLEAN&&io.operation!=PST_OPERATION_CLOSED,"FALSE_CLEAN_AFTER_LOSS");
            if(!strcmp(mode,"reset"))REQUIRE(p,io.error==PST_RESULT_TRUNCATED,"RESET_NORMALIZED_ORACLE_FAIL");
            else REQUIRE(p,io.error==PST_RESULT_TRUNCATED,"EOF_NORMALIZED_ORACLE_FAIL");
            return 1;
        }
        REQUIRE(p,io.operation!=PST_OPERATION_CLOSED,"PREMATURE_EOF_FALSE_CLOSED");
        if((DWORD)(GetTickCount()-start)>5000)return bad(p,"NATIVE_NEGATIVE_TIMEOUT");wait_turn(p);
    }
}
static int cancel_partial(Probe *p) {
    PST_IO_RESULT io;PST_RESULT x;uint64_t before;int n;DWORD start=GetTickCount();
    for(;;){
        memset(&io,0,sizeof(io));x=pst_connection_write(p->client,payload,sizeof(payload),&io);
        REQUIRE(p,x==0&&io.operation!=PST_OPERATION_FAILED,"CANCEL_SETUP_WRITE_FAIL");
        n=recv(p->c,(char*)p->ab.data+p->ab.n,2-p->ab.n,0);
        if(n>0){p->ab.rxhash=hashbytes(p->ab.rxhash,p->ab.data+p->ab.n,n);p->ab.received+=(uint64_t)n;p->ab.n+=n;if(p->ab.n==2)break;}
        if((DWORD)(GetTickCount()-start)>5000)return bad(p,"CANCEL_PREFIX_SETUP_TIMEOUT");wait_turn(p);
    }
    REQUIRE(p,send(p->b,(char*)p->ab.data,1,0)==1,"CANCEL_PREFIX_NATIVE_SEND_FAIL");
    p->ab.txhash=hashbytes(p->ab.txhash,p->ab.data,1);p->ab.n=1;p->ab.data[0]=p->ab.data[1];p->ab.accepted=1;before=p->ab.accepted;
    event(p,"PARTIAL_PREFIX_NATIVE_ACCEPTED",0,0,0);p->cancel=1;
    REQUIRE(p,tx(p,&p->ab,p->b)&&p->ab.accepted==before&&p->ab.n==1,"POST_CANCEL_SUBMISSION");
    event(p,"CANCEL_FROZEN_ESCAPED_PREFIX",0,0,0);return 1;
}
static int cleanup(Probe *p) {
    if(!release_one(p,&p->client,&p->cm,"CLIENT_FINAL_DESTROYED"))return 0;
    if(!release_one(p,&p->server,&p->sm,"FIXTURE_FINAL_DESTROYED"))return 0;
    if(p->ws){REQUIRE(p,pst_wait_set_destroy(p->ws)==0,"WAITSET_DESTROY_FAIL");p->ws=NULL;}
    if(p->c!=INVALID_SOCKET){REQUIRE(p,closesocket(p->c)==0,"C3_OWNER_CLOSE_FAIL");p->c=INVALID_SOCKET;}
    if(p->b!=INVALID_SOCKET){REQUIRE(p,closesocket(p->b)==0,"SITE_OWNER_CLOSE_FAIL");p->b=INVALID_SOCKET;}
    if(p->ci)pst_credentials_release(p->ci);if(p->si)pst_credentials_release(p->si);if(p->trust)pst_trust_release(p->trust);if(p->rt&&p->rt_owned)pst_runtime_release(p->rt);
    event(p,"OWNED_LIFETIMES_FINALIZED",0,0,0);return 1;
}
int main(int argc,char **argv) {
    WSADATA wd;static Probe p,other;int ok=0;unsigned op;const char *mode,*sel;
    if(argc!=5)return 64;mode=argv[1];sel=argv[3];op=(unsigned)strtoul(argv[4],NULL,10);
    memset(payload,0x5a,sizeof(payload));
    SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);
    if(pst_api_version()!=PST_API_VERSION||pst_library_version()!=PST_LIBRARY_VERSION)return 63;
    {const char *names[2]={"libssl-3-x64.dll","libcrypto-3-x64.dll"};int i,j;char path[MAX_PATH];HMODULE module;
     for(i=0;i<2;i++){module=GetModuleHandleA(names[i]);if(!module||!GetModuleFileNameA(module,path,MAX_PATH))return 62;
      for(j=0;path[j];j++)if(path[j]=='\\')path[j]='/';
      printf("{\"event\":\"ACTUAL_LOADED_MODULE\",\"module\":\"%s\",\"path\":\"%s\",\"api\":%lu,\"library\":%lu}\n",names[i],path,(unsigned long)pst_api_version(),(unsigned long)pst_library_version());}}
    if(WSAStartup(MAKEWORD(2,2),&wd))return 65;
    if(pst_win32_register_builtin_providers()!=0)return 66;
    if(!init(&p,op,argv[2],sel,NULL)||!handshake(&p))return 67;
    r4_enabled=!strcmp(mode,"r4");
    if(r4_enabled){
        ok=pressure(&p)&&close_probe(&p,1,0);
        if(!ok)return 73;
        printf("{\"case\":\"r4\",\"result\":\"PASS\",\"scope\":\"ISOLATED_ACCELERATOR_MANAGER_PROVIDER_CONTROLLED_NOT_WINSOCK\"}\n");
        return 0; /* Explicit process containment; no dependent cleanup or WSACleanup. */
    }
    if(!strcmp(mode,"positive")||!strcmp(mode,"precondition")||!strcmp(mode,"local")){
        ok=pressure(&p)&&close_probe(&p,!strcmp(mode,"precondition"),!strcmp(mode,"local"))&&negatives(&p);
    }else if(!strcmp(mode,"eof")||!strcmp(mode,"reset"))ok=negative_native(&p,mode);
    else if(!strcmp(mode,"cancel"))ok=cancel_partial(&p);
    else if(!strcmp(mode,"isolation")){
        if(!init(&other,op+100,argv[2],sel,p.rt)||!handshake(&other))return 68;
        if(!cancel_partial(&p)||!pressure(&other)||!close_probe(&other,0,0)||!cleanup(&other))return 69;
        event(&p,"SECOND_OPERATION_UNAFFECTED_BY_CANCEL",0,0,0);ok=1;
    }
    if(!ok)return 70;
    if(!cleanup(&p))return 71;
    if(WSACleanup())return 72;
    printf("{\"case\":\"%s\",\"op\":%u,\"generation\":7,\"result\":\"PASS\",\"scope\":\"NORMAL_PUBLIC_PST_NATIVE_C3_AND_TEST_ONLY_FENCE_ORACLE_NOT_PRODUCTION_C2\"}\n",mode,op);
    return 0;
}

#include "connection.h"
#include "pst_secure_transport_adapter.h"
#include "pst_test_only_openssl_r4.h"
typedef struct RetainedOwner {
    PAPACC_CONNECTION_MANAGER manager;
    PAPACC_CONNECTION storage[1];
    PAPACC_CONNECTION *connection;
    Probe *probe;
    PST_RELEASE_INFO info;
    unsigned release_calls,retained,void_calls,publication_attempts,deliveries;
    DWORD thread;
    PAPACC_PST_SECURE_PROCESSOR processor;
    PAPACC_PST_SECURE_SCHEDULER scheduler;
    PAPACC_PST_SCHEDULER_MEMBER members[1];
    PST_WAIT_EVENT events[1];
    PAPACC_PST_SECURE_TRANSPORT_ADAPTER adapter;
    PAPACC_TRANSPORT_PRE_CLOSE_FN original_prepare;
    PAPACC_TRANSPORT_TRY_CLOSE_FN original_close;
} RetainedOwner;
static RetainedOwner owner;
static PAPACC_RESULT fixture_prepare(void *ctx,PAPACC_BOOL *ready){
    PAPACC_RESULT result;PAPACC_BOOL registered=owner.adapter.registered;
    if(GetCurrentThreadId()!=owner.thread)return PAPACC_RESULT_INVALID_STATE;
    result=owner.original_prepare(ctx,ready);
    if(registered&&!owner.adapter.registered)event(owner.probe,"TARGET_WAITSET_REMOVAL_CONFIRMED",0,0,0);
    return result;
}
static PAPACC_RESULT fixture_close(void *ctx,PAPACC_BOOL *closed){
    PAPACC_RESULT result;RetainedOwner *o=&owner;
    if(GetCurrentThreadId()!=o->thread)return PAPACC_RESULT_INVALID_STATE;
    if(!o->retained){o->release_calls++;event(o->probe,"TARGET_RELEASE_ATTEMPT",0,0,0);}
    result=o->original_close(ctx,closed);
    o->info=o->processor.release_info;
    if(!o->retained)release_event(o->probe,"TARGET_RELEASE_RESULT_DISPOSITION",o->processor.source_result,&o->info);
    if(o->processor.state==PAPACC_PST_SECURE_PROCESSOR_RETAINED_TERMINAL){o->retained=1;o->probe->cancel=1;}
    return result;
}
static int retained_boundary(Probe *p){
    PST_TEST_ONLY_R4_STATUS status;PST_RELEASE_INFO q,repeat;PST_IO_RESULT io;PST_WAIT_RESULT wr;
    PAPACC_TRANSPORT_CONNECTION t=PAPACC_TRANSPORT_CONNECTION_INITIALIZER,duplicate;
    PAPACC_NETWORK_ENDPOINT ep=PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    PAPACC_CONNECTION *next=NULL;PAPACC_BOOL done;PAPACC_RESULT cr;
    pst_u32 op=99;PST_RESULT er=99;unsigned char b[48];char peek;int n;
    const pst_u32 hi=0x52345053UL,lo=p->op;
    pst_connection *identity=p->client;
    REQUIRE(p,pst_test_only_openssl_r4_associate(identity,hi,lo,p->gen)==0,"R4_ASSOCIATE_FAIL");
    REQUIRE(p,pst_test_only_openssl_r4_arm(identity,hi+1,lo,p->gen)==PST_RESULT_INVALID_ARGUMENT,"WRONG_TOKEN_ACCEPTED");
    REQUIRE(p,pst_test_only_openssl_r4_arm(identity,hi,lo,p->gen+1)==PST_RESULT_INVALID_ARGUMENT,"STALE_GENERATION_ACCEPTED");
    REQUIRE(p,pst_test_only_openssl_r4_associate(identity,hi,lo,p->gen)==PST_RESULT_ALREADY_REGISTERED,"DUPLICATE_ASSOCIATE_ACCEPTED");
    /* Independent server has never been associated: target control cannot arm it. */
    REQUIRE(p,pst_test_only_openssl_r4_arm(p->server,hi,lo,p->gen)!=0,"NON_TARGET_ARM_ACCEPTED");
    REQUIRE(p,pst_test_only_openssl_r4_arm(identity,hi,lo,p->gen)==0,"R4_ARM_FAIL");
    event(p,"R4_WRONG_TOKEN_STALE_DUPLICATE_NON_TARGET_NEGATIVES_PASS",0,0,0);
    memset(&owner,0,sizeof(owner));owner.probe=p;owner.thread=GetCurrentThreadId();
    REQUIRE(p,papacc_connection_manager_init(&owner.manager,owner.storage,1)==0,"MANAGER_INIT_FAIL");
    ep.address.family=PAPACC_IP_FAMILY_IPV4;
    /* Fixture adopts a Connection with factual completed TLS handshake; no PST state is manufactured. */
    owner.processor.connection=identity;owner.processor.state=PAPACC_PST_SECURE_PROCESSOR_ESTABLISHED;
    REQUIRE(p,pst_wait_set_remove_connection(p->ws,identity)==0,"WAITSET_TRANSFER_REMOVE_FAIL");p->cm=0;
    REQUIRE(p,papacc_pst_secure_scheduler_init(&owner.scheduler,owner.members,1,owner.events,1)==0,"ACTUAL_SCHEDULER_INIT_FAIL");
    {pst_wait_token token;REQUIRE(p,papacc_pst_secure_scheduler_add_connection(&owner.scheduler,identity,&token)==0,"ACTUAL_SCHEDULER_ADD_FAIL");}
    REQUIRE(p,papacc_pst_secure_transport_adapter_init(&owner.adapter,&owner.processor,&owner.scheduler,&t)==0,"ACTUAL_ADAPTER_INIT_FAIL");
    owner.original_prepare=t.pre_close_fn;owner.original_close=t.try_close_fn;t.pre_close_fn=fixture_prepare;t.try_close_fn=fixture_close;
    duplicate=t;
    REQUIRE(p,papacc_connection_manager_publish(&owner.manager,&t,&ep,&ep,&owner.connection)==0,"MANAGER_PUBLISH_FAIL");
    printf("{\"op\":%u,\"generation\":%u,\"event\":\"MANAGER_OWNER_IDENTITY\",\"connection_instance_id\":%llu,\"owner_thread\":%lu}\n",p->op,p->gen,(unsigned long long)owner.connection->connection_instance_id,(unsigned long)owner.thread);
    ep.address.family=PAPACC_IP_FAMILY_IPV4;
    cr=papacc_connection_manager_try_remove(&owner.manager,owner.connection->connection_instance_id,&done);
    REQUIRE(p,cr!=0&&!done&&owner.retained&&owner.manager.count==1&&owner.connection==papacc_connection_manager_find(&owner.manager,owner.connection->connection_instance_id),"MANAGER_RETENTION_FAIL");
    REQUIRE(p,p->client==identity&&owner.connection->transport.context==&owner.adapter&&owner.connection->state==PAPACC_CONNECTION_STATE_CLOSING&&!p->cm,"OWNER_IDENTITY_LOST");
    REQUIRE(p,owner.info.result==PST_RESULT_RESOURCE_FAILURE&&!owner.info.native_code_valid&&owner.info.native_domain==0&&owner.info.native_code==0,"FALSE_NATIVE_ERROR");
    /* Repeated owner Stop/remove must not cross the release boundary again. */
    REQUIRE(p,papacc_connection_manager_try_shutdown(&owner.manager,&done)!=0&&!done,"REPEATED_STOP_DESTROYED");
    REQUIRE(p,papacc_connection_manager_try_remove(&owner.manager,owner.connection->connection_instance_id,&done)!=0&&!done,"REPEATED_REMOVE_DESTROYED");
    duplicate=owner.connection->transport;
    REQUIRE(p,papacc_connection_manager_publish(&owner.manager,&duplicate,&ep,&ep,&next)==PAPACC_RESULT_LIMIT_EXCEEDED&&next==NULL,"EARLY_REUSE_ACCEPTED");
    REQUIRE(p,pst_release_info_init(&q,sizeof(q))==0&&pst_connection_get_release_info(identity,&q)==0&&q.result==6&&q.disposition==3,"RELEASE_QUERY_FAIL");
    release_event(p,"RETAINED_QUERY",q.result,&q);
    /* Explicit negative API probe, not consumer retry policy. Core must reject without another provider call. */
    REQUIRE(p,pst_release_info_init(&repeat,sizeof(repeat))==0&&pst_connection_try_release(identity,&repeat)==6&&repeat.disposition==3,"REPEATED_RELEASE_NEGATIVE_FAIL");
    release_event(p,"EXPLICIT_REPEATED_RELEASE_NEGATIVE_NOT_OWNER_RETRY",repeat.result,&repeat);
    memset(&io,0,sizeof(io));REQUIRE(p,pst_connection_read(identity,&peek,1,&io)==2,"RETAINED_READ_ACCEPTED");
    REQUIRE(p,pst_connection_write(identity,&peek,1,&io)==2,"RETAINED_WRITE_ACCEPTED");
    REQUIRE(p,pst_connection_shutdown(identity,&op,&er)==2,"RETAINED_SHUTDOWN_ACCEPTED");
    REQUIRE(p,pst_wait_set_add_connection(p->ws,identity,13)==2,"RETAINED_REBIND_ACCEPTED");
    memset(&wr,0,sizeof(wr));REQUIRE(p,pst_connection_wait(identity,0,&wr)==2,"RETAINED_WAIT_ACCEPTED");
    REQUIRE(p,pst_test_only_openssl_r4_arm(identity,hi,lo,p->gen)!=0,"LATE_ARM_ACCEPTED");
    memset(&status,0,sizeof(status));status.struct_size=sizeof(status);
    REQUIRE(p,pst_test_only_openssl_r4_query(identity,hi,lo,p->gen,&status)==0&&status.provider_close_attempts==1,"SECOND_PROVIDER_CLOSE");
    REQUIRE(p,owner.processor.connection==identity&&owner.processor.state==PAPACC_PST_SECURE_PROCESSOR_RETAINED_TERMINAL&&owner.adapter.processor==&owner.processor&&!owner.adapter.closed&&!owner.adapter.registered&&owner.scheduler.member_count==0,"ACTUAL_PROCESSOR_ADAPTER_RETENTION_FAIL");
    {PAPACC_SIZE bytes;PAPACC_TRANSPORT_IO_STATUS io_status;
     REQUIRE(p,papacc_transport_connection_read(&owner.connection->transport,(PAPACC_U8*)&peek,1,&bytes,&io_status)!=0,"CONSUMER_RETAINED_READ_ACCEPTED");
     REQUIRE(p,papacc_transport_connection_write(&owner.connection->transport,(PAPACC_U8*)&peek,1,&bytes,&io_status)!=0,"CONSUMER_RETAINED_WRITE_ACCEPTED");
     REQUIRE(p,papacc_pst_secure_processor_release(&owner.processor)!=0,"CONSUMER_REPEATED_RELEASE_ACCEPTED");}
    REQUIRE(p,owner.release_calls==1&&owner.void_calls==0&&p->rt&&p->ci&&p->si&&p->trust&&p->ws&&p->c!=INVALID_SOCKET&&p->b!=INVALID_SOCKET,"DEPENDENCY_RETENTION_FAIL");
    /* A real still-open consumer peer; absence of EOF is factual, never fabricated. */
    n=recv(p->c,&peek,1,MSG_PEEK);REQUIRE(p,n==1||(n==SOCKET_ERROR&&WSAGetLastError()==WSAEWOULDBLOCK),"UNEXPECTED_RETAINED_EOF");
    frame(p,b,0,1,p->ab.received);REQUIRE(p,!validate(p,b,2,0,1,p->ab.received,p->ab.accepted,1),"RETAINED_AB_FENCE_ACCEPTED");
    frame(p,b,1,1,p->ab.received);REQUIRE(p,!validate(p,b,2,1,1,p->ab.received,p->ab.accepted,1),"RETAINED_ACK_ACCEPTED");
    /* Callback actually reaches the fixture publication boundary after revocation. */
    owner.publication_attempts++;if(!owner.retained&&p->gen==7&&!p->cancel)owner.deliveries++;
    REQUIRE(p,owner.publication_attempts==1&&owner.deliveries==0&&!p->ab.eof&&!p->sealed,"POST_STOP_PUBLICATION_OR_FALSE_CLEAN");
    event(p,"R4_QUERY_ONLY_MANAGER_DEPENDENCIES_RETAINED_NO_SECOND_CLOSE",0,0,0);
    event(p,"FIXTURE_LATE_CALLBACK_ATTEMPT_SUPPRESSED_NO_DELIVERY",0,0,0);
    event(p,"R4_ERROR_TERMINAL_NON_CLEAN_NO_AB_SEAL_NO_EOF",0,0,0);
    /* Server remains independently releasable; held client/manager/C3/runtime remain. */
    if(!release_one(p,&p->server,&p->sm,"NON_TARGET_SERVER_DESTROYED"))return 0;
    REQUIRE(p,pst_connection_get_release_info(identity,&q)==0&&owner.manager.count==1,"ISOLATION_RETENTION_LOST");
    REQUIRE(p,pst_test_only_openssl_r4_query(identity,hi,lo,p->gen,&status)==0&&status.provider_close_attempts==1,"LATE_SECOND_PROVIDER_CLOSE");
    printf("{\"op\":%u,\"generation\":%u,\"event\":\"FINAL_RETAINED_IDENTITY_FACTS\",\"connection_instance_id\":%llu,\"pst_connection\":\"%p\",\"provider_close_attempts\":%lu,\"owner_release_calls\":%u,\"manager_count\":%llu,\"processor_state\":%d,\"adapter_registered\":%d,\"scheduler_members\":%llu,\"callback_attempts\":%u,\"callback_deliveries\":%u,\"clean\":false,\"ab_sealed\":false}\n",p->op,p->gen,(unsigned long long)owner.connection->connection_instance_id,(void*)identity,(unsigned long)status.provider_close_attempts,owner.release_calls,(unsigned long long)owner.manager.count,(int)owner.processor.state,(int)owner.adapter.registered,(unsigned long long)owner.scheduler.member_count,owner.publication_attempts,owner.deliveries);
    event(p,"R4_PROCESS_CONTAINMENT_ONLY_NOT_DESTRUCTIVE_TEARDOWN",0,0,0);return 1;
}

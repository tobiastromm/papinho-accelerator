#include "server_io_loop_win32.h"
#include "pal_time.h"

#include <string.h>
#include <stdio.h>
#include <ws2tcpip.h>

#define PAPACC_TEST_CAPACITY 4U

typedef struct PAPACC_TEST_EGRESS_CONTEXT {
    PAPACC_NETWORK_ENDPOINT allowed;
} PAPACC_TEST_EGRESS_CONTEXT;

static PAPACC_EGRESS_DECISION papacc_test_egress_endpoint(void *context,
    const PAPACC_NETWORK_ENDPOINT *endpoint, PAPACC_U32 sensitive_classes)
{
    PAPACC_TEST_EGRESS_CONTEXT *test=(PAPACC_TEST_EGRESS_CONTEXT *)context;
    (void)sensitive_classes;
    return papacc_network_endpoint_equal(&test->allowed,endpoint)==PAPACC_TRUE?
        PAPACC_EGRESS_ALLOW:PAPACC_EGRESS_DENY;
}

static PAPACC_RESULT papacc_test_select_profile(void *context,
    const PAPACC_NETWORK_ENDPOINT *endpoint,PAPACC_NETWORK_TRANSPORT_MODE mode,
    PAPACC_U32 *profile_id)
{
    PAPACC_TEST_EGRESS_CONTEXT *test=(PAPACC_TEST_EGRESS_CONTEXT *)context;
    if(profile_id==NULL)return PAPACC_RESULT_INVALID_ARGUMENT;
    *profile_id=0U;
    if(mode!=PAPACC_NETWORK_TRANSPORT_TCP_PLAIN||
        papacc_network_endpoint_equal(&test->allowed,endpoint)!=PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_STATE;
    *profile_id=1U;
    return PAPACC_RESULT_OK;
}

static const PAPACC_U8 papacc_test_open[20] = {
    0x50, 0x41, 0x43, 0x43, 0x01, 0x00, 0x00, 0x10,
    0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04,
    0x00, 0x01, 0x00, 0x00
};
static const PAPACC_U8 papacc_test_accept[20] = {
    0x50, 0x41, 0x43, 0x43, 0x01, 0x00, 0x00, 0x10,
    0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04,
    0x00, 0x01, 0x00, 0x00
};
static const PAPACC_U8 papacc_test_ticket_request[16] = {
    0x50,0x41,0x43,0x43,1,0,0,16,0,3,0,0,0,0,0,0 };
static const PAPACC_U8 papacc_test_data_accept[16] = {
    0x50,0x41,0x43,0x43,1,0,0,16,0,6,0,0,0,0,0,0 };

static SOCKET papacc_test_connect(PAPACC_U16 port)
{
    struct sockaddr_in address;
    DWORD timeout_ms=3000U;
    SOCKET client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client == INVALID_SOCKET) return INVALID_SOCKET;
    (void)setsockopt(client,SOL_SOCKET,SO_RCVTIMEO,
        (const char *)&timeout_ms,(int)sizeof(timeout_ms));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (connect(client, (const struct sockaddr *)&address,
                (int)sizeof(address)) == SOCKET_ERROR) {
        (void)closesocket(client);
        return INVALID_SOCKET;
    }
    return client;
}

static PAPACC_RESULT papacc_test_drive(
    PAPACC_SERVER_IO_LOOP_WIN32 *loop, PAPACC_SIZE passes)
{
    PAPACC_SIZE index;
    for (index = 0; index < passes; ++index) {
        PAPACC_RESULT result = papacc_server_io_loop_win32_poll_once(loop, 10U);
        if (result != PAPACC_RESULT_OK) return result;
    }
    return PAPACC_RESULT_OK;
}

static PAPACC_RESULT papacc_test_maintain(
    PAPACC_SERVER_IO_LOOP_WIN32 *loop,PAPACC_SIZE passes)
{
    PAPACC_SIZE index;
    for(index=0U;index<passes;++index){
        PAPACC_RESULT result=papacc_server_io_loop_win32_maintenance(loop,0U);
        if(result!=PAPACC_RESULT_OK)return result;
    }
    return PAPACC_RESULT_OK;
}

static int papacc_test_request_ticket(
    PAPACC_SERVER_IO_LOOP_WIN32 *loop, SOCKET control, PAPACC_U8 ticket[16])
{
    PAPACC_U8 frame[32];
    if (send(control, (const char *)papacc_test_ticket_request, 16, 0) != 16 ||
        papacc_test_drive(loop, 8) != PAPACC_RESULT_OK ||
        recv(control, (char *)frame, 32, MSG_WAITALL) != 32 ||
        frame[8] != 0 || frame[9] != 4 || frame[15] != 16) return 1;
    memcpy(ticket, &frame[16], 16);
    return 0;
}

static int papacc_test_attach_ticket(
    PAPACC_SERVER_IO_LOOP_WIN32 *loop, PAPACC_U16 port,
    const PAPACC_U8 ticket[16], SOCKET *out_data)
{
    PAPACC_U8 attach[32] = {
        0x50,0x41,0x43,0x43,1,0,0,16,0,5,0,0,0,0,0,16 };
    PAPACC_U8 accepted[16];
    memcpy(&attach[16], ticket, 16);
    *out_data = papacc_test_connect(port);
    if (*out_data == INVALID_SOCKET || papacc_test_drive(loop, 2) !=
            PAPACC_RESULT_OK || send(*out_data, (const char *)attach, 32, 0) != 32 ||
        papacc_test_drive(loop, 8) != PAPACC_RESULT_OK ||
        recv(*out_data, (char *)accepted, 16, MSG_WAITALL) != 16 ||
        memcmp(accepted, papacc_test_data_accept, 16) != 0) return 1;
    return 0;
}

static int papacc_test_attach_data(
    PAPACC_SERVER_IO_LOOP_WIN32 *loop, PAPACC_U16 port, SOCKET control,
    SOCKET *out_data)
{
    PAPACC_U8 ticket[16];
    return papacc_test_request_ticket(loop, control, ticket) != 0 ||
        papacc_test_attach_ticket(loop, port, ticket, out_data) != 0;
}

static int papacc_test_send_network_data(PAPACC_SERVER_IO_LOOP_WIN32 *loop,
    PAPACC_SERVER_PROTOCOL_SLOT_WIN32 *slot,SOCKET data,
    const PAPACC_U8 *payload,PAPACC_SIZE size)
{
    PAPACC_U8 header[16]={0x50,0x41,0x43,0x43,1,0,0,16,0,11,0,0,0,0,0,0};
    PAPACC_NETWORK_STREAM *stream;
    PAPACC_U64 started, now, target_bytes;
    stream = papacc_network_stream_find_channel(&loop->network_stream_manager,
        slot->network_data_protocol.channel_instance_id);
    if (stream == NULL || papacc_pal_monotonic_time_ns(&started) != PAPACC_RESULT_OK)
        return 1;
    target_bytes = stream->bytes_to_remote + size;
    header[12]=(PAPACC_U8)(size>>24);header[13]=(PAPACC_U8)(size>>16);
    header[14]=(PAPACC_U8)(size>>8);header[15]=(PAPACC_U8)size;
    if(send(data,(const char *)header,7,0)!=7||
        send(data,(const char *)(header+7),9,0)!=9||
        send(data,(const char *)payload,(int)size,0)!=(int)size)return 1;
    /* A fixed number of calls does not prove the fragmented frame arrived.
       Keep the same owner progressing until its actual upstream write count
       confirms delivery, before the test blocks in the peer recv. */
    do {
        if (papacc_server_io_loop_win32_poll_once(loop, 10U) != PAPACC_RESULT_OK ||
            papacc_pal_monotonic_time_ns(&now) != PAPACC_RESULT_OK)
            return 1;
        stream = papacc_network_stream_find_channel(&loop->network_stream_manager,
            slot->network_data_protocol.channel_instance_id);
        if (stream == NULL) return 1;
        if (stream->bytes_to_remote >= target_bytes) return 0;
    } while (now - started < 3000000000ULL);
    fprintf(stderr, "SEND_NETWORK_DATA_DELIVERY_TIMEOUT expected=%llu actual=%llu\n",
        (unsigned long long)target_bytes, (unsigned long long)stream->bytes_to_remote);
    return 1;
}

#define PAPACC_TEST_RECEIVE_DEADLINE_NS 3000000000ULL
#define PAPACC_TEST_RECEIVE_WAIT_SLICE_US 10000L
#define PAPACC_TEST_NEGATIVE_DEADLINE_NS 60000000ULL

typedef enum PAPACC_TEST_RECEIVE_FAILURE {
    PAPACC_TEST_RECEIVE_FAILURE_NONE=0,
    PAPACC_TEST_RECEIVE_FAILURE_TIMEOUT,
    PAPACC_TEST_RECEIVE_FAILURE_EOF,
    PAPACC_TEST_RECEIVE_FAILURE_SOCKET,
    PAPACC_TEST_RECEIVE_FAILURE_INVALID_TYPE,
    PAPACC_TEST_RECEIVE_FAILURE_INVALID_SIZE,
    PAPACC_TEST_RECEIVE_FAILURE_MAINTENANCE
} PAPACC_TEST_RECEIVE_FAILURE;

typedef struct PAPACC_TEST_RECEIVE_OBSERVATION {
    PAPACC_TEST_RECEIVE_FAILURE failure;
    PAPACC_SIZE header_received;
    PAPACC_SIZE payload_received;
    PAPACC_SIZE total_received;
    PAPACC_SIZE maintenance_calls;
    PAPACC_RESULT maintenance_result;
    int socket_result;
    int socket_error;
    PAPACC_U64 elapsed_ns;
} PAPACC_TEST_RECEIVE_OBSERVATION;

static int papacc_test_wait_network_readable(PAPACC_SERVER_IO_LOOP_WIN32 *loop,
    SOCKET data,PAPACC_U64 deadline,PAPACC_SIZE *iterations,int *wait_result,
    PAPACC_BOOL progress_enabled,PAPACC_TEST_RECEIVE_OBSERVATION *observation)
{
    for(;;){
        PAPACC_U64 now=0U,remaining;fd_set readable;struct timeval wait;
        PAPACC_RESULT progressed=PAPACC_RESULT_OK;
        if(papacc_pal_monotonic_time_ns(&now)!=PAPACC_RESULT_OK)return -3;
        if(now>=deadline)return 0;
        if(progress_enabled==PAPACC_TRUE){
            progressed=papacc_server_io_loop_win32_maintenance(loop,now);
            ++observation->maintenance_calls;
            observation->maintenance_result=progressed;
            if(progressed!=PAPACC_RESULT_OK)return -4;
        }
        if(papacc_pal_monotonic_time_ns(&now)!=PAPACC_RESULT_OK)return -3;
        if(now>=deadline)return 0;
        remaining=deadline-now;
        wait.tv_sec=0;
        wait.tv_usec=(long)(remaining/1000U);
        if(wait.tv_usec>PAPACC_TEST_RECEIVE_WAIT_SLICE_US)
            wait.tv_usec=PAPACC_TEST_RECEIVE_WAIT_SLICE_US;
        if(wait.tv_usec<1L)wait.tv_usec=1L;
        FD_ZERO(&readable);FD_SET(data,&readable);
        *wait_result=select(0,&readable,NULL,NULL,&wait);
        ++(*iterations);
        if(*wait_result==1)return 1;
        if(*wait_result==SOCKET_ERROR)return -2;
    }
}

static int papacc_test_receive_network_data_observed(
    PAPACC_SERVER_IO_LOOP_WIN32 *loop,SOCKET data,PAPACC_U8 *output,
    PAPACC_SIZE expected,PAPACC_U64 budget_ns,PAPACC_BOOL progress_enabled,
    PAPACC_TEST_RECEIVE_OBSERVATION *observation)
{
    PAPACC_U8 header[16];PAPACC_SIZE total=0U,header_received=0U;
    PAPACC_SIZE payload_size=0U,payload_received=0U,iterations=0U;
    PAPACC_U64 started=0U,deadline,finished=0U;int wait_result=0,received,ready;
    if(observation==NULL)return 1;
    memset(observation,0,sizeof(*observation));
    observation->maintenance_result=PAPACC_RESULT_OK;
    if(loop==NULL||data==INVALID_SOCKET||output==NULL||expected==0U||
        budget_ns==0U||papacc_pal_monotonic_time_ns(&started)!=PAPACC_RESULT_OK)
        return 1;
    deadline=started+budget_ns;
    while(total<expected){
        header_received=0U;
        while(header_received<sizeof(header)){
            ready=papacc_test_wait_network_readable(loop,data,deadline,&iterations,
                &wait_result,progress_enabled,observation);
            if(ready!=1){
                observation->failure=ready==-4?
                    PAPACC_TEST_RECEIVE_FAILURE_MAINTENANCE:
                    (ready==0?PAPACC_TEST_RECEIVE_FAILURE_TIMEOUT:
                    PAPACC_TEST_RECEIVE_FAILURE_SOCKET);
                goto receive_failure;
            }
            received=recv(data,(char *)(header+header_received),
                (int)(sizeof(header)-header_received),0);
            observation->socket_result=received;
            if(received==0){
                observation->failure=PAPACC_TEST_RECEIVE_FAILURE_EOF;
                goto receive_failure;
            }
            if(received==SOCKET_ERROR){
                observation->failure=PAPACC_TEST_RECEIVE_FAILURE_SOCKET;
                observation->socket_error=WSAGetLastError();
                goto receive_failure;
            }
            header_received+=(PAPACC_SIZE)received;
            observation->header_received=header_received;
        }
        if(header[8]!=0U||header[9]!=PAPACC_MESSAGE_TYPE_NETWORK_DATA){
            observation->failure=PAPACC_TEST_RECEIVE_FAILURE_INVALID_TYPE;
            goto receive_failure;
        }
        payload_size=((PAPACC_SIZE)header[12]<<24)|
            ((PAPACC_SIZE)header[13]<<16)|
            ((PAPACC_SIZE)header[14]<<8)|header[15];
        if(payload_size==0U||payload_size>expected-total){
            observation->failure=PAPACC_TEST_RECEIVE_FAILURE_INVALID_SIZE;
            goto receive_failure;
        }
        payload_received=0U;
        while(payload_received<payload_size){
            ready=papacc_test_wait_network_readable(loop,data,deadline,&iterations,
                &wait_result,progress_enabled,observation);
            if(ready!=1){
                observation->failure=ready==-4?
                    PAPACC_TEST_RECEIVE_FAILURE_MAINTENANCE:
                    (ready==0?PAPACC_TEST_RECEIVE_FAILURE_TIMEOUT:
                    PAPACC_TEST_RECEIVE_FAILURE_SOCKET);
                goto receive_failure;
            }
            received=recv(data,(char *)(output+total+payload_received),
                (int)(payload_size-payload_received),0);
            observation->socket_result=received;
            if(received==0){
                observation->failure=PAPACC_TEST_RECEIVE_FAILURE_EOF;
                goto receive_failure;
            }
            if(received==SOCKET_ERROR){
                observation->failure=PAPACC_TEST_RECEIVE_FAILURE_SOCKET;
                observation->socket_error=WSAGetLastError();
                goto receive_failure;
            }
            payload_received+=(PAPACC_SIZE)received;
            observation->payload_received=payload_received;
        }
        total+=payload_size;
        observation->total_received=total;
    }
    observation->failure=PAPACC_TEST_RECEIVE_FAILURE_NONE;
    return 0;
receive_failure:
    (void)papacc_pal_monotonic_time_ns(&finished);
    observation->header_received=header_received;
    observation->payload_received=payload_received;
    observation->total_received=total;
    observation->elapsed_ns=finished-started;
    fprintf(stderr,"receive_network_data deadline_ms=%llu elapsed_ms=%llu iterations=%zu failure=%d header=%zu payload=%zu received=%zu expected=%zu socket_result=%d error=%d maintenance=%d calls=%zu\n",
        (unsigned long long)(budget_ns/1000000U),
        (unsigned long long)(observation->elapsed_ns/1000000U),(size_t)iterations,
        (int)observation->failure,(size_t)header_received,
        (size_t)payload_received,(size_t)total,(size_t)expected,
        observation->socket_result,observation->socket_error,
        (int)observation->maintenance_result,
        (size_t)observation->maintenance_calls);
    return 1;
}

static int papacc_test_receive_network_data(PAPACC_SERVER_IO_LOOP_WIN32 *loop,
    SOCKET data,PAPACC_U8 *output,PAPACC_SIZE expected)
{
    PAPACC_TEST_RECEIVE_OBSERVATION observation;
    return papacc_test_receive_network_data_observed(loop,data,output,expected,
        PAPACC_TEST_RECEIVE_DEADLINE_NS,PAPACC_TRUE,&observation);
}

static int papacc_test_tcp_pair(SOCKET *receiver,SOCKET *sender)
{
    SOCKET listener=INVALID_SOCKET,client=INVALID_SOCKET,accepted=INVALID_SOCKET;
    struct sockaddr_in address;int length=(int)sizeof(address);
    *receiver=INVALID_SOCKET;*sender=INVALID_SOCKET;
    listener=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(listener==INVALID_SOCKET)return 1;
    memset(&address,0,sizeof(address));address.sin_family=AF_INET;
    address.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
    if(bind(listener,(const struct sockaddr *)&address,sizeof(address))==
            SOCKET_ERROR||listen(listener,1)==SOCKET_ERROR||
        getsockname(listener,(struct sockaddr *)&address,&length)==SOCKET_ERROR)
        goto failure;
    client=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(client==INVALID_SOCKET||
        connect(client,(const struct sockaddr *)&address,sizeof(address))==
            SOCKET_ERROR)goto failure;
    accepted=accept(listener,NULL,NULL);
    if(accepted==INVALID_SOCKET)goto failure;
    (void)closesocket(listener);*receiver=accepted;*sender=client;return 0;
failure:
    if(accepted!=INVALID_SOCKET)(void)closesocket(accepted);
    if(client!=INVALID_SOCKET)(void)closesocket(client);
    if(listener!=INVALID_SOCKET)(void)closesocket(listener);
    return 1;
}

static void papacc_test_close_pair(SOCKET *receiver,SOCKET *sender)
{
    if(*receiver!=INVALID_SOCKET)(void)closesocket(*receiver);
    if(*sender!=INVALID_SOCKET)(void)closesocket(*sender);
    *receiver=INVALID_SOCKET;*sender=INVALID_SOCKET;
}

static int papacc_test_receive_negative_case(PAPACC_SERVER_IO_LOOP_WIN32 *loop,
    int scenario)
{
    SOCKET receiver=INVALID_SOCKET,sender=INVALID_SOCKET;
    PAPACC_U8 output[8],header[16]={
        0x50,0x41,0x43,0x43,1,0,0,16,0,11,0,0,0,0,0,4};
    PAPACC_TEST_RECEIVE_OBSERVATION observed;PAPACC_BOOL progress=PAPACC_TRUE;
    struct linger abortive={1,0};int result=1;
    if(papacc_test_tcp_pair(&receiver,&sender)!=0)return 1;
    if(scenario==2&&send(sender,(const char *)header,7,0)!=7)goto cleanup;
    if(scenario==3&&(send(sender,(const char *)header,16,0)!=16||
        send(sender,"ab",2,0)!=2))goto cleanup;
    if(scenario==4){
        if(send(sender,(const char *)header,7,0)!=7)goto cleanup;
        (void)shutdown(sender,SD_BOTH);(void)closesocket(sender);
        sender=INVALID_SOCKET;
    }
    if(scenario==5){
        if(send(sender,(const char *)header,7,0)!=7||
            setsockopt(sender,SOL_SOCKET,SO_LINGER,(const char *)&abortive,
                sizeof(abortive))==SOCKET_ERROR)goto cleanup;
        (void)closesocket(sender);sender=INVALID_SOCKET;
    }
    if(scenario==6){
        header[9]=PAPACC_MESSAGE_TYPE_NETWORK_CLOSE;
        if(send(sender,(const char *)header,16,0)!=16)goto cleanup;
    }
    if(scenario==7&&(send(sender,(const char *)header,16,0)!=16||
        send(sender,"a",1,0)!=1))goto cleanup;
    if(scenario==8)progress=PAPACC_FALSE;
    if(papacc_test_receive_network_data_observed(loop,receiver,output,4U,
            PAPACC_TEST_NEGATIVE_DEADLINE_NS,progress,&observed)==0)goto cleanup;
    if(scenario==1&&observed.failure==PAPACC_TEST_RECEIVE_FAILURE_TIMEOUT&&
        observed.header_received==0U&&observed.maintenance_calls>0U)result=0;
    else if(scenario==2&&observed.failure==PAPACC_TEST_RECEIVE_FAILURE_TIMEOUT&&
        observed.header_received==7U)result=0;
    else if(scenario==3&&observed.failure==PAPACC_TEST_RECEIVE_FAILURE_TIMEOUT&&
        observed.header_received==16U&&observed.payload_received==2U)result=0;
    else if(scenario==4&&observed.failure==PAPACC_TEST_RECEIVE_FAILURE_EOF&&
        observed.header_received==7U)result=0;
    else if(scenario==5&&observed.failure==PAPACC_TEST_RECEIVE_FAILURE_SOCKET&&
        observed.socket_error==WSAECONNRESET)result=0;
    else if(scenario==6&&
        observed.failure==PAPACC_TEST_RECEIVE_FAILURE_INVALID_TYPE)result=0;
    else if(scenario==7&&observed.failure==PAPACC_TEST_RECEIVE_FAILURE_TIMEOUT&&
        observed.header_received==16U&&observed.payload_received==1U&&
        observed.elapsed_ns>=PAPACC_TEST_NEGATIVE_DEADLINE_NS)result=0;
    else if(scenario==8&&observed.failure==PAPACC_TEST_RECEIVE_FAILURE_TIMEOUT&&
        observed.maintenance_calls==0U)result=0;
    fprintf(stderr,"N%d injection=YES expected_failure=YES observed=%d header=%zu payload=%zu deadline_remaining=0 maintenance=%d calls=%zu socket=%d error=%d result=%s\n",
        scenario,(int)observed.failure,(size_t)observed.header_received,
        (size_t)observed.payload_received,(int)observed.maintenance_result,
        (size_t)observed.maintenance_calls,observed.socket_result,
        observed.socket_error,result==0?"PASS":"FAIL");
cleanup:
    papacc_test_close_pair(&receiver,&sender);
    return result;
}

static int papacc_test_receive_negative_selftests(
    PAPACC_SERVER_IO_LOOP_WIN32 *loop)
{
    int scenario;
    for(scenario=1;scenario<=8;++scenario)
        if(papacc_test_receive_negative_case(loop,scenario)!=0)return scenario;
    return 0;
}

static int papacc_test_send_network_terminal(PAPACC_SERVER_IO_LOOP_WIN32 *loop,
    PAPACC_SERVER_PROTOCOL_SLOT_WIN32 *slot,SOCKET data,PAPACC_U16 type)
{
    PAPACC_U8 header[16]={0x50,0x41,0x43,0x43,1,0,0,16,0,0,0,0,0,0,0,0};
    header[8]=(PAPACC_U8)(type>>8);header[9]=(PAPACC_U8)type;
    if(send(data,(const char *)header,16,0)!=16)return 1;
    if(papacc_server_io_loop_win32_process_connection_once(loop,
        slot->connection_instance_id,PAPACC_TRUE,PAPACC_FALSE,0U)!=
            PAPACC_RESULT_OK)return 1;
    return papacc_test_maintain(loop,8U)==PAPACC_RESULT_OK?0:1;
}

static int papacc_test_receive_network_terminal(PAPACC_SERVER_IO_LOOP_WIN32 *loop,
    SOCKET data,PAPACC_U16 type)
{
    PAPACC_U8 header[16];
    if(papacc_test_maintain(loop,16U)!=PAPACC_RESULT_OK||
        recv(data,(char *)header,16,MSG_WAITALL)!=16||header[8]!=(PAPACC_U8)(type>>8)||
        header[9]!=(PAPACC_U8)type||header[12]!=0U||header[13]!=0U||
        header[14]!=0U||header[15]!=0U)return 1;
    return 0;
}

static int papacc_test_open_fixture_stream(PAPACC_SERVER_IO_LOOP_WIN32 *loop,
    PAPACC_U16 server_port,SOCKET control,SOCKET fixture_listener,
    PAPACC_U16 fixture_port,SOCKET *out_data,SOCKET *out_peer,
    PAPACC_SERVER_PROTOCOL_SLOT_WIN32 **out_slot)
{
    PAPACC_U8 open_frame[28]={
        0x50,0x41,0x43,0x43,1,0,0,16,0,9,0,0,0,0,0,12,
        1,2,1,0,0,0,0,4,127,0,0,1};
    PAPACC_U8 open_result[17];PAPACC_SIZE i,pass;
    *out_data=INVALID_SOCKET;*out_peer=INVALID_SOCKET;*out_slot=NULL;
    if(papacc_test_attach_data(loop,server_port,control,out_data)!=0)return 1;
    for(i=0U;i<loop->processor_capacity;++i){
        PAPACC_SERVER_PROTOCOL_SLOT_WIN32*s=&loop->processor_slots[i];
        if(s->in_use==PAPACC_TRUE&&
            s->kind==PAPACC_SERVER_PROTOCOL_SLOT_KIND_DATA_ATTACH&&
            s->network_data_protocol.state==PAPACC_NETWORK_DATA_WAIT_OPEN)
            *out_slot=s;
    }
    if(*out_slot==NULL)return 1;
    open_frame[20]=(PAPACC_U8)(fixture_port>>8);
    open_frame[21]=(PAPACC_U8)fixture_port;
    if(send(*out_data,(const char *)open_frame,28,0)!=28)return 1;
    for(pass=0U;pass<4U;++pass)
        if(papacc_server_io_loop_win32_process_connection_once(loop,
            (*out_slot)->connection_instance_id,PAPACC_TRUE,PAPACC_FALSE,0U)!=
                PAPACC_RESULT_OK)return 1;
    if(papacc_test_drive(loop,12)!=PAPACC_RESULT_OK||
        recv(*out_data,(char *)open_result,17,MSG_WAITALL)!=17||
        open_result[8]!=0U||open_result[9]!=10U||
        open_result[16]!=PAPACC_NETWORK_OPEN_OK)return 1;
    *out_peer=accept(fixture_listener,NULL,NULL);
    return *out_peer==INVALID_SOCKET?1:0;
}

int main(void)
{
    PAPACC_SERVER_NETWORK network = PAPACC_SERVER_NETWORK_INITIALIZER;
    PAPACC_TCP_LISTENER_ENTRY_WIN32 entry =
        PAPACC_TCP_LISTENER_ENTRY_WIN32_INITIALIZER;
    PAPACC_BIND_TARGET target = PAPACC_BIND_TARGET_INITIALIZER;
    PAPACC_SERVER_ACCEPTOR_WIN32 acceptor =
        PAPACC_SERVER_ACCEPTOR_WIN32_INITIALIZER;
    PAPACC_SERVER_IO_LOOP_WIN32 loop = PAPACC_SERVER_IO_LOOP_WIN32_INITIALIZER;
    PAPACC_CONNECTION connections[PAPACC_TEST_CAPACITY];
    PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT
        contexts[PAPACC_TEST_CAPACITY];
    PAPACC_SESSION sessions[PAPACC_TEST_CAPACITY];
    PAPACC_CHANNEL channels[PAPACC_TEST_CAPACITY];
    PAPACC_DATA_ASSOCIATION_ENTRY associations[PAPACC_TEST_CAPACITY];
    PAPACC_SERVER_CONTROL_PROCESSOR_SLOT_WIN32
        processors[PAPACC_TEST_CAPACITY];
    struct sockaddr_in native_address;
    int native_length = (int)sizeof(native_address);
    PAPACC_U16 port = 0;
    SOCKET idle = INVALID_SOCKET;
    SOCKET valid = INVALID_SOCKET;
    SOCKET malformed = INVALID_SOCKET;
    SOCKET truncated = INVALID_SOCKET;
    SOCKET rejected = INVALID_SOCKET;
    SOCKET established = INVALID_SOCKET;
    SOCKET data_client = INVALID_SOCKET;
    SOCKET session_fail = INVALID_SOCKET;
    SOCKET channel_fail = INVALID_SOCKET;
    SOCKET round_robin_a = INVALID_SOCKET;
    SOCKET round_robin_b = INVALID_SOCKET;
    SOCKET cascade_control = INVALID_SOCKET;
    SOCKET cascade_data_a = INVALID_SOCKET;
    SOCKET cascade_data_b = INVALID_SOCKET;
    SOCKET recovery_control = INVALID_SOCKET;
    SOCKET second_control = INVALID_SOCKET;
    SOCKET recovery_data = INVALID_SOCKET;
    SOCKET second_data = INVALID_SOCKET;
    SOCKET invalid_data = INVALID_SOCKET;
    SOCKET replay_data = INVALID_SOCKET;
    SOCKET fixture_listener = INVALID_SOCKET;
    SOCKET fixture_peer = INVALID_SOCKET;
    SOCKET lifecycle_data = INVALID_SOCKET;
    SOCKET lifecycle_peer = INVALID_SOCKET;
    SOCKET cascade_stream_data_a = INVALID_SOCKET;
    SOCKET cascade_stream_data_b = INVALID_SOCKET;
    SOCKET cascade_stream_peer_a = INVALID_SOCKET;
    SOCKET cascade_stream_peer_b = INVALID_SOCKET;
    PAPACC_U8 received[20];
    PAPACC_SIZE index;
    int result = 0;
    PAPACC_BOOL read_interest = PAPACC_FALSE;
    PAPACC_BOOL write_interest = PAPACC_FALSE;
    PAPACC_TEST_EGRESS_CONTEXT egress_context;
    PAPACC_EGRESS_POLICY egress_policy=PAPACC_EGRESS_POLICY_INITIALIZER;
    PAPACC_EGRESS_PROFILE egress_profile=PAPACC_EGRESS_PROFILE_INITIALIZER;
    PAPACC_EGRESS_RUNTIME_CONFIG egress_config=
        PAPACC_EGRESS_RUNTIME_CONFIG_INITIALIZER;

    for (index = 0; index < PAPACC_TEST_CAPACITY; ++index)
        contexts[index] = (PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT)
            PAPACC_TCP_CONNECTION_TRANSPORT_WIN32_CONTEXT_INITIALIZER;
    if (papacc_tcp_platform_init(&network.tcp_platform) != PAPACC_RESULT_OK ||
        papacc_ip_address_set_ipv4(&target.address, 127, 0, 0, 1) !=
            PAPACC_RESULT_OK ||
        papacc_tcp_socket_win32_bind(
            &network.tcp_platform, &target, 0, &entry.socket) !=
            PAPACC_RESULT_OK ||
        papacc_tcp_socket_win32_listen(&entry.socket) != PAPACC_RESULT_OK ||
        getsockname(entry.socket.native_socket,
                    (struct sockaddr *)&native_address, &native_length) ==
            SOCKET_ERROR) return 1;
    port = (PAPACC_U16)ntohs(native_address.sin_port);
    entry.target = target;
    network.listener_set.entries = &entry;
    network.listener_set.capacity = 1;
    network.listener_set.count = 1;
    network.listener_set.bound_port = port;
    network.listener_set.is_active = PAPACC_TRUE;
    network.listener_storage = &entry;
    network.listener_storage_capacity = 1;
    network.is_active = PAPACC_TRUE;
    if (papacc_server_acceptor_win32_init(
            &acceptor, &network, connections, PAPACC_TEST_CAPACITY,
            contexts, PAPACC_TEST_CAPACITY) != PAPACC_RESULT_OK ||
        papacc_server_io_loop_win32_init(
            &loop, &network, &acceptor, sessions, PAPACC_TEST_CAPACITY,
            channels, PAPACC_TEST_CAPACITY, associations, PAPACC_TEST_CAPACITY,
            processors,
            PAPACC_TEST_CAPACITY, 1000000000ULL) != PAPACC_RESULT_OK) {
        result = 2;
        goto cleanup;
    }

    {
        int negative_result=papacc_test_receive_negative_selftests(&loop);
        if(negative_result!=0){
            result=70+negative_result;
            goto cleanup;
        }
    }

    /* Dedicated processor-capacity exhaustion and reuse. */
    loop.processor_capacity = 1;
    idle = papacc_test_connect(port);
    if (idle == INVALID_SOCKET || papacc_test_drive(&loop, 2) !=
            PAPACC_RESULT_OK || loop.connection_manager->count != 1 ||
        papacc_server_io_loop_win32_processor_interest(
            &loop, 0, &read_interest, &write_interest) != PAPACC_RESULT_OK ||
        read_interest != PAPACC_TRUE || write_interest != PAPACC_FALSE) {
        result = 10;
        goto cleanup;
    }
    rejected = papacc_test_connect(port);
    if (rejected == INVALID_SOCKET || papacc_test_drive(&loop, 2) !=
            PAPACC_RESULT_OK || loop.connection_manager->count != 1 ||
        processors[0].in_use != PAPACC_TRUE) {
        result = 11;
        goto cleanup;
    }
    processors[0].classifier.establishment_deadline_ns = 0;
    if (papacc_test_drive(&loop, 1) != PAPACC_RESULT_OK ||
        loop.connection_manager->count != 0 ||
        processors[0].in_use != PAPACC_FALSE) {
        result = 12;
        goto cleanup;
    }
    (void)closesocket(idle);
    idle = INVALID_SOCKET;
    (void)closesocket(rejected);
    rejected = INVALID_SOCKET;
    established = papacc_test_connect(port);
    if (established == INVALID_SOCKET || papacc_test_drive(&loop, 2) !=
            PAPACC_RESULT_OK ||
        send(established, (const char *)papacc_test_open, 20, 0) != 20) {
        result = 13;
        goto cleanup;
    }
    for (index = 0; index < 8 && processors[0].control_processor.state !=
            PAPACC_CONTROL_PROCESSOR_STATE_WRITING_CONTROL_ACCEPT; ++index) {
        if (papacc_test_drive(&loop, 1) != PAPACC_RESULT_OK) {
            result = 14;
            goto cleanup;
        }
    }
    if (papacc_server_io_loop_win32_processor_interest(
            &loop, 0, &read_interest, &write_interest) != PAPACC_RESULT_OK ||
        read_interest != PAPACC_FALSE || write_interest != PAPACC_TRUE ||
        papacc_test_drive(&loop, 4) != PAPACC_RESULT_OK ||
        recv(established, (char *)received, 20, MSG_WAITALL) != 20 ||
        memcmp(received, papacc_test_accept, 20) != 0 ||
        papacc_server_io_loop_win32_processor_interest(
            &loop, 0, &read_interest, &write_interest) != PAPACC_RESULT_OK ||
        read_interest != PAPACC_TRUE || write_interest != PAPACC_FALSE) {
        result = 15;
        goto cleanup;
    }
    loop.processor_capacity = PAPACC_TEST_CAPACITY;

    /* Explicit, test-only policy for one exact loopback fixture endpoint. */
    fixture_listener=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    native_address.sin_family=AF_INET;
    native_address.sin_port=0;
    native_address.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
    native_length=(int)sizeof(native_address);
    if(fixture_listener==INVALID_SOCKET||bind(fixture_listener,
        (const struct sockaddr *)&native_address,sizeof(native_address))==SOCKET_ERROR||
        listen(fixture_listener,1)==SOCKET_ERROR||getsockname(fixture_listener,
        (struct sockaddr *)&native_address,&native_length)==SOCKET_ERROR){result=43;goto cleanup;}
    egress_context.allowed=(PAPACC_NETWORK_ENDPOINT)PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    (void)papacc_ip_address_set_ipv4(&egress_context.allowed.address,127,0,0,1);
    egress_context.allowed.port=(PAPACC_U16)ntohs(native_address.sin_port);
    egress_policy.master_enabled=PAPACC_TRUE;
    egress_policy.explicitly_allowed_sensitive_classes=PAPACC_EGRESS_SENSITIVE_LOOPBACK;
    egress_policy.endpoint_policy=papacc_test_egress_endpoint;
    egress_policy.context=&egress_context;
    egress_profile.profile_id=1U;
    memcpy(egress_profile.name,"fixture-default",16U);
    egress_profile.use_default_route=PAPACC_TRUE;
    egress_profile.available=PAPACC_TRUE;
    egress_config.allow_network_egress=PAPACC_TRUE;
    egress_config.policy=&egress_policy;
    egress_config.profiles=&egress_profile;
    egress_config.profile_count=1U;
    egress_config.select_profile=papacc_test_select_profile;
    egress_config.select_profile_context=&egress_context;
    if(papacc_server_io_loop_win32_set_egress_config(&loop,&egress_config)!=
        PAPACC_RESULT_OK){result=44;goto cleanup;}

    {
        const PAPACC_U8 negotiate[32]={
            0x50,0x41,0x43,0x43,1,0,0,16,0,7,0,0,0,0,0,16,
            0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1};
        PAPACC_U8 negotiation_result[24];
        if(send(established,(const char *)negotiate,32,0)!=32||
            papacc_test_drive(&loop,8)!=PAPACC_RESULT_OK||
            recv(established,(char *)negotiation_result,24,MSG_WAITALL)!=24||
            negotiation_result[8]!=0||negotiation_result[9]!=8||
            negotiation_result[16]!=0||negotiation_result[17]!=1){
            result=45;goto cleanup;
        }
    }

    /* Real post-Control ticket request and second-Connection DATA attach. */
    {
        PAPACC_U8 ticket_frame[32];
        PAPACC_U8 attach_frame[32] = {
            0x50,0x41,0x43,0x43,1,0,0,16,0,5,0,0,0,0,0,16 };
        PAPACC_CHANNEL *data_channel;
        if (send(established, (const char *)papacc_test_ticket_request, 16, 0) != 16 ||
            papacc_test_drive(&loop, 8) != PAPACC_RESULT_OK ||
            recv(established, (char *)ticket_frame, 32, MSG_WAITALL) != 32 ||
            ticket_frame[8] != 0 || ticket_frame[9] != 4 ||
            ticket_frame[15] != 16) {
            result = 24;
            goto cleanup;
        }
        memcpy(&attach_frame[16], &ticket_frame[16], 16);
        data_client = papacc_test_connect(port);
        if (data_client == INVALID_SOCKET || papacc_test_drive(&loop, 2) !=
                PAPACC_RESULT_OK ||
            send(data_client, (const char *)attach_frame, 32, 0) != 32 ||
            papacc_test_drive(&loop, 8) != PAPACC_RESULT_OK ||
            recv(data_client, (char *)received, 16, MSG_WAITALL) != 16 ||
            memcmp(received, papacc_test_data_accept, 16) != 0 ||
            loop.session_manager.count != 1 || loop.channel_manager.count != 2 ||
            loop.connection_manager->count != 2) {
            result = 25;
            goto cleanup;
        }
        {
            PAPACC_U8 open_frame[28]={
                0x50,0x41,0x43,0x43,1,0,0,16,0,9,0,0,0,0,0,12,
                1,2,1,0,0,0,0,4,127,0,0,1};
            PAPACC_U8 open_result[17];
            open_frame[20]=(PAPACC_U8)(egress_context.allowed.port>>8);
            open_frame[21]=(PAPACC_U8)egress_context.allowed.port;
            {
            int sent=send(data_client,(const char *)open_frame,28,0);
            PAPACC_RESULT drove=PAPACC_RESULT_OK;
            PAPACC_SIZE open_pass=0U;
            int readable=0;
            while(open_pass<300U&&drove==PAPACC_RESULT_OK&&readable==0){
                fd_set read_set;
                struct timeval no_wait;
                drove=papacc_server_io_loop_win32_poll_once(&loop,10U);
                FD_ZERO(&read_set);
                FD_SET(data_client,&read_set);
                no_wait.tv_sec=0;
                no_wait.tv_usec=0;
                readable=select(0,&read_set,NULL,NULL,&no_wait);
                ++open_pass;
            }
            int got=recv(data_client,(char *)open_result,17,MSG_WAITALL);
            if(sent!=28||drove!=PAPACC_RESULT_OK||readable!=1||got!=17||
                open_result[8]!=0||open_result[9]!=10||open_result[15]!=1||
                open_result[16]!=PAPACC_NETWORK_OPEN_OK||
                loop.network_stream_manager.count!=1U||
                loop.network_job_manager.count!=0U){
                fprintf(stderr,"open failure sent=%d drove=%d readable=%d passes=%zu got=%d type=%u length=%u status=%u streams=%zu jobs=%zu\n",
                    sent,(int)drove,readable,(size_t)open_pass,got,(unsigned)open_result[9],
                    (unsigned)open_result[15],(unsigned)open_result[16],
                    (size_t)loop.network_stream_manager.count,
                    (size_t)loop.network_job_manager.count);
                result=46;goto cleanup;}
            }
            fixture_peer=accept(fixture_listener,NULL,NULL);
            if(fixture_peer==INVALID_SOCKET){result=47;goto cleanup;}
            {
                DWORD timeout_ms=3000U;
                PAPACC_U8 sent_bytes[512],remote_bytes[512],returned[512];
                PAPACC_SIZE transfer,total=0U;
                (void)setsockopt(fixture_peer,SOL_SOCKET,SO_RCVTIMEO,
                    (const char *)&timeout_ms,(int)sizeof(timeout_ms));
                for(transfer=0U;transfer<sizeof(sent_bytes);++transfer)
                    sent_bytes[transfer]=(PAPACC_U8)((transfer*37U)^0xFFU);
                sent_bytes[0]=0U;sent_bytes[1]=0xFFU;
                while(total<32768U){
                    int transfer_stage=0,operation_result=0,actual_bytes=0;
                    int socket_error=0;
                    transfer_stage=1;
                    operation_result=papacc_test_send_network_data(&loop,
                        &processors[1],data_client,sent_bytes,
                        sizeof(sent_bytes));
                    if(operation_result==0){
                        transfer_stage=2;
                        actual_bytes=recv(fixture_peer,(char *)remote_bytes,
                            sizeof(remote_bytes),MSG_WAITALL);
                        if(actual_bytes==(int)sizeof(remote_bytes)){
                            transfer_stage=3;
                            operation_result=memcmp(remote_bytes,sent_bytes,
                                sizeof(sent_bytes));
                            if(operation_result==0){
                                transfer_stage=4;
                                actual_bytes=send(fixture_peer,
                                    (const char *)remote_bytes,173,0);
                                if(actual_bytes==173){
                                    transfer_stage=5;
                                    actual_bytes=send(fixture_peer,
                                        (const char *)(remote_bytes+173),
                                        (int)sizeof(remote_bytes)-173,0);
                                    if(actual_bytes==(int)sizeof(remote_bytes)-173){
                                        transfer_stage=6;
                                        operation_result=
                                            papacc_test_receive_network_data(
                                                &loop,data_client,returned,
                                                sizeof(returned));
                                        if(operation_result==0){
                                            transfer_stage=7;
                                            operation_result=memcmp(returned,
                                                sent_bytes,sizeof(sent_bytes));
                                        }
                                    }
                                }
                            }
                        }
                    }
                    if((transfer_stage==1&&operation_result!=0)||
                        (transfer_stage==2&&actual_bytes!=(int)sizeof(remote_bytes))||
                        (transfer_stage==3&&operation_result!=0)||
                        (transfer_stage==4&&actual_bytes!=173)||
                        (transfer_stage==5&&actual_bytes!=(int)sizeof(remote_bytes)-173)||
                        (transfer_stage==6&&operation_result!=0)||
                        (transfer_stage==7&&operation_result!=0)){
                        socket_error=WSAGetLastError();
                        fprintf(stderr,"exit48 stage=%d expected=%u actual=%d operation=%d total=%zu socket_error=%d stream_state=%d data_state=%d accepted_terminal=%u generation=%llu streams=%zu\n",
                            transfer_stage,(unsigned)sizeof(sent_bytes),actual_bytes,
                            operation_result,(size_t)total,socket_error,
                            (int)loop.network_stream_storage[0].state,
                            (int)processors[1].network_data_protocol.state,
                            (unsigned)loop.network_stream_storage[0].accepted_terminal_type,
                            (unsigned long long)loop.network_stream_storage[0].accepted_terminal_generation,
                            (size_t)loop.network_stream_manager.count);
                        result=48;goto cleanup;
                    }
                    total+=sizeof(sent_bytes);
                }
                if(loop.network_stream_storage[0].buffer_capacity!=
                    PAPACC_SERVER_NETWORK_STREAM_BUFFER_CAPACITY||
                    loop.network_stream_storage[0].to_remote_size>
                        loop.network_stream_storage[0].buffer_capacity||
                    loop.network_stream_storage[0].to_client_size>
                        loop.network_stream_storage[0].buffer_capacity||
                    loop.network_stream_storage[0].bytes_to_remote!=total||
                    loop.network_stream_storage[0].bytes_to_client!=total){
                    result=49;goto cleanup;
                }
                {
                    PAPACC_U8 request[5]={0x00U,0x10U,0x20U,0x30U,0xFFU};
                    PAPACC_U8 response[6]={0xFEU,0x00U,0xDCU,0xBAU,0x98U,0x76U};
                    PAPACC_U8 observed[6];
                    if(papacc_test_send_network_data(&loop,&processors[1],
                            data_client,request,sizeof(request))!=0||
                        papacc_test_send_network_terminal(&loop,&processors[1],
                            data_client,PAPACC_MESSAGE_TYPE_NETWORK_HALF_CLOSE)!=0||
                        recv(fixture_peer,(char *)remote_bytes,
                            sizeof(request),MSG_WAITALL)!=(int)sizeof(request)||
                        memcmp(remote_bytes,request,sizeof(request))!=0||
                        recv(fixture_peer,(char *)remote_bytes,1,0)!=0||
                        send(fixture_peer,(const char *)response,
                            sizeof(response),0)!=(int)sizeof(response)||
                        shutdown(fixture_peer,SD_SEND)!=0||
                        papacc_test_receive_network_data(&loop,data_client,
                            observed,sizeof(observed))!=0||
                        memcmp(observed,response,sizeof(response))!=0||
                        papacc_test_receive_network_terminal(&loop,data_client,
                            PAPACC_MESSAGE_TYPE_NETWORK_HALF_CLOSE)!=0||
                        papacc_test_maintain(&loop,4U)!=PAPACC_RESULT_OK||
                        loop.network_stream_manager.count!=0U){
                        result=50;goto cleanup;
                    }
                }
            }
        }
        data_channel = papacc_channel_manager_find_by_connection(
            &loop.channel_manager, processors[1].connection_instance_id);
        if (data_channel != NULL || papacc_test_drive(&loop, 2) != PAPACC_RESULT_OK ||
            loop.session_manager.count != 1 || loop.channel_manager.count != 1 ||
            loop.connection_manager->count != 1||
            loop.network_stream_manager.count != 0U) {
            result = 26;
            goto cleanup;
        }
        (void)closesocket(data_client);
        data_client = INVALID_SOCKET;
        (void)closesocket(fixture_peer);
        fixture_peer = INVALID_SOCKET;
        {
            PAPACC_SERVER_PROTOCOL_SLOT_WIN32*lifecycle_slot=NULL;
            if(papacc_test_open_fixture_stream(&loop,port,established,
                    fixture_listener,egress_context.allowed.port,&lifecycle_data,
                    &lifecycle_peer,&lifecycle_slot)!=0||
                loop.network_stream_manager.count!=1U||
                papacc_test_send_network_terminal(&loop,lifecycle_slot,
                    lifecycle_data,PAPACC_MESSAGE_TYPE_NETWORK_CLOSE)!=0||
                loop.network_stream_manager.count!=0U||
                papacc_test_maintain(&loop,4U)!=PAPACC_RESULT_OK){
                result=51;goto cleanup;
            }
            (void)closesocket(lifecycle_data);lifecycle_data=INVALID_SOCKET;
            (void)closesocket(lifecycle_peer);lifecycle_peer=INVALID_SOCKET;
            lifecycle_slot=NULL;
            if(papacc_test_open_fixture_stream(&loop,port,established,
                    fixture_listener,egress_context.allowed.port,&lifecycle_data,
                    &lifecycle_peer,&lifecycle_slot)!=0||
                loop.network_stream_manager.count!=1U||
                loop.network_stream_storage[0].to_remote_size!=0U||
                loop.network_stream_storage[0].to_client_size!=0U||
                papacc_test_send_network_terminal(&loop,lifecycle_slot,
                    lifecycle_data,PAPACC_MESSAGE_TYPE_NETWORK_ABORT)!=0||
                loop.network_stream_manager.count!=0U||
                papacc_test_maintain(&loop,4U)!=PAPACC_RESULT_OK){
                result=52;goto cleanup;
            }
            (void)closesocket(lifecycle_data);lifecycle_data=INVALID_SOCKET;
            (void)closesocket(lifecycle_peer);lifecycle_peer=INVALID_SOCKET;
        }
    }

    /* Dedicated Session capacity isolation through the real loop. */
    loop.session_manager.capacity = 1;
    session_fail = papacc_test_connect(port);
    if (session_fail == INVALID_SOCKET || papacc_test_drive(&loop, 2) !=
            PAPACC_RESULT_OK ||
        send(session_fail, (const char *)papacc_test_open, 20, 0) != 20 ||
        papacc_test_drive(&loop, 8) != PAPACC_RESULT_OK ||
        loop.session_manager.count != 1 || loop.channel_manager.count != 1 ||
        loop.connection_manager->count != 1 ||
        sessions[0].state != PAPACC_SESSION_STATE_ACTIVE) {
        result = 16;
        goto cleanup;
    }
    loop.session_manager.capacity = PAPACC_TEST_CAPACITY;

    /* Dedicated Channel capacity rollback: no orphan Session. */
    loop.channel_manager.capacity = 1;
    channel_fail = papacc_test_connect(port);
    if (channel_fail == INVALID_SOCKET || papacc_test_drive(&loop, 2) !=
            PAPACC_RESULT_OK ||
        send(channel_fail, (const char *)papacc_test_open, 20, 0) != 20 ||
        papacc_test_drive(&loop, 8) != PAPACC_RESULT_OK ||
        loop.session_manager.count != 1 || loop.channel_manager.count != 1 ||
        loop.connection_manager->count != 1) {
        result = 17;
        goto cleanup;
    }
    loop.channel_manager.capacity = PAPACC_TEST_CAPACITY;

    idle = papacc_test_connect(port);
    valid = papacc_test_connect(port);
    if (idle == INVALID_SOCKET || valid == INVALID_SOCKET ||
        papacc_test_drive(&loop, 3) != PAPACC_RESULT_OK ||
        send(valid, (const char *)papacc_test_open, 5, 0) != 5 ||
        papacc_test_drive(&loop, 2) != PAPACC_RESULT_OK ||
        send(valid, (const char *)&papacc_test_open[5], 7, 0) != 7 ||
        papacc_test_drive(&loop, 2) != PAPACC_RESULT_OK ||
        send(valid, (const char *)&papacc_test_open[12], 8, 0) != 8 ||
        papacc_test_drive(&loop, 10) != PAPACC_RESULT_OK ||
        recv(valid, (char *)received, 20, MSG_WAITALL) != 20 ||
        memcmp(received, papacc_test_accept, 20) != 0 ||
        loop.session_manager.count != 2 || loop.channel_manager.count != 2 ||
        loop.connection_manager->count != 3 ||
        sessions[0].state != PAPACC_SESSION_STATE_ACTIVE) {
        result = 3;
        goto cleanup;
    }
    malformed = papacc_test_connect(port);
    if (malformed == INVALID_SOCKET ||
        papacc_test_drive(&loop, 2) != PAPACC_RESULT_OK ||
        send(malformed, "BAD!", 4, 0) != 4 ||
        send(malformed, (const char *)&papacc_test_open[4], 16, 0) != 16 ||
        papacc_test_drive(&loop, 4) != PAPACC_RESULT_OK ||
        loop.connection_manager->count != 3 ||
        loop.session_manager.count != 2 || loop.channel_manager.count != 2) {
        result = 4;
        goto cleanup;
    }
    for (index = 0; index < PAPACC_TEST_CAPACITY; ++index) {
        if (processors[index].in_use == PAPACC_TRUE &&
            papacc_control_processor_is_established(
                &processors[index].control_processor) != PAPACC_TRUE)
            processors[index].classifier.establishment_deadline_ns = 0;
    }
    if (papacc_test_drive(&loop, 1) != PAPACC_RESULT_OK ||
        loop.connection_manager->count != 2) {
        result = 5;
        goto cleanup;
    }
    truncated = papacc_test_connect(port);
    if (truncated == INVALID_SOCKET ||
        papacc_test_drive(&loop, 2) != PAPACC_RESULT_OK ||
        send(truncated, (const char *)papacc_test_open, 8, 0) != 8 ||
        shutdown(truncated, SD_SEND) != 0 ||
        papacc_test_drive(&loop, 5) != PAPACC_RESULT_OK ||
        loop.connection_manager->count != 2 ||
        loop.session_manager.count != 2 || loop.channel_manager.count != 2) {
        result = 6;
        goto cleanup;
    }
    {
        PAPACC_SIZE first_order[PAPACC_TEST_CAPACITY];
        PAPACC_SIZE second_order[PAPACC_TEST_CAPACITY];
        PAPACC_SIZE first_ready_position = PAPACC_TEST_CAPACITY;
        PAPACC_SIZE second_ready_position = PAPACC_TEST_CAPACITY;
        round_robin_a = papacc_test_connect(port);
        round_robin_b = papacc_test_connect(port);
        if (round_robin_a == INVALID_SOCKET ||
            round_robin_b == INVALID_SOCKET ||
            papacc_test_drive(&loop, 3) != PAPACC_RESULT_OK ||
            loop.connection_manager->count != 4 ||
            send(round_robin_a, "a", 1, 0) != 1 ||
            send(round_robin_b, "b", 1, 0) != 1) {
            result = 18;
            goto cleanup;
        }
        for (index = 0; index < PAPACC_TEST_CAPACITY; ++index) {
            if (papacc_server_io_loop_win32_processor_scan_index(
                    &loop, index, &first_order[index]) != PAPACC_RESULT_OK) {
                result = 19;
                goto cleanup;
            }
            if (papacc_control_processor_is_established(
                    &processors[first_order[index]].control_processor) != PAPACC_TRUE) {
                if (first_ready_position == PAPACC_TEST_CAPACITY)
                    first_ready_position = index;
                else if (second_ready_position == PAPACC_TEST_CAPACITY)
                    second_ready_position = index;
            }
        }
        if (first_ready_position == PAPACC_TEST_CAPACITY ||
            second_ready_position == PAPACC_TEST_CAPACITY ||
            papacc_test_drive(&loop, 1) != PAPACC_RESULT_OK) {
            result = 20;
            goto cleanup;
        }
        for (index = 0; index < PAPACC_TEST_CAPACITY; ++index) {
            if (papacc_server_io_loop_win32_processor_scan_index(
                    &loop, index, &second_order[index]) != PAPACC_RESULT_OK ||
                second_order[index] !=
                    first_order[(index + 1U) % PAPACC_TEST_CAPACITY]) {
                result = 21;
                goto cleanup;
            }
        }
        (void)shutdown(round_robin_a, SD_SEND);
        (void)shutdown(round_robin_b, SD_SEND);
        if (papacc_test_drive(&loop, 4) != PAPACC_RESULT_OK ||
            loop.connection_manager->count != 2) {
            result = 22;
            goto cleanup;
        }
    }
    {
        PAPACC_SIZE saved_count = network.listener_set.count;
        PAPACC_SIZE saved_capacity = network.listener_set.capacity;
        PAPACC_SIZE connection_count = loop.connection_manager->count;
        PAPACC_SIZE session_count = loop.session_manager.count;
        PAPACC_SIZE channel_count = loop.channel_manager.count;
        network.listener_set.count = (PAPACC_SIZE)FD_SETSIZE + 1U;
        network.listener_set.capacity = (PAPACC_SIZE)FD_SETSIZE + 1U;
        if (papacc_server_io_loop_win32_poll_once(&loop, 0) !=
                PAPACC_RESULT_LIMIT_EXCEEDED ||
            loop.connection_manager->count != connection_count ||
            loop.session_manager.count != session_count ||
            loop.channel_manager.count != channel_count) {
            result = 23;
        }
        network.listener_set.count = saved_count;
        network.listener_set.capacity = saved_capacity;
    }
    if (result == 0) {
        PAPACC_U8 partial_open[10]={
            0x50,0x41,0x43,0x43,1,0,0,16,0,9};
        PAPACC_SIZE streams_before=loop.network_stream_manager.count;
        PAPACC_SERVER_PROTOCOL_SLOT_WIN32*partial_slot=NULL;
        PAPACC_SIZE partial_index;
        PAPACC_U64 partial_connection_id=0U;
        if(papacc_test_attach_data(&loop,port,established,
                &lifecycle_data)!=0){
            result=61;goto cleanup;
        }
        for(partial_index=0U;partial_index<loop.processor_capacity;
                ++partial_index){
            PAPACC_SERVER_PROTOCOL_SLOT_WIN32*candidate=
                &loop.processor_slots[partial_index];
            if(candidate->in_use==PAPACC_TRUE&&candidate->kind==
                    PAPACC_SERVER_PROTOCOL_SLOT_KIND_DATA_ATTACH&&
                candidate->network_data_protocol.state==
                    PAPACC_NETWORK_DATA_WAIT_OPEN)
                partial_slot=candidate;
        }
        if(partial_slot==NULL||send(lifecycle_data,(const char *)partial_open,
                (int)sizeof(partial_open),0)!=(int)sizeof(partial_open)||
            papacc_server_io_loop_win32_process_connection_once(&loop,
                partial_slot->connection_instance_id,PAPACC_TRUE,
                PAPACC_FALSE,0U)!=PAPACC_RESULT_OK){result=61;goto cleanup;}
        partial_connection_id=partial_slot->connection_instance_id;
        (void)shutdown(lifecycle_data,SD_BOTH);
        (void)closesocket(lifecycle_data);lifecycle_data=INVALID_SOCKET;
        for(partial_index=0U;partial_index<100U&&
            papacc_server_io_loop_win32_find_connection_slot(&loop,
                partial_connection_id)!=NULL;++partial_index){
            if(papacc_server_io_loop_win32_poll_once(&loop,10U)!=
                    PAPACC_RESULT_OK){result=62;goto cleanup;}
        }
        if(papacc_test_maintain(&loop,8U)!=PAPACC_RESULT_OK||
            papacc_server_io_loop_win32_find_connection_slot(&loop,
                partial_connection_id)!=NULL||
            loop.network_stream_manager.count!=streams_before||
            loop.network_job_manager.count!=0U){
            result=62;goto cleanup;
        }
    }
    if (result == 0) {
        PAPACC_SERVER_PROTOCOL_SLOT_WIN32*stream_slot_a=NULL;
        PAPACC_SERVER_PROTOCOL_SLOT_WIN32*stream_slot_b=NULL;
        PAPACC_U8 isolated_byte=0xC3U,observed_byte=0U;
        PAPACC_U64 stream_owner_session=0U;
        struct linger abortive_close;
        if(papacc_test_open_fixture_stream(&loop,port,established,
                fixture_listener,egress_context.allowed.port,
                &cascade_stream_data_a,&cascade_stream_peer_a,
                &stream_slot_a)!=0||
            papacc_test_open_fixture_stream(&loop,port,established,
                fixture_listener,egress_context.allowed.port,
                &cascade_stream_data_b,&cascade_stream_peer_b,
                &stream_slot_b)!=0||loop.network_stream_manager.count!=2U){
            result=53;goto cleanup;
        }
        abortive_close.l_onoff=1;abortive_close.l_linger=0;
        (void)setsockopt(cascade_stream_peer_a,SOL_SOCKET,SO_LINGER,
            (const char *)&abortive_close,(int)sizeof(abortive_close));
        (void)closesocket(cascade_stream_peer_a);
        cascade_stream_peer_a=INVALID_SOCKET;
        if(papacc_test_drive(&loop,8)!=PAPACC_RESULT_OK){result=54;goto cleanup;}
        if(loop.network_stream_manager.count!=1U){result=56;goto cleanup;}
        if(papacc_test_send_network_data(&loop,stream_slot_b,
                cascade_stream_data_b,&isolated_byte,1U)!=0){result=57;goto cleanup;}
        if(recv(cascade_stream_peer_b,(char *)&observed_byte,1,
                MSG_WAITALL)!=1||observed_byte!=isolated_byte){result=58;goto cleanup;}
        {
            PAPACC_SERVER_PROTOCOL_SLOT_WIN32*loss_slot=NULL;
            if(papacc_test_open_fixture_stream(&loop,port,established,
                    fixture_listener,egress_context.allowed.port,&lifecycle_data,
                    &lifecycle_peer,&loss_slot)!=0||
                loop.network_stream_manager.count!=2U){result=59;goto cleanup;}
            (void)shutdown(lifecycle_data,SD_BOTH);
            (void)closesocket(lifecycle_data);lifecycle_data=INVALID_SOCKET;
            if(papacc_test_drive(&loop,8)!=PAPACC_RESULT_OK||
                loop.network_stream_manager.count!=1U){result=60;goto cleanup;}
            (void)closesocket(lifecycle_peer);lifecycle_peer=INVALID_SOCKET;
        }
        stream_owner_session=stream_slot_b->network_data_protocol.session_instance_id;
        (void)shutdown(established, SD_SEND);
        if(papacc_test_drive(&loop,8)!=PAPACC_RESULT_OK||
            loop.network_stream_manager.count!=0U||
            papacc_session_capability_store_find(&loop.capability_store,
                stream_owner_session)!=NULL){
            result=55;goto cleanup;
        }
        (void)shutdown(valid, SD_SEND);
        if (papacc_test_drive(&loop, 8) != PAPACC_RESULT_OK ||
            loop.connection_manager->count != 0 || loop.session_manager.count != 0 ||
            loop.channel_manager.count != 0) {
            result = 27;
            goto cleanup;
        }
        cascade_control = papacc_test_connect(port);
        if (cascade_control == INVALID_SOCKET || papacc_test_drive(&loop, 2) !=
                PAPACC_RESULT_OK ||
            send(cascade_control, (const char *)papacc_test_open, 20, 0) != 20 ||
            papacc_test_drive(&loop, 8) != PAPACC_RESULT_OK ||
            recv(cascade_control, (char *)received, 20, MSG_WAITALL) != 20 ||
            papacc_test_attach_data(&loop, port, cascade_control,
                &cascade_data_a) != 0 ||
            papacc_test_attach_data(&loop, port, cascade_control,
                &cascade_data_b) != 0 || loop.session_manager.count != 1 ||
            loop.channel_manager.count != 3 || loop.connection_manager->count != 3) {
            result = 28;
            goto cleanup;
        }
        (void)shutdown(cascade_control, SD_SEND);
        if (papacc_test_drive(&loop, 10) != PAPACC_RESULT_OK ||
            loop.session_manager.count != 0 || loop.channel_manager.count != 0 ||
            loop.connection_manager->count != 0 || loop.association_manager.count != 0) {
            result = 29;
            goto cleanup;
        }
        recovery_control = papacc_test_connect(port);
        if (recovery_control == INVALID_SOCKET || papacc_test_drive(&loop, 2) !=
                PAPACC_RESULT_OK ||
            send(recovery_control, (const char *)papacc_test_open, 20, 0) != 20 ||
            papacc_test_drive(&loop, 8) != PAPACC_RESULT_OK ||
            recv(recovery_control, (char *)received, 20, MSG_WAITALL) != 20 ||
            memcmp(received, papacc_test_accept, 20) != 0) result = 30;
    }
    if (result == 0) {
        PAPACC_U8 recovery_ticket[16];
        PAPACC_U8 second_ticket[16];
        PAPACC_U8 invalid_ticket[16];
        PAPACC_U8 invalid_attach[32] = {
            0x50,0x41,0x43,0x43,1,0,0,16,0,5,0,0,0,0,0,16 };
        PAPACC_U64 data_a = 0, data_b = 0;
        PAPACC_U64 maintenance_connection_id = 0;
        PAPACC_SIZE connections_before_maintenance;
        PAPACC_SIZE channels_before_maintenance;
        PAPACC_SIZE connections_after_maintenance;
        PAPACC_SIZE channels_after_maintenance;
        PAPACC_SIZE index2;
        memset(invalid_ticket, 0xa5, sizeof(invalid_ticket));
        second_control = papacc_test_connect(port);
        if (second_control == INVALID_SOCKET || papacc_test_drive(&loop, 2) !=
                PAPACC_RESULT_OK ||
            send(second_control, (const char *)papacc_test_open, 20, 0) != 20 ||
            papacc_test_drive(&loop, 8) != PAPACC_RESULT_OK ||
            recv(second_control, (char *)received, 20, MSG_WAITALL) != 20 ||
            papacc_test_request_ticket(&loop, recovery_control,
                recovery_ticket) != 0 ||
            papacc_test_request_ticket(&loop, second_control,
                second_ticket) != 0 ||
            memcmp(recovery_ticket, second_ticket, 16) == 0) {
            result = 31;
            goto cleanup;
        }
        memcpy(&invalid_attach[16], invalid_ticket, 16);
        invalid_data = papacc_test_connect(port);
        if (invalid_data == INVALID_SOCKET || papacc_test_drive(&loop, 2) !=
                PAPACC_RESULT_OK ||
            send(invalid_data, (const char *)invalid_attach, 32, 0) != 32 ||
            papacc_test_drive(&loop, 8) != PAPACC_RESULT_OK ||
            recv(invalid_data, (char *)received, 1, 0) != 0 ||
            loop.session_manager.count != 2) {
            result = 32;
            goto cleanup;
        }
        if (papacc_test_attach_ticket(&loop, port, recovery_ticket,
                &recovery_data) != 0) {
            result = 33;
            goto cleanup;
        }
        replay_data = papacc_test_connect(port);
        memcpy(&invalid_attach[16], recovery_ticket, 16);
        if (replay_data == INVALID_SOCKET || papacc_test_drive(&loop, 2) !=
                PAPACC_RESULT_OK ||
            send(replay_data, (const char *)invalid_attach, 32, 0) != 32 ||
            papacc_test_drive(&loop, 8) != PAPACC_RESULT_OK ||
            recv(replay_data, (char *)received, 1, 0) != 0 ||
            papacc_test_attach_ticket(&loop, port, second_ticket,
                &second_data) != 0 || loop.session_manager.count != 2 ||
            loop.channel_manager.count != 4) {
            result = 34;
            goto cleanup;
        }
        for (index2 = 0; index2 < loop.channel_manager.capacity; ++index2) {
            PAPACC_CHANNEL *channel = &loop.channel_manager.storage[index2];
            if (channel->state == PAPACC_CHANNEL_STATE_BOUND &&
                channel->role == PAPACC_CHANNEL_ROLE_DATA) {
                if (maintenance_connection_id == 0)
                    maintenance_connection_id = channel->connection_instance_id;
                if (data_a == 0) data_a = channel->session_instance_id;
                else data_b = channel->session_instance_id;
            }
        }
        if (data_a == 0 || data_b == 0 || data_a == data_b ||
            maintenance_connection_id == 0) result = 35;
        if (result == 0) {
            PAPACC_CONNECTION *maintenance_connection;
            connections_before_maintenance = loop.connection_manager->count;
            channels_before_maintenance = loop.channel_manager.count;
            if (papacc_server_io_loop_win32_maintenance(&loop, 1) !=
                    PAPACC_RESULT_OK ||
                loop.connection_manager->count !=
                    connections_before_maintenance ||
                loop.channel_manager.count != channels_before_maintenance) {
                result = 36;
                goto cleanup;
            }
            maintenance_connection = papacc_connection_manager_find(
                loop.connection_manager, maintenance_connection_id);
            if (maintenance_connection == NULL) {
                result = 37;
                goto cleanup;
            }
            papacc_connection_close(maintenance_connection);
            if (papacc_server_io_loop_win32_maintenance(&loop, 2) !=
                    PAPACC_RESULT_OK) {
                result = 38;
                goto cleanup;
            }
            if (loop.connection_manager->count !=
                    connections_before_maintenance - 1U) {
                result = 40;
                goto cleanup;
            }
            if (loop.channel_manager.count >= channels_before_maintenance) {
                result = 41;
                goto cleanup;
            }
            if (papacc_server_io_loop_win32_find_connection_slot(
                    &loop, maintenance_connection_id) != NULL) {
                result = 42;
                goto cleanup;
            }
            connections_after_maintenance = loop.connection_manager->count;
            channels_after_maintenance = loop.channel_manager.count;
            if (papacc_server_io_loop_win32_maintenance(&loop, 3) !=
                    PAPACC_RESULT_OK ||
                loop.connection_manager->count !=
                    connections_after_maintenance ||
                loop.channel_manager.count != channels_after_maintenance)
                result = 39;
        }
    }

cleanup:
    papacc_server_io_loop_win32_shutdown(&loop);
    papacc_server_acceptor_win32_shutdown(&acceptor);
    if (cascade_stream_peer_b != INVALID_SOCKET) (void)closesocket(cascade_stream_peer_b);
    if (cascade_stream_peer_a != INVALID_SOCKET) (void)closesocket(cascade_stream_peer_a);
    if (cascade_stream_data_b != INVALID_SOCKET) (void)closesocket(cascade_stream_data_b);
    if (cascade_stream_data_a != INVALID_SOCKET) (void)closesocket(cascade_stream_data_a);
    if (lifecycle_peer != INVALID_SOCKET) (void)closesocket(lifecycle_peer);
    if (lifecycle_data != INVALID_SOCKET) (void)closesocket(lifecycle_data);
    if (round_robin_b != INVALID_SOCKET) (void)closesocket(round_robin_b);
    if (recovery_control != INVALID_SOCKET) (void)closesocket(recovery_control);
    if (replay_data != INVALID_SOCKET) (void)closesocket(replay_data);
    if (invalid_data != INVALID_SOCKET) (void)closesocket(invalid_data);
    if (second_data != INVALID_SOCKET) (void)closesocket(second_data);
    if (recovery_data != INVALID_SOCKET) (void)closesocket(recovery_data);
    if (second_control != INVALID_SOCKET) (void)closesocket(second_control);
    if (cascade_data_b != INVALID_SOCKET) (void)closesocket(cascade_data_b);
    if (cascade_data_a != INVALID_SOCKET) (void)closesocket(cascade_data_a);
    if (cascade_control != INVALID_SOCKET) (void)closesocket(cascade_control);
    if (round_robin_a != INVALID_SOCKET) (void)closesocket(round_robin_a);
    if (channel_fail != INVALID_SOCKET) (void)closesocket(channel_fail);
    if (session_fail != INVALID_SOCKET) (void)closesocket(session_fail);
    if (established != INVALID_SOCKET) (void)closesocket(established);
    if (data_client != INVALID_SOCKET) (void)closesocket(data_client);
    if (fixture_peer != INVALID_SOCKET) (void)closesocket(fixture_peer);
    if (fixture_listener != INVALID_SOCKET) (void)closesocket(fixture_listener);
    if (rejected != INVALID_SOCKET) (void)closesocket(rejected);
    if (truncated != INVALID_SOCKET) (void)closesocket(truncated);
    if (malformed != INVALID_SOCKET) (void)closesocket(malformed);
    if (valid != INVALID_SOCKET) (void)closesocket(valid);
    if (idle != INVALID_SOCKET) (void)closesocket(idle);
    papacc_tcp_socket_win32_close(&entry.socket);
    papacc_tcp_platform_shutdown(&network.tcp_platform);
    return result;
}

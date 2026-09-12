#include "data_association.h"

#define CHECK(x,n) do { if (!(x)) return (n); } while (0)

typedef struct GENERATOR { PAPACC_U8 value; } GENERATOR;
static PAPACC_RESULT generate(void *context,
    PAPACC_DATA_ASSOCIATION_TICKET *ticket)
{
    GENERATOR *generator = (GENERATOR *)context;
    ticket->bytes[0] = ++generator->value;
    return PAPACC_RESULT_OK;
}
static PAPACC_RESULT read_stub(void *c, PAPACC_U8 *b, PAPACC_SIZE n,
    PAPACC_SIZE *o, PAPACC_TRANSPORT_IO_STATUS *s)
{ (void)c;(void)b;(void)n;*o=0;*s=PAPACC_TRANSPORT_IO_STATUS_WOULD_BLOCK;return PAPACC_RESULT_OK; }
static PAPACC_RESULT write_stub(void *c, const PAPACC_U8 *b, PAPACC_SIZE n,
    PAPACC_SIZE *o, PAPACC_TRANSPORT_IO_STATUS *s)
{ (void)c;(void)b;*o=n;*s=PAPACC_TRANSPORT_IO_STATUS_PROGRESS;return PAPACC_RESULT_OK; }
static void close_stub(void *c) { (void)c; }

static PAPACC_RESULT publish_connection(PAPACC_CONNECTION_MANAGER *manager,
    PAPACC_CONNECTION **out)
{
    PAPACC_TRANSPORT_CONNECTION transport = PAPACC_TRANSPORT_CONNECTION_INITIALIZER;
    PAPACC_NETWORK_ENDPOINT endpoint = PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    static int contexts[4];
    transport.context = &contexts[manager->count];
    transport.read_fn = read_stub; transport.write_fn = write_stub;
    transport.close_fn = close_stub;
    (void)papacc_ip_address_set_ipv4(&endpoint.address,127,0,0,1);
    endpoint.port = 1;
    return papacc_connection_manager_publish(manager, &transport,
        &endpoint, &endpoint, out);
}

int main(void)
{
    PAPACC_CONNECTION_MANAGER connections=PAPACC_CONNECTION_MANAGER_INITIALIZER;
    PAPACC_SESSION_MANAGER sessions=PAPACC_SESSION_MANAGER_INITIALIZER;
    PAPACC_CHANNEL_MANAGER channels=PAPACC_CHANNEL_MANAGER_INITIALIZER;
    PAPACC_DATA_ASSOCIATION_MANAGER associations=PAPACC_DATA_ASSOCIATION_MANAGER_INITIALIZER;
    PAPACC_CONNECTION connection_storage[4]; PAPACC_SESSION session_storage[2];
    PAPACC_CHANNEL channel_storage[4]; PAPACC_DATA_ASSOCIATION_ENTRY entries[2];
    PAPACC_CONNECTION *control_connection; PAPACC_SESSION *session;
    PAPACC_CHANNEL *control; PAPACC_DATA_ASSOCIATION_TICKET ticket, second;
    PAPACC_U64 deadline, resolved; GENERATOR generator={0};
    CHECK(papacc_connection_manager_init(&connections,connection_storage,4)==PAPACC_RESULT_OK,1);
    CHECK(papacc_session_manager_init(&sessions,session_storage,2)==PAPACC_RESULT_OK,2);
    CHECK(papacc_channel_manager_init(&channels,channel_storage,4,&connections,&sessions)==PAPACC_RESULT_OK,3);
    CHECK(papacc_data_association_manager_init(&associations,entries,2,&sessions,&channels,generate,&generator,10)==PAPACC_RESULT_OK,4);
    CHECK(publish_connection(&connections,&control_connection)==PAPACC_RESULT_OK,5);
    CHECK(papacc_session_manager_publish(&sessions,&session)==PAPACC_RESULT_OK,6);
    CHECK(papacc_channel_manager_bind(&channels,session->session_instance_id,control_connection->connection_instance_id,PAPACC_CHANNEL_ROLE_CONTROL,&control)==PAPACC_RESULT_OK,7);
    CHECK(papacc_session_activate(session)==PAPACC_RESULT_OK,8);

    CHECK(papacc_data_association_manager_issue(&associations,session->session_instance_id,100,&ticket,&deadline)==PAPACC_RESULT_OK,9);
    CHECK(papacc_data_association_manager_inspect(&associations,&ticket,101,&resolved)==PAPACC_RESULT_OK&&resolved==session->session_instance_id&&associations.count==1,10);
    CHECK(papacc_data_association_manager_commit(&associations,&ticket,session->session_instance_id+1,101)==PAPACC_RESULT_INVALID_STATE&&associations.count==1,11);
    CHECK(papacc_data_association_manager_commit(&associations,&ticket,session->session_instance_id,101)==PAPACC_RESULT_OK&&associations.count==0,12);
    CHECK(papacc_data_association_manager_commit(&associations,&ticket,session->session_instance_id,101)==PAPACC_RESULT_INVALID_STATE,13);

    CHECK(papacc_data_association_manager_issue(&associations,session->session_instance_id,200,&ticket,&deadline)==PAPACC_RESULT_OK,14);
    CHECK(papacc_data_association_manager_inspect(&associations,&ticket,201,&resolved)==PAPACC_RESULT_OK,15);
    CHECK(papacc_data_association_manager_commit(&associations,&ticket,session->session_instance_id,deadline)==PAPACC_RESULT_INVALID_STATE&&associations.count==0,16);
    CHECK(papacc_data_association_manager_issue(&associations,session->session_instance_id,300,&ticket,&deadline)==PAPACC_RESULT_OK,17);
    CHECK(papacc_data_association_manager_inspect(&associations,&ticket,301,&resolved)==PAPACC_RESULT_OK,18);
    CHECK(papacc_data_association_manager_consume(&associations,&ticket,301,&resolved)==PAPACC_RESULT_OK,19);
    CHECK(papacc_data_association_manager_commit(&associations,&ticket,session->session_instance_id,301)==PAPACC_RESULT_INVALID_STATE,20);
    CHECK(papacc_data_association_manager_issue(&associations,session->session_instance_id,400,&second,&deadline)==PAPACC_RESULT_OK,21);
    CHECK(papacc_data_association_manager_inspect(&associations,&second,401,&resolved)==PAPACC_RESULT_OK,22);
    CHECK(papacc_channel_manager_close(&channels,control->channel_instance_id)==PAPACC_RESULT_OK,23);
    CHECK(papacc_data_association_manager_commit(&associations,&second,session->session_instance_id,401)==PAPACC_RESULT_INVALID_STATE&&associations.count==0,24);
    papacc_data_association_manager_shutdown(&associations);
    papacc_channel_manager_shutdown(&channels);papacc_session_manager_shutdown(&sessions);papacc_connection_manager_shutdown(&connections);
    return 0;
}

#include "secure_data_association.h"

#define CHECK(x,n) do { if (!(x)) return (n); } while (0)

typedef struct POLICY {
    PAPACC_AUTHORIZATION_DECISION decision;
    PAPACC_RESULT result;
    PAPACC_U32 calls;
} POLICY;
typedef struct SECURITY_REGISTRY {
    PAPACC_U64 ids[2];
    PAPACC_SESSION_SECURITY_CONTEXT contexts[2];
    PAPACC_BOOL present[2];
    PAPACC_U32 lookup_calls;
    PAPACC_BOOL invalidate_on_second_lookup;
} SECURITY_REGISTRY;
typedef struct GENERATOR { PAPACC_U8 value; } GENERATOR;

static PAPACC_RESULT authorize(void *context, const PAPACC_PRINCIPAL *principal,
    PAPACC_AUTHORIZATION_ACTION action, PAPACC_AUTHORIZATION_DECISION *decision)
{
    POLICY *policy=(POLICY *)context;(void)principal;
    if(action!=PAPACC_AUTHORIZATION_ASSOCIATE_DATA)return PAPACC_RESULT_INVALID_ARGUMENT;
    ++policy->calls;*decision=policy->decision;return policy->result;
}
static PAPACC_RESULT lookup(void *context,PAPACC_U64 id,
    const PAPACC_SESSION_SECURITY_CONTEXT **out)
{
    SECURITY_REGISTRY *registry=(SECURITY_REGISTRY *)context;PAPACC_SIZE i;
    *out=NULL;++registry->lookup_calls;
    if(registry->invalidate_on_second_lookup&&registry->lookup_calls==2)return PAPACC_RESULT_INVALID_STATE;
    for(i=0;i<2;++i)if(registry->present[i]&&registry->ids[i]==id){*out=&registry->contexts[i];return PAPACC_RESULT_OK;}
    return PAPACC_RESULT_INVALID_STATE;
}
static PAPACC_RESULT generate(void *context,PAPACC_DATA_ASSOCIATION_TICKET *ticket)
{ GENERATOR *g=(GENERATOR *)context;ticket->bytes[0]=++g->value;return PAPACC_RESULT_OK; }
static PAPACC_RESULT rd(void*c,PAPACC_U8*b,PAPACC_SIZE n,PAPACC_SIZE*o,PAPACC_TRANSPORT_IO_STATUS*s)
{(void)c;(void)b;(void)n;*o=0;*s=PAPACC_TRANSPORT_IO_STATUS_WOULD_BLOCK;return PAPACC_RESULT_OK;}
static PAPACC_RESULT wr(void*c,const PAPACC_U8*b,PAPACC_SIZE n,PAPACC_SIZE*o,PAPACC_TRANSPORT_IO_STATUS*s)
{(void)c;(void)b;*o=n;*s=PAPACC_TRANSPORT_IO_STATUS_PROGRESS;return PAPACC_RESULT_OK;}
static void cl(void*c){(void)c;}
static PAPACC_RESULT publish_connection(PAPACC_CONNECTION_MANAGER *manager,
    PAPACC_CONNECTION **out)
{
    static int contexts[16];PAPACC_TRANSPORT_CONNECTION t=PAPACC_TRANSPORT_CONNECTION_INITIALIZER;
    PAPACC_NETWORK_ENDPOINT e=PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    t.context=&contexts[manager->count];t.read_fn=rd;t.write_fn=wr;t.close_fn=cl;
    (void)papacc_ip_address_set_ipv4(&e.address,127,0,0,1);e.port=1;
    return papacc_connection_manager_publish(manager,&t,&e,&e,out);
}
static PAPACC_RESULT create_session(PAPACC_CONNECTION_MANAGER *connections,
    PAPACC_SESSION_MANAGER *sessions,PAPACC_CHANNEL_MANAGER *channels,
    PAPACC_SESSION **out_session)
{
    PAPACC_CONNECTION *connection;PAPACC_CHANNEL *control;
    PAPACC_RESULT r=publish_connection(connections,&connection);if(r!=PAPACC_RESULT_OK)return r;
    r=papacc_session_manager_publish(sessions,out_session);if(r!=PAPACC_RESULT_OK)return r;
    r=papacc_channel_manager_bind(channels,(*out_session)->session_instance_id,
        connection->connection_instance_id,PAPACC_CHANNEL_ROLE_CONTROL,&control);
    return r==PAPACC_RESULT_OK?papacc_session_activate(*out_session):r;
}
static PAPACC_CONNECTION_SECURITY_CONTEXT connection_context(PAPACC_U8 value)
{
    PAPACC_CONNECTION_SECURITY_CONTEXT c=PAPACC_CONNECTION_SECURITY_CONTEXT_INITIALIZER;
    c.state=PAPACC_SECURITY_CONTEXT_AUTHORIZED;c.principal.valid=PAPACC_TRUE;c.principal.value[0]=value;return c;
}
static PAPACC_SESSION_SECURITY_CONTEXT session_context(PAPACC_U8 value)
{
    PAPACC_CONNECTION_SECURITY_CONTEXT c=connection_context(value);
    PAPACC_SESSION_SECURITY_CONTEXT s=PAPACC_SESSION_SECURITY_CONTEXT_INITIALIZER;
    (void)papacc_session_security_context_publish(&s,&c);return s;
}

int main(void)
{
    PAPACC_CONNECTION_MANAGER connections=PAPACC_CONNECTION_MANAGER_INITIALIZER;
    PAPACC_SESSION_MANAGER sessions=PAPACC_SESSION_MANAGER_INITIALIZER;
    PAPACC_CHANNEL_MANAGER channels=PAPACC_CHANNEL_MANAGER_INITIALIZER;
    PAPACC_DATA_ASSOCIATION_MANAGER associations=PAPACC_DATA_ASSOCIATION_MANAGER_INITIALIZER;
    PAPACC_CONNECTION connection_storage[16],*data;PAPACC_SESSION session_storage[2],*s1,*s2;
    PAPACC_CHANNEL channel_storage[16];PAPACC_DATA_ASSOCIATION_ENTRY entries[2];
    SECURITY_REGISTRY registry;GENERATOR generator={0};POLICY policy={PAPACC_AUTHORIZATION_ALLOW,PAPACC_RESULT_OK,0};
    PAPACC_CONNECTION_SECURITY_CONTEXT a=connection_context(1),b=connection_context(2),invalid=PAPACC_CONNECTION_SECURITY_CONTEXT_INITIALIZER;
    PAPACC_DATA_ASSOCIATION_TICKET ticket;PAPACC_U64 deadline,session_id,channel_id;
    PAPACC_SECURE_DATA_OUTCOME outcome;PAPACC_SIZE before;
    CHECK(papacc_connection_manager_init(&connections,connection_storage,16)==PAPACC_RESULT_OK,1);
    CHECK(papacc_session_manager_init(&sessions,session_storage,2)==PAPACC_RESULT_OK,2);
    CHECK(papacc_channel_manager_init(&channels,channel_storage,16,&connections,&sessions)==PAPACC_RESULT_OK,3);
    CHECK(papacc_data_association_manager_init(&associations,entries,2,&sessions,&channels,generate,&generator,100)==PAPACC_RESULT_OK,4);
    CHECK(create_session(&connections,&sessions,&channels,&s1)==PAPACC_RESULT_OK&&create_session(&connections,&sessions,&channels,&s2)==PAPACC_RESULT_OK,5);
    registry.ids[0]=s1->session_instance_id;registry.ids[1]=s2->session_instance_id;
    registry.contexts[0]=session_context(1);registry.contexts[1]=session_context(2);
    registry.present[0]=registry.present[1]=PAPACC_TRUE;

    CHECK(papacc_secure_data_ticket_issue(&associations,s1->session_instance_id,&registry.contexts[0],authorize,&policy,1,&ticket,&deadline,&outcome)==PAPACC_RESULT_OK&&outcome==PAPACC_SECURE_DATA_OUTCOME_ACCEPTED&&associations.count==1,6);
    CHECK(publish_connection(&connections,&data)==PAPACC_RESULT_OK,7);
    CHECK(papacc_secure_data_attach(&associations,&channels,&ticket,2,data->connection_instance_id,&b,lookup,&registry,authorize,&policy,&session_id,&channel_id,&outcome)==PAPACC_RESULT_OK&&outcome==PAPACC_SECURE_DATA_OUTCOME_PRINCIPAL_MISMATCH&&associations.count==1&&channel_id==0,8);
    CHECK(publish_connection(&connections,&data)==PAPACC_RESULT_OK,9);
    CHECK(papacc_secure_data_attach(&associations,&channels,&ticket,2,data->connection_instance_id,&a,lookup,&registry,authorize,&policy,&session_id,&channel_id,&outcome)==PAPACC_RESULT_OK&&outcome==PAPACC_SECURE_DATA_OUTCOME_ACCEPTED&&session_id==s1->session_instance_id&&channel_id!=0&&associations.count==0,10);
    CHECK(publish_connection(&connections,&data)==PAPACC_RESULT_OK,11);
    CHECK(papacc_secure_data_attach(&associations,&channels,&ticket,2,data->connection_instance_id,&a,lookup,&registry,authorize,&policy,&session_id,&channel_id,&outcome)==PAPACC_RESULT_OK&&outcome==PAPACC_SECURE_DATA_OUTCOME_STRUCTURAL_TICKET_INVALID,12);

    CHECK(papacc_secure_data_ticket_issue(&associations,s1->session_instance_id,&registry.contexts[0],authorize,&policy,10,&ticket,&deadline,&outcome)==PAPACC_RESULT_OK&&outcome==PAPACC_SECURE_DATA_OUTCOME_ACCEPTED,13);
    CHECK(publish_connection(&connections,&data)==PAPACC_RESULT_OK,14);policy.decision=PAPACC_AUTHORIZATION_DENY;
    CHECK(papacc_secure_data_attach(&associations,&channels,&ticket,11,data->connection_instance_id,&a,lookup,&registry,authorize,&policy,&session_id,&channel_id,&outcome)==PAPACC_RESULT_OK&&outcome==PAPACC_SECURE_DATA_OUTCOME_AUTHORIZATION_DENIED&&associations.count==1,15);
    policy.decision=PAPACC_AUTHORIZATION_ALLOW;
    CHECK(papacc_secure_data_attach(&associations,&channels,&ticket,11,data->connection_instance_id,&a,lookup,&registry,authorize,&policy,&session_id,&channel_id,&outcome)==PAPACC_RESULT_OK&&outcome==PAPACC_SECURE_DATA_OUTCOME_ACCEPTED&&associations.count==0,16);

    CHECK(papacc_secure_data_ticket_issue(&associations,s1->session_instance_id,&registry.contexts[0],authorize,&policy,20,&ticket,&deadline,&outcome)==PAPACC_RESULT_OK,17);
    CHECK(publish_connection(&connections,&data)==PAPACC_RESULT_OK,18);policy.result=PAPACC_RESULT_INTERNAL_ERROR;
    CHECK(papacc_secure_data_attach(&associations,&channels,&ticket,21,data->connection_instance_id,&a,lookup,&registry,authorize,&policy,&session_id,&channel_id,&outcome)==PAPACC_RESULT_OK&&outcome==PAPACC_SECURE_DATA_OUTCOME_AUTHORIZATION_ERROR&&associations.count==1,19);
    policy.result=PAPACC_RESULT_OK;before=associations.count;
    CHECK(papacc_secure_data_attach(&associations,&channels,&ticket,21,data->connection_instance_id,&invalid,lookup,&registry,authorize,&policy,&session_id,&channel_id,&outcome)==PAPACC_RESULT_OK&&outcome==PAPACC_SECURE_DATA_OUTCOME_DATA_CONTEXT_INVALID&&associations.count==before,20);
    registry.present[0]=PAPACC_FALSE;
    CHECK(papacc_secure_data_attach(&associations,&channels,&ticket,21,data->connection_instance_id,&a,lookup,&registry,authorize,&policy,&session_id,&channel_id,&outcome)==PAPACC_RESULT_OK&&outcome==PAPACC_SECURE_DATA_OUTCOME_SESSION_CONTEXT_INVALID&&associations.count==before,21);
    registry.present[0]=PAPACC_TRUE;
    registry.lookup_calls=0;registry.invalidate_on_second_lookup=PAPACC_TRUE;
    CHECK(papacc_secure_data_attach(&associations,&channels,&ticket,21,data->connection_instance_id,&a,lookup,&registry,authorize,&policy,&session_id,&channel_id,&outcome)==PAPACC_RESULT_OK&&outcome==PAPACC_SECURE_DATA_OUTCOME_COMMIT_CONFLICT&&associations.count==before,29);
    registry.invalidate_on_second_lookup=PAPACC_FALSE;
    CHECK(papacc_data_association_manager_invalidate_session(&associations,s1->session_instance_id)==PAPACC_RESULT_OK,22);

    before=associations.count;
    CHECK(papacc_secure_data_ticket_issue(&associations,s1->session_instance_id,NULL,authorize,&policy,30,&ticket,&deadline,&outcome)==PAPACC_RESULT_OK&&outcome==PAPACC_SECURE_DATA_OUTCOME_SESSION_CONTEXT_INVALID&&associations.count==before,23);
    policy.decision=PAPACC_AUTHORIZATION_DENY;
    CHECK(papacc_secure_data_ticket_issue(&associations,s1->session_instance_id,&registry.contexts[0],authorize,&policy,30,&ticket,&deadline,&outcome)==PAPACC_RESULT_OK&&outcome==PAPACC_SECURE_DATA_OUTCOME_AUTHORIZATION_DENIED&&associations.count==before,24);
    policy.decision=PAPACC_AUTHORIZATION_ERROR;
    CHECK(papacc_secure_data_ticket_issue(&associations,s1->session_instance_id,&registry.contexts[0],authorize,&policy,30,&ticket,&deadline,&outcome)==PAPACC_RESULT_OK&&outcome==PAPACC_SECURE_DATA_OUTCOME_AUTHORIZATION_ERROR&&associations.count==before,25);
    policy.decision=PAPACC_AUTHORIZATION_ALLOW;policy.result=PAPACC_RESULT_OK;

    registry.contexts[1]=session_context(1);
    CHECK(papacc_secure_data_ticket_issue(&associations,s1->session_instance_id,&registry.contexts[0],authorize,&policy,40,&ticket,&deadline,&outcome)==PAPACC_RESULT_OK&&outcome==PAPACC_SECURE_DATA_OUTCOME_ACCEPTED,26);
    CHECK(publish_connection(&connections,&data)==PAPACC_RESULT_OK,27);
    CHECK(papacc_secure_data_attach(&associations,&channels,&ticket,41,data->connection_instance_id,&a,lookup,&registry,authorize,&policy,&session_id,&channel_id,&outcome)==PAPACC_RESULT_OK&&outcome==PAPACC_SECURE_DATA_OUTCOME_ACCEPTED&&session_id==s1->session_instance_id&&session_id!=s2->session_instance_id,28);
    papacc_data_association_manager_shutdown(&associations);papacc_channel_manager_shutdown(&channels);papacc_session_manager_shutdown(&sessions);papacc_connection_manager_shutdown(&connections);return 0;
}

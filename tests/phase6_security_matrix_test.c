#include "builtin_capabilities.h"
#include "destination.h"
#include "egress_runtime_config.h"
#include "network_authorization.h"
#include "session_capability_store.h"
#include <string.h>
#include "exact_policy.h"

#define CHECK(condition, code) do { if (!(condition)) return (code); } while (0)

typedef struct TEST_POLICY_CONTEXT {
    PAPACC_EGRESS_DECISION host_decision;
    PAPACC_EGRESS_DECISION endpoint_decision;
    int host_calls;
    int endpoint_calls;
    PAPACC_U16 allowed_port;
} TEST_POLICY_CONTEXT;

typedef struct TEST_ADMIN_POLICY_CONTEXT {
    PAPACC_NETWORK_ENDPOINT denied_management;
    PAPACC_NETWORK_ENDPOINT denied_self_listener;
} TEST_ADMIN_POLICY_CONTEXT;

typedef struct TEST_RESOLVER_CONTEXT {
    PAPACC_NETWORK_ENDPOINT candidates[8];
    PAPACC_SIZE count;
    int calls;
} TEST_RESOLVER_CONTEXT;

typedef struct TEST_CAPABILITY_POLICY_CONTEXT {
    PAPACC_CAPABILITY_POLICY_DECISION decision;
    PAPACC_RESULT result;
} TEST_CAPABILITY_POLICY_CONTEXT;

typedef struct TEST_PROFILE_SELECTOR_CONTEXT {
    PAPACC_U32 profile_id;
    PAPACC_RESULT result;
} TEST_PROFILE_SELECTOR_CONTEXT;

static PAPACC_EGRESS_DECISION test_host_policy(void *context, const char *host,
    PAPACC_SIZE host_size, PAPACC_U16 port)
{
    TEST_POLICY_CONTEXT *state = (TEST_POLICY_CONTEXT *)context;
    ++state->host_calls;
    if (host == NULL || host_size == 0U || port == 0U ||
        (state->allowed_port != 0U && port != state->allowed_port))
        return PAPACC_EGRESS_ERROR;
    return state->host_decision;
}

static PAPACC_EGRESS_DECISION test_admin_endpoint_policy(void *context,
    const PAPACC_NETWORK_ENDPOINT *endpoint, PAPACC_U32 sensitive_classes)
{
    TEST_ADMIN_POLICY_CONTEXT *state=(TEST_ADMIN_POLICY_CONTEXT *)context;
    (void)sensitive_classes;
    if (papacc_network_endpoint_equal(endpoint,&state->denied_management) ||
        papacc_network_endpoint_equal(endpoint,&state->denied_self_listener))
        return PAPACC_EGRESS_DENY;
    return PAPACC_EGRESS_ALLOW;
}

static PAPACC_EGRESS_DECISION test_endpoint_policy(void *context,
    const PAPACC_NETWORK_ENDPOINT *endpoint, PAPACC_U32 sensitive_classes)
{
    TEST_POLICY_CONTEXT *state = (TEST_POLICY_CONTEXT *)context;
    (void)sensitive_classes;
    ++state->endpoint_calls;
    if (endpoint == NULL)
        return PAPACC_EGRESS_ERROR;
    return state->endpoint_decision;
}

static PAPACC_RESULT test_resolve(void *context, const char *host,
    PAPACC_SIZE host_size, PAPACC_U16 port, PAPACC_NETWORK_ENDPOINT *out,
    PAPACC_SIZE capacity, PAPACC_SIZE *count)
{
    TEST_RESOLVER_CONTEXT *state = (TEST_RESOLVER_CONTEXT *)context;
    PAPACC_SIZE i;
    (void)host;
    (void)host_size;
    ++state->calls;
    if (capacity < state->count)
        return PAPACC_RESULT_LIMIT_EXCEEDED;
    for (i = 0U; i < state->count; ++i) {
        out[i] = state->candidates[i];
        out[i].port = port;
    }
    *count = state->count;
    return PAPACC_RESULT_OK;
}

static PAPACC_RESULT test_capability_policy(void *context,
    const PAPACC_CAPABILITY_KEY *key,
    PAPACC_CAPABILITY_POLICY_DECISION *decision)
{
    TEST_CAPABILITY_POLICY_CONTEXT *state =
        (TEST_CAPABILITY_POLICY_CONTEXT *)context;
    (void)key;
    *decision = state->decision;
    return state->result;
}

static PAPACC_RESULT test_select_profile(void *context,
    const PAPACC_NETWORK_ENDPOINT *endpoint,
    PAPACC_NETWORK_TRANSPORT_MODE mode, PAPACC_U32 *profile_id)
{
    TEST_PROFILE_SELECTOR_CONTEXT *state =
        (TEST_PROFILE_SELECTOR_CONTEXT *)context;
    (void)endpoint;
    (void)mode;
    *profile_id = state->profile_id;
    return state->result;
}

static PAPACC_NETWORK_ENDPOINT ipv4(PAPACC_U8 a, PAPACC_U8 b, PAPACC_U8 c,
    PAPACC_U8 d, PAPACC_U16 port)
{
    PAPACC_NETWORK_ENDPOINT endpoint = PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    (void)papacc_ip_address_set_ipv4(&endpoint.address, a, b, c, d);
    endpoint.port = port;
    return endpoint;
}

static PAPACC_NETWORK_ENDPOINT ipv6(const PAPACC_U8 bytes[16], PAPACC_U16 port)
{
    PAPACC_NETWORK_ENDPOINT endpoint = PAPACC_NETWORK_ENDPOINT_INITIALIZER;
    (void)papacc_ip_address_set_ipv6(&endpoint.address, bytes);
    endpoint.port = port;
    return endpoint;
}

static int test_sensitive_destination_matrix(void)
{
    static const PAPACC_U8 loopback6[16] =
        {0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,1U};
    static const PAPACC_U8 link_local6[16] =
        {0xFEU,0x80U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,1U};
    static const PAPACC_U8 private6[16] =
        {0xFCU,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,1U};
    PAPACC_NETWORK_ENDPOINT endpoints[9];
    PAPACC_U32 expected[9];
    PAPACC_EGRESS_POLICY policy = PAPACC_EGRESS_POLICY_INITIALIZER;
    TEST_POLICY_CONTEXT state = {PAPACC_EGRESS_ALLOW,
        PAPACC_EGRESS_ALLOW, 0, 0, 0U};
    PAPACC_SIZE i;

    endpoints[0] = ipv4(127U,0U,0U,1U,443U);
    endpoints[1] = ipv6(loopback6,443U);
    endpoints[2] = ipv4(169U,254U,10U,20U,443U);
    endpoints[3] = ipv6(link_local6,443U);
    endpoints[4] = ipv4(10U,0U,0U,1U,443U);
    endpoints[5] = ipv4(172U,31U,255U,254U,443U);
    endpoints[6] = ipv4(192U,168U,1U,1U,443U);
    endpoints[7] = ipv6(private6,443U);
    endpoints[8] = ipv4(100U,64U,0U,1U,443U);
    expected[0] = PAPACC_EGRESS_SENSITIVE_LOOPBACK;
    expected[1] = PAPACC_EGRESS_SENSITIVE_LOOPBACK;
    expected[2] = PAPACC_EGRESS_SENSITIVE_LINK_LOCAL;
    expected[3] = PAPACC_EGRESS_SENSITIVE_LINK_LOCAL;
    expected[4] = PAPACC_EGRESS_SENSITIVE_PRIVATE;
    expected[5] = PAPACC_EGRESS_SENSITIVE_PRIVATE;
    expected[6] = PAPACC_EGRESS_SENSITIVE_PRIVATE;
    expected[7] = PAPACC_EGRESS_SENSITIVE_PRIVATE;
    expected[8] = PAPACC_EGRESS_SENSITIVE_SHARED;
    policy.master_enabled = PAPACC_TRUE;
    policy.endpoint_policy = test_endpoint_policy;
    policy.context = &state;
    for (i = 0U; i < 9U; ++i) {
        CHECK((papacc_egress_endpoint_sensitive_classes(&endpoints[i]) &
            expected[i]) == expected[i], 10 + (int)i);
        CHECK(papacc_egress_endpoint_allowed(&policy,&endpoints[i]) ==
            PAPACC_FALSE, 20 + (int)i);
        policy.explicitly_allowed_sensitive_classes = expected[i];
        CHECK(papacc_egress_endpoint_allowed(&policy,&endpoints[i]) ==
            PAPACC_TRUE, 30 + (int)i);
        policy.explicitly_allowed_sensitive_classes = 0U;
    }
    endpoints[0] = ipv4(169U,254U,169U,254U,443U);
    CHECK((papacc_egress_endpoint_sensitive_classes(&endpoints[0]) &
        (PAPACC_EGRESS_SENSITIVE_LINK_LOCAL |
         PAPACC_EGRESS_SENSITIVE_METADATA)) ==
        (PAPACC_EGRESS_SENSITIVE_LINK_LOCAL |
         PAPACC_EGRESS_SENSITIVE_METADATA), 40);
    policy.explicitly_allowed_sensitive_classes =
        PAPACC_EGRESS_SENSITIVE_METADATA;
    CHECK(papacc_egress_endpoint_allowed(&policy,&endpoints[0]) ==
        PAPACC_FALSE,41);
    policy.explicitly_allowed_sensitive_classes =
        PAPACC_EGRESS_SENSITIVE_LINK_LOCAL |
        PAPACC_EGRESS_SENSITIVE_METADATA;
    CHECK(papacc_egress_endpoint_allowed(&policy,&endpoints[0]) ==
        PAPACC_TRUE,42);
    policy.master_enabled = PAPACC_FALSE;
    endpoints[0] = ipv4(8U,8U,8U,8U,443U);
    CHECK(papacc_egress_endpoint_allowed(&policy,&endpoints[0]) ==
        PAPACC_FALSE,43);
    return 0;
}

static int test_structural_and_administrative_policy(void)
{
    PAPACC_NETWORK_ENDPOINT endpoint;
    PAPACC_EGRESS_POLICY policy = PAPACC_EGRESS_POLICY_INITIALIZER;
    TEST_POLICY_CONTEXT state = {PAPACC_EGRESS_ALLOW,
        PAPACC_EGRESS_ALLOW,0,0,0U};
    TEST_ADMIN_POLICY_CONTEXT admin;
    policy.master_enabled = PAPACC_TRUE;
    policy.endpoint_policy = test_endpoint_policy;
    policy.context = &state;
    endpoint = ipv4(8U,8U,8U,8U,0U);
    CHECK(papacc_egress_endpoint_allowed(&policy,&endpoint)==PAPACC_FALSE,50);
    endpoint = ipv4(0U,0U,0U,0U,443U);
    CHECK(papacc_egress_endpoint_allowed(&policy,&endpoint)==PAPACC_FALSE,51);
    endpoint = ipv4(224U,0U,0U,1U,443U);
    CHECK(papacc_egress_endpoint_allowed(&policy,&endpoint)==PAPACC_FALSE,52);
    endpoint = ipv4(203U,0U,113U,10U,443U);
    state.endpoint_decision=PAPACC_EGRESS_DENY;
    CHECK(papacc_egress_endpoint_allowed(&policy,&endpoint)==PAPACC_FALSE,53);
    state.endpoint_decision=PAPACC_EGRESS_ERROR;
    CHECK(papacc_egress_endpoint_allowed(&policy,&endpoint)==PAPACC_FALSE,54);
    state.endpoint_decision=PAPACC_EGRESS_ALLOW;
    CHECK(papacc_egress_endpoint_allowed(&policy,&endpoint)==PAPACC_TRUE,55);
    policy.endpoint_policy=NULL;
    CHECK(papacc_egress_endpoint_allowed(&policy,&endpoint)==PAPACC_FALSE,56);
    admin.denied_management=ipv4(198U,18U,0U,10U,443U);
    admin.denied_self_listener=ipv4(203U,0U,113U,20U,7443U);
    policy.endpoint_policy=test_admin_endpoint_policy;
    policy.context=&admin;
    CHECK(papacc_egress_endpoint_allowed(&policy,&admin.denied_management)==
        PAPACC_FALSE,57);
    CHECK(papacc_egress_endpoint_allowed(&policy,&admin.denied_self_listener)==
        PAPACC_FALSE,58);
    endpoint=ipv4(203U,0U,113U,21U,443U);
    CHECK(papacc_egress_endpoint_allowed(&policy,&endpoint)==PAPACC_TRUE,59);
    return 0;
}

static int normalize_hostname(const PAPACC_U8 *name, PAPACC_SIZE size,
    PAPACC_U16 port, PAPACC_NORMALIZED_DESTINATION *out)
{
    PAPACC_NETWORK_OPEN open = PAPACC_NETWORK_OPEN_INITIALIZER;
    open.destination_kind=PAPACC_NETWORK_DESTINATION_DNS_ASCII;
    open.transport_mode=PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED;
    open.port=port;
    open.destination_size=(PAPACC_U16)size;
    memcpy(open.destination,name,size);
    return (int)papacc_destination_normalize(&open,out);
}

static int test_hostname_and_dns_matrix(void)
{
    PAPACC_NORMALIZED_DESTINATION destination =
        PAPACC_NORMALIZED_DESTINATION_INITIALIZER;
    PAPACC_EGRESS_POLICY policy = PAPACC_EGRESS_POLICY_INITIALIZER;
    TEST_POLICY_CONTEXT policy_state = {PAPACC_EGRESS_ALLOW,
        PAPACC_EGRESS_ALLOW,0,0,443U};
    TEST_RESOLVER_CONTEXT resolver_state;
    PAPACC_DNS_RESOLVER resolver;
    PAPACC_NETWORK_ENDPOINT output[8];
    PAPACC_SIZE count=0U;
    const PAPACC_U8 embedded_nul[3]={'a',0U,'b'};
    const PAPACC_U8 non_ascii[2]={0xC3U,0xA9U};
    PAPACC_U8 mapped[16]={0U,0U,0U,0U,0U,0U,0U,0U,0U,0U,
        0xFFU,0xFFU,127U,0U,0U,1U};
    memset(&resolver_state,0,sizeof(resolver_state));
    resolver.context=&resolver_state;
    resolver.resolve=test_resolve;
    policy.master_enabled=PAPACC_TRUE;
    policy.host_policy=test_host_policy;
    policy.endpoint_policy=test_endpoint_policy;
    policy.context=&policy_state;
    CHECK(normalize_hostname((const PAPACC_U8 *)"ExAmPle.COM",11U,443U,
        &destination)==PAPACC_RESULT_OK &&
        strcmp(destination.hostname,"example.com")==0,60);
    CHECK(normalize_hostname((const PAPACC_U8 *)"example.com.",12U,443U,
        &destination)==PAPACC_RESULT_PROTOCOL_ERROR,61);
    CHECK(normalize_hostname((const PAPACC_U8 *)"-example.com",12U,443U,
        &destination)==PAPACC_RESULT_PROTOCOL_ERROR,62);
    CHECK(normalize_hostname((const PAPACC_U8 *)"example..com",12U,443U,
        &destination)==PAPACC_RESULT_PROTOCOL_ERROR,63);
    CHECK(normalize_hostname(embedded_nul,3U,443U,&destination)==
        PAPACC_RESULT_PROTOCOL_ERROR,64);
    CHECK(normalize_hostname(non_ascii,2U,443U,&destination)==
        PAPACC_RESULT_PROTOCOL_ERROR,65);
    CHECK(normalize_hostname((const PAPACC_U8 *)"example.com",11U,0U,
        &destination)==PAPACC_RESULT_INVALID_ARGUMENT,66);
    CHECK(normalize_hostname((const PAPACC_U8 *)"example.com",11U,65535U,
        &destination)==PAPACC_RESULT_OK,67);
    CHECK(papacc_egress_hostname_allowed(&policy,destination.hostname,
        destination.hostname_size,65535U)==PAPACC_FALSE,72);
    destination.port=443U;
    policy_state.host_calls=0;
    policy_state.endpoint_calls=0;
    resolver_state.candidates[0]=ipv4(8U,8U,8U,8U,443U);
    resolver_state.candidates[1]=ipv4(127U,0U,0U,1U,443U);
    resolver_state.candidates[2]=ipv6(mapped,443U);
    resolver_state.candidates[3]=ipv4(192U,168U,1U,10U,443U);
    resolver_state.count=4U;
    CHECK(papacc_destination_resolve_and_filter(&destination,&resolver,&policy,
        output,8U,&count)==PAPACC_RESULT_OK && count==1U &&
        resolver_state.calls==1 && policy_state.host_calls==1,68);
    policy.explicitly_allowed_sensitive_classes=
        PAPACC_EGRESS_SENSITIVE_LOOPBACK | PAPACC_EGRESS_SENSITIVE_PRIVATE;
    CHECK(papacc_destination_resolve_and_filter(&destination,&resolver,&policy,
        output,8U,&count)==PAPACC_RESULT_OK && count==4U &&
        resolver_state.calls==2 && output[2].address.family==
        PAPACC_IP_FAMILY_IPV4,69);
    policy_state.endpoint_decision=PAPACC_EGRESS_DENY;
    CHECK(papacc_destination_resolve_and_filter(&destination,&resolver,&policy,
        output,8U,&count)==PAPACC_RESULT_INVALID_STATE && count==0U &&
        resolver_state.calls==3,70);
    policy_state.endpoint_decision=PAPACC_EGRESS_ALLOW;
    policy_state.host_decision=PAPACC_EGRESS_ERROR;
    CHECK(papacc_destination_resolve_and_filter(&destination,&resolver,&policy,
        output,8U,&count)==PAPACC_RESULT_INVALID_STATE &&
        resolver_state.calls==3,71);
    return 0;
}

static PAPACC_RESULT initialize_capability_inputs(
    PAPACC_CAPABILITY_EVALUATION_INPUTS *inputs,
    PAPACC_CAPABILITY_REGISTRY *registry,
    PAPACC_CAPABILITY_DESCRIPTOR descriptors[3],
    PAPACC_CAPABILITY_SET *supported, PAPACC_CAPABILITY_KEY supported_keys[2],
    PAPACC_CAPABILITY_SET *enabled, PAPACC_CAPABILITY_KEY enabled_keys[2],
    PAPACC_CAPABILITY_SET *client, PAPACC_CAPABILITY_KEY client_keys[2],
    TEST_CAPABILITY_POLICY_CONTEXT *policy)
{
    PAPACC_RESULT result;
    result=papacc_capability_registry_init(registry,descriptors,3U);
    if(result!=PAPACC_RESULT_OK)return result;
    result=papacc_builtin_capabilities_register(registry);
    if(result!=PAPACC_RESULT_OK)return result;
    result=papacc_capability_set_init(supported,supported_keys,2U);
    if(result!=PAPACC_RESULT_OK)return result;
    result=papacc_capability_set_init(enabled,enabled_keys,2U);
    if(result!=PAPACC_RESULT_OK)return result;
    result=papacc_capability_set_init(client,client_keys,2U);
    if(result!=PAPACC_RESULT_OK)return result;
    (void)papacc_capability_set_insert(supported,
        &PAPACC_CAPABILITY_NETWORK_EGRESS_V1);
    (void)papacc_capability_set_insert(supported,
        &PAPACC_CAPABILITY_TLS_OFFLOAD_V1);
    (void)papacc_capability_set_insert(enabled,
        &PAPACC_CAPABILITY_NETWORK_EGRESS_V1);
    (void)papacc_capability_set_insert(enabled,
        &PAPACC_CAPABILITY_TLS_OFFLOAD_V1);
    (void)papacc_capability_set_insert(client,
        &PAPACC_CAPABILITY_NETWORK_EGRESS_V1);
    (void)papacc_capability_set_insert(client,
        &PAPACC_CAPABILITY_TLS_OFFLOAD_V1);
    memset(inputs,0,sizeof(*inputs));
    inputs->registry=registry;
    inputs->server_supported=supported;
    inputs->server_enabled=enabled;
    inputs->principal_policy=test_capability_policy;
    inputs->principal_policy_context=policy;
    inputs->client_supported=client;
    inputs->client_preference=client;
    return PAPACC_RESULT_OK;
}

static int test_capability_authorization_and_isolation(void)
{
    PAPACC_CAPABILITY_DESCRIPTOR descriptors[3];
    PAPACC_CAPABILITY_REGISTRY registry=PAPACC_CAPABILITY_REGISTRY_INITIALIZER;
    PAPACC_CAPABILITY_KEY supported_keys[2],enabled_keys[2],client_keys[2];
    PAPACC_CAPABILITY_SET supported=PAPACC_CAPABILITY_SET_INITIALIZER;
    PAPACC_CAPABILITY_SET enabled=PAPACC_CAPABILITY_SET_INITIALIZER;
    PAPACC_CAPABILITY_SET client=PAPACC_CAPABILITY_SET_INITIALIZER;
    PAPACC_CAPABILITY_EVALUATION_INPUTS inputs;
    TEST_CAPABILITY_POLICY_CONTEXT policy={PAPACC_CAPABILITY_POLICY_ALLOW,
        PAPACC_RESULT_OK};
    PAPACC_SESSION_CAPABILITY_BINDING bindings[2];
    PAPACC_CAPABILITY_KEY store_keys[4];
    PAPACC_SESSION_CAPABILITY_STORE store=
        PAPACC_SESSION_CAPABILITY_STORE_INITIALIZER;
    PAPACC_SESSION_CAPABILITY_SNAPSHOT *a;
    CHECK(initialize_capability_inputs(&inputs,&registry,descriptors,
        &supported,supported_keys,&enabled,enabled_keys,&client,client_keys,
        &policy)==PAPACC_RESULT_OK,80);
    CHECK(papacc_session_capability_store_init(&store,bindings,store_keys,2U,2U)
        ==PAPACC_RESULT_OK,81);
    CHECK(papacc_session_capability_store_publish(&store,101U,&inputs)==
        PAPACC_RESULT_OK,82);
    a=papacc_session_capability_store_find(&store,101U);
    CHECK(a!=NULL && a->published==PAPACC_TRUE &&
        papacc_session_capability_store_find(&store,202U)==NULL,83);
    CHECK(papacc_network_open_authorize(PAPACC_TRUE,a,&enabled,
        test_capability_policy,&policy,PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED)==
        PAPACC_RESULT_OK,84);
    CHECK(papacc_network_open_authorize(PAPACC_FALSE,a,&enabled,
        test_capability_policy,&policy,PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED)==
        PAPACC_RESULT_INVALID_STATE,85);
    CHECK(papacc_network_open_authorize(PAPACC_TRUE,NULL,&enabled,
        test_capability_policy,&policy,PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED)==
        PAPACC_RESULT_INVALID_STATE,86);
    policy.decision=PAPACC_CAPABILITY_POLICY_DENY;
    CHECK(papacc_network_open_authorize(PAPACC_TRUE,a,&enabled,
        test_capability_policy,&policy,PAPACC_NETWORK_TRANSPORT_TCP_PLAIN)==
        PAPACC_RESULT_INVALID_STATE,87);
    policy.decision=PAPACC_CAPABILITY_POLICY_ERROR;
    CHECK(papacc_network_open_authorize(PAPACC_TRUE,a,&enabled,
        test_capability_policy,&policy,PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED)==
        PAPACC_RESULT_INVALID_STATE,88);
    policy.decision=PAPACC_CAPABILITY_POLICY_ALLOW;
    enabled.count=1U;
    CHECK(papacc_network_open_authorize(PAPACC_TRUE,a,&enabled,
        test_capability_policy,&policy,PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED)==
        PAPACC_RESULT_INVALID_STATE,89);
    CHECK(papacc_network_open_authorize(PAPACC_TRUE,a,&enabled,
        test_capability_policy,&policy,PAPACC_NETWORK_TRANSPORT_TCP_PLAIN)==
        PAPACC_RESULT_OK,90);
    CHECK(papacc_session_capability_store_publish(&store,202U,&inputs)==
        PAPACC_RESULT_OK && store.count==2U,91);
    CHECK(papacc_session_capability_store_publish(&store,303U,&inputs)==
        PAPACC_RESULT_LIMIT_EXCEEDED && store.count==2U,92);
    CHECK(papacc_session_capability_store_remove(&store,101U)==PAPACC_RESULT_OK
        && papacc_session_capability_store_publish(&store,303U,&inputs)==
        PAPACC_RESULT_OK && store.count==2U,93);
    papacc_session_capability_store_shutdown(&store);
    return 0;
}

static int test_master_gate_and_profile_fail_closed(void)
{
    PAPACC_NETWORK_ENDPOINT endpoint=ipv4(203U,0U,113U,10U,443U);
    PAPACC_EGRESS_POLICY policy=PAPACC_EGRESS_POLICY_INITIALIZER;
    TEST_POLICY_CONTEXT policy_state={PAPACC_EGRESS_ALLOW,
        PAPACC_EGRESS_ALLOW,0,0,0U};
    PAPACC_EGRESS_PROFILE profiles[2];
    PAPACC_EGRESS_RUNTIME_CONFIG config=PAPACC_EGRESS_RUNTIME_CONFIG_INITIALIZER;
    TEST_PROFILE_SELECTOR_CONTEXT selector={7U,PAPACC_RESULT_OK};
    const PAPACC_EGRESS_PROFILE *selected=NULL;
    profiles[0]=(PAPACC_EGRESS_PROFILE)PAPACC_EGRESS_PROFILE_INITIALIZER;
    profiles[0].profile_id=7U;
    memcpy(profiles[0].name,"default-route",14U);
    profiles[0].use_default_route=PAPACC_TRUE;
    profiles[0].available=PAPACC_TRUE;
    profiles[1]=(PAPACC_EGRESS_PROFILE)PAPACC_EGRESS_PROFILE_INITIALIZER;
    profiles[1].profile_id=8U;
    memcpy(profiles[1].name,"unavailable",12U);
    profiles[1].use_default_route=PAPACC_TRUE;
    profiles[1].available=PAPACC_FALSE;
    policy.master_enabled=PAPACC_TRUE;
    policy.endpoint_policy=test_endpoint_policy;
    policy.context=&policy_state;
    config.allow_network_egress=PAPACC_TRUE;
    config.policy=&policy;
    config.profiles=profiles;
    config.profile_count=2U;
    config.select_profile=test_select_profile;
    config.select_profile_context=&selector;
    CHECK(papacc_egress_runtime_resolve(&config,&endpoint,
        PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED,&selected)==PAPACC_RESULT_OK &&
        selected==&profiles[0],100);
    config.allow_network_egress=PAPACC_FALSE;
    CHECK(papacc_egress_runtime_resolve(&config,&endpoint,
        PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED,&selected)==
        PAPACC_RESULT_INVALID_STATE && selected==NULL,101);
    config.allow_network_egress=PAPACC_TRUE;
    policy.master_enabled=PAPACC_FALSE;
    CHECK(papacc_egress_runtime_resolve(&config,&endpoint,
        PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED,&selected)==
        PAPACC_RESULT_INVALID_STATE,102);
    policy.master_enabled=PAPACC_TRUE;
    selector.profile_id=9U;
    CHECK(papacc_egress_runtime_resolve(&config,&endpoint,
        PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED,&selected)==
        PAPACC_RESULT_INVALID_STATE,103);
    selector.profile_id=8U;
    CHECK(papacc_egress_runtime_resolve(&config,&endpoint,
        PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED,&selected)==
        PAPACC_RESULT_INVALID_STATE,104);
    selector.profile_id=0U;
    CHECK(papacc_egress_runtime_resolve(&config,&endpoint,
        PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED,&selected)==
        PAPACC_RESULT_INVALID_STATE,105);
    selector.profile_id=7U;
    selector.result=PAPACC_RESULT_INTERNAL_ERROR;
    CHECK(papacc_egress_runtime_resolve(&config,&endpoint,
        PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED,&selected)==
        PAPACC_RESULT_INTERNAL_ERROR && selected==NULL,106);
    selector.result=PAPACC_RESULT_OK;
    profiles[0].name[0]='\0';
    CHECK(papacc_egress_runtime_resolve(&config,&endpoint,
        PAPACC_NETWORK_TRANSPORT_TLS_REQUIRED,&selected)==
        PAPACC_RESULT_INVALID_STATE,107);
    return 0;
}

static int test_exact_policy_snapshot(void)
{
    PAPACC_EXACT_EGRESS_POLICY_SNAPSHOT snapshot =
        PAPACC_EXACT_EGRESS_POLICY_SNAPSHOT_INITIALIZER;
    PAPACC_EGRESS_POLICY policy = PAPACC_EGRESS_POLICY_INITIALIZER;
    PAPACC_NETWORK_ENDPOINT allowed = ipv4(127U, 0U, 0U, 1U, 8443U);
    PAPACC_NETWORK_ENDPOINT nearby = ipv4(127U, 0U, 0U, 2U, 8443U);

    CHECK(papacc_exact_egress_policy_init(&snapshot) == PAPACC_RESULT_OK,
        110);
    CHECK(papacc_exact_egress_policy_add_host(&snapshot,
        "lab.example", 11U, 8443U) == PAPACC_RESULT_OK, 111);
    CHECK(papacc_exact_egress_policy_add_endpoint(&snapshot, &allowed) ==
        PAPACC_RESULT_OK, 112);
    CHECK(papacc_exact_egress_policy_publish(&snapshot, &policy) ==
        PAPACC_RESULT_OK, 113);
    CHECK(policy.host_policy(policy.context, "lab.example", 11U, 8443U) ==
        PAPACC_EGRESS_ALLOW, 114);
    CHECK(policy.host_policy(policy.context, "lab.example", 11U, 443U) ==
        PAPACC_EGRESS_DENY, 115);
    CHECK(policy.host_policy(policy.context, "other.example", 13U, 8443U) ==
        PAPACC_EGRESS_DENY, 116);
    CHECK(policy.endpoint_policy(policy.context, &allowed,
        PAPACC_EGRESS_SENSITIVE_LOOPBACK) == PAPACC_EGRESS_ALLOW, 117);
    CHECK(policy.endpoint_policy(policy.context, &nearby,
        PAPACC_EGRESS_SENSITIVE_LOOPBACK) == PAPACC_EGRESS_DENY, 118);
    CHECK(papacc_exact_egress_policy_add_host(&snapshot,
        "later.example", 13U, 8443U) == PAPACC_RESULT_INVALID_ARGUMENT,
        119);
    return 0;
}

int main(void)
{
    int result=test_sensitive_destination_matrix();
    if(result!=0)return result;
    result=test_structural_and_administrative_policy();
    if(result!=0)return result;
    result=test_hostname_and_dns_matrix();
    if(result!=0)return result;
    result=test_capability_authorization_and_isolation();
    if(result!=0)return result;
    result=test_master_gate_and_profile_fail_closed();
    if(result!=0)return result;
    return test_exact_policy_snapshot();
}

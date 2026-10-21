#include "../src/core/ed11_process_profile.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "server_c2_participant_win32.h"
#include <stdio.h>
#include <string.h>
static PAPACC_SERVER_C2_PARTICIPANT a,b,c;
static int root_identity,other_root;
static unsigned checks;
#define CHECK(x) do {++checks;if(!(x)){printf("FAIL line %d: %s\n",__LINE__,#x);return 1;}}while(0)
int main(void)
{
 PAPACC_SERVER_CONFIG model=PAPACC_SERVER_CONFIG_INITIALIZER;
 PAPACC_CAPABILITY_DESCRIPTOR descriptors[2];
 PAPACC_CAPABILITY_REGISTRY registry=PAPACC_CAPABILITY_REGISTRY_INITIALIZER;
 PAPACC_CAPABILITY_KEY supported_keys[2],enabled_keys[2];
 PAPACC_CAPABILITY_SET supported=PAPACC_CAPABILITY_SET_INITIALIZER,enabled=PAPACC_CAPABILITY_SET_INITIALIZER;
 PAPACC_CAPABILITY_EVALUATION_INPUTS inputs;
 const PAPACC_CAPABILITY_KEY tls={2U,1U},mode={3U,1U};
 PAPACC_BOOL permit=PAPACC_TRUE;PAPACC_SIZE old_reservation;
 PAPACC_NETWORK_INTERFACE_PERSISTENT_ID selected={PAPACC_TRUE,42U};
 memset(&inputs,0,sizeof(inputs));
 CHECK(papacc_capability_registry_init(&registry,descriptors,2U)==PAPACC_RESULT_OK);
 CHECK(papacc_capability_registry_register(&registry,&tls,"TLS_OFFLOAD")==PAPACC_RESULT_OK);
 CHECK(papacc_capability_registry_register(&registry,&mode,"MODE_C")==PAPACC_RESULT_OK);
 CHECK(papacc_capability_set_init(&supported,supported_keys,2U)==PAPACC_RESULT_OK);
 CHECK(papacc_capability_set_init(&enabled,enabled_keys,2U)==PAPACC_RESULT_OK);
 CHECK(papacc_capability_set_insert(&supported,&tls)==PAPACC_RESULT_OK);
 CHECK(papacc_capability_set_insert(&supported,&mode)==PAPACC_RESULT_OK);
 CHECK(papacc_capability_set_insert(&enabled,&mode)==PAPACC_RESULT_OK);
 inputs.registry=&registry;inputs.server_supported=&supported;inputs.server_enabled=&enabled;
 model.control_port=4433U;model.bind_selection.mode=PAPACC_BIND_SELECTION_ALL_INTERFACES;
 model.listener_profile=PAPACC_SERVER_LISTENER_PROFILE_SECURE_PRINCIPAL;
 model.security_configuration_ref_valid=PAPACC_TRUE;model.security_configuration_ref_length=1U;
 model.security_configuration_ref[0]='s';
 CHECK(papacc_server_c2_participant_validate(NULL,&root_identity,1U)==PAPACC_RESULT_INVALID_STATE);
 CHECK(papacc_server_c2_participant_validate(&a,&root_identity,1U)==PAPACC_RESULT_INVALID_STATE);
 CHECK(papacc_server_c2_participant_prepare(&a,&root_identity,0U,&model,&inputs,0U)==PAPACC_RESULT_INVALID_ARGUMENT);
 CHECK(papacc_server_c2_participant_prepare(&a,&root_identity,1U,&model,&inputs,PAPACC_ED11_PROCESS_LIMIT)==PAPACC_RESULT_LIMIT_EXCEEDED);
 CHECK(a.state==PAPACC_C2_PARTICIPANT_EMPTY&&!a.manager.initialized&&!a.reserved);
 CHECK(papacc_server_c2_participant_prepare(&a,&root_identity,1U,&model,&inputs,0U)==PAPACC_RESULT_INVALID_STATE);
 CHECK(!a.manager.initialized); /* MODE C without TLS refused, no partial prepare. */
 CHECK(papacc_capability_set_insert(&enabled,&tls)==PAPACC_RESULT_OK);
 model.control_port=0U;
 CHECK(papacc_server_c2_participant_prepare(&a,&root_identity,1U,&model,&inputs,0U)==PAPACC_RESULT_INVALID_STATE);
 CHECK(!a.manager.initialized);model.control_port=4433U;
 CHECK(papacc_server_c2_participant_prepare(&a,&root_identity,1U,&model,&inputs,100U)==PAPACC_RESULT_OK);
 CHECK(a.state==PAPACC_C2_PARTICIPANT_READY&&a.manager.initialized);
 CHECK(a.reserved==sizeof(a)&&a.combined==sizeof(a)+100U);
 CHECK(papacc_server_c2_participant_validate(&a,&root_identity,1U)==PAPACC_RESULT_OK);
 CHECK(papacc_server_c2_participant_validate(&a,&other_root,1U)==PAPACC_RESULT_INVALID_STATE);
 CHECK(papacc_server_c2_participant_validate(&a,&root_identity,2U)==PAPACC_RESULT_INVALID_STATE);
 CHECK(a.manager.policy(a.manager.context,NULL,NULL,0U,&permit)==PAPACC_RESULT_OK&&!permit);
 permit=PAPACC_TRUE;
 CHECK(a.manager.capacity(a.manager.context,1U,&permit)==PAPACC_RESULT_OK&&!permit);
 CHECK(!a.manager.sessions[0].session&&a.manager.operations[0].state==PAPACC_C2_OP_EMPTY);
 CHECK(papacc_server_c2_participant_prepare(&a,&root_identity,1U,&model,&inputs,0U)==PAPACC_RESULT_INVALID_STATE);
 /* Caller config changes do not mutate participant snapshot. No commit, CURRENT,
  * epoch, grant, Connection or operational API exists in this target. */
 model.security_configuration_ref[0]='x';CHECK(a.model.security_configuration_ref[0]=='s');
 CHECK(papacc_server_c2_participant_rollback(&a,&root_identity,1U)==PAPACC_RESULT_OK);
 CHECK(a.state==PAPACC_C2_PARTICIPANT_CLOSED&&!a.reserved&&!a.manager.initialized);
 CHECK(papacc_server_c2_participant_stop(&a,&root_identity,1U)==PAPACC_RESULT_OK);
 CHECK(papacc_server_c2_participant_prepare(&a,&root_identity,2U,&model,&inputs,0U)==PAPACC_RESULT_INVALID_STATE);
 CHECK(papacc_server_c2_participant_prepare(&b,&root_identity,2U,&model,&inputs,0U)==PAPACC_RESULT_OK);
 old_reservation=b.reserved;b.manager.operations[0].callbacks=1U;
 CHECK(papacc_server_c2_participant_stop(&b,&root_identity,2U)==PAPACC_RESULT_INVALID_STATE);
 CHECK(b.state==PAPACC_C2_PARTICIPANT_RETAINED&&b.reserved==old_reservation&&b.manager.initialized);
 CHECK(papacc_server_c2_participant_rollback(&b,&root_identity,2U)==PAPACC_RESULT_INVALID_STATE);
 model.bind_selection.mode=PAPACC_BIND_SELECTION_SELECTED_INTERFACES;
 model.bind_selection.interface_persistent_ids=&selected;model.bind_selection.interface_persistent_id_count=1U;
 CHECK(papacc_server_c2_participant_prepare(&c,&root_identity,3U,&model,&inputs,
 PAPACC_C2_PARTICIPANT_LIMIT-sizeof(c))==PAPACC_RESULT_OK);
 selected.value=99U;
 CHECK(c.ids[0].value==42U&&c.model.bind_selection.interface_persistent_ids==c.ids);
 CHECK(papacc_server_c2_participant_validate(&c,&root_identity,3U)==PAPACC_RESULT_OK);
 CHECK(c.combined==PAPACC_C2_PARTICIPANT_LIMIT);
 CHECK(papacc_server_c2_participant_stop(&c,&root_identity,3U)==PAPACC_RESULT_OK);
 CHECK(c.state==PAPACC_C2_PARTICIPANT_CLOSED);
 printf("PASS checks=%u participant=%llu manager=%llu model=%llu retained_charge=%llu harness_static=%llu\n",
 checks,(unsigned long long)sizeof(a),(unsigned long long)sizeof(a.manager),
 (unsigned long long)sizeof(a.model),(unsigned long long)b.reserved,(unsigned long long)(sizeof(a)+sizeof(b)+sizeof(c)));
 return 0;
}
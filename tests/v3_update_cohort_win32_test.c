/* Native public CA/hash/canonical ticket operations with MODEL CURRENT.
 * Real singleton negatives are separate. No store, authority or candidate pin. */
#define _WIN32_WINNT 0x0600
#include "server_c8_private_storage_layout_win32.h"
#include "v3_backing_win32_test.c"
static PAPACC_ROOT_CONFIGURATION_PREPARATION root_preparation_before;
static int cohort_cases(const char *ca_path)
{
 PAPACC_CONFIG_PIN pin={0},spectator={0},stale;PAPACC_CONFIG_V3_BORROW borrow;
 PAPACC_CONFIG_SLOT published;PAPACC_ISSUER_WINDOWS_WORKSPACE workspace;PAPACC_U8 key[32];
 PAPACC_SIZE cn,bn;PAPACC_V3_UPDATE_COHORT *u=&owner.v3_update;unsigned index;
 PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *canonical=NULL;PAPACC_SERVER_NETWORK empty_network={0};
 PAPACC_SERVER_CONFIG config=PAPACC_SERVER_CONFIG_INITIALIZER;PAPACC_LOGGER logger={NULL,NULL,PAPACC_LOG_LEVEL_OFF};
 char path[MAX_PATH];DWORD len,start;
 len=GetFullPathNameA("v3-update-cohort-MODEL-NO-STORE",MAX_PATH,path,NULL);CHECK(len&&len<MAX_PATH);
 CHECK(sizeof(*u)==304&&papacc_configuration_commit_init03(&owner,path)==0);
 CHECK(load_ca(ca_path,&cn,key));CHECK(setup3(4,4,4));CHECK(bind_ca_data(key));CHECK(cp3());bn=z3;memcpy(base3,e3,bn);
 memset(&workspace,0,sizeof(workspace));workspace.bytes=arena;workspace.capacity=sizeof(arena);
 config_init(0,2);owner.slots[1].view=view;owner.slots[1].serial=++owner.next_serial;
 owner.pending=1;owner.prepared=1;memcpy(owner.domain,base3+72,32);
 CHECK(papacc_configuration_v3_capture_data(&owner,e3,z3,ca_der,cn,&workspace,component3,NULL)==0);
 owner.prepared=0;owner.current=1;owner.published=1;owner.store02=8;
 owner.slots[1].context_epoch=18;owner.slots[1].operational_epoch=9;owner.slots[1].issuer_revision=r64(base3+120);
 memcpy(owner.issuer_snapshots[1],base3+r32(base3+48)+8192,8192);owner.issuer_snapshot_valid[1]=1;
 memcpy(owner.committed,base3,bn);owner.committed_bytes=(DWORD)bn;
 CHECK(papacc_configuration_pin(&owner,&pin,&view)==0);CHECK(papacc_configuration_pin(&owner,&spectator,&view)==0);
 published=owner.slots[1];CHECK(next3(3,bn));
 CHECK(papacc_configuration_v3_update_hold_data(&owner,&pin,e3,z3,PAPACC_ED11_PROCESS_LIMIT,71)==PAPACC_RESULT_LIMIT_EXCEEDED);
 CHECK(pin.ticket&&!u->phase&&!owner.prepared);
 owner.slots[0].retained=1;owner.slots[2].retained=1;
 CHECK(papacc_configuration_v3_update_hold_data(&owner,&pin,e3,z3,2225439U,71)==PAPACC_RESULT_LIMIT_EXCEEDED);
 CHECK(pin.ticket&&!u->phase&&!owner.prepared);owner.slots[0].retained=0;owner.slots[2].retained=0;
 stale=pin;stale.ticket=owner.next_ticket+1U;CHECK(papacc_configuration_v3_update_hold_data(&owner,&stale,e3,z3,2225439U,71)!=0&&!u->phase);
 owner.slots[1].context_epoch++;CHECK(papacc_configuration_v3_update_hold_data(&owner,&pin,e3,z3,2225439U,71)!=0);owner.slots[1].context_epoch--;
 e3[112]^=1;CHECK(papacc_configuration_v3_update_hold_data(&owner,&pin,e3,z3,2225439U,71)!=0);e3[112]^=1;
 e3[r32(e3+48)+8192+112]^=1;CHECK(papacc_configuration_v3_update_hold_data(&owner,&pin,e3,z3,2225439U,71)!=0);e3[r32(e3+48)+8192+112]^=1;
 e3[r32(e3+56)]^=1;CHECK(papacc_configuration_v3_update_hold_data(&owner,&pin,e3,z3,2225439U,71)!=0);e3[r32(e3+56)]^=1;
 e3[r32(e3+260)+64]^=1;CHECK(papacc_configuration_v3_update_hold_data(&owner,&pin,e3,z3,2225439U,71)!=0);e3[r32(e3+260)+64]^=1;
 CHECK(papacc_configuration_v3_update_hold_data(&owner,&pin,e3,z3,2225439U,71)==0);
 CHECK(!pin.ticket&&u->phase==PAPACC_V3_UPDATE_HELD&&u->predecessor_pin.ticket&&u->invocation==71);
 index=u->candidate_index;CHECK(index!=owner.current&&owner.prepared&&owner.pending==index);
 CHECK(sizeof(u->act)==224&&u->act.state==PAPACC_V3_ACT_EMPTY);
 CHECK(papacc_configuration_v3_update_act_revalidate(&owner)!=0);
 CHECK(papacc_configuration_v3_update_act_capture(&owner,e3,z3-1)!=0&&!u->act.state);
 e3[600]^=1;CHECK(papacc_configuration_v3_update_act_capture(&owner,e3,z3)!=0&&!u->act.state);e3[600]^=1;
 /* Model correlation only, not an administrative identity or factual CURRENT. */
 u->act.state=PAPACC_V3_ACT_CAPTURED;u->act.status=PAPACC_V3_STATUS_SOURCE_ABSENT;
 CHECK(papacc_configuration_issuer_hash06(&owner,e3,z3,u->act.candidate_hash));
 CHECK(papacc_configuration_issuer_hash06(&owner,base3,bn,u->act.predecessor_hash));
 CHECK(papacc_configuration_v3_update_act_match_data(&owner)==0);
 u->candidate_serial++;CHECK(papacc_configuration_v3_update_act_match_data(&owner)!=0);u->candidate_serial--;
 owner.v3_backings[index].packet[600]^=1;CHECK(papacc_configuration_v3_update_act_match_data(&owner)!=0);owner.v3_backings[index].packet[600]^=1;
 owner.pending_commit.previous[600]^=1;CHECK(papacc_configuration_v3_update_act_match_data(&owner)!=0);owner.pending_commit.previous[600]^=1;
 CHECK(papacc_configuration_v3_update_act_revalidate(&owner)!=0&&u->act.state==PAPACC_V3_ACT_INVALID);
 memset(&u->act,0,sizeof(u->act)); /* No native capture/handle occurred in MODEL. */
 /* Native token query only; never elevate or claim a positive operational act. */
 {PAPACC_RESULT act=papacc_configuration_v3_update_act_capture(&owner,e3,z3);
  CHECK(act!=0||u->act.state==PAPACC_V3_ACT_CAPTURED);
  CHECK(!u->act.retained_token&&!u->act.retained_process_token);
  if(act==0)CHECK(u->act.status==PAPACC_V3_STATUS_SOURCE_ABSENT);
  printf("NATIVE_TOKEN_CAPTURE=%s MODEL_CURRENT_NOT_AUTHORITY\n",act==0?"ELIGIBLE_LOCAL_TOKEN":"DENIED");
 }

 CHECK(!owner.v3_backings[index].serial&&owner.v3_backings[index].packet_bytes==z3);
 CHECK(!memcmp(owner.v3_backings[index].packet,e3,z3)&&!memcmp(owner.pending_commit.previous,base3,bn));
 CHECK(!memcmp(owner.committed,base3,bn)&&!memcmp(&owner.slots[1].view,&published.view,sizeof(view)));
 CHECK(owner.slots[1].serial==published.serial&&owner.slots[1].readers==published.readers&&owner.slots[1].context_epoch==18&&owner.slots[1].operational_epoch==9);
 CHECK(papacc_configuration_current(&owner,&published.view)==PAPACC_TRUE);
 CHECK(papacc_configuration_v3_borrow_data(&owner,&spectator,&borrow)==0&&!memcmp(borrow.packet,base3,bn));
 e3[600]^=1;CHECK(owner.v3_backings[index].packet[600]!=e3[600]);e3[600]^=1;
 CHECK(papacc_configuration_commit(&owner)!=0&&papacc_configuration_publish(&owner)!=0&&papacc_configuration_abort(&owner)!=0);
 CHECK(papacc_configuration_v3_capture_data(&owner,e3,z3,ca_der,cn,&workspace,component3,NULL)!=0);
 /* Injected unresolved-child/slot state is MODEL only, not a fabricated lease. */
 /* Simulated uncertain native close: no real handle is created/closed. */
 u->act.retained_process_token=(HANDLE)(ULONG_PTR)1;u->act.state=PAPACC_V3_ACT_RETAINED;
 CHECK(papacc_configuration_v3_update_abort_data(&owner)!=0&&u->phase==PAPACC_V3_UPDATE_RETAINED);
 CHECK(u->predecessor_pin.ticket&&u->act.retained_process_token&&!memcmp(owner.committed,base3,bn));
 u->act.retained_process_token=NULL; /* MODEL fault removed, not native closure proof. */
 owner.slots[index].readers=1;CHECK(papacc_configuration_v3_update_abort_data(&owner)!=0&&u->phase==PAPACC_V3_UPDATE_RETAINED);
 CHECK(u->predecessor_pin.ticket&&!memcmp(owner.committed,base3,bn));owner.slots[index].readers=0;
 CHECK(papacc_configuration_v3_update_abort_data(&owner)==0&&!u->phase&&!owner.prepared&&owner.pending==1);
 CHECK(!u->act.state&&!u->act.retained_token&&!u->act.retained_process_token); /* No act resurrection after abort. */
 CHECK(owner.slots[1].readers==1&&papacc_configuration_current(&owner,&published.view)==PAPACC_TRUE);
 CHECK(!memcmp(borrow.packet,base3,bn)&&!memcmp(owner.committed,base3,bn)&&!owner.pending_commit.previous_bytes);
 CHECK(papacc_configuration_pin(&owner,&pin,&view)==0);CHECK(papacc_configuration_v3_update_hold_data(&owner,&pin,e3,z3,2225439U,72)==0);
 u->act.state=PAPACC_V3_ACT_CAPTURED;u->act.status=PAPACC_V3_STATUS_SOURCE_ABSENT; /* MODEL only */
 start=GetTickCount();CHECK(papacc_configuration_stop(&owner)==0&&GetTickCount()-start<1000);
 CHECK(u->predecessor_pin.ticket&&u->phase==PAPACC_V3_UPDATE_HELD&&!memcmp(borrow.packet,base3,bn));
 CHECK(u->act.state==PAPACC_V3_ACT_CAPTURED&&papacc_configuration_v3_update_act_revalidate(&owner)!=0);
 CHECK(papacc_configuration_finish(&owner)!=0);CHECK(papacc_configuration_v3_update_abort_data(&owner)==0);
 CHECK(!u->act.state);
 CHECK(papacc_configuration_finish(&owner)!=0);CHECK(papacc_configuration_unpin(&owner,&spectator,0)==0);CHECK(papacc_configuration_finish(&owner)==0);
 /* Actual root, no CURRENT/source/store. All positive stages must deny. */
 config.control_port=4433;config.bind_selection.mode=PAPACC_BIND_SELECTION_ALL_INTERFACES;
 CHECK(papacc_server_launcher_owner_acquire(&empty_network,&config,&logger,&canonical)==0);
 root_preparation_before=canonical->configuration_preparation;
 CHECK(papacc_server_launcher_configuration_v3_update_hold(canonical,e3,z3,2225439U)!=0);
 CHECK(papacc_server_launcher_configuration_v3_update_approve(canonical,e3,z3)!=0);
 CHECK(papacc_server_launcher_configuration_v3_update_validate(canonical)!=0);
 CHECK(papacc_server_launcher_configuration_v3_update_commit(canonical)!=0);
 CHECK(papacc_server_launcher_configuration_v3_update_abort(canonical)!=0);
 CHECK(!memcmp(&root_preparation_before,&canonical->configuration_preparation,sizeof(root_preparation_before)));
 CHECK(!canonical->configuration_commit.v3_update.phase&&!canonical->configuration_commit.initialized);
 CHECK(papacc_server_launcher_owner_finish(canonical,&empty_network,PAPACC_TRUE)==0);
 printf("PASS %u UPDATE_COHORT_MODEL_NATIVE_HASH_CANONICAL_PIN_REAL_ROOT_NEGATIVE owner_delta=304 CURRENT_MODEL_UNCHANGED NO_CANDIDATE_PIN_NO_STORE_NO_CURRENT_NO_TRUST\n",checks);return 1;
}
int main(int argc,char **argv){return argc==2&&cohort_cases(argv[1])?0:1;}

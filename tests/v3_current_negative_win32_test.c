/* CP3 DATA relation and actual singleton CURRENT-absent preflight.
 * No store/commit/publication, signing, source factory or synthetic lease. */
#define _WIN32_WINNT 0x0600
#include "server_c8_private_storage_layout_win32.h"
#include "v3_backing_win32_test.c"
#include "trust.h"
static PAPACC_SERVER_NETWORK empty_network;
static PAPACC_SERVER_CONFIG empty_config=PAPACC_SERVER_CONFIG_INITIALIZER;
static PAPACC_LOGGER silent_logger={NULL,NULL,PAPACC_LOG_LEVEL_OFF};
static int current_negative(const char *ca_path)
{
 PAPACC_CONFIG_PIN pin={0};PAPACC_ROOT_CURRENT_PIN root_pin={0};PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *canonical=NULL;
 PAPACC_CONFIG_V3_BORROW borrow;PAPACC_ISSUER_WINDOWS_WORKSPACE capture;
 PAPACC_U8 key_digest[32];PAPACC_SIZE ca_bytes;char path[MAX_PATH];DWORD n;
 const SP_TRUST_LEASE *lease=NULL;
 n=GetFullPathNameA("current-v3-MODEL-NO-STORE",MAX_PATH,path,NULL);CHECK(n&&n<MAX_PATH);
 CHECK(papacc_configuration_commit_init03(&owner,path)==PAPACC_RESULT_OK);
 CHECK(load_ca(ca_path,&ca_bytes,key_digest));CHECK(setup3(1,1,1));CHECK(bind_ca_data(key_digest));CHECK(cp3());
 config_init(0,2);owner.slots[1].view=view;owner.slots[1].serial=++owner.next_serial;
 owner.pending=1;owner.prepared=1;memcpy(owner.domain,e3+72,32);
 memset(&capture,0,sizeof(capture));capture.bytes=arena;capture.capacity=sizeof(arena);
 CHECK(papacc_configuration_v3_capture_data(&owner,e3,z3,ca_der,ca_bytes,&capture,component3,NULL)==PAPACC_RESULT_OK);
 /* MODEL metadata and bytes for DATA equations only. No native file/current. */
 owner.prepared=0;owner.current=1;owner.published=1;
 owner.slots[1].context_epoch=18;owner.slots[1].operational_epoch=9;owner.slots[1].issuer_revision=r64(e3+120);
 owner.issuer_snapshot_valid[1]=1;memcpy(owner.issuer_snapshots[1],e3+r32(e3+48)+8192,8192);
 CHECK(z3<=sizeof(owner.committed));memcpy(owner.committed,e3,z3);owner.committed_bytes=(DWORD)z3;
 CHECK(papacc_configuration_pin(&owner,&pin,&view)==PAPACC_RESULT_OK);
 CHECK(papacc_configuration_v3_borrow_data(&owner,&pin,&borrow)==PAPACC_RESULT_OK);
 CHECK(papacc_configuration_v3_checkpoint_match_data(&owner,&pin)==PAPACC_RESULT_OK);
 owner.committed[600]^=1;CHECK(papacc_configuration_v3_checkpoint_match_data(&owner,&pin)!=PAPACC_RESULT_OK);owner.committed[600]^=1;
 owner.committed_bytes--;CHECK(papacc_configuration_v3_checkpoint_match_data(&owner,&pin)!=PAPACC_RESULT_OK);owner.committed_bytes++;
 owner.slots[1].context_epoch++;CHECK(papacc_configuration_v3_checkpoint_match_data(&owner,&pin)!=PAPACC_RESULT_OK);owner.slots[1].context_epoch--;
 owner.slots[1].operational_epoch++;CHECK(papacc_configuration_v3_checkpoint_match_data(&owner,&pin)!=PAPACC_RESULT_OK);owner.slots[1].operational_epoch--;
 owner.slots[1].issuer_revision++;CHECK(papacc_configuration_v3_checkpoint_match_data(&owner,&pin)!=PAPACC_RESULT_OK);owner.slots[1].issuer_revision--;
 owner.slots[1].view.predecessor++;CHECK(papacc_configuration_v3_checkpoint_match_data(&owner,&pin)!=PAPACC_RESULT_OK);owner.slots[1].view.predecessor--;
 pin.ticket++;CHECK(papacc_configuration_v3_checkpoint_match_data(&owner,&pin)!=PAPACC_RESULT_OK);pin.ticket--;
 owner.authority_lost=1;CHECK(papacc_configuration_v3_checkpoint_match_data(&owner,&pin)!=PAPACC_RESULT_OK);owner.authority_lost=0;
 owner.unresolved=1;CHECK(papacc_configuration_v3_checkpoint_match_data(&owner,&pin)!=PAPACC_RESULT_OK);owner.unresolved=0;
 /* Never use the diagnostic commit owner as the actual root singleton. */
 CHECK(papacc_server_launcher_configuration_v3_evidence_preflight(NULL,&root_pin)!=PAPACC_RESULT_OK);
 CHECK(sp_trust_acquire(NULL,NULL,NULL,&lease)==SP_TRUST_DENIED&&!lease);
 CHECK(sp_trust_release(NULL)==SP_TRUST_RELEASE_INVALID);
 CHECK(papacc_configuration_stop(&owner)==PAPACC_RESULT_OK);
 CHECK(papacc_configuration_v3_checkpoint_match_data(&owner,&pin)!=PAPACC_RESULT_OK);
 CHECK(!memcmp(borrow.packet,e3,z3));CHECK(papacc_configuration_finish(&owner)!=PAPACC_RESULT_OK);
 CHECK(papacc_configuration_unpin(&owner,&pin,0)==PAPACC_RESULT_OK);
 CHECK(papacc_configuration_finish(&owner)==PAPACC_RESULT_OK);
 /* Real canonical singleton, no preparation/store/CURRENT/evidence fabricated. */
 empty_config.control_port=4433;empty_config.bind_selection.mode=PAPACC_BIND_SELECTION_ALL_INTERFACES;
 CHECK(papacc_server_launcher_owner_acquire(&empty_network,&empty_config,&silent_logger,&canonical)==PAPACC_RESULT_OK);
 CHECK(papacc_server_launcher_configuration_v3_evidence_preflight(canonical,NULL)!=PAPACC_RESULT_OK);
 CHECK(papacc_server_launcher_configuration_v3_evidence_preflight(canonical,&root_pin)!=PAPACC_RESULT_OK);
 root_pin.invocation=canonical->invocation+1;CHECK(papacc_server_launcher_configuration_v3_evidence_preflight(canonical,&root_pin)!=PAPACC_RESULT_OK);
 CHECK(!canonical->configuration_commit.initialized&&!canonical->configuration_preparation.normative_committed&&!canonical->configuration_preparation.administrative_published);
 CHECK(sp_trust_acquire(NULL,NULL,NULL,&lease)==SP_TRUST_DENIED&&!lease);
 CHECK(papacc_server_launcher_owner_finish(canonical,&empty_network,PAPACC_TRUE)==PAPACC_RESULT_OK);
 printf("PASS %u V3_CHECKPOINT_DATA_ONLY_CANONICAL_CURRENT_ABSENT_E0_DENIED NO_STORE_NO_PUBLICATION_NO_TRUST\n",checks);return 1;
}
int main(int argc,char **argv){return argc==2&&current_negative(argv[1])?0:1;}

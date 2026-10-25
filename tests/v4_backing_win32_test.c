/* Real canonical storage tickets/hash/public CA parsing; synthetic state only.
 * No native CURRENT, source authority, private key, network or worker effect. */
#define _WIN32_WINNT 0x0600
#include "config_root8_v4_test.c"
#include <wincrypt.h>
#include "root8_backing_fixtures.inc"
static int prepare_slot(unsigned index,PAPACC_U64 revision)
{
 config_init(0,revision);owner.slots[index].view=view;owner.slots[index].serial=++owner.next_serial;
 owner.pending=index;owner.prepared=1;memcpy(owner.domain,view.domain,32);return 1;
}
static int backing4_cycle(const char *ca_path,int retained)
{
 PAPACC_CONFIG_PIN prior_pin={0},time_pin={0},third={0},stale;PAPACC_CONFIG_V3_BORROW b3,bad3;
 PAPACC_CONFIG_V4_BORROW b4,bad4,untouched;PAPACC_ISSUER_WINDOWS_WORKSPACE workspace;
 PAPACC_SIZE cn,bn;PAPACC_U8 key[32],*row;char path[MAX_PATH];DWORD length;unsigned i;
 length=GetFullPathNameA("v4-backing-data-NO-STORE",MAX_PATH,path,NULL);CHECK(length&&length<MAX_PATH);CHECK(papacc_configuration_commit_init03(&owner,path)==PAPACC_RESULT_OK);
 CHECK(load_ca(ca_path,&cn,key));memset(&workspace,0,sizeof(workspace));workspace.bytes=arena;workspace.capacity=sizeof(arena);
 CHECK(setup3(retained?4U:1U,retained?4U:1U,retained?4U:1U));CHECK(bind_ca_data(key));CHECK(cp3());bn=z3;memcpy(base3,e3,bn);CHECK(prepare_slot(1,2));
 CHECK(papacc_configuration_v3_capture_data(&owner,e3,z3,ca_der,cn,&workspace,component3,NULL)==PAPACC_RESULT_OK);
 owner.prepared=0;owner.published=1;owner.current=1; /* diagnostic DATA ticket only */
 CHECK(papacc_configuration_pin(&owner,&prior_pin,&view)==PAPACC_RESULT_OK);CHECK(papacc_configuration_v3_borrow_data(&owner,&prior_pin,&b3)==PAPACC_RESULT_OK);
 CHECK(next3(3,bn));CHECK(upgrade4(3));row=f4+r32(f4+276)+64U;w16(row+16,3);CHECK(seal4());CHECK(cp4());if(retained)CHECK(z4==PC_ROOT8_V4_MAX&&bn==PC_ROOT8_V3_MAX);CHECK(prepare_slot(2,3));
 memset(&bad4,0x55,sizeof(bad4));untouched=bad4;
 CHECK(papacc_configuration_v4_borrow_data(&owner,&time_pin,&bad4)!=PAPACC_RESULT_OK&&!memcmp(&bad4,&untouched,sizeof(bad4)));
 CHECK(papacc_configuration_v4_capture_data(&owner,e4,z4,NULL,cn,&workspace,component4,NULL)!=PAPACC_RESULT_OK&&!owner.v3_backings[2].serial);
 workspace.capacity=sizeof(arena)+8U;CHECK(papacc_configuration_v4_capture_data(&owner,e4,z4,ca_der,cn,&workspace,component4,NULL)!=PAPACC_RESULT_OK);workspace.capacity=sizeof(arena);
 workspace.stopped=1;CHECK(papacc_configuration_v4_capture_data(&owner,e4,z4,ca_der,cn,&workspace,component4,NULL)!=PAPACC_RESULT_OK);workspace.stopped=0;
 owner.slots[2].view.revision++;CHECK(papacc_configuration_v4_capture_data(&owner,e4,z4,ca_der,cn,&workspace,component4,NULL)!=PAPACC_RESULT_OK);owner.slots[2].view.revision--;
 e4[r32(e4+276)+64U+88U]^=1;CHECK(papacc_configuration_v4_capture_data(&owner,e4,z4,ca_der,cn,&workspace,component4,NULL)!=PAPACC_RESULT_OK);e4[r32(e4+276)+64U+88U]^=1;
 deny_source=1;CHECK(papacc_configuration_v4_capture_data(&owner,e4,z4,ca_der,cn,&workspace,component4,NULL)!=PAPACC_RESULT_OK&&!owner.v3_backings[2].serial);deny_source=0;
 CHECK(papacc_configuration_v4_capture_data(&owner,e4,z4,ca_der,cn,&workspace,component4,NULL)==PAPACC_RESULT_OK);
 CHECK(papacc_configuration_v4_capture_data(&owner,e4,z4,ca_der,cn,&workspace,component4,NULL)!=PAPACC_RESULT_OK); /* immutable generation */
 CHECK(!memcmp(b3.packet,base3,bn));CHECK(papacc_configuration_v3_borrow_data(&owner,&prior_pin,&bad3)==PAPACC_RESULT_OK);
 owner.prepared=0;owner.current=2;CHECK(papacc_configuration_pin(&owner,&time_pin,&view)==PAPACC_RESULT_OK);
 CHECK(papacc_configuration_v4_borrow_data(&owner,&time_pin,&b4)==PAPACC_RESULT_OK);CHECK(b4.base.packet==owner.v3_backings[2].packet&&b4.base.revision==3);
 CHECK(b4.tsc==b4.base.packet+r32(e4+276)&&b4.tsc_bytes==r32(e4+280)&&b4.tsc[64+16]==3);
 CHECK(!memcmp(b4.base.packet,e4,z4)&&!memcmp(b4.base.ca,ca_der,cn));
 CHECK(papacc_configuration_v3_borrow_data(&owner,&time_pin,&bad3)!=PAPACC_RESULT_OK);CHECK(papacc_configuration_v4_borrow_data(&owner,&prior_pin,&bad4)!=PAPACC_RESULT_OK&&!memcmp(&bad4,&untouched,sizeof(bad4)));
 stale=time_pin;stale.ticket++;CHECK(papacc_configuration_v4_borrow_data(&owner,&stale,&bad4)!=PAPACC_RESULT_OK);stale=time_pin;stale.serial++;CHECK(papacc_configuration_v4_borrow_data(&owner,&stale,&bad4)!=PAPACC_RESULT_OK);
 owner.v3_backings[2].packet[r32(e4+276)+64U+88U]^=1;CHECK(papacc_configuration_v4_borrow_data(&owner,&time_pin,&bad4)!=PAPACC_RESULT_OK);owner.v3_backings[2].packet[r32(e4+276)+64U+88U]^=1;
 owner.slots[2].issuer_revision=b4.base.issuer_revision+1;CHECK(papacc_configuration_v4_borrow_data(&owner,&time_pin,&bad4)!=PAPACC_RESULT_OK);owner.slots[2].issuer_revision=0;
 owner.unresolved=1;CHECK(papacc_configuration_v4_borrow_data(&owner,&time_pin,&bad4)!=PAPACC_RESULT_OK);owner.unresolved=0;owner.authority_lost=1;CHECK(papacc_configuration_v4_borrow_data(&owner,&time_pin,&bad4)!=PAPACC_RESULT_OK);owner.authority_lost=0;
 e4[600]^=1;ca_der[10]^=1;memset(arena,0,sizeof(arena));CHECK(b4.base.packet[600]!=e4[600]&&b4.base.ca[10]!=ca_der[10]);e4[600]^=1;ca_der[10]^=1;
 CHECK(papacc_configuration_v4_borrow_data(&owner,&time_pin,&bad4)==PAPACC_RESULT_OK);
 /* Fill the third existing slot; no second table/owner or extra allocation. */
 CHECK(prepare_slot(0,3));CHECK(papacc_configuration_v4_capture_data(&owner,e4,z4,ca_der,cn,&workspace,component4,NULL)==PAPACC_RESULT_OK);
 owner.prepared=0;owner.current=0;CHECK(papacc_configuration_pin(&owner,&third,&view)==PAPACC_RESULT_OK);
 for(i=0;i<3;i++)CHECK(owner.slots[i].readers==1&&owner.v3_backings[i].serial==owner.slots[i].serial);
 owner.pending=2;owner.prepared=1;CHECK(papacc_configuration_v4_capture_data(&owner,e4,z4,ca_der,cn,&workspace,component4,NULL)!=PAPACC_RESULT_OK);owner.prepared=0;
 CHECK(!memcmp(b3.packet,base3,bn)&&!memcmp(b4.base.packet,e4,z4));
 CHECK(papacc_configuration_d3_begin08(&owner,&time_pin,e4,z4,INVALID_HANDLE_VALUE,0)!=PAPACC_RESULT_OK&&!owner.pending_commit.active); /* worker not upgraded */
 CHECK(papacc_configuration_stop(&owner)==PAPACC_RESULT_OK);CHECK(papacc_configuration_v4_borrow_data(&owner,&time_pin,&bad4)!=PAPACC_RESULT_OK);CHECK(!memcmp(b4.base.packet,e4,z4)&&!memcmp(b3.packet,base3,bn));
 CHECK(papacc_configuration_finish(&owner)!=PAPACC_RESULT_OK);
 /* Every synchronous use/child has returned before any non-retained unpin. */
 CHECK(papacc_configuration_unpin(&owner,&prior_pin,0)==PAPACC_RESULT_OK);CHECK(papacc_configuration_unpin(&owner,&third,0)==PAPACC_RESULT_OK);stale=time_pin;
 CHECK(papacc_configuration_unpin(&owner,&time_pin,retained)==PAPACC_RESULT_OK);
 CHECK(papacc_configuration_v4_borrow_data(&owner,&stale,&bad4)!=PAPACC_RESULT_OK);
 if(retained){CHECK(owner.slots[2].retained&&papacc_configuration_finish(&owner)!=PAPACC_RESULT_OK);CHECK(!memcmp(owner.v3_backings[2].packet,e4,z4));}
 else CHECK(papacc_configuration_finish(&owner)==PAPACC_RESULT_OK);
 printf("V4_BACKING_CYCLE retained=%d old_v3_bytes=%Iu new_v4_bytes=%Iu slot=%Iu all_backings=%Iu caller_arena=%Iu borrow=%Iu NO_CURRENT_NO_AUTHORITY\n",retained,bn,z4,sizeof(PAPACC_CONFIG_V3_BACKING),sizeof(owner.v3_backings),sizeof(arena),sizeof(b4));return 1;
}
int main(int argc,char **argv){if(argc!=2||!backing4_cycle(argv[1],0))return 1;memset(&owner,0,sizeof(owner));if(!backing4_cycle(argv[1],1))return 1;printf("PASS %u V4_CANONICAL_PIN_LIFETIME_SCOPED_DATA SYNTHETIC_KEY_ADAPTER NO_NATIVE_CURRENT NO_WORKER_V4 NO_UTC retained_not_clean=1\n",checks);return 0;}

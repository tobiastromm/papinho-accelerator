/* Actual canonical dispatch/G7 borrow/public DER. Positive signer callback
 * is explicitly MODEL in one executable; other executable links native X509
 * and must reject the existing three-byte synthetic certificate. No keys,
 * signature, certificate or package fixtures are generated. */
#include "v3_backing_win32_test.c"
#include "server_config_root8_signer_win32.h"
static unsigned dispatch_model_calls;
static int dispatch_model_deny;
#ifdef PAPACC_DISPATCH_MODEL
PC_ROOT8_RESULT papacc_root8_signer_data(void *opaque,unsigned kind,const PAPACC_U8 *packet,
 PAPACC_SIZE bytes,const PAPACC_U8 *pdg,const PAPACC_U8 *whole)
{
 PAPACC_ROOT8_SIGNER_DATA *model=opaque;dispatch_model_calls++;
 if(dispatch_model_deny||kind!=8U||!model||!model->ca||!model->ca->SubjectPublicKeyInfo.PublicKey.cbData||
 !model->workspace||model->workspace->stopped||bytes!=16384U||!packet||!pdg||!whole)return PC_ROOT8_INVALID;
 return PC_ROOT8_OK; /* MODEL ROUTING ONLY, not cryptographic profile or trust */
}
#define EXPECT_DISPATCH PC_ROOT8_OK
#else
#define EXPECT_DISPATCH PC_ROOT8_INVALID
#endif
static unsigned __int64 ca_decode_arena[4096],signer_arena[4096];
static PAPACC_U8 signer_der[8192];
static int dispatch_cases(const char *ca_path)
{
 PAPACC_ROOT8_CARRIER_DATA context;PAPACC_CONFIG_PIN pin={0};PAPACC_CONFIG_V3_BORROW borrow;
 PAPACC_ISSUER_WINDOWS_WORKSPACE ca_workspace,signer_workspace,capture_workspace;
 PAPACC_U8 digest_key[32];PAPACC_SIZE ca_bytes;const PAPACC_U8 *carrier_data,*row_data;
 char path[MAX_PATH];DWORD len;unsigned previous_calls;PAPACC_SIZE reserved;
 len=GetFullPathNameA("kind8-dispatch-MODEL-NO-STORE",MAX_PATH,path,NULL);CHECK(len&&len<MAX_PATH);
 CHECK(papacc_configuration_commit_init03(&owner,path)==PAPACC_RESULT_OK);
 CHECK(load_ca(ca_path,&ca_bytes,digest_key));CHECK(setup3(4,4,4));CHECK(bind_ca_data(digest_key));
 w16(f3+r32(f3+260)+64+16,2);CHECK(seal3()); /* REVOKED history is DATA */
 config_init(0,2);owner.slots[1].view=view;owner.slots[1].serial=++owner.next_serial;
 owner.pending=1;owner.prepared=1;memcpy(owner.domain,e3+72,32);
 memset(&capture_workspace,0,sizeof(capture_workspace));capture_workspace.bytes=arena;capture_workspace.capacity=sizeof(arena);
 CHECK(papacc_configuration_v3_capture_data(&owner,e3,z3,ca_der,ca_bytes,&capture_workspace,component3,NULL)==PAPACC_RESULT_OK);
 /* Explicit in-memory publication MODEL permits a storage ticket only.
  * No factual CURRENT, normative commit, CP3 publication or E0 source. */
 owner.prepared=0;owner.published=1;owner.current=1;
 CHECK(papacc_configuration_pin(&owner,&pin,&view)==PAPACC_RESULT_OK);
 CHECK(papacc_configuration_v3_borrow_data(&owner,&pin,&borrow)==PAPACC_RESULT_OK);
 memset(&context,0,sizeof(context));context.owner=&owner;context.v3_version=1;context.v3_pin=pin;
 memset(&ca_workspace,0,sizeof(ca_workspace));ca_workspace.bytes=ca_decode_arena;ca_workspace.capacity=sizeof(ca_decode_arena);
 memset(&signer_workspace,0,sizeof(signer_workspace));signer_workspace.bytes=signer_arena;signer_workspace.capacity=sizeof(signer_arena);
 context.v3_ca_workspace=&ca_workspace;context.v3_signer_workspace=&signer_workspace;
 context.v3_certificate=signer_der;context.v3_certificate_capacity=sizeof(signer_der);
 reserved=2220463U+3U*(PC_ROOT8_V3_MAX-PC_ROOT8_V2_MAX)+sizeof(PAPACC_V3_UPDATE_COHORT)+sizeof(ca_decode_arena)+sizeof(signer_arena)+sizeof(signer_der)+sizeof(context)+sizeof(ca_workspace)+sizeof(signer_workspace)+2U*sizeof(borrow)+sizeof(PAPACC_ROOT8_SIGNER_DATA);
 context.v3_reserved=reserved;carrier_data=borrow.packet+r32(borrow.packet+48);row_data=borrow.pdg+64;
 previous_calls=dispatch_model_calls;(void)previous_calls;
 CHECK(papacc_root8_issuer_carrier_data(&context,8,carrier_data,16384,row_data,borrow.packet)==EXPECT_DISPATCH);
#ifdef PAPACC_DISPATCH_MODEL
 CHECK(dispatch_model_calls==previous_calls+1);
#endif
 CHECK(papacc_root8_issuer_carrier_data(&context,8,carrier_data,16384,row_data+3U*384U,borrow.packet)==EXPECT_DISPATCH);
 CHECK(papacc_root8_issuer_carrier_data(&context,8,carrier_data,16384,row_data+1,borrow.packet)!=PC_ROOT8_OK);
 CHECK(papacc_root8_issuer_carrier_data(&context,8,e3+r32(e3+48),16384,e3+r32(e3+260)+64,e3)!=PC_ROOT8_OK);
 CHECK(papacc_root8_issuer_carrier_data(&context,9,carrier_data,16384,row_data,borrow.packet)==PC_ROOT8_UNSUPPORTED);
 context.v3_version=0;CHECK(papacc_root8_issuer_carrier_data(&context,8,carrier_data,16384,row_data,borrow.packet)!=PC_ROOT8_OK);context.v3_version=1;
 context.v3_pin.ticket++;CHECK(papacc_root8_issuer_carrier_data(&context,8,carrier_data,16384,row_data,borrow.packet)!=PC_ROOT8_OK);context.v3_pin=pin;
 owner.v3_backings[1].serial=0;CHECK(papacc_root8_issuer_carrier_data(&context,8,carrier_data,16384,row_data,borrow.packet)!=PC_ROOT8_OK);owner.v3_backings[1].serial=pin.serial;
 owner.v3_backings[1].packet[600]^=1;CHECK(papacc_root8_issuer_carrier_data(&context,8,carrier_data,16384,row_data,borrow.packet)!=PC_ROOT8_OK);owner.v3_backings[1].packet[600]^=1;
 owner.v3_backings[1].ca[10]^=1;CHECK(papacc_root8_issuer_carrier_data(&context,8,carrier_data,16384,row_data,borrow.packet)!=PC_ROOT8_OK);owner.v3_backings[1].ca[10]^=1;
 owner.slots[1].view.revision++;CHECK(papacc_root8_issuer_carrier_data(&context,8,carrier_data,16384,row_data,borrow.packet)!=PC_ROOT8_OK);owner.slots[1].view.revision--;
 owner.slots[1].issuer_revision=borrow.issuer_revision+1;CHECK(papacc_root8_issuer_carrier_data(&context,8,carrier_data,16384,row_data,borrow.packet)!=PC_ROOT8_OK);owner.slots[1].issuer_revision=0;
 context.v3_ca_workspace=NULL;CHECK(papacc_root8_issuer_carrier_data(&context,8,carrier_data,16384,row_data,borrow.packet)!=PC_ROOT8_OK);context.v3_ca_workspace=&ca_workspace;
 ca_workspace.stopped=1;CHECK(papacc_root8_issuer_carrier_data(&context,8,carrier_data,16384,row_data,borrow.packet)!=PC_ROOT8_OK);ca_workspace.stopped=0;
 signer_workspace.stopped=1;CHECK(papacc_root8_issuer_carrier_data(&context,8,carrier_data,16384,row_data,borrow.packet)!=PC_ROOT8_OK);signer_workspace.stopped=0;
 ca_workspace.bytes=signer_arena;CHECK(papacc_root8_issuer_carrier_data(&context,8,carrier_data,16384,row_data,borrow.packet)!=PC_ROOT8_OK);ca_workspace.bytes=ca_decode_arena;
 context.v3_certificate=(BYTE *)&owner;CHECK(papacc_root8_issuer_carrier_data(&context,8,carrier_data,16384,row_data,borrow.packet)!=PC_ROOT8_OK);context.v3_certificate=signer_der;
 context.v3_reserved=0;CHECK(papacc_root8_issuer_carrier_data(&context,8,carrier_data,16384,row_data,borrow.packet)!=PC_ROOT8_OK);context.v3_reserved=reserved;
 dispatch_model_deny=1;CHECK(papacc_root8_issuer_carrier_data(&context,8,carrier_data,16384,row_data,borrow.packet)!=PC_ROOT8_OK);dispatch_model_deny=0;
 owner.authority_lost=1;CHECK(papacc_root8_issuer_carrier_data(&context,8,carrier_data,16384,row_data,borrow.packet)!=PC_ROOT8_OK);owner.authority_lost=0;
 owner.unresolved=1;CHECK(papacc_root8_issuer_carrier_data(&context,8,carrier_data,16384,row_data,borrow.packet)!=PC_ROOT8_OK);owner.unresolved=0;
 CHECK(papacc_configuration_stop(&owner)==PAPACC_RESULT_OK);
 CHECK(papacc_root8_issuer_carrier_data(&context,8,carrier_data,16384,row_data,borrow.packet)!=PC_ROOT8_OK);
 CHECK(!memcmp(borrow.packet,e3,z3));CHECK(papacc_configuration_finish(&owner)!=PAPACC_RESULT_OK);
 CHECK(papacc_configuration_unpin(&owner,&pin,1)==PAPACC_RESULT_OK);
 CHECK(papacc_root8_issuer_carrier_data(&context,8,carrier_data,16384,row_data,borrow.packet)!=PC_ROOT8_OK);
 CHECK(owner.slots[1].retained&&papacc_configuration_finish(&owner)!=PAPACC_RESULT_OK);
 printf("PASS %u KIND8_CANONICAL_DISPATCH_MODEL_OR_NATIVE_NEGATIVE NO_CURRENT_NO_TRUST retained_not_clean=1 context=%Iu caller_CA=%Iu caller_signer=%Iu cert=%Iu named_scenario=%Iu model_calls=%u\n",checks,sizeof(context),sizeof(ca_decode_arena),sizeof(signer_arena),sizeof(signer_der),reserved,dispatch_model_calls);return 1;
}
int main(int argc,char **argv){return argc==2&&dispatch_cases(argv[1])?0:1;}

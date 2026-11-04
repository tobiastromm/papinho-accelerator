/* Explicit model of original ceremony/provider/root sources. Positive callbacks
 * exist ONLY in this test translation unit. No native CA/key creation or publication. */
#include "server_configuration_commit_win32.h"
static PAPACC_CONFIGURATION_COMMIT source_owner;
static int source_native_ok=1;
static unsigned fixture_ca_creates,fixture_ca_opens,fixture_create_taken;
static SECURITY_STATUS source_create_error;
static int source_create_stop;
static SECURITY_STATUS source_create(PAPACC_CA_KEY *k,const unsigned char a[16],const unsigned char g[16],PSID op,PSID svc)
{
 unsigned i,z=0;const wchar_t *prefix=L"PapinhoAccelerator.CA.",*hex=L"0123456789abcdef";
 fixture_ca_creates++;fixture_create_taken=1;
 if(source_create_error)return source_create_error;
 if(native_crypto){DWORD n=sizeof(ephemeral_ca_public.bytes);
  SECURITY_STATUS r=ephemeral_new(&ephemeral_ca);if(r)return r;
  if(!ephemeral_public(ephemeral_ca,ephemeral_ca_public.bytes,n))return NTE_FAIL;
  ca_public=*(CERT_PUBLIC_KEY_INFO *)ephemeral_ca_public.bytes;
  if(!xspki(&ca_public,fixture_view.ca_spki_digest,pubder,sizeof(pubder)))return NTE_FAIL;
 }
 k->thread=GetCurrentThreadId();k->state=PAPACC_CA_KEY_READY;k->key=101;k->provider=202;k->created=1;k->diagnostic_finalization=2;
 if(native_crypto){k->key=ephemeral_ca;k->provider=ephemeral_provider;}
 k->operator_size=k->service_size=4;memcpy(k->operator_sid,op,4);memcpy(k->service_sid,svc,4);
 while(*prefix)k->name[z++]=*prefix++;
 for(i=0;i<16;i++){k->name[z++]=hex[a[i]>>4];k->name[z++]=hex[a[i]&15];}k->name[z++]=L'.';
 for(i=0;i<16;i++){k->name[z++]=hex[g[i]>>4];k->name[z++]=hex[g[i]&15];}k->name[z]=0;
 if(source_create_stop)source_owner.pending_commit.stop=1;
 return ERROR_SUCCESS;
}
static int personal_native_ceremony_ready(const PAPACC_CONFIG_PENDING *p){(void)p;return source_native_ok;}
static PAPACC_RESULT personal_packet_current(PAPACC_CONFIG_PENDING *p){(void)p;return PAPACC_RESULT_OK;}
static PAPACC_RESULT personal_packet_source(PAPACC_CONFIG_PENDING *p){(void)p;return PAPACC_RESULT_OK;}
static BOOL WINAPI source_file(HANDLE h,LPBY_HANDLE_FILE_INFORMATION f){(void)h;memset(f,0,sizeof(*f));f->dwFileAttributes=FILE_ATTRIBUTE_DIRECTORY;f->dwVolumeSerialNumber=20;f->nFileIndexHigh=30;f->nFileIndexLow=40;return TRUE;}
#define papacc_configuration_personal_ca_initialize source_initialize
#define papacc_ca_key_create source_create
#define papacc_configuration_personal_ca_match_data source_match
#define papacc_configuration_personal_ca_bind source_bind
#define papacc_configuration_personal_ca_current source_current
#define papacc_configuration_personal_ca_release source_release
#define papacc_configuration_personal_ca_handoff source_handoff
#define GetFileInformationByHandle source_file
#define papacc_ca_key_validate model_validate
#define papacc_ca_key_borrow model_borrow
#define papacc_ca_key_return model_return
#include "../apps/server/server_configuration_personal_ca_win32.inc"
#undef papacc_configuration_personal_ca_initialize
#undef papacc_ca_key_create
#undef papacc_ca_key_validate
#undef papacc_ca_key_borrow
#undef papacc_ca_key_return
#undef GetFileInformationByHandle
#undef papacc_configuration_personal_ca_match_data
#undef papacc_configuration_personal_ca_bind
#undef papacc_configuration_personal_ca_current
#undef papacc_configuration_personal_ca_release
#undef papacc_configuration_personal_ca_handoff
#define source_activate papacc_configuration_personal_ca_activate_normal
/* Test-only instance construction. Synthetic key handles and signature bytes;
 * deliberately no NCrypt create/open/sign or production creation entry. */
static BYTE initial_ca_tbs[1024],initial_ca_der[2048];
static DWORD initial_ca_der_bytes;
static union {ULONGLONG alignment;BYTE bytes[2048];} initial_ca_decode;
static int fixture_create_instance(PAPACC_CONFIG_PENDING *p)
{
 PAPACC_ISSUER_VIEW genesis=fixture_view;CERT_SIGNED_CONTENT_INFO encoded;BYTE signature[256];DWORD n,z;
 const unsigned char service[4]={4,3,2,1};
 {SECURITY_STATUS r=source_initialize(&source_owner,&key,p->personal_data.attempt,fixture_view.authority,fixture_view.generation,service,4,
  sizeof(source_owner)+sizeof(key)+8192);if(r!=ERROR_SUCCESS){return 0;}}
 genesis=fixture_view;genesis.high_water=1;genesis.revision=genesis.epoch=genesis.administrative_epoch=0;genesis.proofs=0;
 if(papacc_issuer_x509_ca_tbs(&genesis,1690000000ULL,&ca_public,initial_ca_tbs,sizeof(initial_ca_tbs),&n)!=0){return 0;}
 memset(&encoded,0,sizeof(encoded));memset(signature,0x42,sizeof(signature));encoded.ToBeSigned.pbData=initial_ca_tbs;encoded.ToBeSigned.cbData=n;
 encoded.SignatureAlgorithm.pszObjId=(LPSTR)szOID_RSA_SHA256RSA;encoded.SignatureAlgorithm.Parameters.pbData=(BYTE *)"\x05\x00";encoded.SignatureAlgorithm.Parameters.cbData=2;
 if(native_crypto&&!ephemeral_signature(ephemeral_ca,initial_ca_tbs,n,signature)){puts("NATIVE_TEST self-sign failed");return 0;}
 encoded.Signature.pbData=signature;encoded.Signature.cbData=256;z=sizeof(initial_ca_der);
 if(!CryptEncodeObjectEx(X509_ASN_ENCODING,X509_CERT,&encoded,native_crypto?CRYPT_ENCODE_NO_SIGNATURE_BYTE_REVERSAL_FLAG:0,NULL,initial_ca_der,&z))return 0;
 initial_ca_der_bytes=z;
 /* Keep the original public TBS backing, not workspace used for CSR/signing.
  * The synthetic outer signature is NOT native CA authentication. */
 z=sizeof(initial_ca_decode.bytes);
 if(!CryptDecodeObjectEx(X509_ASN_ENCODING,X509_CERT_TO_BE_SIGNED,initial_ca_tbs,n,CRYPT_DECODE_NOCOPY_FLAG,NULL,initial_ca_decode.bytes,&z))return 0;
 fixture_ca=*(CERT_INFO *)initial_ca_decode.bytes;return 1;
}
static int source_setup_stage(const CERT_PUBLIC_KEY_INFO *client,int before_create)
{
 PAPACC_CONFIG_PENDING *p;PAPACC_PERSONAL_NATIVE_CONTEXT *n;PAPACC_PERSONAL_EFFECT_SPEC spec;
 PAPACC_U8 parent[12]={10,0,0,0,11,0,0,0,12,0,0,0},op[4]={1,2,3,4},service[4]={4,3,2,1};
 const PAPACC_U8 config[]="format_version 1\nclient_admission DISABLED\n";
 const char prefix[]="OWNER_NEW_INSTALL_CA_INITIALIZE ",hex[]="0123456789abcdef";
 char reply[128];size_t bytes,z,i;PAPACC_ISSUER_VIEW selected;
 normal_profile=1;defer_core_init=1;CHECK(!initialize(client));defer_core_init=0;source_native_ok=1;source_create_error=0;source_create_stop=0;
 memset(&source_owner,0,sizeof(source_owner));source_owner.initialized=1;source_owner.writer=GetCurrentThreadId();memcpy(source_owner.domain,fixture_view.root,32);
 p=&source_owner.pending_commit;n=&p->personal_native;n->phase=1;n->handoff_state=PAPACC_PERSONAL_HANDOFF_VERIFIED;
 memcpy(n->handoff_attempt,fixture_view.root,32);memcpy(n->plan_leaf,"child",6);memcpy(p->previous,"child",6);memcpy(p->next,config,sizeof(config)-1);
 n->parent.dwVolumeSerialNumber=10;n->parent.nFileIndexHigh=11;n->parent.nFileIndexLow=12;memcpy(n->sid,op,4);n->sid_bytes=4;
 p->bootstrap_created=1;p->bootstrap_parent=(HANDLE)(ULONG_PTR)1;p->bootstrap_container=(HANDLE)(ULONG_PTR)2;
 p->bootstrap_volume=20;p->bootstrap_high=30;p->bootstrap_low=40;p->result=PAPACC_CAS_SUCCESS;
 key.created=0;key.key=key.provider=0;key.thread=0;key.state=PAPACC_CA_KEY_EMPTY;fixture_ca_creates=fixture_ca_opens=fixture_create_taken=0;key.operator_size=key.service_size=4;memcpy(key.operator_sid,op,4);memcpy(key.service_sid,service,4);
 memset(&spec,0,sizeof(spec));spec.operator_id=op;spec.operator_bytes=4;spec.parent_id=parent;spec.leaf=(const PAPACC_U8 *)"child";spec.leaf_bytes=5;
 spec.authority=fixture_view.authority;spec.generation=fixture_view.generation;spec.service_id=service;spec.service_bytes=4;spec.configuration=config;spec.configuration_bytes=sizeof(config)-1;
 bytes=papacc_personal_effect_build(&spec,p->previous+1024,PAPACC_PERSONAL_PLAN_MAX);CHECK(bytes);
 CHECK(papacc_personal_data_capture(&p->personal_data,fixture_view.root,config,sizeof(config)-1));
 CHECK(papacc_personal_effect_upgrade(&p->personal_data,p->previous+1024,bytes)&&papacc_personal_data_present(&p->personal_data));
 z=sizeof(prefix)-1;memcpy(reply,prefix,z);for(i=0;i<32;i++){reply[z+i*2]=hex[fixture_view.root[i]>>4];reply[z+i*2+1]=hex[fixture_view.root[i]&15];}
 CHECK(papacc_personal_effect_confirm(&p->personal_data,fixture_view.root,p->previous+1024,bytes,reply,z+64));
 CHECK(papacc_personal_effect_consume(&p->personal_data,fixture_view.root,p->previous+1024,bytes));
 selected=fixture_view;fixture_view.revision=fixture_view.epoch=fixture_view.administrative_epoch=0;fixture_view.proofs=0;
 CHECK(!fixture_ca_creates&&!key.created&&!key.key&&!key.provider); /* no previous CA */
 if(before_create)return 0;
 p->stop=1;CHECK(!fixture_create_instance(p)&&!fixture_ca_creates);p->stop=0;
 p->personal_data.effectful=0;CHECK(!fixture_create_instance(p)&&!fixture_ca_creates);p->personal_data.effectful=1;
 CHECK(fixture_create_instance(p)&&fixture_ca_creates==1U);
 CHECK(!fixture_create_instance(p)&&fixture_ca_creates==1U); /* no replay */
 CHECK(source_bind(&source_owner,&key,fixture_view.root,fixture_view.authority,fixture_view.generation)==0&&key.borrower==p);
 CHECK(source_handoff(&source_owner,&fixture_backend,fixture_view.root,fixture_view.authority,fixture_view.generation)==0&&key.borrower==&fixture_backend&&fixture_backend.custody);
 CHECK(fixture_backend.original_source_root==&source_owner);
 /* Model completed/retired ceremony: no original handle is real here.
  * Root worker storage may be reused, the key borrow never returned. */
 if(native_crypto)memcpy(selected.ca_spki_digest,fixture_view.ca_spki_digest,32);
 memset(p,0,sizeof(*p));fixture_view=selected;return 0;
}
static int source_setup(const CERT_PUBLIC_KEY_INFO *client){return source_setup_stage(client,0);}
static int initialize_entry_cases(const CERT_PUBLIC_KEY_INFO *client)
{
 PAPACC_CONFIG_PENDING *p=&source_owner.pending_commit;const unsigned char service[4]={4,3,2,1};unsigned i;SECURITY_STATUS status;
 for(i=0;i<11;i++){
  CHECK(!source_setup_stage(client,1));
  if(i==0)p->personal_data.consumed=0;if(i==1)p->personal_data.effectful=0;
  if(i==2)p->personal_native.handoff_attempt[0]^=1;if(i==3)p->personal_native.sid[0]^=1;
  if(i==4)p->personal_native.parent.nFileIndexLow^=1;if(i==5)p->next[0]^=1;
  if(i==6)key.key=1;if(i==7)p->active=1;if(i==8)source_native_ok=0;if(i==9)source_owner.recovery_selected=1;if(i==10)p->personal_native.phase=3;
  CHECK(source_initialize(&source_owner,&key,p->personal_data.attempt,fixture_view.authority,fixture_view.generation,service,4,sizeof(source_owner)+sizeof(key)+8192)!=0);
  CHECK(!fixture_ca_creates&&!p->ca_initialize_phase&&!p->personal_ca_candidate);
 }
 CHECK(!source_setup_stage(client,1));
 CHECK(source_initialize(&source_owner,&key,p->personal_data.attempt,fixture_view.authority,fixture_view.generation,service,4,1)==NTE_NO_MEMORY&&!fixture_ca_creates);
 for(i=0;i<2;i++){
  CHECK(!source_setup_stage(client,1));if(i==0)source_create_error=NTE_FAIL;else source_create_stop=1;
  status=source_initialize(&source_owner,&key,p->personal_data.attempt,fixture_view.authority,fixture_view.generation,service,4,sizeof(source_owner)+sizeof(key)+8192);
  CHECK(status!=0&&fixture_ca_creates==1&&p->ca_initialize_phase==4&&p->personal_ca_candidate==&key&&source_owner.unresolved);
  CHECK(source_initialize(&source_owner,&key,p->personal_data.attempt,fixture_view.authority,fixture_view.generation,service,4,sizeof(source_owner)+sizeof(key)+8192)!=0&&fixture_ca_creates==1&&p->ca_initialize_first==status);
 }
 puts("PASS CA_INITIALIZE_REAL_ENTRY_TEST_ONLY_KEY_API ONE_CREATE RETAINED_FAILURE NO_PRODUCT_EFFECTS");return 0;
}
static DWORD WINAPI source_other_thread(void *ctx)
{
 PAPACC_ISSUER_RESULT *results=ctx;PAPACC_ISSUER_VIEW value;
 results[0]=papacc_issuer_cng_backend_normal_source(&fixture_backend,&value);
 results[1]=source_activate(&source_owner,&fixture_backend,fixture_view.root,fixture_view.authority,fixture_view.generation);return 0;
}
static int normal_source_composition_cases(const CERT_PUBLIC_KEY_INFO *client)
{
 PAPACC_ISSUER_VIEW observed;PAPACC_ISSUER_RESULT result;PAPACC_CONFIG_PENDING *p=&source_owner.pending_commit;unsigned i;
 CHECK(!initialize_entry_cases(client));
 CHECK(!source_setup(client));CHECK(papacc_issuer_cng_backend_normal_source(&fixture_backend,&observed)==0&&!fixture_core.initialized&&!private_calls&&key.borrower==&fixture_backend);
 CHECK(source_activate(&source_owner,&fixture_backend,fixture_view.root,fixture_view.authority,fixture_view.generation)==0&&fixture_core.initialized==1&&fixture_backend.custody&&key.borrower==&fixture_backend&&!p->personal_ca);
 CHECK(source_activate(&source_owner,&fixture_backend,fixture_view.root,fixture_view.authority,fixture_view.generation)!=0); /* no second handoff */
 CHECK(papacc_issuer_issue(&fixture_core,record.request.id,csr,csr_size,out,sizeof(out))==0&&private_calls==1&&key.borrower==&fixture_backend&&!fixture_backend.active);
 papacc_issuer_stop(&fixture_core);papacc_issuer_cng_backend_stop(&fixture_backend);CHECK(papacc_issuer_finish(&fixture_core)==0);
 CHECK(papacc_issuer_cng_backend_release_custody(&fixture_backend)==0&&!key.borrower);
 for(i=0;i<10U;i++){
  CHECK(!source_setup(client));
  if(i==0U)fixture_view.proofs=0;if(i==1U)fixture_backend.key=NULL;if(i==2U)fixture_ca.SubjectPublicKeyInfo=*client;
  if(i==3U)key.name[22]^=1U;if(i==4U)fixture_backend.original_attempt[0]^=1U;if(i==5U)source_owner.domain[0]^=1U;
  if(i==6U)source_owner.stopped=1;if(i==7U)fixture_core.busy=1;if(i==8U)key.borrower=&fixture_core;if(i==9U)fixture_ca.NotAfter.dwLowDateTime^=1U;
  result=source_activate(&source_owner,&fixture_backend,fixture_view.root,fixture_view.authority,fixture_view.generation);
  CHECK(result!=0&&!fixture_core.initialized&&!private_calls&&fixture_backend.custody&&!p->personal_ca);
 }
 for(i=1U;i<=8U;i++){
  CHECK(!source_setup(client));source_reads=0;source_fault_at=2;source_fault=i;
  CHECK(source_activate(&source_owner,&fixture_backend,fixture_view.root,fixture_view.authority,fixture_view.generation)!=0&&!fixture_core.initialized&&fixture_backend.custody&&key.borrower==&fixture_backend&&!private_calls);
 }
 for(i=1U;i<=8U;i++){
  CHECK(!source_setup(client));source_reads=0;source_fault_at=3;source_fault=i;
  CHECK(source_activate(&source_owner,&fixture_backend,fixture_view.root,fixture_view.authority,fixture_view.generation)!=0&&fixture_backend.custody&&fixture_backend.retained&&key.borrower==&fixture_backend&&fixture_core.retained&&source_owner.unresolved&&!private_calls);
  CHECK(source_activate(&source_owner,&fixture_backend,fixture_view.root,fixture_view.authority,fixture_view.generation)!=0&&key.borrower==&fixture_backend);
 }
 CHECK(!source_setup(client));source_reads=0;source_fault_at=4;source_fault=4;
 CHECK(source_activate(&source_owner,&fixture_backend,fixture_view.root,fixture_view.authority,fixture_view.generation)==PAPACC_ISSUER_UNCERTAIN&&fixture_core.initialized==1&&fixture_backend.retained&&key.borrower==&fixture_backend&&!private_calls);
 CHECK(!source_setup(client));
 {PAPACC_ISSUER_RESULT results[2]={PAPACC_ISSUER_OK,PAPACC_ISSUER_OK};HANDLE thread=CreateThread(NULL,0,source_other_thread,results,0,NULL);
  CHECK(thread&&WaitForSingleObject(thread,10000)==WAIT_OBJECT_0&&CloseHandle(thread));
  CHECK(results[0]!=PAPACC_ISSUER_OK&&results[1]!=PAPACC_ISSUER_OK&&!fixture_core.initialized&&!private_calls&&key.borrower==&fixture_backend);
 }
 puts("PASS NORMAL_SOURCE_COMPOSITION_MODEL BORROW_ONLY SOURCE_AND_CNG_COMPOSED NO_NATIVE_PRIVATE_OPERATIONS NO_PRODUCT_AUTHORITY");return 0;
}

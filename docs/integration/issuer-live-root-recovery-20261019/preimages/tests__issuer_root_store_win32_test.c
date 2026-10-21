/* Offline component fixture. No CA, key, credential or operational authority.
 * Native TxF writes only the explicitly supplied new fixture path. */
#include "server_configuration_body05_win32.h"
#include "issuer_codec.h"
#include <stdio.h>
#include <string.h>
static PAPACC_CONFIGURATION_COMMIT root,recovered;
static PAPACC_CONFIG_VIEW pinned;static PAPACC_CONFIG_PIN pin;
static PAPACC_ISSUER_VIEW metadata,decoded;
static PAPACC_ISSUER_RECORD record,found;
static PAPACC_U8 image[8192],saved[8192],base[8192],hash[32],cert_hash[32];
static unsigned checks;
extern PAPACC_SIZE papacc_issuer_preimage_owner_size(void);
#define CHECK(x) do{checks++;if(!(x)){printf("FAIL line=%d code=%lu expression=%s\n",__LINE__,GetLastError(),#x);return 1;}}while(0)
static void metadata_init(void){
 memset(&metadata,0,sizeof(metadata));memcpy(metadata.root,root.domain,32);metadata.authority[0]=1;metadata.generation[0]=2;metadata.prefix[0]=3;
 metadata.policy[0]=4;metadata.ca_spki_digest[0]=5;metadata.revision=2;metadata.high_water=1;metadata.ca_not_after=1000000;
}
static void request_init(unsigned id){
 memset(&record,0,sizeof(record));record.request.id[0]=(PAPACC_U8)id;record.request.device[0]=7;record.request.key_digest[0]=(PAPACC_U8)id;record.request.csr_digest[0]=8;
 record.request.not_before=100;record.request.not_after=500;record.request.purpose=1;record.state=PAPACC_ISSUER_PREPARED;record.approved_epoch=9;
 memcpy(record.authority,metadata.authority,16);memcpy(record.generation,metadata.generation,16);memcpy(record.policy,metadata.policy,32);
}
static int codec_cases(void){
 unsigned i;PAPACC_ISSUER_RESULT s;PAPACC_U8 out[4];PAPACC_SIZE n;
 metadata_init();memset(hash,1,32);
 CHECK(papacc_issuer_image_init(image,&metadata,hash,papacc_configuration_issuer_hash06,&root)==0);
 CHECK(papacc_issuer_image_validate(image,&decoded,papacc_configuration_issuer_hash06,&root)==0&&!decoded.epoch&&!decoded.proofs);
 request_init(1);CHECK(papacc_issuer_image_append(image,&record,NULL,0,papacc_configuration_issuer_hash06,&root)==0);
 CHECK(papacc_issuer_image_append(image,&record,NULL,0,papacc_configuration_issuer_hash06,&root)==PAPACC_ISSUER_CONFLICT);
 record.counter=2;memcpy(record.serial,metadata.prefix,8);record.serial[15]=2;record.state=PAPACC_ISSUER_SERIAL_RESERVED;
 CHECK(papacc_issuer_image_append(image,&record,NULL,0,papacc_configuration_issuer_hash06,&root)==0);
 CHECK(papacc_issuer_image_validate(image,&decoded,papacc_configuration_issuer_hash06,&root)==0&&decoded.high_water==2);
 memcpy(saved,image,8192);record.counter=1;record.serial[15]=1;record.state=PAPACC_ISSUER_SIGN_REQUESTED;
 CHECK(papacc_issuer_image_append(image,&record,NULL,0,papacc_configuration_issuer_hash06,&root)==PAPACC_ISSUER_CONFLICT&&!memcmp(image,saved,8192));
 record.counter=2;record.serial[15]=2;CHECK(papacc_issuer_image_append(image,&record,NULL,0,papacc_configuration_issuer_hash06,&root)==0);
 CHECK(papacc_configuration_issuer_hash06(&root,(const PAPACC_U8 *)"DER",3,cert_hash));record.state=PAPACC_ISSUER_ISSUED_UNPUBLISHED;record.certificate_size=3;memcpy(record.certificate_digest,cert_hash,32);
 CHECK(papacc_issuer_image_append(image,&record,(const PAPACC_U8 *)"DER",3,papacc_configuration_issuer_hash06,&root)==0);
 record.state=PAPACC_ISSUER_PUBLISHED;record.published_epoch=10;CHECK(papacc_issuer_image_append(image,&record,NULL,0,papacc_configuration_issuer_hash06,&root)==0);
 record.state=PAPACC_ISSUER_REVOKED;CHECK(papacc_issuer_image_append(image,&record,NULL,0,papacc_configuration_issuer_hash06,&root)==0);
 CHECK(papacc_issuer_image_find(image,record.request.id,&found,papacc_configuration_issuer_hash06,&root)==0&&found.state==PAPACC_ISSUER_REVOKED);
 CHECK(papacc_issuer_image_certificate(image,record.request.id,out,sizeof(out),&n,papacc_configuration_issuer_hash06,&root)==0&&n==3&&!memcmp(out,"DER",3));
 request_init(2);record.request.predecessor[0]=1;CHECK(papacc_issuer_image_append(image,&record,NULL,0,papacc_configuration_issuer_hash06,&root)==0);
 record.state=PAPACC_ISSUER_SERIAL_RESERVED;record.counter=3;memcpy(record.serial,metadata.prefix,8);record.serial[15]=3;CHECK(papacc_issuer_image_append(image,&record,NULL,0,papacc_configuration_issuer_hash06,&root)==0);
 record.state=PAPACC_ISSUER_SIGN_REQUESTED;CHECK(papacc_issuer_image_append(image,&record,NULL,0,papacc_configuration_issuer_hash06,&root)==0);
 record.state=PAPACC_ISSUER_RECONCILIATION_REQUIRED;CHECK(papacc_issuer_image_append(image,&record,NULL,0,papacc_configuration_issuer_hash06,&root)==0);
 CHECK(papacc_issuer_image_find(image,record.request.id,&found,papacc_configuration_issuer_hash06,&root)==0&&found.counter==3&&found.state==PAPACC_ISSUER_RECONCILIATION_REQUIRED);
 memcpy(saved,image,8192);for(i=0;i<8192;i+=257){image[i]^=1;CHECK(papacc_issuer_image_validate(image,&decoded,papacc_configuration_issuer_hash06,&root)!=0);memcpy(image,saved,8192);}
 image[8]=1;CHECK(papacc_issuer_image_validate(image,&decoded,papacc_configuration_issuer_hash06,&root)!=0);memcpy(image,saved,8192);
 /* Fill finite physical image, never discard transition history for space. */
 for(i=3;i<100;i++){request_init(i);s=papacc_issuer_image_append(image,&record,NULL,0,papacc_configuration_issuer_hash06,&root);if(s==PAPACC_ISSUER_LIMIT)break;CHECK(s==0);}
 CHECK(i<100&&papacc_issuer_image_validate(image,&decoded,papacc_configuration_issuer_hash06,&root)==0&&decoded.high_water==3);
 return 0;
}
static void config_init(PAPACC_SERVER_CONFIG *c){
 PAPACC_SITE_TLS_CATALOG *t;*c=(PAPACC_SERVER_CONFIG)PAPACC_SERVER_CONFIG_INITIALIZER;c->control_port=4433;c->listener_profile=1;c->bind_selection.mode=PAPACC_BIND_SELECTION_ALL_INTERFACES;
 c->security_configuration_ref_valid=1;c->security_configuration_ref_length=1;c->security_configuration_ref[0]='x';c->policy.present=1;c->policy.representation=PAPACC_ADMIN_REPRESENTATION_04;c->policy.identity[0]=1;c->policy.profile=1;c->policy.listener_port=4433;
 t=&c->site_tls;t->present=t->version=t->trust_system=t->hostname_required=t->sni_required=t->tls13_only=1;
 t->identity[0]=1;t->profile_identity[0]=2;t->administrative_origin[0]=3;t->revision=t->profile_revision=1;t->reference=7;
 t->not_before_utc_ms=1000;t->expires_utc_ms=2000;t->max_bytes=4096;t->max_duration_ms=1000;t->max_concurrent=1;t->alpn_length=3;t->alpn[0]=2;t->alpn[1]='h';t->alpn[2]='2';
}
#ifdef PAPACC_ISSUER_STORAGE_COMPONENT_TEST
static int persist_step(const PAPACC_U8 *der,PAPACC_SIZE size){
 PAPACC_ISSUER_VIEW before;PAPACC_U64 rev=root.slots[root.current].view.revision;
 CHECK(papacc_issuer_image_validate(image,&before,papacc_configuration_issuer_hash06,&root)==0);
 CHECK(papacc_issuer_image_append(image,&record,der,size,papacc_configuration_issuer_hash06,&root)==0);
 CHECK(papacc_configuration_issuer_prepare06(&root,image,rev,before.high_water)==0);CHECK(papacc_configuration_commit(&root)==0);CHECK(papacc_configuration_publish(&root)==0);return 0;
}
#endif
int main(int argc,char **argv){
 PAPACC_SERVER_CONFIG c;PAPACC_RESULT result;PAPACC_U64 expected,high;PAPACC_U8 historical[1024];
 if(argc!=3){puts("usage: test --codec-only|--native NEW_ABSOLUTE_FIXTURE_PATH");return 2;}
 CHECK(papacc_configuration_commit_init05(&root,argv[2])==PAPACC_RESULT_OK);
 CHECK(codec_cases()==0);CHECK(sizeof(root)-papacc_issuer_preimage_owner_size()==16408U);
 printf("CENSUS_OWNER_PRE=%zu POST=%zu DELTA=%zu STAGING=8192 COMMITTED_EXTENSION=8192\n",papacc_issuer_preimage_owner_size(),sizeof(root),sizeof(root)-papacc_issuer_preimage_owner_size());
 if(!strcmp(argv[1],"--codec-only")){CHECK(papacc_configuration_stop(&root)==0);CHECK(papacc_configuration_finish(&root)==0);printf("PASS codec_checks=%u owner=%zu storage=%u model=NATIVE_SHA256_NO_ISSUANCE\n",checks,sizeof(root),PAPACC_CONFIG_STORAGE_BYTES);return 0;}
 if(strcmp(argv[1],"--native")&&strcmp(argv[1],"--native-history")&&strcmp(argv[1],"--native-stop"))return 2;
 config_init(&c);CHECK(papacc_server_config_validate(&c)==0);CHECK(papacc_configuration_prepare05(&root,&c,0)==0);
 result=papacc_configuration_commit(&root);if(result!=0){printf("NATIVE_ROOT_COMMIT_NOT_PROVEN result=%d unresolved=%d\n",result,root.unresolved);return 3;}
 CHECK(papacc_configuration_publish(&root)==0);memcpy(base,root.committed,8192);memset(historical,0,sizeof(historical));
 CHECK(papacc_configuration_body05_encode(&root.slots[root.current].view,historical,sizeof(historical))==0);
 metadata_init();CHECK(papacc_configuration_issuer_hash06(&root,base,8192,hash));CHECK(papacc_issuer_image_init(image,&metadata,hash,papacc_configuration_issuer_hash06,&root)==0);
 CHECK(papacc_configuration_pin(&root,&pin,&pinned)==0);CHECK(papacc_configuration_issuer_prepare06(&root,image,1,1)!=0);CHECK(papacc_configuration_unpin(&root,&pin,0)==0);
 CHECK(papacc_configuration_issuer_prepare06(&root,image,1,1)==0);
 if(!strcmp(argv[1],"--native-stop")){
  HANDLE file;LARGE_INTEGER bytes;
  CHECK(papacc_configuration_stop(&root)==0);CHECK(papacc_configuration_commit(&root)!=0);CHECK(papacc_configuration_abort(&root)==0);CHECK(papacc_configuration_finish(&root)==0);
  file=CreateFileA(argv[2],GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);CHECK(file!=INVALID_HANDLE_VALUE);CHECK(GetFileSizeEx(file,&bytes)&&bytes.QuadPart==8192);CHECK(CloseHandle(file));
  printf("PASS native_stop_checks=%u PREPARED_NOT_COMMITTED HISTORICAL_BYTES_PRESERVED NO_NETWORK\n",checks);return 0;
 }
 CHECK(papacc_configuration_commit(&root)==0);
 CHECK(root.store02==6&&!memcmp(base,root.committed,8192));
#ifdef PAPACC_ISSUER_STORAGE_COMPONENT_TEST
 CHECK(papacc_configuration_publish(&root)==0);
 request_init(1);expected=2;high=1;
 CHECK(papacc_issuer_image_append(image,&record,NULL,0,papacc_configuration_issuer_hash06,&root)==0);
 CHECK(papacc_configuration_issuer_prepare06(&root,image,expected-1,high)!=0);CHECK(papacc_configuration_issuer_prepare06(&root,image,expected,high)==0);CHECK(papacc_configuration_commit(&root)==0);CHECK(papacc_configuration_publish(&root)==0);expected++;
 record.counter=2;record.serial[0]=3;record.serial[15]=2;record.state=PAPACC_ISSUER_SERIAL_RESERVED;
 CHECK(papacc_issuer_image_append(image,&record,NULL,0,papacc_configuration_issuer_hash06,&root)==0);CHECK(papacc_configuration_issuer_prepare06(&root,image,expected,1)==0);CHECK(papacc_configuration_commit(&root)==0);CHECK(papacc_configuration_publish(&root)==0);expected++;high=2;
 record.state=PAPACC_ISSUER_SIGN_REQUESTED;CHECK(papacc_issuer_image_append(image,&record,NULL,0,papacc_configuration_issuer_hash06,&root)==0);CHECK(papacc_configuration_issuer_prepare06(&root,image,expected,high)==0);CHECK(papacc_configuration_commit(&root)==0);CHECK(papacc_configuration_publish(&root)==0);
 if(!strcmp(argv[1],"--native-history")){
  record.state=PAPACC_ISSUER_ISSUED_UNPUBLISHED;record.certificate_size=3;CHECK(papacc_configuration_issuer_hash06(&root,(const PAPACC_U8 *)"DER",3,record.certificate_digest));CHECK(persist_step((const PAPACC_U8 *)"DER",3)==0);
  record.state=PAPACC_ISSUER_PUBLISHED;record.published_epoch=10;CHECK(persist_step(NULL,0)==0);
  record.state=PAPACC_ISSUER_REVOKED;CHECK(persist_step(NULL,0)==0);
  request_init(2);record.request.predecessor[0]=1;CHECK(persist_step(NULL,0)==0);
  record.state=PAPACC_ISSUER_SERIAL_RESERVED;record.counter=3;record.serial[0]=3;record.serial[15]=3;CHECK(persist_step(NULL,0)==0);
  record.state=PAPACC_ISSUER_SIGN_REQUESTED;CHECK(persist_step(NULL,0)==0);
  record.state=PAPACC_ISSUER_RECONCILIATION_REQUIRED;CHECK(persist_step(NULL,0)==0);high=3;
 }
 CHECK(!memcmp(base,root.committed,8192));CHECK(papacc_configuration_stop(&root)==0);CHECK(papacc_configuration_finish(&root)==0);
 CHECK(papacc_configuration_recovery_init02(&recovered,argv[2])==0);CHECK(papacc_configuration_issuer_recovery06(&recovered,argv[2])==PAPACC_RESULT_NOT_SUPPORTED);
 CHECK(recovered.recovery_selected&&recovered.recovery_guard&&!recovered.published);CHECK(papacc_configuration_issuer_copy06(&recovered,saved)==0);
 CHECK(papacc_issuer_image_find(saved,record.request.id,&found,papacc_configuration_issuer_hash06,&recovered)==0&&found.counter==record.counter&&found.state==record.state);
 CHECK(papacc_issuer_image_validate(saved,&decoded,papacc_configuration_issuer_hash06,&recovered)==0&&decoded.high_water==high&&!decoded.proofs&&!decoded.epoch);
 if(!strcmp(argv[1],"--native-history")){PAPACC_U8 first_id[16]={1};CHECK(papacc_issuer_image_find(saved,first_id,&found,papacc_configuration_issuer_hash06,&recovered)==0&&found.state==PAPACC_ISSUER_REVOKED);}
 {HANDLE blocked=CreateFileA(argv[2],GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);CHECK(blocked==INVALID_HANDLE_VALUE&&GetLastError()==ERROR_SHARING_VIOLATION);}
 CHECK(papacc_configuration_issuer_prepare06(&recovered,image,decoded.revision,decoded.high_water)!=0);
 CHECK(papacc_configuration_publish(&recovered)==PAPACC_RESULT_NOT_SUPPORTED);CHECK(papacc_configuration_stop(&recovered)==0);CHECK(papacc_configuration_finish(&recovered)==0);
#else
 (void)expected;(void)high;CHECK(papacc_configuration_publish(&root)==PAPACC_RESULT_NOT_SUPPORTED);
 CHECK(papacc_configuration_stop(&root)==0);CHECK(papacc_configuration_finish(&root)!=0); /* COMMITTED_PENDING retained */
#endif
 printf("PASS native_component_checks=%u ROOT06=COMMITTED_NOT_NORMATIVE_CURRENT owner=%zu key_operations=NOT_RUN\n",checks,sizeof(root));return 0;
}
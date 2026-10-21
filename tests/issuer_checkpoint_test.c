/* Native SHA256 of synthetic metadata only. No CNG key/sign/CA operations. */
#include "issuer_codec.h"
#include <windows.h>
#include <bcrypt.h>
#include <stdio.h>
#include <string.h>
static PAPACC_U8 image[8192],floor_image[8192],fork_image[8192];
static PAPACC_ISSUER_CHECKPOINT_REPORT report;
static PAPACC_ISSUER_RECORD record;
static unsigned checks;
#define CHECK(x) do{++checks;if(!(x)){printf("FAIL %d: %s\n",__LINE__,#x);return 1;}}while(0)
static int failed_hash(void *ctx,const PAPACC_U8 *b,PAPACC_SIZE n,PAPACC_U8 out[32]){
 (void)ctx;(void)b;(void)n;(void)out;return 0;
}
static BCRYPT_ALG_HANDLE provider;
static int native_hash(void *ctx,const PAPACC_U8 *b,PAPACC_SIZE n,PAPACC_U8 out[32]){
 (void)ctx;if(n>0xffffffffU)return 0;
 return BCryptHash(provider,NULL,0,(PUCHAR)b,(ULONG)n,out,32)==0;
}
static PAPACC_ISSUER_RESULT assess(const PAPACC_U8 *b,const PAPACC_U8 *f){
 return papacc_issuer_checkpoint_assess(b,8192,f,f?8192:0,native_hash,NULL,0,&report);
}
int main(void){
 PAPACC_ISSUER_VIEW v;PAPACC_U8 base_hash[32];PAPACC_U64 reserved;
 CHECK(BCryptOpenAlgorithmProvider(&provider,BCRYPT_SHA256_ALGORITHM,NULL,0)==0);
 memset(&v,0,sizeof(v));memset(base_hash,1,32);
 v.root[0]=1;v.authority[0]=2;v.generation[0]=3;v.prefix[0]=4;
 v.policy[0]=5;v.ca_spki_digest[0]=6;v.revision=2;v.high_water=1;v.ca_not_after=1000000;
 CHECK(papacc_issuer_image_init(image,&v,base_hash,native_hash,NULL)==0);
 memcpy(floor_image,image,8192);
 CHECK(assess(image,floor_image)==PAPACC_ISSUER_DENIED);
 CHECK(report.version==1&&report.state==PAPACC_ISSUER_CHECKPOINT_DATA_VALID_RECOVERY_DENIED);
 CHECK(!report.data.proofs&&!report.data.epoch);
 CHECK(assess(image,NULL)==PAPACC_ISSUER_DENIED&&report.state==PAPACC_ISSUER_CHECKPOINT_INCOMPLETE);
 CHECK(papacc_issuer_checkpoint_assess(image,8191,floor_image,8192,native_hash,NULL,0,&report)==PAPACC_ISSUER_INVALID);
 CHECK(papacc_issuer_checkpoint_assess(image,8192,NULL,8192,native_hash,NULL,0,&report)==PAPACC_ISSUER_INVALID);
 CHECK(papacc_issuer_checkpoint_assess(image,8192,floor_image,8192,native_hash,NULL,1,&report)==PAPACC_ISSUER_STOPPED);
 CHECK(report.state==PAPACC_ISSUER_CHECKPOINT_STOPPED&&!report.data.high_water);
 CHECK(papacc_issuer_checkpoint_assess(image,8192,floor_image,8192,failed_hash,NULL,0,&report)==PAPACC_ISSUER_UNCERTAIN);
 CHECK(report.state==PAPACC_ISSUER_CHECKPOINT_INCOMPLETE);
 memset(&record,0,sizeof(record));record.request.id[0]=1;record.request.device[0]=7;
 record.request.csr_digest[0]=8;record.request.key_digest[0]=9;record.request.purpose=1;
 record.request.not_before=100;record.request.not_after=500;record.approved_epoch=10;
 memcpy(record.authority,v.authority,16);memcpy(record.generation,v.generation,16);memcpy(record.policy,v.policy,32);
 record.state=PAPACC_ISSUER_PREPARED;
 CHECK(papacc_issuer_image_append(image,&record,NULL,0,native_hash,NULL)==0);
 memcpy(fork_image,image,8192);
 CHECK(assess(image,floor_image)==PAPACC_ISSUER_DENIED&&report.events==1);
 CHECK(assess(floor_image,image)==PAPACC_ISSUER_CONFLICT&&report.state==PAPACC_ISSUER_CHECKPOINT_ROLLBACK);
 record.state=PAPACC_ISSUER_SERIAL_RESERVED;record.counter=2;memcpy(record.serial,v.prefix,8);record.serial[15]=2;
 CHECK(papacc_issuer_image_append(image,&record,NULL,0,native_hash,NULL)==0);
 CHECK(assess(image,fork_image)==PAPACC_ISSUER_DENIED&&report.data.high_water==2);
 reserved=report.data.high_water;
 CHECK(assess(fork_image,image)==PAPACC_ISSUER_CONFLICT&&report.state==PAPACC_ISSUER_CHECKPOINT_ROLLBACK);
 memcpy(floor_image,image,8192);record.state=PAPACC_ISSUER_SIGN_REQUESTED;
 CHECK(papacc_issuer_image_append(image,&record,NULL,0,native_hash,NULL)==0);
 record.state=PAPACC_ISSUER_RECONCILIATION_REQUIRED;
 CHECK(papacc_issuer_image_append(image,&record,NULL,0,native_hash,NULL)==0);
 CHECK(assess(image,floor_image)==PAPACC_ISSUER_DENIED&&report.data.high_water==reserved);
 CHECK(papacc_issuer_image_find(image,record.request.id,&record,native_hash,NULL)==0&&record.state==PAPACC_ISSUER_RECONCILIATION_REQUIRED);
 /* Valid alternate branch under same identity, not a higher counter proof. */
 memcpy(fork_image,floor_image,8192);record.state=PAPACC_ISSUER_ABANDONED;
 CHECK(papacc_issuer_image_append(fork_image,&record,NULL,0,native_hash,NULL)==0);
 CHECK(assess(image,fork_image)==PAPACC_ISSUER_CONFLICT&&report.state==PAPACC_ISSUER_CHECKPOINT_DIVERGENT);
 memcpy(fork_image,image,8192);fork_image[300]^=1;
 CHECK(assess(fork_image,floor_image)==PAPACC_ISSUER_INVALID);
 CHECK(assess(image,fork_image)==PAPACC_ISSUER_INVALID&&report.state==PAPACC_ISSUER_CHECKPOINT_INCOMPLETE);
 v.generation[0]=99;CHECK(papacc_issuer_image_init(fork_image,&v,base_hash,native_hash,NULL)==0);
 CHECK(assess(image,fork_image)==PAPACC_ISSUER_CONFLICT&&report.state==PAPACC_ISSUER_CHECKPOINT_DIVERGENT);
 /* Separate complete synthetic publication/revocation history. */
 CHECK(papacc_issuer_image_init(image,&v,base_hash,native_hash,NULL)==0);
 memset(&record,0,sizeof(record));record.request.id[0]=2;record.request.device[0]=7;
 record.request.csr_digest[0]=8;record.request.key_digest[0]=9;record.request.purpose=1;
 record.request.not_before=100;record.request.not_after=500;record.approved_epoch=10;
 memcpy(record.authority,v.authority,16);memcpy(record.generation,v.generation,16);memcpy(record.policy,v.policy,32);
 record.state=PAPACC_ISSUER_PREPARED;CHECK(papacc_issuer_image_append(image,&record,NULL,0,native_hash,NULL)==0);
 record.state=PAPACC_ISSUER_SERIAL_RESERVED;record.counter=2;memcpy(record.serial,v.prefix,8);record.serial[15]=2;
 CHECK(papacc_issuer_image_append(image,&record,NULL,0,native_hash,NULL)==0);
 record.state=PAPACC_ISSUER_SIGN_REQUESTED;CHECK(papacc_issuer_image_append(image,&record,NULL,0,native_hash,NULL)==0);
 record.state=PAPACC_ISSUER_ISSUED_UNPUBLISHED;record.certificate_size=3;
 CHECK(native_hash(NULL,(const PAPACC_U8 *)"DER",3,record.certificate_digest));
 CHECK(papacc_issuer_image_append(image,&record,(const PAPACC_U8 *)"DER",3,native_hash,NULL)==0);
 record.state=PAPACC_ISSUER_PUBLISHED;record.published_epoch=11;
 CHECK(papacc_issuer_image_append(image,&record,NULL,0,native_hash,NULL)==0);
 memcpy(floor_image,image,8192);record.state=PAPACC_ISSUER_REVOKED;
 CHECK(papacc_issuer_image_append(image,&record,NULL,0,native_hash,NULL)==0);
 CHECK(assess(image,floor_image)==PAPACC_ISSUER_DENIED&&report.data.high_water==2);
 CHECK(papacc_issuer_image_find(image,record.request.id,&record,native_hash,NULL)==0&&record.state==PAPACC_ISSUER_REVOKED);
 CHECK(assess(floor_image,image)==PAPACC_ISSUER_CONFLICT&&report.state==PAPACC_ISSUER_CHECKPOINT_ROLLBACK);
 CHECK(papacc_issuer_checkpoint_assess(image,8192,floor_image,8192,native_hash,NULL,1,&report)==PAPACC_ISSUER_STOPPED);
 CHECK(BCryptCloseAlgorithmProvider(provider,0)==0);
 printf("PASS checks=%u report_bytes=%zu synthetic_metadata_native_sha256 authoritative_recovery=DENY\n",checks,sizeof(report));return 0;
}

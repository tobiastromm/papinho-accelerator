/* Isolated fixture-owned real BCrypt references; no injected native failure.
 * Retained states model the adapter result already tested by controlled shims. */
#include "server_configuration_commit_win32.h"
#include <stdio.h>
#include <string.h>
static PAPACC_CONFIGURATION_COMMIT o;
static unsigned checks,failures;
static void ck(int b,unsigned n){checks++;if(!b){failures++;printf("FAIL %u\n",n);}}
#define CK(b) ck((b),__LINE__)
int main(void){
 PAPACC_STORE_HASH *a=&o.store_hash,*b=&o.pending_commit.store_hash;
 unsigned char data[32];
 BCRYPT_ALG_HANDLE aa,ba;BCRYPT_HASH_HANDLE ah,bh;char path[MAX_PATH];DWORD n;
 n=GetTempPathA(MAX_PATH,path);CK(n&&n<MAX_PATH-48U);strcat_s(path,MAX_PATH,"papacc-hash-owner-NO-STORE");
 CK(papacc_configuration_commit_init03(&o,path)==PAPACC_RESULT_OK);
 CK(BCryptOpenAlgorithmProvider(&a->algorithm,BCRYPT_SHA256_ALGORITHM,NULL,0)==0);
 CK(BCryptCreateHash(a->algorithm,&a->hash,NULL,0,NULL,0,0)==0);
 CK(BCryptOpenAlgorithmProvider(&b->algorithm,BCRYPT_SHA256_ALGORITHM,NULL,0)==0);
 CK(BCryptCreateHash(b->algorithm,&b->hash,NULL,0,NULL,0,0)==0);
 aa=a->algorithm;ah=a->hash;ba=b->algorithm;bh=b->hash;a->retained=b->retained=1;
 a->cleanup_status=b->cleanup_status=(NTSTATUS)0xc0000001L;
 memset(data,0x55,sizeof(data));CK(!papacc_configuration_storage_hash(&o,(const unsigned char *)"abc",3U,data));
 CK(data[0]==0x55&&data[31]==0x55);CK(a->algorithm==aa&&a->hash==ah&&b->algorithm==ba&&b->hash==bh);
 CK(papacc_configuration_stop(&o)==PAPACC_RESULT_OK);
 CK(a->stopped&&b->stopped&&a->retained&&b->retained);
 CK(a->algorithm==aa&&a->hash==ah&&b->algorithm==ba&&b->hash==bh);
 CK(papacc_configuration_finish(&o)!=PAPACC_RESULT_OK&&o.initialized);
 CK(a->algorithm==aa&&a->hash==ah&&b->algorithm==ba&&b->hash==bh);
 /* Test created these valid resources directly; no native cleanup was attempted
  * before this known fixture teardown. This does NOT authorize production retry. */
 CK(BCryptDestroyHash(ah)==0);a->hash=NULL;CK(BCryptCloseAlgorithmProvider(aa,0)==0);a->algorithm=NULL;a->retained=0;
 CK(papacc_configuration_finish(&o)!=PAPACC_RESULT_OK&&o.initialized);
 CK(BCryptDestroyHash(bh)==0);b->hash=NULL;CK(BCryptCloseAlgorithmProvider(ba,0)==0);b->algorithm=NULL;b->retained=0;
 CK(papacc_configuration_finish(&o)==PAPACC_RESULT_OK&&!o.initialized);
 printf("checks=%u failures=%u SAME_OWNER_TWO_CONTEXTS_FOUR_REAL_REFERENCES_NO_STORE_NO_CURRENT\n",checks,failures);return failures?1:0;
}

/* Isolated fault shims. Actual cleanup failures are NOT forced/claimed.
 * Withheld native calls remain factually owned by this test until teardown. */
#include "server_store_hash_win32.h"
#include <stdio.h>
#include <string.h>
static unsigned checks,failures,opens,creates,hashes,finishes,destroys,closes;
static int fault,stop_during,reenter,nested;
static PAPACC_STORE_HASH *current;
static NTSTATUS fail=(NTSTATUS)0xc0000001L;
static NTSTATUS WINAPI open_test(BCRYPT_ALG_HANDLE *a,LPCWSTR n,LPCWSTR p,ULONG f){
 opens++;if(fault==1)return fail;
 if(fault==8)return (NTSTATUS)0x103;
 if(fault==9){NTSTATUS s=BCryptOpenAlgorithmProvider(a,n,p,f);return s?s:fail;}
 return BCryptOpenAlgorithmProvider(a,n,p,f);
}
static NTSTATUS WINAPI create_test(BCRYPT_ALG_HANDLE a,BCRYPT_HASH_HANDLE *h,PUCHAR o,ULONG on,PUCHAR k,ULONG kn,ULONG f){
 creates++;if(fault==2)return fail;
 if(fault==10){NTSTATUS s=BCryptCreateHash(a,h,o,on,k,kn,f);return s?s:fail;}
 return BCryptCreateHash(a,h,o,on,k,kn,f);
}
static NTSTATUS WINAPI data_test(BCRYPT_HASH_HANDLE h,PUCHAR p,ULONG n,ULONG f){
 unsigned char out[32];NTSTATUS s;hashes++;
 if(reenter)nested=papacc_store_hash(current,p,n,out);
 if(fault==3||fault==7)return fail;
 s=BCryptHashData(h,p,n,f);if(stop_during)InterlockedExchange(&current->stopped,1);return s;
}
static NTSTATUS WINAPI finish_test(BCRYPT_HASH_HANDLE h,PUCHAR p,ULONG n,ULONG f){finishes++;return fault==4?fail:BCryptFinishHash(h,p,n,f);}
static NTSTATUS WINAPI destroy_test(BCRYPT_HASH_HANDLE h){destroys++;return fault==5||fault==7?fail:BCryptDestroyHash(h);}
static NTSTATUS WINAPI close_test(BCRYPT_ALG_HANDLE a,ULONG f){closes++;return fault==6?fail:BCryptCloseAlgorithmProvider(a,f);}
#define BCryptOpenAlgorithmProvider open_test
#define BCryptCreateHash create_test
#define BCryptHashData data_test
#define BCryptFinishHash finish_test
#define BCryptDestroyHash destroy_test
#define BCryptCloseAlgorithmProvider close_test
#include "server_store_hash_win32.inc"
#undef BCryptOpenAlgorithmProvider
#undef BCryptCreateHash
#undef BCryptHashData
#undef BCryptFinishHash
#undef BCryptDestroyHash
#undef BCryptCloseAlgorithmProvider
static void ck(int b,unsigned line){checks++;if(!b){failures++;printf("FAIL %u\n",line);}}
#define CK(b) ck((b),__LINE__)
static void dispose_known_fixture(PAPACC_STORE_HASH *c){
 /* Shims deliberately did not perform these native cleanups. This direct
  * teardown is justified only by fixture provenance, not a production retry. */
 if(c->hash){CK(BCryptDestroyHash(c->hash)==0);c->hash=NULL;}
 if(c->algorithm){CK(BCryptCloseAlgorithmProvider(c->algorithm,0)==0);c->algorithm=NULL;}
}
int main(void){
 PAPACC_STORE_HASH c;unsigned char out[32],saved[32];unsigned i,before;
 static const unsigned char abc[32]={0xba,0x78,0x16,0xbf,0x8f,0x01,0xcf,0xea,0x41,0x41,0x40,0xde,0x5d,0xae,0x22,0x23,0xb0,0x03,0x61,0xa3,0x96,0x17,0x7a,0x9c,0xb4,0x10,0xff,0x61,0xf2,0x00,0x15,0xad};
 for(i=0;i<=10;i++){
  memset(&c,0,sizeof(c));current=&c;fault=(int)i;opens=creates=hashes=finishes=destroys=closes=0;
  memset(out,0x55,32);memcpy(saved,out,32);
  CK(papacc_store_hash(&c,(const unsigned char *)"abc",3,out)==(i==0));
  CK(!c.active);
  if(!i){CK(!memcmp(out,abc,32));CK(!c.hash&&!c.algorithm&&!c.retained);CK(destroys==1&&closes==1);}
  else CK(!memcmp(out,saved,32));
  if(i==1||i==8){CK(opens==1&&!creates&&!destroys&&!closes);CK(!c.hash&&!c.algorithm&&!c.retained);}
  if(i==2){CK(creates==1&&closes==1&&!destroys&&!c.retained);}
  if(i==3||i==4){CK(destroys==1&&closes==1&&!c.retained);CK(c.operation_status==fail);}
  if(i==5||i==7||i==9||i==10){CK(c.retained&&c.algorithm);if(i!=9)CK(c.hash!=NULL);
   if(i==5||i==7)CK(destroys==1&&!closes);
   if(i==7)CK(c.operation_status==fail&&c.cleanup_status==fail);
   before=opens+creates+hashes+finishes+destroys+closes;
   CK(!papacc_store_hash(&c,(const unsigned char *)"abc",3,out));
   InterlockedExchange(&c.stopped,1);CK(!papacc_store_hash(&c,(const unsigned char *)"abc",3,out));
   CK(before==opens+creates+hashes+finishes+destroys+closes);CK(papacc_store_hash_blocked(&c));
  }
  if(i==6){CK(c.retained&&c.algorithm&&!c.hash);CK(destroys==1&&closes==1);before=closes;
   CK(!papacc_store_hash(&c,(const unsigned char *)"abc",3,out));CK(closes==before);}
  dispose_known_fixture(&c);
 }
 fault=0;memset(&c,0,sizeof(c));current=&c;c.stopped=1;before=opens;CK(!papacc_store_hash(&c,(const unsigned char *)"abc",3,out));CK(opens==before);
 memset(&c,0,sizeof(c));stop_during=1;memset(out,0x55,32);CK(!papacc_store_hash(&c,(const unsigned char *)"abc",3,out));CK(c.stopped&&!c.active&&!c.hash&&!c.algorithm&&!c.retained);CK(c.operation_status==(NTSTATUS)0xc0000120L);CK(out[0]==0x55);stop_during=0;
 memset(&c,0,sizeof(c));reenter=1;nested=99;CK(papacc_store_hash(&c,(const unsigned char *)"abc",3,out));CK(nested==0&&!c.active);reenter=0;
 CK(!papacc_store_hash(NULL,(const unsigned char *)"abc",3,out));CK(!papacc_store_hash(&c,NULL,3,out));CK(!papacc_store_hash(&c,(const unsigned char *)"abc",3,NULL));
 printf("checks=%u failures=%u state_bytes=%Iu max_owned_refs_per_state=2 REAL_CNG_SUCCESS_CONTROLLED_SHIM_FAILURES_NO_AUTHORITY\n",checks,failures,sizeof(c));return failures?1:0;
}

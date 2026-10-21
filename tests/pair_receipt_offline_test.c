/* Synthetic metadata/owner guards only. No PST acquisition or network. */
#include "server_c2_owner_win32.h"
#include <stdio.h>
#include <string.h>
#define CHECK(x) do{if(!(x)){printf("FAIL line=%d\n",__LINE__);return 1;}}while(0)
static PAPACC_SERVER_C2_OWNER_WIN32 owner;
static PAPACC_C2_OPERATION_MANAGER manager;
static PAPACC_SERVER_SECURE_IO_WIN32 secure;
int main(void){
 PAPACC_U8 p[108]={0};PAPACC_C2_PAIR_OPEN_RECEIPT f,before;unsigned i;
 for(i=0;i<5U;++i)p[i*8U+7U]=(PAPACC_U8)(i+1U);
 p[35]=7U;p[39]=8U;p[42]=1U;memset(p+44U,0xa5,64U);
 CHECK(papacc_c2_pair_open_receipt(p,108U,9U,&f)==PAPACC_RESULT_OK);
 CHECK(f.request==1U&&f.operation==2U&&f.generation==3U&&f.epoch==4U&&f.attempt_generation==7U&&f.resolution_generation==8U);
 before=f;p[41]=1U;CHECK(papacc_c2_pair_open_receipt(p,108U,9U,&f)!=PAPACC_RESULT_OK&&!memcmp(&f,&before,sizeof(f)));
 p[41]=0U;p[35]=0U;CHECK(papacc_c2_pair_open_receipt(p,108U,9U,&f)!=PAPACC_RESULT_OK);p[35]=7U;
 CHECK(papacc_c2_pair_open_receipt(p,107U,9U,&f)!=PAPACC_RESULT_OK);
 CHECK(papacc_c2_pair_open_receipt(p,108U,0U,&f)!=PAPACC_RESULT_OK);
 owner.manager=&manager;owner.secure=&secure;manager.owner=GetCurrentThreadId();owner.initialized=PAPACC_TRUE;owner.e10.enabled=PAPACC_TRUE;owner.e10.pair_open[0]=before;
 CHECK(papacc_server_c2_pair_copy(&owner,0U,&f)!=PAPACC_RESULT_OK);
 owner.e10.sealed=PAPACC_TRUE;CHECK(papacc_server_c2_pair_copy(&owner,0U,&f)==PAPACC_RESULT_OK);
 CHECK(!memcmp(&f,&before,sizeof(f)));
 CHECK(papacc_server_c2_pair_copy(&owner,0U,&owner.e10.pair_open[0])!=PAPACC_RESULT_OK);
 secure.dispatch_active=PAPACC_TRUE;CHECK(papacc_server_c2_pair_copy(&owner,0U,&f)!=PAPACC_RESULT_OK);secure.dispatch_active=PAPACC_FALSE;
 owner.e10.overflow=PAPACC_TRUE;CHECK(papacc_server_c2_pair_copy(&owner,0U,&f)!=PAPACC_RESULT_OK);owner.e10.overflow=PAPACC_FALSE;
 owner.e10.reentrant=PAPACC_TRUE;CHECK(papacc_server_c2_pair_copy(&owner,0U,&f)!=PAPACC_RESULT_OK);owner.e10.reentrant=PAPACC_FALSE;
 CHECK(papacc_server_c2_pair_copy(&owner,1U,&f)!=PAPACC_RESULT_OK);
 CHECK(papacc_server_c2_pair_copy(&owner,2U,&f)!=PAPACC_RESULT_OK);
 printf("PAIR_RECEIPT_SIZE=%llu COLLECTOR_ADDITIONAL=%llu SYNTHETIC_ONLY PASS\n",(unsigned long long)sizeof(f),(unsigned long long)sizeof(owner.e10.pair_open));return 0;
}
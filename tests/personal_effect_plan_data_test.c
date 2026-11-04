#include "personal_plan_data.h"
#include <stdio.h>
#include <string.h>
static PAPACC_PERSONAL_PLAN_DATA p;
static unsigned char plan[8192],copy[8192],canary[8192];
#define CHECK(x) do{if(!(x)){printf("FAIL %u\n",(unsigned)__LINE__);return 1;}}while(0)
int main(void)
{
 unsigned char op[4]={1,2,3,4},service[4]={4,3,2,1},parent[12]={1},a[16]={1},g[16]={2},id[32]={1};
 const unsigned char config[]="format_version 1\nclient_admission DISABLED\n";const unsigned char *source;
 PAPACC_PERSONAL_EFFECT_SPEC v;size_t n,bytes,i,z;char reply[128];
 static const char hex[]="0123456789abcdef",prefix[]="OWNER_NEW_INSTALL_CA_INITIALIZE ";
 memset(&v,0,sizeof(v));v.operator_id=op;v.operator_bytes=4;v.service_id=service;v.service_bytes=4;
 v.parent_id=parent;v.leaf=(const unsigned char *)"child";v.leaf_bytes=5;v.authority=a;v.generation=g;v.configuration=config;v.configuration_bytes=sizeof(config)-1U;
 n=papacc_personal_effect_build(&v,plan,sizeof(plan));CHECK(n&&n<sizeof(plan));memcpy(copy,plan,n);
 CHECK(papacc_personal_effect_source(plan,n,&source,&bytes)&&bytes==sizeof(config)-1U&&!memcmp(source,config,bytes));
 memset(canary,0xa5,sizeof(canary));CHECK(!papacc_personal_effect_build(&v,canary,n-1U));for(i=0;i<sizeof(canary);i++)CHECK(canary[i]==0xa5);
 CHECK(!papacc_personal_effect_build(&v,(unsigned char *)config,sizeof(config))); /* insufficient capacity, no write */
 v.service_id=op;CHECK(!papacc_personal_effect_build(&v,canary,sizeof(canary)));v.service_id=service;
 v.leaf=(const unsigned char *)"../x";v.leaf_bytes=4;CHECK(!papacc_personal_effect_build(&v,canary,sizeof(canary)));v.leaf=(const unsigned char *)"child";v.leaf_bytes=5;
 CHECK(papacc_personal_data_capture(&p,id,config,sizeof(config)-1U));
 CHECK(papacc_personal_effect_upgrade(&p,plan,n)&&p.effectful);
 CHECK(papacc_personal_data_present(&p));
 CHECK(!papacc_personal_data_confirm(&p,id,plan,n)); /* read-only API cannot confirm effects */
 CHECK(!papacc_personal_effect_confirm(&p,id,plan,n,"OWNER_NEW_INSTALL",17U));CHECK(p.phase==PAPACC_PERSONAL_DATA_INVALID&&!p.consumed);
 memset(&p,0,sizeof(p));CHECK(papacc_personal_data_capture(&p,id,config,sizeof(config)-1U));
 CHECK(papacc_personal_data_present(&p)&&papacc_personal_data_confirm(&p,id,config,sizeof(config)-1U));
 CHECK(!papacc_personal_effect_upgrade(&p,plan,n)); /* F10 confirmed scope cannot be promoted */
 memset(&p,0,sizeof(p));CHECK(papacc_personal_data_capture(&p,id,config,sizeof(config)-1U));CHECK(papacc_personal_effect_upgrade(&p,plan,n)&&papacc_personal_data_present(&p));
 z=sizeof(prefix)-1U;memcpy(reply,prefix,z);for(i=0;i<32U;i++){reply[z+2U*i]=hex[id[i]>>4];reply[z+2U*i+1U]=hex[id[i]&15U];}reply[z+64U]=0;
 plan[n-1U]^=1U;CHECK(!papacc_personal_effect_confirm(&p,id,plan,n,reply,z+64U)&&p.phase==PAPACC_PERSONAL_DATA_INVALID);memcpy(plan,copy,n);
 memset(&p,0,sizeof(p));CHECK(papacc_personal_data_capture(&p,id,config,sizeof(config)-1U));CHECK(papacc_personal_effect_upgrade(&p,plan,n)&&papacc_personal_data_present(&p));
 CHECK(papacc_personal_effect_confirm(&p,id,plan,n,reply,z+64U));
 CHECK(!papacc_personal_data_consume(&p,id,plan,n));CHECK(papacc_personal_effect_consume(&p,id,plan,n));
 CHECK(!papacc_personal_effect_consume(&p,id,plan,n)&&!papacc_personal_effect_upgrade(&p,plan,n));
 papacc_personal_data_invalidate(&p);CHECK(p.consumed&&!papacc_personal_effect_consume(&p,id,plan,n));CHECK(papacc_personal_data_retire(&p));
 for(i=0;i<sizeof(p.plan);i++)CHECK(p.plan[i]==0U);
 {PAPACC_PERSONAL_PREPARED_SPEC f;unsigned char target[12]={3},image[12]={4},digest[32]={5};
  memset(&f,0,sizeof(f));f.selection=v;f.selection.leaf=(const unsigned char *)"Accelerator";f.selection.leaf_bytes=11;
  f.target_id=target;f.source_id=parent;f.image_id=image;f.image_digest=digest;
  n=papacc_personal_prepared_build(&f,plan,sizeof(plan));CHECK(n&&papacc_personal_prepared_matches(plan,n,&f));
  CHECK(papacc_personal_effect_source(plan,n,&source,&bytes)&&bytes==sizeof(config)-1U);
  image[1]=1;CHECK(!papacc_personal_prepared_matches(plan,n,&f));image[1]=0;
  CHECK(!papacc_personal_prepared_build(&f,plan,n-1U));
  memset(&p,0,sizeof(p));CHECK(papacc_personal_data_capture(&p,id,config,sizeof(config)-1U));
  CHECK(papacc_personal_prepared_upgrade(&p,plan,n)&&p.effectful==3U&&papacc_personal_data_present(&p));
  CHECK(!papacc_personal_effect_confirm(&p,id,plan,n,reply,z+64U));
  memset(&p,0,sizeof(p));CHECK(papacc_personal_data_capture(&p,id,config,sizeof(config)-1U));
  CHECK(papacc_personal_prepared_upgrade(&p,plan,n)&&papacc_personal_data_present(&p));
  z=strlen("OWNER_PREPARED_INSTANCE_VALIDATE ");memcpy(reply,"OWNER_PREPARED_INSTANCE_VALIDATE ",z);
  for(i=0;i<32U;i++){reply[z+2U*i]=hex[id[i]>>4];reply[z+2U*i+1U]=hex[id[i]&15U];}
  CHECK(papacc_personal_prepared_confirm(&p,id,plan,n,reply,z+64U));
  CHECK(!papacc_personal_data_consume(&p,id,plan,n));CHECK(papacc_personal_effect_consume(&p,id,plan,n));
  CHECK(!papacc_personal_effect_consume(&p,id,plan,n)&&!papacc_personal_prepared_upgrade(&p,plan,n));
  f.selection.leaf=(const unsigned char *)"Papinho";f.selection.leaf_bytes=7;CHECK(!papacc_personal_prepared_build(&f,plan,sizeof(plan)));
 }
 puts("PASS EFFECT_PLAN_DATA_ONLY FCR_SCOPE REPLAY_SUBSTITUTION_CROSS_PURPOSE_DENY NO_NATIVE_EFFECT");return 0;
}

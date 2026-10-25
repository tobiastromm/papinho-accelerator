#include "server_store_hash_win32.h"
#ifndef PAPACC_FIXTURE_STORE_HASH_DEFINED
#define PAPACC_FIXTURE_STORE_HASH_DEFINED
static PAPACC_STORE_HASH fixture_store_hash;
#endif
/* Synthetic source/disposition/issuer model only. No real authority/UTC.
 * Reuse the historical v3/v2 builders; existing native SHA/BODY helpers.
 * kind9 uses the real independent mathematical DATA adapter; no authority. */
#define PAPACC_V3_BACKING_FIXTURE 1
#include "config_root8_v3_test.c"
#include "config_root8_v4.h"
#include "config_root8_time_source_key.h"
static PAPACC_U8 f4[PC_ROOT8_V4_FIELDS_MAX],save4[PC_ROOT8_V4_FIELDS_MAX];
static PAPACC_U8 e4[PC_ROOT8_V4_MAX+1U],d4[PC_ROOT8_V4_FIELDS_MAX],base4[PC_ROOT8_V4_MAX];
static PAPACC_SIZE n4,z4;static int deny_source;static unsigned key_calls;
static PC_ROOT8_RESULT component4(void *ctx,unsigned kind,const PAPACC_U8 *p,PAPACC_SIZE n,const PAPACC_U8 *row,const PAPACC_U8 *header)
{if(kind==9U){key_calls++;if(deny_source)return PC_ROOT8_INCOMPLETE;return pc_root8_time_source_key_data(NULL,kind,p,n,row,header);}return component3(ctx,kind,p,n,row,header);}
static int seal4(void)
{
 PAPACC_SIZE n;CHECK(pc_root8_v4_snt_encode(f4,n4,part,sizeof(part),&n,hash,&owner)==PC_ROOT8_OK);memcpy(f4+r32(f4+244),part,n);
 CHECK(pc_root8_v4_pdg_encode(f4,n4,part,sizeof(part),&n,hash,&owner)==PC_ROOT8_OK);memcpy(f4+r32(f4+260),part,n);
 CHECK(pc_root8_v4_tsc_encode(f4,n4,part,sizeof(part),&n,hash,&owner)==PC_ROOT8_OK);memcpy(f4+r32(f4+276),part,n);
 CHECK(pc_root8_v4_encode(f4,n4,e4,sizeof(e4),&z4,hash,&owner,component4,NULL)==PC_ROOT8_OK);return 1;
}
static int upgrade4(unsigned H)
{
 PAPACC_SIZE X=r32(f3+40),F=96U+384U*H;PAPACC_U8 *s,*a;unsigned j;
 memset(f4,0,sizeof(f4));n4=n3+F;memcpy(f4,f3,X);memcpy(f4+X+F,f3+X,n3-X);
 w16(f4+8,4);w16(f4+240,3);w32(f4+12,(PAPACC_U32)(n4+32U));w32(f4+40,(PAPACC_U32)(X+F));
 w32(f4+48,r32(f3+48)+(PAPACC_U32)F);w32(f4+56,r32(f3+56)+(PAPACC_U32)F);w32(f4+64,r32(f3+64)+(PAPACC_U32)F);
 w16(f4+274,1);w32(f4+276,(PAPACC_U32)X);w32(f4+280,(PAPACC_U32)F);w16(f4+284,H);w16(f4+286,384);w16(f4+288,1);
 memset(f4+r32(f4+56),0,512);s=f4+X;memcpy(s,"PACTSC01",8);w16(s+8,1);w16(s+10,64);w32(s+12,(PAPACC_U32)F);w16(s+16,H);w16(s+18,384);memcpy(s+24,f4+72,32);w64(s+56,r64(f4+104));
 for(j=0;j<H;j++){
  a=s+64U+384U*j;memcpy(a,"PACTSR01",8);w16(a+8,1);w16(a+10,384);w32(a+12,1);w16(a+16,1);w16(a+18,1);w16(a+20,1);w16(a+22,4);
  a[24]=(PAPACC_U8)(j+1);a[56]=(PAPACC_U8)(j+4);memset(a+88,0x66,32);a[88]=0x58;CHECK(hash(&owner,a+88,32,a+120));
  a[152]=192;a[153]=0;a[154]=2;a[155]=(PAPACC_U8)(j+1);w16(a+168,5319);a[176]=17;w64(a+208,1);w64(a+216,1700000000);w64(a+224,1700003600);
  a[232]=20;a[264]=21;a[296]=22;
 }
 return seal4();
}
static int cp4(void)
{
 PAPACC_SIZE n;PAPACC_U8 *c=f4+r32(f4+56);memset(c,0,512);memcpy(c,"PACR8CP4",8);w16(c+8,4);w16(c+10,512);w32(c+12,1);memcpy(c+16,f4+72,32);
 w64(c+48,r64(f4+104));w64(c+56,r64(f4+112));w64(c+64,18);w64(c+72,9);w64(c+80,r64(f4+120));memcpy(c+120,f4+176,64);CHECK(hash(&owner,f4,r32(f4+56),c+88));
 CHECK(pc_root8_v4_cp_encode(f4,n4,part,sizeof(part),&n,hash,&owner)==PC_ROOT8_OK&&n==512);memcpy(c,part,512);return seal4();
}
static int next4(PAPACC_SIZE bn)
{
 PAPACC_SIZE L,n;PAPACC_U8 *a,*b;unsigned j,N=f4[20];PAPACC_U64 R=r64(base4+104)+1U;
 w16(f4+240,4);w64(f4+104,R);w64(f4+112,R-1);w64(f4+136,R-1);w64(f4+144,18);w64(f4+152,9);w64(f4+160,r64(base4+120));
 memcpy(f4+176,base4+r32(base4+56)+88,32);memcpy(f4+208,base4+bn-32U,32);memset(f4+r32(f4+56),0,512);
 w64(f4+r32(f4+244)+56,R);w64(f4+r32(f4+260)+56,R);w64(f4+r32(f4+276)+56,R);
 for(j=0;j<N;j++){a=f4+512U+1536U*j;config_init(j,R);CHECK(papacc_configuration_body05_encode(&fixture_store_hash,&view,a+512,1024)==PAPACC_RESULT_OK);CHECK(hash(&owner,a+512,1024,a+312));}
 L=r32(f4+44);b=f4+r32(f4+40);memset(&lst,0,sizeof(lst));memcpy(lst.fields,b,L-32U);w64(lst.fields+56,R-1);w64(lst.fields+88,R-2);w64(lst.fields+64,18);w64(lst.fields+72,9);w64(lst.fields+80,r64(base4+120));
 for(j=0;j<N;j++){memcpy(lst.fields+128U+512U*j,base4+512U+1536U*j,512);memset(lst.fields+128U+512U*j+344U,0,40);}
 CHECK(pc_lst1_encode(&lst,b,L,&n,hash,&owner)==PC_LST1_OK);return seal4();
}
static int run4(void)
{
 unsigned N,B,D,H,state,i;PAPACC_SIZE n,bn;int complete;PAPACC_U8 *a,*s;PAPACC_CONFIG_PIN pin;PC_ROOT8_RESULT result;char path[MAX_PATH];DWORD length;
 CHECK(run3()); /* existing v3 regressions end with factual finish */
 memset(&owner,0,sizeof(owner));length=GetFullPathNameA("v4-data-unused-no-store",MAX_PATH,path,NULL);CHECK(length&&length<MAX_PATH);
 CHECK(papacc_configuration_commit_init03(&owner,path)==PAPACC_RESULT_OK);
 for(N=1;N<=4;N++)for(B=1;B<=4;B++)for(D=0;D<=N;D++)for(H=0;H<=3;H++){
  CHECK(setup3(N,B,D));CHECK(upgrade4(H));CHECK(z4==n4+32U);CHECK(pc_root8_v4_validate(e4,z4,hash,&owner,component4,NULL)==PC_ROOT8_OK);
  CHECK(pc_root8_v4_decode(e4,z4,d4,sizeof(d4),&n,&complete,hash,&owner,component4,NULL)==PC_ROOT8_OK&&complete&&n==n4&&!memcmp(f4,d4,n4));
  CHECK(pc_root8_v3_validate(e4,z4,hash,&owner,component3,NULL)!=PC_ROOT8_OK);CHECK(cp4());
 }
 CHECK(setup3(1,1,1));CHECK(upgrade4(3));CHECK(cp4());memcpy(save4,f4,n4);
 {PAPACC_ROOT8_CARRIER_DATA context;PAPACC_U8 *keyrow=f4+r32(f4+276)+64U;
 memset(&context,0,sizeof(context));context.owner=&owner;
 CHECK(papacc_root8_issuer_carrier_data(NULL,9U,keyrow,384U,keyrow,f4)==PC_ROOT8_INCOMPLETE);
 CHECK(papacc_root8_issuer_carrier_data(&context,9U,keyrow,384U,keyrow,f4)==PC_ROOT8_OK);
 owner.stopped=1;CHECK(papacc_root8_issuer_carrier_data(&context,9U,keyrow,384U,keyrow,f4)==PC_ROOT8_INCOMPLETE);owner.stopped=0;
 }
 /* Rehashed valid TSR1 integrity must not hide an invalid key. */
 memcpy(f4,save4,n4);memset(f4+r32(f4+56),0,512);a=f4+r32(f4+276)+64U;
 memset(a+88,0,32);a[88]=1;CHECK(hash(&owner,a+88,32,a+120));
 CHECK(pc_root8_v4_tsc_encode(f4,n4,part,sizeof(part),&n,hash,&owner)==PC_ROOT8_OK);
 memcpy(f4+r32(f4+276),part,n);n=99;
 CHECK(pc_root8_v4_encode(f4,n4,e4,sizeof(e4),&n,hash,&owner,component4,NULL)==PC_ROOT8_INVALID&&n==0);
 /* Signed status is policy DATA, never a positive source authority. */
 for(state=0;state<=3;state++){memcpy(f4,save4,n4);memset(f4+r32(f4+56),0,512);w16(f4+r32(f4+276)+64U+16U,state);CHECK(seal4());}
 memcpy(f4,save4,n4);memset(f4+r32(f4+56),0,512);CHECK(seal4());deny_source=1;CHECK(pc_root8_v4_validate(e4,z4,hash,&owner,component4,NULL)==PC_ROOT8_INCOMPLETE);deny_source=0;
 CHECK(pc_root8_v4_validate(e4,z4,hash,&owner,NULL,NULL)==PC_ROOT8_INCOMPLETE);
 for(n=0;n<z4;n++)CHECK(pc_root8_v4_validate(e4,n,hash,&owner,component4,NULL)!=PC_ROOT8_OK);
 e4[z4]=0;CHECK(pc_root8_v4_validate(e4,z4+1,hash,&owner,component4,NULL)!=PC_ROOT8_OK);
 for(n=0;n<z4;n++){e4[n]^=1;CHECK(pc_root8_v4_validate(e4,z4,hash,&owner,component4,NULL)!=PC_ROOT8_OK);e4[n]^=1;}
 CHECK(pc_root8_v4_validate(e4,z4,fail_hash,NULL,component4,NULL)==PC_ROOT8_HASH_FAILED);
 memset(d4,0x55,sizeof(d4));n=99;complete=99;e4[0]^=1;CHECK(pc_root8_v4_decode(e4,z4,d4,sizeof(d4),&n,&complete,hash,&owner,component4,NULL)!=PC_ROOT8_OK&&n==0&&!complete&&d4[0]==0x55);e4[0]^=1;
 CHECK(pc_root8_v4_encode(f4,n4,f4,sizeof(f4),&n,hash,&owner,component4,NULL)==PC_ROOT8_INVALID);
 /* Semantic header/collection/CP4 checks do not rely on outer W4 alone. */
 CHECK(cp4());memcpy(save4,f4,n4);
 {static const unsigned offsets[]={8,10,12,16,20,40,48,56,64,104,112,120,136,168,172,240,274,276,280,284,286,288,290,292};
 for(i=0;i<sizeof(offsets)/sizeof(offsets[0]);i++){memcpy(f4,save4,n4);f4[offsets[i]]^=1;CHECK(pc_root8_v4_encode(f4,n4,e4,sizeof(e4),&n,hash,&owner,component4,NULL)!=PC_ROOT8_OK);}}
 {static const unsigned offsets[]={0,8,10,12,16,18,20,24,56};
 for(i=0;i<sizeof(offsets)/sizeof(offsets[0]);i++){memcpy(f4,save4,n4);s=f4+r32(f4+276);s[offsets[i]]^=1;CHECK(pc_root8_v4_tsc_encode(f4,n4,part,sizeof(part),&n,hash,&owner)==PC_ROOT8_INVALID);}}
 {static const unsigned offsets[]={0,8,10,12,16,48,56,64,72,80,88,120,152,184};
 for(i=0;i<sizeof(offsets)/sizeof(offsets[0]);i++){memcpy(f4,save4,n4);s=f4+r32(f4+56);s[offsets[i]]^=1;if(offsets[i]==64||offsets[i]==72)w64(s+offsets[i],0);
 CHECK(pc_root8_v4_cp_encode(f4,n4,part,sizeof(part),&n,hash,&owner)==PC_ROOT8_INVALID);}}
 memcpy(f4,save4,n4);memset(f4+r32(f4+56),0,512);CHECK(seal4());
 /* Explicit rehashed semantics: invalid rows cannot be sealed. */
 memcpy(save4,f4,n4);a=f4+r32(f4+276)+64U;
 {static const unsigned offsets[]={0,8,10,12,18,20,22,120,168,170,208,224,232,264,296,328};
 for(i=0;i<sizeof(offsets)/sizeof(offsets[0]);i++){memcpy(f4,save4,n4);a=f4+r32(f4+276)+64U;a[offsets[i]]^=1;
  if(offsets[i]==208)w64(a+208,r64(f4+104)+1U);if(offsets[i]==224)w64(a+224,0);if(offsets[i]==232||offsets[i]==264||offsets[i]==296)memset(a+offsets[i],0,32);
  result=pc_root8_v4_tsc_encode(f4,n4,part,sizeof(part),&n,hash,&owner);CHECK(result==PC_ROOT8_INVALID);}}
 memcpy(f4,save4,n4);a=f4+r32(f4+276)+64U;w16(a+16,4);CHECK(pc_root8_v4_tsc_encode(f4,n4,part,sizeof(part),&n,hash,&owner)==PC_ROOT8_INVALID);
 memcpy(f4,save4,n4);a=f4+r32(f4+276)+64U;memcpy(a+384+24,a+24,32);CHECK(pc_root8_v4_tsc_encode(f4,n4,part,sizeof(part),&n,hash,&owner)==PC_ROOT8_INVALID);
 memcpy(f4,save4,n4);a=f4+r32(f4+276)+64U;memset(a+88,0,32);CHECK(pc_root8_v4_tsc_encode(f4,n4,part,sizeof(part),&n,hash,&owner)==PC_ROOT8_INVALID);
 memcpy(f4,save4,n4);a=f4+r32(f4+276)+64U;a[88]^=1;CHECK(pc_root8_v4_tsc_encode(f4,n4,part,sizeof(part),&n,hash,&owner)==PC_ROOT8_INVALID);
 memcpy(f4,save4,n4);a=f4+r32(f4+276)+64U;w16(a+22,6);memset(a+152,0,16);a[167]=1;CHECK(seal4());
 a[152]=0xfe;a[153]=0x80;CHECK(pc_root8_v4_tsc_encode(f4,n4,part,sizeof(part),&n,hash,&owner)==PC_ROOT8_INVALID);
 /* Complete predecessor linkage, no factual approval/CAS/CURRENT inferred. */
 CHECK(setup3(1,1,1));CHECK(cp3());bn=z3;memcpy(base3,e3,bn);CHECK(next3(3,bn));CHECK(upgrade4(0));CHECK(pc_root8_v4_base_link(e4,z4,base3,bn,hash,&owner,component4,NULL)==PC_ROOT8_OK);
 CHECK(pc_root8_v4_base_link(e4,z4,NULL,0,hash,&owner,component4,NULL)==PC_ROOT8_INCOMPLETE);base3[0]^=1;CHECK(pc_root8_v4_base_link(e4,z4,base3,bn,hash,&owner,component4,NULL)!=PC_ROOT8_OK);base3[0]^=1;
 CHECK(upgrade4(3));CHECK(pc_root8_v4_base_link(e4,z4,base3,bn,hash,&owner,component4,NULL)==PC_ROOT8_INCOMPLETE);
 CHECK(cp4());bn=z4;memcpy(base4,e4,bn);CHECK(next4(bn));CHECK(pc_root8_v4_base_link(e4,z4,base4,bn,hash,&owner,component4,NULL)==PC_ROOT8_OK);
 a=f4+r32(f4+276)+64U;w16(a+16,3);w64(a+208,r64(f4+104));CHECK(seal4());CHECK(pc_root8_v4_base_link(e4,z4,base4,bn,hash,&owner,component4,NULL)==PC_ROOT8_INCOMPLETE);
 memset(&pin,0,sizeof(pin));CHECK(papacc_configuration_d3_begin08(&owner,&pin,e4,z4,INVALID_HANDLE_VALUE,0)!=PAPACC_RESULT_OK);CHECK(!owner.pending_commit.active&&!owner.pending_commit.thread&&!owner.published);
 memcpy(d4,f4,n4);CHECK(papacc_configuration_stop(&owner)==PAPACC_RESULT_OK);CHECK(!memcmp(d4,f4,n4));CHECK(papacc_configuration_d3_begin08(&owner,&pin,e4,z4,INVALID_HANDLE_VALUE,0)!=PAPACC_RESULT_OK);
 CHECK(papacc_configuration_finish(&owner)==PAPACC_RESULT_OK);s=f4+r32(f4+276);CHECK(!memcmp(s,"PACTSC01",8));
 printf("PASS %u V4_DATA_MODEL_ONLY NO_TRUST NO_CURRENT NO_UTC key_model_calls=%u codec_owned_bytes=0 test_arrays=%Iu max=%u\n",checks,key_calls,sizeof(f4)+sizeof(save4)+sizeof(e4)+sizeof(d4)+sizeof(base4),PC_ROOT8_V4_MAX);return 1;
}
#ifdef PAPACC_V4_BACKING_FIXTURE
int v4_fixture_entry_not_run(void){return run4()?0:1;}
#else
int main(void){return run4()?0:1;}
#endif

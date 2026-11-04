/* Accepted L1 DATA and SAME owner; synthetic manifests never authority. */
#define PAPACC_V4_BACKING_FIXTURE 1
#include "config_root8_v4_test.c"
#include "config_root8_v5.h"
static PAPACC_U8 f5[PC_ROOT8_V5_FIELDS_MAX],e5[PC_ROOT8_V5_MAX],saved5[PC_ROOT8_V5_MAX],origin5[PC_ROOT8_V5_MAX];
static PAPACC_SIZE n5,z5;
static unsigned q4_calls;static int deny_q4;
static void wb64(PAPACC_U8 *p,PAPACC_U64 v){unsigned i;for(i=0;i<8U;i++)p[7U-i]=(PAPACC_U8)(v>>(8U*i));}
static PC_ROOT8_RESULT component5(void *ctx,unsigned kind,const PAPACC_U8 *p,PAPACC_SIZE n,const PAPACC_U8 *row,const PAPACC_U8 *h)
{if(kind==9U&&h[8]==5U)return pc_root8_v5_key_data(ctx,kind,p,n,row,h);if(kind==10U){q4_calls++;return deny_q4?PC_ROOT8_INCOMPLETE:pc_root8_q4_data(p,n,hash,&owner);}return component4(ctx,kind,p,n,row,h);}
static int seal5(void)
{
 PAPACC_SIZE n;PAPACC_U8 *q=f5+r32(f5+292);
 CHECK(hash(&owner,q,340,q+340));
 CHECK(pc_root8_v5_snt_encode(f5,n5,part,sizeof(part),&n,hash,&owner)==0);memcpy(f5+r32(f5+244),part,n);
 CHECK(pc_root8_v5_pdg_encode(f5,n5,part,sizeof(part),&n,hash,&owner)==0);memcpy(f5+r32(f5+260),part,n);
 CHECK(pc_root8_v5_tsc_encode(f5,n5,part,sizeof(part),&n,hash,&owner)==0);memcpy(f5+r32(f5+276),part,n);
 CHECK(pc_root8_v5_encode(f5,n5,e5,sizeof(e5),&z5,hash,&owner,component5,NULL)==0);return 1;
}
static int upgrade5(void)
{
 unsigned Q=r32(f4+56),i;PAPACC_U8 *q;
 n5=n4+512U;memset(f5,0,sizeof(f5));memcpy(f5,f4,Q);memcpy(f5+Q+512U,f4+Q,n4-Q);
 w16(f5+8,5);w16(f5+240,4);w32(f5+12,(PAPACC_U32)(n5+32U));w32(f5+56,Q+512U);w32(f5+64,Q+1024U);
 w32(f5+292,Q);w32(f5+296,512);w16(f5+300,1);w16(f5+302,1);
 q=f5+Q;memcpy(q,"PACQ4D01",8);q[8]=1;q[9]=1;wb64(q+12,10);wb64(q+20,11);wb64(q+28,100);wb64(q+36,200);
 for(i=0;i<9U;i++)q[52U+32U*i]=(PAPACC_U8)(i+1U);
 memcpy(q+52,f5+72,32);CHECK(hash(&owner,base4,z4,q+84));return seal5();
}
static int cp5(void)
{
 PAPACC_U8 *c=f5+r32(f5+56);PAPACC_SIZE n;memset(c,0,512);memcpy(c,"PACR8CP5",8);w16(c+8,5);w16(c+10,512);w32(c+12,1);memcpy(c+16,f5+72,32);
 w64(c+48,r64(f5+104));w64(c+56,r64(f5+112));w64(c+64,18);w64(c+72,9);w64(c+80,r64(f5+120));memcpy(c+120,f5+176,64);CHECK(hash(&owner,f5,r32(f5+56),c+88));
 CHECK(pc_root8_v5_cp_encode(f5,n5,part,sizeof(part),&n,hash,&owner)==0);memcpy(c,part,n);return seal5();
}
static int next5(PAPACC_SIZE bn,unsigned state)
{
 unsigned j,N=f5[20];PAPACC_SIZE L,n;PAPACC_U8 *a,*b;PAPACC_U64 R=r64(saved5+104)+1U;
 w16(f5+240,5);w64(f5+104,R);w64(f5+112,R-1);w64(f5+136,R-1);memcpy(f5+176,saved5+r32(saved5+56)+88,32);memcpy(f5+208,saved5+bn-32U,32);memset(f5+r32(f5+56),0,512);
 w64(f5+r32(f5+244)+56,R);w64(f5+r32(f5+260)+56,R);w64(f5+r32(f5+276)+56,R);
 for(j=0;j<N;j++){a=f5+512U+1536U*j;config_init(j,R);CHECK(papacc_configuration_body05_encode(&fixture_store_hash,&view,a+512,1024)==0);CHECK(hash(&owner,a+512,1024,a+312));}
 L=r32(f5+44);b=f5+r32(f5+40);memset(&lst,0,sizeof(lst));memcpy(lst.fields,b,L-32U);w64(lst.fields+56,R-1);w64(lst.fields+88,R-2);
 for(j=0;j<N;j++){memcpy(lst.fields+128U+512U*j,saved5+512U+1536U*j,512);memset(lst.fields+128U+512U*j+344U,0,40);}
 CHECK(pc_lst1_encode(&lst,b,L,&n,hash,&owner)==0);a=f5+r32(f5+292);a[9]=(PAPACC_U8)state;if(state==4U)wb64(a+44,10);return seal5();
}
static int initial5(unsigned N,unsigned B,unsigned D,unsigned H)
{
 PAPACC_SIZE bn;CHECK(setup3(N,B,D));CHECK(upgrade4(H));CHECK(cp4());bn=z4;memcpy(base4,e4,bn);CHECK(next4(bn));CHECK(upgrade5());
 if(N==B)CHECK(pc_root8_v5_base_link(e5,z5,base4,bn,hash,&owner,component5,NULL)==0);else CHECK(pc_root8_v5_base_link(e5,z5,base4,bn,hash,&owner,component5,NULL)!=0);return 1;
}
#ifndef PAPACC_V5_BACKING_FIXTURE
static int run5(void)
{
 unsigned N,B,D,H,i;PAPACC_SIZE n,bn;int complete;PAPACC_U8 *q;PC_ROOT8_RESULT status;char path[MAX_PATH];DWORD length;
 length=GetFullPathNameA("v5-unused-no-store",MAX_PATH,path,NULL);CHECK(length&&length<MAX_PATH);CHECK(papacc_configuration_commit_init03(&owner,path)==0);
 for(N=1;N<=4U;N++)for(B=N;B<=N;B++)for(D=0;D<=N;D++)for(H=0;H<=3U;H++){
 CHECK(initial5(N,B,D,H));CHECK(pc_root8_v5_validate(e5,z5,hash,&owner,component5,NULL)==0);
 CHECK(pc_root8_v5_decode(e5,z5,saved5,sizeof(saved5),&n,&complete,hash,&owner,component5,NULL)==0&&complete&&n==n5&&!memcmp(f5,saved5,n5));
 CHECK(pc_root8_v4_validate(e5,z5,hash,&owner,component4,NULL)!=0);CHECK(cp5());}
 CHECK(initial5(4,4,4,3));CHECK(z5==PC_ROOT8_V5_MAX);memcpy(origin5,e5,z5);
 for(n=0;n<z5;n++){e5[n]^=1;CHECK(pc_root8_v5_validate(e5,z5,hash,&owner,component5,NULL)!=0);e5[n]^=1;}
 for(n=0;n<z5;n++)CHECK(pc_root8_v5_validate(e5,n,hash,&owner,component5,NULL)!=0);
 deny_q4=1;CHECK(pc_root8_v5_validate(e5,z5,hash,&owner,component5,NULL)==PC_ROOT8_INCOMPLETE);deny_q4=0;
 CHECK(pc_root8_v5_validate(e5,z5,hash,&owner,NULL,NULL)==PC_ROOT8_INCOMPLETE);
 CHECK(pc_root8_v5_validate(e5,z5,fail_hash,NULL,component5,NULL)==PC_ROOT8_HASH_FAILED);
 q=f5+r32(f5+292);
 for(i=0;i<512U;i++){q[i]^=1;status=pc_root8_q4_data(q,512,hash,&owner);q[i]^=1;CHECK(status!=0);}
 /* Rehashed structure attacks, not merely accidental outer corruption. */
 {static const unsigned offsets[]={8,10,12,56,64,104,112,292,296,300,302,304};
 for(i=0;i<sizeof(offsets)/sizeof(offsets[0]);i++){memcpy(f5,origin5,n5);f5[offsets[i]]^=1;n=99;CHECK(pc_root8_v5_encode(f5,n5,e5,sizeof(e5),&n,hash,&owner,component5,NULL)!=0&&n==0);}}
 memcpy(f5,origin5,n5);w16(f5+240,5);CHECK(seal5());CHECK(pc_root8_v5_base_link(e5,z5,base4,z4,hash,&owner,component5,NULL)!=0);
 memcpy(f5,origin5,n5);q=f5+r32(f5+292);
 wb64(q+12,(PAPACC_U64)-1);wb64(q+20,0);CHECK(hash(&owner,q,340,q+340));CHECK(pc_root8_q4_data(q,512,hash,&owner)!=0);
 memcpy(f5,origin5,n5);q=f5+r32(f5+292);wb64(q+28,0x100000000ULL);CHECK(hash(&owner,q,340,q+340));CHECK(pc_root8_q4_data(q,512,hash,&owner)!=0);
 memcpy(f5,origin5,n5);q=f5+r32(f5+292);q[9]=4;CHECK(hash(&owner,q,340,q+340));CHECK(pc_root8_q4_data(q,512,hash,&owner)!=0);
 memcpy(f5,origin5,n5);q=f5+r32(f5+292);memcpy(q+84,f5+208,32);CHECK(seal5());
 CHECK(pc_root8_v5_base_link(e5,z5,base4,z4,hash,&owner,component5,NULL)!=0); /* prefix digest is not whole-file identity */
 memcpy(f5,origin5,n5);config_init(0,r64(f5+104));view.config.site_tls.max_bytes++;
 CHECK(papacc_configuration_body05_encode(&fixture_store_hash,&view,f5+1024,1024)==0);CHECK(hash(&owner,f5+1024,1024,f5+824));CHECK(seal5());
 CHECK(pc_root8_v5_validate(e5,z5,hash,&owner,component5,NULL)==0);CHECK(pc_root8_v5_base_link(e5,z5,base4,z4,hash,&owner,component5,NULL)!=0);
 memcpy(f5,origin5,n5);CHECK(seal5());
 CHECK(cp5());bn=z5;memcpy(saved5,e5,bn);CHECK(next5(bn,2));CHECK(pc_root8_v5_base_link(e5,z5,saved5,bn,hash,&owner,component5,NULL)==0);
 q=f5+r32(f5+292);q[9]=3;CHECK(seal5());CHECK(pc_root8_v5_base_link(e5,z5,saved5,bn,hash,&owner,component5,NULL)!=0);
 q[9]=2;q[116]^=1;CHECK(seal5());CHECK(pc_root8_v5_base_link(e5,z5,saved5,bn,hash,&owner,component5,NULL)!=0);q[116]^=1;
 q[9]=4;wb64(q+44,11);CHECK(seal5());CHECK(pc_root8_v5_base_link(e5,z5,saved5,bn,hash,&owner,component5,NULL)==0);
 q[9]=2;wb64(q+44,0);CHECK(seal5());CHECK(cp5());bn=z5;memcpy(saved5,e5,bn);CHECK(next5(bn,3));CHECK(pc_root8_v5_base_link(e5,z5,saved5,bn,hash,&owner,component5,NULL)==0);
 CHECK(cp5());bn=z5;memcpy(saved5,e5,bn);CHECK(next5(bn,3));CHECK(pc_root8_v5_base_link(e5,z5,saved5,bn,hash,&owner,component5,NULL)!=0);
 CHECK(!owner.published&&!owner.pending_commit.active);CHECK(papacc_configuration_stop(&owner)==0);CHECK(papacc_configuration_finish(&owner)==0);
 printf("PASS V5_DATA assertions=%u q4_calls=%u max=%u owner=%Iu pending=%Iu no_io_no_authority\n",checks,q4_calls,PC_ROOT8_V5_MAX,sizeof(owner),sizeof(owner.pending_commit));return 1;
}
int main(void){return run5()?0:1;}
#endif

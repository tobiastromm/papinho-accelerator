/* Reuse the existing v2 DATA fixtures/builders, never native CURRENT/trust.
 * Public three-byte "certificate" is explicitly a MODEL, not X509 evidence. */
#define main v2_fixture_entry_not_run
#include "config_root8_v2_test.c"
#undef main
#include "config_root8_v3.h"
static PAPACC_U8 f3[PC_ROOT8_V3_FIELDS_MAX],save3[PC_ROOT8_V3_FIELDS_MAX];
static PAPACC_U8 e3[PC_ROOT8_V3_MAX+1U],d3[PC_ROOT8_V3_FIELDS_MAX],p3[PC_PDG1_MAX];
static PAPACC_U8 base3[PC_ROOT8_V3_MAX];
static PAPACC_SIZE n3,z3;static int reject_signer;
static PC_ROOT8_RESULT component3(void *ctx,unsigned kind,const PAPACC_U8 *p,PAPACC_SIZE n,const PAPACC_U8 *row,const PAPACC_U8 *header)
{if(kind==8U){(void)ctx;(void)p;(void)row;(void)header;return reject_signer?PC_ROOT8_INCOMPLETE:n==16384U?PC_ROOT8_OK:PC_ROOT8_INVALID;}return model_component(ctx,kind,p,n,row,header);}
static int seal3(void)
{
 PAPACC_SIZE n;CHECK(pc_root8_v3_snt_encode(f3,n3,part,sizeof(part),&n,hash,&owner)==PC_ROOT8_OK);memcpy(f3+r32(f3+244),part,n);
 CHECK(pc_root8_v3_pdg_encode(f3,n3,p3,sizeof(p3),&n,hash,&owner)==PC_ROOT8_OK);memcpy(f3+r32(f3+260),p3,n);
 CHECK(pc_root8_v3_encode(f3,n3,e3,sizeof(e3),&z3,hash,&owner,component3,NULL)==PC_ROOT8_OK);return 1;
}
static int setup3(unsigned N,unsigned B,unsigned D)
{
 PAPACC_SIZE U,V=96U+384U*D,off;PAPACC_U8 *pg,*a,*image;PAPACC_ISSUER_VIEW v;PAPACC_ISSUER_RECORD rec;
 PAPACC_U8 cert[3]={1,2,3};unsigned j;
 CHECK(setup(N,B,1));memset(f3,0,sizeof(f3));U=r32(fields+40);n3=prefix+V;
 memcpy(f3,fields,U);memcpy(f3+U+V,fields+U,prefix-U);
 w16(f3+8,3);w16(f3+240,2);w32(f3+12,(PAPACC_U32)(n3+32U));w32(f3+40,(PAPACC_U32)(U+V));
 w32(f3+48,r32(fields+48)+(PAPACC_U32)V);w32(f3+56,r32(fields+56)+(PAPACC_U32)V);w32(f3+64,r32(fields+64)+(PAPACC_U32)V);
 w16(f3+258,1);w32(f3+260,(PAPACC_U32)U);w32(f3+264,(PAPACC_U32)V);w16(f3+268,D);w16(f3+270,384);w16(f3+272,1);
 pg=f3+U;memcpy(pg,"PACPDG01",8);w16(pg+8,1);w16(pg+10,64);w32(pg+12,(PAPACC_U32)V);w16(pg+16,D);w16(pg+18,384);pg[24]=7;w64(pg+56,2);
 off=r32(f3+48);image=f3+off+8192;CHECK(papacc_issuer_image_validate(image,&v,hash,&owner)==PAPACC_ISSUER_OK);
 if(D){
 memset(&rec,0,sizeof(rec));rec.request.id[0]=15;rec.request.device[0]=16;rec.request.csr_digest[0]=17;rec.request.key_digest[0]=18;
 rec.request.purpose=PAPACC_ISSUER_PACKAGE;rec.request.not_before=1700000000;rec.request.not_after=1700003600;rec.approved_epoch=8;
 memcpy(rec.authority,v.authority,16);memcpy(rec.generation,v.generation,16);memcpy(rec.policy,v.policy,32);rec.state=PAPACC_ISSUER_PREPARED;
 CHECK(papacc_issuer_image_append(image,&rec,NULL,0,hash,&owner)==PAPACC_ISSUER_OK);
 rec.state=PAPACC_ISSUER_SERIAL_RESERVED;rec.counter=2;memcpy(rec.serial,v.prefix,8);rec.serial[15]=2;
 CHECK(papacc_issuer_image_append(image,&rec,NULL,0,hash,&owner)==PAPACC_ISSUER_OK);rec.state=PAPACC_ISSUER_SIGN_REQUESTED;
 CHECK(papacc_issuer_image_append(image,&rec,NULL,0,hash,&owner)==PAPACC_ISSUER_OK);rec.state=PAPACC_ISSUER_ISSUED_UNPUBLISHED;rec.certificate_size=3;
 CHECK(hash(&owner,cert,3,rec.certificate_digest));CHECK(papacc_issuer_image_append(image,&rec,cert,3,hash,&owner)==PAPACC_ISSUER_OK);
 rec.state=PAPACC_ISSUER_PUBLISHED;rec.published_epoch=18;CHECK(papacc_issuer_image_append(image,&rec,NULL,0,hash,&owner)==PAPACC_ISSUER_OK);
 CHECK(papacc_issuer_image_validate(image,&v,hash,&owner)==PAPACC_ISSUER_OK);w64(f3+120,v.revision);
 for(j=0;j<D;j++){
 a=pg+64U+384U*j;memcpy(a,"PACPDL01",8);w16(a+8,1);w16(a+10,384);w32(a+12,1);w16(a+16,1);w16(a+18,1);w16(a+20,1);w16(a+22,j);w64(a+24,2);
 memcpy(a+32,f3+512U+1536U*j+280U,32);memcpy(a+64,v.authority,16);memcpy(a+80,v.generation,16);memcpy(a+96,rec.request.id,16);
 memcpy(a+112,rec.certificate_digest,32);memcpy(a+144,rec.request.key_digest,32);w64(a+176,1700000100);w64(a+184,1700003000);
 w16(a+192,2);memcpy(a+196,f3+512U+1536U*j+24U,64);memcpy(a+260,v.ca_spki_digest,32);
 }}
 return seal3();
}
static int cp3(void)
{
 PAPACC_U8 *c=f3+r32(f3+56);PAPACC_SIZE n;memset(c,0,512);memcpy(c,"PACR8CP3",8);w16(c+8,3);w16(c+10,512);w32(c+12,1);
 memcpy(c+16,f3+72,32);w64(c+48,r64(f3+104));w64(c+56,r64(f3+112));w64(c+64,18);w64(c+72,9);w64(c+80,r64(f3+120));memcpy(c+120,f3+176,64);
 CHECK(hash(&owner,f3,r32(f3+56),c+88));CHECK(pc_root8_v3_cp_encode(f3,n3,part,sizeof(part),&n,hash,&owner)==PC_ROOT8_OK&&n==512);memcpy(c,part,512);return seal3();
}
static int next3(unsigned version,PAPACC_SIZE bn)
{
 PAPACC_U8 *q,*b;PAPACC_SIZE n,L;unsigned j,N=(unsigned)base3[20];
 w16(f3+240,version);w64(f3+104,3);w64(f3+112,2);w64(f3+136,2);w64(f3+144,18);w64(f3+152,9);w64(f3+160,r64(base3+120));
 memcpy(f3+176,base3+r32(base3+56)+88,32);memcpy(f3+208,base3+bn-32U,32);memset(f3+r32(f3+56),0,512);
 w64(f3+r32(f3+244)+56,3);w64(f3+r32(f3+260)+56,3);
 for(j=0;j<(unsigned)f3[20];j++){q=f3+512U+1536U*j;config_init(j,3);CHECK(papacc_configuration_body05_encode(&view,q+512,1024)==PAPACC_RESULT_OK);CHECK(hash(&owner,q+512,1024,q+312));}
 L=r32(f3+44);b=f3+r32(f3+40);memset(&lst,0,sizeof(lst));memcpy(lst.fields,b,L-32U);w64(lst.fields+56,2);w64(lst.fields+88,1);w64(lst.fields+64,18);w64(lst.fields+72,9);w64(lst.fields+80,r64(base3+120));
 for(j=0;j<N;j++){memcpy(lst.fields+128U+512U*j,base3+512U+1536U*j,512);memset(lst.fields+128U+512U*j+344U,0,40);}
 CHECK(pc_lst1_encode(&lst,b,L,&n,hash,&owner)==PC_LST1_OK);return seal3();
}
static int run3(void)
{
 unsigned N,B,D,j;PAPACC_SIZE n,bn;int complete;PAPACC_U8 *a,*image;PAPACC_ISSUER_RECORD rec;PAPACC_ISSUER_VIEW iv;
 PAPACC_CONFIG_PIN pin={0};char path[MAX_PATH];DWORD len;
 len=GetFullPathNameA("v3-data-unused-no-store",MAX_PATH,path,NULL);CHECK(len&&len<MAX_PATH);CHECK(papacc_configuration_commit_init03(&owner,path)==PAPACC_RESULT_OK);
 for(N=1;N<=4;N++)for(B=1;B<=4;B++)for(D=0;D<=N;D++){
 CHECK(setup3(N,B,D));CHECK(pc_root8_v3_decode(e3,z3,d3,sizeof(d3),&n,&complete,hash,&owner,component3,NULL)==PC_ROOT8_OK&&complete&&n==n3&&!memcmp(f3,d3,n));CHECK(cp3());
 }
 CHECK(setup3(1,1,1));memcpy(save3,f3,n3);a=f3+r32(f3+260)+64U;
 w16(a+16,2);CHECK(seal3()); /* revoked DATA remains representable, never trust */
 memcpy(f3,save3,n3);CHECK(seal3());
 for(j=0;j<z3;j++){e3[j]^=1;CHECK(pc_root8_v3_validate(e3,z3,hash,&owner,component3,NULL)!=PC_ROOT8_OK);e3[j]^=1;}
 for(n=1;n<z3;n++)CHECK(pc_root8_v3_validate(e3,n,hash,&owner,component3,NULL)!=PC_ROOT8_OK);
 CHECK(pc_root8_v3_validate(e3,z3+1,hash,&owner,component3,NULL)!=PC_ROOT8_OK);
 {static const unsigned bad[]={0,8,10,12,16,18,20,22,24,32,64,80,96,112,144,192,194,196,260,292};
 for(j=0;j<sizeof(bad)/sizeof(bad[0]);j++){
 memcpy(f3,save3,n3);a=f3+r32(f3+260)+64U;a[bad[j]]^=255;
 if(pc_root8_v3_pdg_encode(f3,n3,p3,sizeof(p3),&n,hash,&owner)==PC_ROOT8_OK){memcpy(f3+r32(f3+260),p3,n);CHECK(pc_root8_v3_encode(f3,n3,e3,sizeof(e3),&n,hash,&owner,component3,NULL)!=PC_ROOT8_OK);}
 else CHECK(n==0);
 }}
 memcpy(f3,save3,n3);a=f3+r32(f3+260)+64U;w64(a+176,1699999999);CHECK(pc_root8_v3_pdg_encode(f3,n3,p3,sizeof(p3),&n,hash,&owner)==PC_ROOT8_OK);memcpy(f3+r32(f3+260),p3,n);CHECK(pc_root8_v3_encode(f3,n3,e3,sizeof(e3),&n,hash,&owner,component3,NULL)==PC_ROOT8_INVALID);
 memcpy(f3,save3,n3);a=f3+r32(f3+260)+64U;w64(a+184,1700003601);CHECK(pc_root8_v3_pdg_encode(f3,n3,p3,sizeof(p3),&n,hash,&owner)==PC_ROOT8_OK);memcpy(f3+r32(f3+260),p3,n);CHECK(pc_root8_v3_encode(f3,n3,e3,sizeof(e3),&n,hash,&owner,component3,NULL)==PC_ROOT8_INVALID);
 memcpy(f3,save3,n3);CHECK(seal3());reject_signer=1;CHECK(pc_root8_v3_validate(e3,z3,hash,&owner,component3,NULL)==PC_ROOT8_INCOMPLETE);reject_signer=0;
 CHECK(pc_root8_v3_validate(e3,z3,hash,&owner,model_component,NULL)==PC_ROOT8_UNSUPPORTED);
 CHECK(pc_root8_v3_validate(e3,z3,hash,&owner,NULL,NULL)==PC_ROOT8_INCOMPLETE);
 CHECK(pc_root8_v3_validate(e3,z3,fail_hash,NULL,component3,NULL)==PC_ROOT8_HASH_FAILED);
 CHECK(pc_root8_v2_validate(e3,z3,hash,&owner,component3,NULL)!=PC_ROOT8_OK);
 n=99;complete=99;CHECK(pc_root8_v3_decode(e3,z3,e3,sizeof(e3),&n,&complete,hash,&owner,component3,NULL)==PC_ROOT8_INVALID&&n==99&&complete==99);
 /* Rehashed/fragment-level negatives exercise shape, not just outer SHA. */
 {static const unsigned bad[]={8,10,12,16,20,22,24,26,28,30,32,36,40,44,48,52,56,60,64,68,104,112,120,128,132,134,136,168,172,240,242,244,248,252,254,256,258,260,264,268,270,272,274};
 memcpy(save3,f3,n3);
 for(j=0;j<sizeof(bad)/sizeof(bad[0]);j++){memcpy(f3,save3,n3);f3[bad[j]]^=255;n=99;CHECK(pc_root8_v3_encode(f3,n3,e3,sizeof(e3),&n,hash,&owner,component3,NULL)!=PC_ROOT8_OK&&!n);}
 memcpy(f3,save3,n3);CHECK(seal3());}
 {static const unsigned bad[]={8,12,16,48,56,64,72,80,88,120,152,184};PAPACC_U8 *c;
 for(j=0;j<sizeof(bad)/sizeof(bad[0]);j++){
 CHECK(cp3());c=f3+r32(f3+56);if(bad[j]==64U||bad[j]==72U)memset(c+bad[j],0,8);else c[bad[j]]^=255;
 n=99;CHECK(pc_root8_v3_cp_encode(f3,n3,part,sizeof(part),&n,hash,&owner)!=PC_ROOT8_OK&&!n);
 }
 memset(f3+r32(f3+56),0,512);CHECK(seal3());}
 CHECK(cp3());memcpy(base3,e3,z3);bn=z3;CHECK(next3(3,bn));CHECK(pc_root8_v3_base_link(e3,z3,base3,bn,hash,&owner,component3,NULL)==PC_ROOT8_OK);
 CHECK(pc_root8_v3_base_link(e3,z3,NULL,0,hash,&owner,component3,NULL)==PC_ROOT8_INCOMPLETE);
 CHECK(pc_root8_v3_base_link(e3,z3,e3,z3,hash,&owner,component3,NULL)==PC_ROOT8_INVALID);
 f3[176]^=1;CHECK(seal3());CHECK(pc_root8_v3_base_link(e3,z3,base3,bn,hash,&owner,component3,NULL)==PC_ROOT8_INVALID);f3[176]^=1;
 a=f3+r32(f3+260)+64U;w16(a+16,2);w64(a+24,3);CHECK(seal3());CHECK(pc_root8_v3_base_link(e3,z3,base3,bn,hash,&owner,component3,NULL)==PC_ROOT8_INCOMPLETE);
 /* v2 adoption absent delegation is DATA-only; no approval can be inferred. */
 CHECK(setup3(1,1,0));CHECK(checkpoints());memcpy(base3,encoded,used);bn=used;CHECK(next3(2,bn));CHECK(pc_root8_v3_base_link(e3,z3,base3,bn,hash,&owner,component3,NULL)==PC_ROOT8_OK);
 /* Complete issuer status/high-water survives codec; no serial allocation. */
 CHECK(setup3(1,1,1));image=f3+r32(f3+48)+8192;memset(&rec,0,sizeof(rec));rec.request.id[0]=15;
 CHECK(papacc_issuer_image_find(image,rec.request.id,&rec,hash,&owner)==PAPACC_ISSUER_OK);rec.state=PAPACC_ISSUER_REVOKED;
 CHECK(papacc_issuer_image_append(image,&rec,NULL,0,hash,&owner)==PAPACC_ISSUER_OK);CHECK(papacc_issuer_image_validate(image,&iv,hash,&owner)==PAPACC_ISSUER_OK&&iv.high_water==2);w64(f3+120,iv.revision);CHECK(seal3());
 CHECK(papacc_configuration_d3_begin08(&owner,&pin,e3,z3,INVALID_HANDLE_VALUE,0)!=PAPACC_RESULT_OK); /* no CURRENT; no worker started */
 CHECK(!owner.pending_commit.active&&!owner.pending_commit.thread&&!owner.published);
 CHECK(papacc_configuration_stop(&owner)==PAPACC_RESULT_OK);CHECK(papacc_configuration_d3_begin08(&owner,&pin,e3,z3,INVALID_HANDLE_VALUE,0)!=PAPACC_RESULT_OK);
 CHECK(papacc_configuration_finish(&owner)==PAPACC_RESULT_OK);
 printf("PASS %u V3_DATA_MODEL_ONLY NO_TRUST NO_CURRENT NO_SIGNATURE operation_buffers=%Iu max=%u\n",checks,sizeof(f3)+sizeof(save3)+sizeof(e3)+sizeof(d3)+sizeof(base3)+sizeof(p3),PC_ROOT8_V3_MAX);return 1;
}
#ifdef PAPACC_V3_BACKING_FIXTURE
int v3_fixture_entry_not_run(void){return run3()?0:1;}
#else
int main(void){return run3()?0:1;}
#endif

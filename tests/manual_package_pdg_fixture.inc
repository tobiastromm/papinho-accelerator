/* TEST ONLY: synthetic BODY/approval/carrier context, no actual CURRENT,
 * signer certificate profile or independent confirmation. No files or keys. */
#include "../src/issuer/issuer_codec.h"
static unsigned char pdg_fields[PC_ROOT8_V3_FIELDS_MAX],pdg_root[PC_ROOT8_V3_MAX],pdg_part[PC_SNT1_MAX];
static PC_LST1_MODEL pdg_lst;
static size_t pdg_prefix,pdg_bytes;
static int pdg_stop_after_data;
static void pl16(unsigned char *p,unsigned v){p[0]=(unsigned char)v;p[1]=(unsigned char)(v>>8);}
static void pl32(unsigned char *p,size_t v){unsigned j;for(j=0;j<4;j++)p[j]=(unsigned char)(v>>(8*j));}
static void pl64(unsigned char *p,PAPACC_U64 v){unsigned j;for(j=0;j<8;j++)p[j]=(unsigned char)(v>>(8*j));}
static unsigned pr32(const unsigned char *p){return (unsigned)p[0]|((unsigned)p[1]<<8)|((unsigned)p[2]<<16)|((unsigned)p[3]<<24);}
static int phash(void *ctx,const PAPACC_U8 *p,PAPACC_SIZE n,PAPACC_U8 out[32]){return mp_crypto_public_sha256(ctx,p,n,out)==MP_CRYPTO_ONLY;}
static PC_ROOT8_RESULT pdg_model_component(void *ctx,unsigned kind,const PAPACC_U8 *p,PAPACC_SIZE n,const PAPACC_U8 *row,const PAPACC_U8 *header){
 (void)ctx;(void)p;(void)row;(void)header;
 if(kind==8U&&pdg_stop_after_data)pdg_stop_after_data=2;
 return (kind==5U&&n==1024U)||((kind==6U||kind==7U||kind==8U)&&n==16384U)?PC_ROOT8_OK:PC_ROOT8_INVALID;
}
static int pdg_stop(void *ctx){(void)ctx;return pdg_stop_after_data==2;}
static int pdg_fixture_rehash(MP_CRYPTO *c){
 size_t U=pr32(pdg_fields+260),V=pr32(pdg_fields+264);
 if(!phash(c,pdg_fields+U,V-32,pdg_fields+U+V-32))return 0;
 memcpy(pdg_root,pdg_fields,pdg_prefix);
 return phash(c,pdg_root,pdg_prefix,pdg_root+pdg_prefix);
}
static int pdg_fixture_make(MP_CRYPTO *c,const unsigned char spki_digest[32],unsigned D){
 size_t O=2048,S=864,U=2912,V=96+384*D,L=672,Q=U+V+L+16384,n;unsigned char *b,*a,*carrier,*image,*q;
 PAPACC_ISSUER_VIEW v;PAPACC_ISSUER_RECORD rec;unsigned char anchor[32],cert[3]={1,2,3};
 memset(pdg_fields,0,sizeof(pdg_fields));pdg_stop_after_data=0;pdg_prefix=Q+512;pdg_bytes=pdg_prefix+32;
 b=pdg_fields;memcpy(b,"PACROOT8",8);pl16(b+8,3);pl16(b+10,512);pl32(b+12,pdg_bytes);pl32(b+16,3);pl16(b+20,1);pl16(b+22,512);pl16(b+24,1024);pl16(b+26,1);pl16(b+28,512);pl16(b+30,1);
 pl32(b+32,512);pl32(b+36,1536);pl32(b+40,U+V);pl32(b+44,L);pl32(b+48,U+V+L);pl32(b+52,16384);pl32(b+56,Q);pl32(b+60,512);pl32(b+64,Q+512);pl32(b+68,32);
 b[72]=7;pl64(b+104,2);pl64(b+112,1);pl64(b+136,1);pl64(b+144,8);pl64(b+152,9);pl64(b+160,7);pl32(b+128,31);pl16(b+132,5);pl16(b+134,1);pl32(b+168,15);b[176]=1;b[208]=2;
 pl16(b+240,2);pl16(b+242,1);pl32(b+244,O);pl32(b+248,S);pl16(b+252,1);pl16(b+254,768);pl16(b+256,1);pl16(b+258,1);pl32(b+260,U);pl32(b+264,V);pl16(b+268,D);pl16(b+270,384);pl16(b+272,1);
 q=b+512;pl32(q,127);q[4]=q[5]=q[6]=1;pl16(q+8,7443);pl16(q+10,2);pl16(q+12,6);pl16(q+16,6);memcpy(q+24,"La",2);memcpy(q+88,"source",6);memcpy(q+216,"policy",6);q[280]=1;memcpy(q+344,"PACCD001",8);
 if(!phash(c,q+512,1024,q+312))return 0;
 memset(&pdg_lst,0,sizeof(pdg_lst));a=pdg_lst.fields;memcpy(a,"PACLST01",8);pl16(a+8,1);pl16(a+10,128);pl32(a+12,L);pl16(a+16,1);pl16(a+18,512);pl32(a+20,31);a[24]=7;pl64(a+56,1);pl64(a+64,8);pl64(a+72,9);pl64(a+80,7);pl16(a+96,1);memcpy(a+128,q,512);memset(a+128+344,0,40);
 if(pc_lst1_encode(&pdg_lst,b+U+V,L,&n,phash,c)!=PC_LST1_OK)return 0;
 carrier=b+U+V+L;memcpy(carrier,"MODELANC",8);if(!phash(c,carrier,8192,anchor))return 0;image=carrier+8192;
 memset(&v,0,sizeof(v));v.root[0]=7;v.authority[0]=2;v.generation[0]=3;v.prefix[0]=4;v.policy[0]=5;v.ca_spki_digest[0]=6;v.revision=7;v.high_water=1;v.ca_not_after=2000000000;
 if(papacc_issuer_image_init(image,&v,anchor,phash,c)!=PAPACC_ISSUER_OK)return 0;
 memset(&rec,0,sizeof(rec));rec.request.id[0]=15;rec.request.device[0]=16;rec.request.csr_digest[0]=17;memcpy(rec.request.key_digest,spki_digest,32);rec.request.purpose=PAPACC_ISSUER_PACKAGE;rec.request.not_before=1700000000;rec.request.not_after=1700003600;rec.approved_epoch=8;
 memcpy(rec.authority,v.authority,16);memcpy(rec.generation,v.generation,16);memcpy(rec.policy,v.policy,32);rec.state=PAPACC_ISSUER_PREPARED;
 if(papacc_issuer_image_append(image,&rec,NULL,0,phash,c)!=PAPACC_ISSUER_OK)return 0;
 rec.state=PAPACC_ISSUER_SERIAL_RESERVED;rec.counter=2;memcpy(rec.serial,v.prefix,8);rec.serial[15]=2;
 if(papacc_issuer_image_append(image,&rec,NULL,0,phash,c)!=PAPACC_ISSUER_OK)return 0;rec.state=PAPACC_ISSUER_SIGN_REQUESTED;
 if(papacc_issuer_image_append(image,&rec,NULL,0,phash,c)!=PAPACC_ISSUER_OK)return 0;rec.state=PAPACC_ISSUER_ISSUED_UNPUBLISHED;rec.certificate_size=3;
 if(!phash(c,cert,3,rec.certificate_digest)||papacc_issuer_image_append(image,&rec,cert,3,phash,c)!=PAPACC_ISSUER_OK)return 0;rec.state=PAPACC_ISSUER_PUBLISHED;rec.published_epoch=18;
 if(papacc_issuer_image_append(image,&rec,NULL,0,phash,c)!=PAPACC_ISSUER_OK||papacc_issuer_image_validate(image,&v,phash,c)!=PAPACC_ISSUER_OK)return 0;pl64(b+120,v.revision);
 a=b+O;memcpy(a,"PACSNT01",8);pl16(a+8,1);pl16(a+10,64);pl32(a+12,S);pl16(a+16,1);pl16(a+18,768);a[24]=7;pl64(a+56,2);
 a+=64;memcpy(a,"PACSNI01",8);pl16(a+8,1);pl16(a+10,768);pl32(a+12,15);a[18]=a[19]=1;pl16(a+20,1);a[22]=1;pl16(a+24,2);pl16(a+26,6);pl16(a+28,13);pl64(a+32,2);memcpy(a+40,v.authority,16);memcpy(a+56,v.generation,16);memcpy(a+184,"La",2);memcpy(a+248,"source",6);memcpy(a+312,"accel.example",13);memcpy(a+568,v.ca_spki_digest,32);
 if(pc_root8_v3_snt_encode(b,pdg_prefix,pdg_part,sizeof(pdg_part),&n,phash,c)!=PC_ROOT8_OK)return 0;memcpy(b+O,pdg_part,n);
 a=b+U;memcpy(a,"PACPDG01",8);pl16(a+8,1);pl16(a+10,64);pl32(a+12,V);pl16(a+16,D);pl16(a+18,384);a[24]=7;pl64(a+56,2);
 if(D){a+=64;memcpy(a,"PACPDL01",8);pl16(a+8,1);pl16(a+10,384);pl32(a+12,1);pl16(a+16,1);pl16(a+18,1);pl16(a+20,1);pl64(a+24,2);a[32]=1;memcpy(a+64,v.authority,16);memcpy(a+80,v.generation,16);memcpy(a+96,rec.request.id,16);memcpy(a+112,rec.certificate_digest,32);memcpy(a+144,spki_digest,32);pl64(a+176,1700000100);pl64(a+184,1700003000);pl16(a+192,2);memcpy(a+196,"La",2);memcpy(a+260,v.ca_spki_digest,32);}
 if(pc_root8_v3_pdg_encode(b,pdg_prefix,pdg_part,sizeof(pdg_part),&n,phash,c)!=PC_ROOT8_OK)return 0;memcpy(b+U,pdg_part,n);
 return pc_root8_v3_encode(b,pdg_prefix,pdg_root,sizeof(pdg_root),&pdg_bytes,phash,c,pdg_model_component,NULL)==PC_ROOT8_OK;
}

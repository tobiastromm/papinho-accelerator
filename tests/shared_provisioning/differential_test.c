/* PUBLIC structural fixtures only, no key/cert/crypto/protected package/file. */
#include "structure.h"
#include "papinho_papacc_content.h"
#include "papinho_papacc_frame.h"
#include "papinho_papacc_crypto.h"
#ifdef SP_WITH_ACCELERATOR
#include "manual_package.h"
#endif
#include <stdio.h>
#include <string.h>
#include "structural_sample.h"
int browser_metadata_valid(const unsigned char *,size_t,unsigned);
#ifdef SP_WITH_ACCELERATOR
int accelerator_metadata_valid(const unsigned char *,size_t,unsigned);
#endif
static unsigned long checks;
#define CHECK(x) do{++checks;if(!(x)){printf("FAIL line %u: %s\n",(unsigned)__LINE__,#x);return 0;}}while(0)
static unsigned char work[49655],saved[49655],envelope[131073],cfg[4096],value[254];
static SP_PAYLOAD original;
static void put16(unsigned char *p,unsigned n){p[0]=(unsigned char)(n>>8);p[1]=(unsigned char)n;}
static void put32(unsigned char *p,unsigned long n){p[0]=(unsigned char)(n>>24);p[1]=(unsigned char)(n>>16);p[2]=(unsigned char)(n>>8);p[3]=(unsigned char)n;}
static int same_bytes(const void *p,unsigned char v,size_t n){const unsigned char *q=(const unsigned char *)p;size_t i;for(i=0;i<n;i++)if(q[i]!=v)return 0;return 1;}
static int zip_compare(const unsigned char *p,size_t n)
{
 SP_PAYLOAD a;papinho_papacc_content_view b;SP_RESULT ar;int br;unsigned i;
#ifdef SP_WITH_ACCELERATOR
 MP_PAYLOAD c;MP_RESULT cr;memset(&c,0xcc,sizeof(c));cr=mp_payload_inspect(p,n,&c,0);
#endif
 memset(&a,0xcc,sizeof(a));memset(&b,0xcc,sizeof(b));
 ar=sp_payload_inspect(p,n,&a,NULL,NULL);br=papinho_papacc_content_inspect(p,n,&b,NULL,NULL);
 CHECK((ar==SP_DATA_ONLY)==(br==PAPINHO_PAPACC_CONTENT_DATA_ONLY));
#ifdef SP_WITH_ACCELERATOR
 CHECK((ar==SP_DATA_ONLY)==(cr==MP_DATA_ONLY));
#endif
 if(ar==SP_DATA_ONLY){for(i=0;i<5;i++){
 CHECK(a.member[i].size==b.member[i].size && a.member[i].data-p==b.member[i].data-p);
 CHECK(a.member[i].data>=p && a.member[i].data+a.member[i].size<=p+n);
#ifdef SP_WITH_ACCELERATOR
 CHECK(a.member[i].size==c.member[i].size && a.member[i].data-p==c.member[i].data-p);
#endif
 }}else{CHECK(same_bytes(&a,0xcc,sizeof(a)));CHECK(same_bytes(&b,0xcc,sizeof(b)));}
 return 1;
}
static int env_compare(const unsigned char *p,size_t n)
{
 SP_ENVELOPE a;papinho_papacc_frame b;SP_RESULT ar;int br;
#ifdef SP_WITH_ACCELERATOR
 MP_ENVELOPE c;MP_RESULT cr;memset(&c,0xcc,sizeof(c));cr=mp_envelope_inspect(p,n,&c,0);
#endif
 memset(&a,0xcc,sizeof(a));memset(&b,0xcc,sizeof(b));ar=sp_envelope_inspect(p,n,&a,NULL,NULL);br=papinho_papacc_frame_read(p,n,&b);
 CHECK((ar==SP_DATA_ONLY)==(br==1));
#ifdef SP_WITH_ACCELERATOR
 CHECK((ar==SP_DATA_ONLY)==(cr==MP_DATA_ONLY));
#endif
 if(ar==SP_DATA_ONLY){CHECK(a.spki.data-p==(ptrdiff_t)b.spki_offset && a.spki.size==b.spki_length);CHECK(a.ciphertext.data-p==(ptrdiff_t)b.ciphertext_offset && a.ciphertext.size==b.ciphertext_length);CHECK(a.tag.data-p==(ptrdiff_t)b.tag_offset && a.tag.size==16);CHECK(a.signature.data-p==(ptrdiff_t)b.signature_offset && a.signature.size==256);
#ifdef SP_WITH_ACCELERATOR
 CHECK(a.spki.data==c.spki.data&&a.ciphertext.data==c.ciphertext.data&&a.tag.data==c.tag.data&&a.signature.data==c.signature.data);
#endif
 }else{CHECK(same_bytes(&a,0xcc,sizeof(a)));CHECK(same_bytes(&b,0xcc,sizeof(b)));}
 return 1;
}
static int metadata_compare(const unsigned char *p,size_t n,unsigned kind)
{
 SP_SLICE v;int a,b;v.data=p;v.size=n;a=sp_metadata_validate(v,kind)==SP_DATA_ONLY;b=browser_metadata_valid(p,n,kind);CHECK(a==b);
#ifdef SP_WITH_ACCELERATOR
 CHECK(a==accelerator_metadata_valid(p,n,kind));
#endif
 return 1;
}
static size_t config_value(unsigned tag,const unsigned char *v,size_t n)
{
 size_t off=12;unsigned i;SP_SLICE a;memcpy(cfg,"PACCFG02",8);put16(cfg+8,2);put16(cfg+10,16);
 for(i=1;i<=16;i++){a=sp_payload_field(&original,0,i);if(i==tag){a.data=v;a.size=n;}put16(cfg+off,i);put32(cfg+off+2,(unsigned long)a.size);off+=6;memcpy(cfg+off,a.data,a.size);off+=a.size;}return off;
}
static void envelope_make(unsigned long spki,unsigned long cipher)
{
 size_t n=(size_t)(364UL+spki+cipher);memset(envelope,0,n+1);memcpy(envelope,"PACCMP02",8);put16(envelope+8,2);put16(envelope+10,1);put32(envelope+16,(unsigned long)n);put32(envelope+20,cipher);put32(envelope+24,600000UL);put32(envelope+88,spki);
}
static int parser_tests(void)
{
 size_t n=sizeof(sample_zip),i,k;unsigned t;SP_SLICE a;SP_PAYLOAD local;SP_ENVELOPE e;
 memcpy(work,sample_zip,n);CHECK(sp_payload_inspect(work,n,&original,NULL,NULL)==SP_DATA_ONLY);CHECK(zip_compare(work,n));memcpy(saved,work,n);
 for(i=0;i<n;i++){work[i]^=1;CHECK(zip_compare(work,n));CHECK(sp_payload_inspect(work,n,&local,NULL,NULL)!=SP_DATA_ONLY);work[i]^=1;}
 for(i=1;i<n;i++)CHECK(zip_compare(work,i));work[n]=0;CHECK(zip_compare(work,n+1));
 memcpy(work+30,"../bad",6);CHECK(zip_compare(work,n));CHECK(sp_payload_inspect(work,n,&local,NULL,NULL)==SP_INVALID);memcpy(work,saved,n);
 memcpy(work+75,"cliente.pk8",11);CHECK(zip_compare(work,n));CHECK(sp_payload_inspect(work,n,&local,NULL,NULL)==SP_INVALID);memcpy(work,saved,n);
 CHECK(zip_compare(work,49655));CHECK(sp_payload_inspect(work,49655,&local,NULL,NULL)==SP_LIMIT);
 CHECK(sp_payload_inspect(work,n,(SP_PAYLOAD *)work,NULL,NULL)==SP_INVALID);CHECK(!memcmp(work,saved,n));
 CHECK(sp_payload_inspect(work,n,&original,NULL,NULL)==SP_DATA_ONLY);
 for(t=0;t<2;t++){a=original.member[t?4:3];CHECK(metadata_compare(a.data,a.size,t));for(i=0;i<a.size;i++){memcpy(cfg,a.data,a.size);cfg[i]^=1;CHECK(metadata_compare(cfg,a.size,t));}for(i=0;i<a.size;i++)CHECK(metadata_compare(a.data,i,t));}
 a=sp_payload_field(&original,1,5);CHECK(a.data!=NULL&&a.size==0);a=sp_payload_field(&original,1,6);CHECK(!a.data&&!a.size);a=sp_payload_field(&original,2,1);CHECK(!a.data&&!a.size);
 /* Repairing ZIP CRC cannot hide malformed TLV: direct metadata oracles agree. */
 n=config_value(13,(const unsigned char *)"TEST",4);put16(cfg+12,2);CHECK(metadata_compare(cfg,n,0));a.data=cfg;a.size=n;CHECK(sp_metadata_validate(a,0)==SP_INVALID);
 {static const unsigned char u[][4]={{0xc3,0xa9,'x','x'},{0xc0,0x80,'x','x'},{0xed,0xa0,0x80,'x'},{0xc2,0x80,'x','x'},{0xf0,0x90,0x80,0x80},{0,0,0,0}};
 for(i=0;i<sizeof(u)/sizeof(u[0]);i++){n=config_value(13,u[i],4);CHECK(metadata_compare(cfg,n,0));a.data=cfg;a.size=n;CHECK((sp_metadata_validate(a,0)==SP_DATA_ONLY)==(i==0U||i==4U));}}
 memset(value,'a',sizeof(value));value[63]='.';value[127]='.';value[191]='.';n=config_value(1,value,253);a.data=cfg;a.size=n;CHECK(metadata_compare(cfg,n,0));CHECK(sp_metadata_validate(a,0)==SP_DATA_ONLY);
 value[252]='.';n=config_value(1,value,253);CHECK(metadata_compare(cfg,n,0));a.size=n;CHECK(sp_metadata_validate(a,0)==SP_INVALID);
 n=config_value(1,(const unsigned char *)"123.123.123.1",13);CHECK(metadata_compare(cfg,n,0));n=config_value(1,(const unsigned char *)"*.example.com",13);CHECK(metadata_compare(cfg,n,0));
 for(i=0;i<4;i++){unsigned long sp=i&1?1024UL:1UL,ci=i&2?65536UL:1UL;envelope_make(sp,ci);CHECK(env_compare(envelope,(size_t)(364UL+sp+ci)));}
 envelope_make(1,1);for(k=0;k<366;k++){envelope[k]^=1;CHECK(env_compare(envelope,366));envelope[k]^=1;}
 for(i=1;i<366;i++)CHECK(env_compare(envelope,i));CHECK(env_compare(envelope,367));
 CHECK(sp_envelope_inspect(envelope,366,(SP_ENVELOPE *)envelope,NULL,NULL)==SP_INVALID);
 memset(&e,0xcc,sizeof(e));CHECK(sp_envelope_inspect(NULL,366,&e,NULL,NULL)==SP_INVALID&&same_bytes(&e,0xcc,sizeof(e)));
 return 1;
}
typedef struct stop_state {unsigned calls,at;} stop_state;
static int stopped(void *v){stop_state *s=(stop_state *)v;return ++s->calls>=s->at;}
static int lifecycle_tests(void)
{
 stop_state stop;SP_PAYLOAD a;SP_ENVELOPE e;unsigned i;SP_RESULT z;
 for(i=1;i<=12;i++){stop.calls=0;stop.at=i;memset(&a,0xcc,sizeof(a));z=sp_payload_inspect(sample_zip,sizeof(sample_zip),&a,stopped,&stop);CHECK(z==SP_STOPPED&&same_bytes(&a,0xcc,sizeof(a)));}
 stop.calls=0;stop.at=2;memset(&e,0xcc,sizeof(e));CHECK(sp_envelope_inspect(envelope,366,&e,stopped,&stop)==SP_STOPPED&&same_bytes(&e,0xcc,sizeof(e)));
 CHECK(!memcmp(work,saved,sizeof(sample_zip)));CHECK(sp_payload_inspect(saved,sizeof(sample_zip),&a,NULL,NULL)==SP_DATA_ONLY);CHECK(a.member[0].data>=saved&&a.member[0].data<saved+sizeof(sample_zip));
 return 1;
}
static int normalization_tests(void)
{
 unsigned char input[129],a[32],b[32];size_t i,n;int ar,br;
 memset(input,'A',32);input[32]=0;memset(a,0xcc,32);memset(b,0xcc,32);CHECK(sp_password_normalize(input,32,a)==SP_DATA_ONLY);CHECK(papinho_papacc_crypto_password(input,32,b)==PAPINHO_PAPACC_CRYPTO_ONLY);CHECK(!memcmp(a,b,32));
 for(i=0;i<256;i++){input[0]=(unsigned char)i;memset(a,0xcc,32);memset(b,0xcc,32);ar=sp_password_normalize(input,32,a);br=papinho_papacc_crypto_password(input,32,b);CHECK((ar==SP_DATA_ONLY)==(br==PAPINHO_PAPACC_CRYPTO_ONLY));CHECK(!memcmp(a,b,32));}
 memset(input,'a',32);CHECK(sp_password_normalize(input,32,a)==SP_DATA_ONLY&&same_bytes(a,'A',32));
 for(n=0;n<=129;n++){memset(input,'A',sizeof(input));memset(a,0xcc,32);memset(b,0xcc,32);ar=sp_password_normalize(input,n,a);br=papinho_papacc_crypto_password(input,n,b);CHECK((ar==SP_DATA_ONLY)==(br==PAPINHO_PAPACC_CRYPTO_ONLY));CHECK(!memcmp(a,b,32));}
 memset(input,'A',32);memcpy(saved,input,32);CHECK(sp_password_normalize(input,32,input)==SP_INVALID&&!memcmp(input,saved,32));
 memset(a,0xcc,32);CHECK(sp_password_normalize(NULL,0,a)==SP_INVALID&&same_bytes(a,0,32));return 1;
}
int main(void)
{
 if(!parser_tests()||!lifecycle_tests()||!normalization_tests())return 1;
 printf("PASS %lu structural differential checks; DATA_ONLY; no crypto/trust/keys/files/network\n",checks);
 printf("SIZES slice=%lu payload=%lu envelope=%lu normalized_temp=32; heap=0; retained_internal=0\n",(unsigned long)sizeof(SP_SLICE),(unsigned long)sizeof(SP_PAYLOAD),(unsigned long)sizeof(SP_ENVELOPE));
#ifdef SP_WITH_ACCELERATOR
 printf("ORACLES=ACCELERATOR_BROWSER_SHARED\n");
#else
 printf("ORACLES=VC6_BROWSER_SHARED; original Accelerator stdint source tested separately on modern compiler\n");
#endif
 return 0;
}

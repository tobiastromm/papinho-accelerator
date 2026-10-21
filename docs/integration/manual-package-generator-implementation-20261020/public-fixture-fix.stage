/* PUBLIC SYNTHETIC DATA ONLY: zero fake materials, no real key/cert/signature.
 * These tests cannot authenticate a package or qualify a crypto backend. */
#include "manual_package.h"
#include <stdio.h>
#include <string.h>
static unsigned checks;
#define CHECK(x) do{checks++;if(!(x)){printf("FAIL %u %s\n",(unsigned)__LINE__,#x);return 1;}}while(0)
static unsigned char cfg[4096],iss[4096],zip[MP_ARCHIVE_MAX],old[MP_ARCHIVE_MAX],out[MP_INPUT_MAX];
static unsigned char header[92],spki[1]={0},tagbytes[16],sig[256],fake[9]={'1','2','3','4','5','6','7','8','9'};
static void b16(unsigned char *p,unsigned n){p[0]=(unsigned char)(n>>8);p[1]=(unsigned char)n;}
static void b32(unsigned char *p,unsigned long n){p[0]=(unsigned char)(n>>24);p[1]=(unsigned char)(n>>16);p[2]=(unsigned char)(n>>8);p[3]=(unsigned char)n;}
static size_t field(unsigned char *p,size_t off,unsigned t,const unsigned char *v,size_t n)
{b16(p+off,t);b32(p+off+2,(unsigned long)n);if(n)memcpy(p+off+6,v,n);return off+6+n;}
static void setup(MP_PAYLOAD *v)
{
 unsigned t;size_t n,o=12;unsigned char x[256];memset(cfg,0,sizeof(cfg));memcpy(cfg,"PACCFG02",8);b16(cfg+8,2);b16(cfg+10,16);
 for(t=1;t<=16;t++){
 memset(x,0,sizeof(x));n=32;x[0]=1;
 if(t==1){n=13;memcpy(x,"accel.example",n);}else if(t==2){n=2;b16(x,7443);}
 else if(t==6||t==8||t>=14){n=8;memset(x,0,n);x[7]=(unsigned char)(t==16?2:1);}
 else if(t==9){n=8;memcpy(x,"papacc/1",n);}else if(t==10){n=2;b16(x,1);}else if(t==13){n=4;memcpy(x,"TEST",n);}
 o=field(cfg,o,t,x,n);
 }v->member[3].data=cfg;v->member[3].size=o;
 memset(iss,0,sizeof(iss));memcpy(iss,"PACISS02",8);b16(iss+8,2);b16(iss+10,5);o=12;
 for(t=1;t<=5;t++){memset(x,1,sizeof(x));n=t<=2?32:t==3?16:t==4?4:0;if(t==4)memcpy(x,"TEST",4);o=field(iss,o,t,x,n);}
 v->member[4].data=iss;v->member[4].size=o;
 for(t=0;t<3;t++){v->member[t].data=fake;v->member[t].size=sizeof(fake);}
}
int main(void)
{
 MP_PAYLOAD p,q;MP_ENVELOPE e,f;size_t n,m,i,z;unsigned char sentinel;
 setup(&p);n=99;CHECK(mp_payload_prepare(&p,zip,sizeof(zip),&n,0)==MP_DATA_ONLY);CHECK(n==502U+27U+p.member[3].size+p.member[4].size);
 CHECK(zip[14]==0x26&&zip[15]==0x39&&zip[16]==0xf4&&zip[17]==0xcb); /* standard ZIP CRC32 check vector */
 CHECK(mp_payload_inspect(zip,n,&q,0)==MP_DATA_ONLY);CHECK(q.member[2].size==9&&!memcmp(q.member[2].data,fake,9));
 memcpy(old,zip,n);
 /* Named attack regressions: fixed-name/order grammar, no extraction. */
 memcpy(zip+30,"../bad",6);CHECK(mp_payload_inspect(zip,n,&q,0)==MP_INVALID);memcpy(zip,old,n);
 memcpy(zip+30U+6U+9U+30U,"cliente.pk8",11);CHECK(mp_payload_inspect(zip,n,&q,0)==MP_INVALID);memcpy(zip,old,n);
 memcpy(zip+30,"xx.der",6);CHECK(mp_payload_inspect(zip,n,&q,0)==MP_INVALID);memcpy(zip,old,n);
 for(i=0;i<n;i++){zip[i]^=1;CHECK(mp_payload_inspect(zip,n,&q,0)!=MP_DATA_ONLY);zip[i]^=1;}
 for(i=1;i<n;i++)CHECK(mp_payload_inspect(zip,i,&q,0)!=MP_DATA_ONLY);
 zip[n]=0;CHECK(mp_payload_inspect(zip,n+1,&q,0)!=MP_DATA_ONLY);
 CHECK(mp_payload_inspect(zip,n,(MP_PAYLOAD *)zip,0)==MP_INVALID);
 m=99;sentinel=zip[0];CHECK(mp_payload_prepare(&p,zip,sizeof(zip),&m,1)==MP_STOPPED&&!m&&zip[0]==sentinel);
 CHECK(mp_payload_inspect(zip,n,&q,1)==MP_STOPPED&&!q.member[0].data);
 m=99;CHECK(mp_payload_prepare(&p,zip,n-1,&m,0)==MP_LIMIT&&!m);
 cfg[18]='*';m=99;CHECK(mp_payload_prepare(&p,zip,sizeof(zip),&m,0)==MP_INVALID&&!m);setup(&p);
 cfg[10]=1;CHECK(mp_payload_prepare(&p,zip,sizeof(zip),&m,0)==MP_INVALID);setup(&p);
 /* caller-owned old snapshot remains available; no hidden pin or retained job */
 CHECK(mp_payload_prepare(&p,zip,sizeof(zip),&m,0)==MP_DATA_ONLY&&m==n&&!memcmp(old,zip,n));
 CHECK(mp_payload_inspect(old,n,&q,0)==MP_DATA_ONLY&&q.member[0].data>=old&&q.member[0].data<old+n);
 memset(header,0,sizeof(header));memcpy(header,"PACCMP02",8);b16(header+8,2);b16(header+10,1);z=364U+1U+n;b32(header+16,(unsigned long)z);b32(header+20,(unsigned long)n);b32(header+24,600000UL);b32(header+88,1);
 e.spki.data=spki;e.spki.size=1;e.ciphertext.data=zip;e.ciphertext.size=n;e.tag.data=tagbytes;e.tag.size=16;e.signature.data=sig;e.signature.size=256;
 CHECK(mp_envelope_prepare(header,&e,out,sizeof(out),&m,0)==MP_DATA_ONLY&&m==z);
 CHECK(mp_envelope_inspect(out,m,&f,0)==MP_DATA_ONLY&&f.ciphertext.size==n);
 CHECK(mp_package_validate(out,m,0)==MP_CRYPTO_UNAVAILABLE); /* NEVER fake signature/password/X509 success */
 out[92U+1U]^=1;CHECK(mp_package_validate(out,m,0)==MP_CRYPTO_UNAVAILABLE);out[92U+1U]^=1;
 out[m-1]^=1;CHECK(mp_package_validate(out,m,0)==MP_CRYPTO_UNAVAILABLE);out[m-1]^=1;
 CHECK(mp_package_validate(out,m,1)==MP_STOPPED);
 out[9]=3;CHECK(mp_package_validate(out,m,0)==MP_INVALID);out[9]=2;
 out[25]^=1;CHECK(mp_package_validate(out,m,0)==MP_INVALID);out[25]^=1;
 CHECK(mp_envelope_inspect(out,m-1,&f,0)==MP_INVALID);out[m]=0;CHECK(mp_envelope_inspect(out,m+1,&f,0)==MP_INVALID);
 CHECK(mp_envelope_inspect(out,m,(MP_ENVELOPE *)out,0)==MP_INVALID);
 i=99;CHECK(mp_envelope_prepare(header,&e,out,sizeof(out),&i,1)==MP_STOPPED&&!i);
 CHECK(mp_envelope_prepare(header,&e,out,m-1,&i,0)==MP_LIMIT&&!i);
 printf("PASS %u STRUCTURAL_DATA_ONLY CRYPTO_UNAVAILABLE NO_AUTHORITY NO_KEYS NO_FILES\n",checks);return 0;
}

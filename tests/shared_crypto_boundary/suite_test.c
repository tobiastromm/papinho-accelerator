/* Public structural arrays only. No key, DER, crypto, protected fixture or trust. */
#include "suite.h"
#include "papinho_papacc_frame.h"
#include <stdio.h>
#include <string.h>
static unsigned long checks;
#define C(x) do { ++checks;if(!(x)){printf("FAIL %u\n",(unsigned)__LINE__);return 1;} } while(0)
static unsigned char raw[1400],pw[32],salt[16];
static void be32(unsigned char *p,unsigned long n){p[0]=(unsigned char)(n>>24);p[1]=(unsigned char)(n>>16);p[2]=(unsigned char)(n>>8);p[3]=(unsigned char)n;}
static int stop(void *p){return *(int *)p;}
int main(void)
{
 SP_SLICE p,s;SP_SIGNED_PARTS v,before;papinho_papacc_frame f;
 size_t i,n=665;int halted=0;
 memset(pw,'A',sizeof(pw));p.data=pw;p.size=32;s.data=salt;s.size=16;
 C(sp_suite1_kdf_shape(p,s,600000UL)==SP_DATA_ONLY);
 for(i=0;i<256;++i){pw[0]=(unsigned char)i;C((sp_suite1_kdf_shape(p,s,600000UL)==SP_DATA_ONLY)==((i>='A' && i<='Z') || (i>='2' && i<='7')));}
 pw[0]='A';C(sp_suite1_kdf_shape(p,s,599999UL)==SP_INVALID);
 C(sp_suite1_kdf_shape(p,s,600001UL)==SP_INVALID);p.size=31;
 C(sp_suite1_kdf_shape(p,s,600000UL)==SP_INVALID);p.size=32;s.size=15;
 C(sp_suite1_kdf_shape(p,s,600000UL)==SP_INVALID);
 C(sp_suite1_aead_shape(32,12,0,1,16)==SP_DATA_ONLY);
 C(sp_suite1_aead_shape(32,12,1116,65536UL,16)==SP_DATA_ONLY);
 C(sp_suite1_aead_shape(31,12,0,1,16)==SP_INVALID);
 C(sp_suite1_aead_shape(32,11,0,1,16)==SP_INVALID);
 C(sp_suite1_aead_shape(32,12,1117,1,16)==SP_INVALID);
 C(sp_suite1_aead_shape(32,12,0,0,16)==SP_INVALID);
 C(sp_suite1_aead_shape(32,12,0,65537UL,16)==SP_INVALID);
 C(sp_suite1_aead_shape(32,12,0,1,15)==SP_INVALID);
 memset(raw,0,sizeof(raw));memcpy(raw,"PACCMP02",8);raw[9]=2;raw[11]=1;
 be32(raw+16,(unsigned long)n);be32(raw+20,1);be32(raw+24,600000UL);be32(raw+88,300);
 C(papinho_papacc_frame_read(raw,n,&f)==1);
 C(sp_suite1_signed_parts(raw,n,&v,stop,&halted)==SP_DATA_ONLY);
 C(v.prefix.data==raw && v.prefix.size==f.signature_offset);
 C(v.spki.data==raw+f.spki_offset && v.spki.size==f.spki_length);
 C(v.signature.data==raw+f.signature_offset && v.signature.size==256);
 C(v.domain.size==sizeof("PAPACC-COMPLETE-MANUAL-V2"));
 C(!memcmp(v.domain.data,"PAPACC-COMPLETE-MANUAL-V2",v.domain.size));
 before=v;halted=1;C(sp_suite1_signed_parts(raw,n,&v,stop,&halted)==SP_STOPPED);
 C(!memcmp(&v,&before,sizeof(v)));halted=0;
 for(i=0;i<n;++i){C(sp_suite1_signed_parts(raw,i,&v,0,0)!=SP_DATA_ONLY);C(!memcmp(&v,&before,sizeof(v)));}
 raw[n]=0;C(sp_suite1_signed_parts(raw,n+1,&v,0,0)==SP_INVALID);
 raw[9]=3;C(sp_suite1_signed_parts(raw,n,&v,0,0)==SP_INVALID);raw[9]=2;
 C(sp_suite1_signed_parts(raw,n,(SP_SIGNED_PARTS *)raw,0,0)==SP_INVALID);
 printf("%lu public suite policy checks; DATA ONLY; plan=%u\n",checks,(unsigned)sizeof(v));return 0;
}

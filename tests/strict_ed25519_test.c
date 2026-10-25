#include "strict_ed25519.h"
#include <stdio.h>
#include <string.h>
static unsigned checks,failures;
#define CK(x) do { ++checks; if (!(x)) { ++failures; printf("FAIL line=%u\n",(unsigned)__LINE__); } } while(checks == 0)
static int unhex(unsigned char *p,const char *s) { unsigned a,b; int n=0; while(s[0]&&s[1]) { a=(unsigned)(s[0]>='a'?s[0]-'a'+10:s[0]-'0'); b=(unsigned)(s[1]>='a'?s[1]-'a'+10:s[1]-'0'); if(a>15||b>15)return -1; p[n++]=(unsigned char)(a*16+b);s+=2;}return *s?-1:n; }
static void key_tests(void) {
 unsigned char k[64], saved[64]; size_t n; unsigned sign,j;
 CK(unhex(k,"d75a980182b10ab7d54bfed3c964073a0ee172f3daa62325af021a68f707511a")==32);memset(k+32,0x55,32);memcpy(saved,k,64);
 CK(papacc_time_source_ed25519_check(k,32)==PAPACC_ED25519_VALID);
 for(n=0;n<32;++n)CK(papacc_time_source_ed25519_check(k,n)==PAPACC_ED25519_INVALID);
 for(n=33;n<=64;++n)CK(papacc_time_source_ed25519_check(k,n)==PAPACC_ED25519_INVALID);
 CK(papacc_time_source_ed25519_check(k,(size_t)-1)==PAPACC_ED25519_INVALID);
 for(n=0;n<=64;++n)CK(papacc_time_source_ed25519_check(NULL,n)==PAPACC_ED25519_INVALID);
 CK(!memcmp(k,saved,64));
 for(j=0;j<100;++j)CK(papacc_time_source_ed25519_check(k,32)==PAPACC_ED25519_VALID);
 CK(!memcmp(k,saved,64));
 for(sign=0;sign<2;++sign) {
  memset(k,0,32);k[0]=1;k[31]=(unsigned char)(sign*128);CK(papacc_time_source_ed25519_check(k,32)==PAPACC_ED25519_INVALID);
  memset(k,0xff,32);k[0]=0xec;k[31]=(unsigned char)(0x7f+sign*128);CK(papacc_time_source_ed25519_check(k,32)==PAPACC_ED25519_INVALID);
  memset(k,0,32);k[31]=(unsigned char)(sign*128);CK(papacc_time_source_ed25519_check(k,32)==PAPACC_ED25519_INVALID);
  memset(k,0,32);k[0]=2;k[31]=(unsigned char)(sign*128);CK(papacc_time_source_ed25519_check(k,32)==PAPACC_ED25519_INVALID);
  memset(k,0x99,32);k[0]=0x95;k[31]=(unsigned char)(0x19+sign*128);CK(papacc_time_source_ed25519_check(k,32)==PAPACC_ED25519_INVALID);
  for(j=0;j<19;++j) {memset(k,0xff,32);k[0]=(unsigned char)(0xed+j);k[31]=(unsigned char)(0x7f+sign*128);CK(papacc_time_source_ed25519_check(k,32)==PAPACC_ED25519_INVALID);}
  memset(k,0x66,32);k[0]=0x58;k[31]=(unsigned char)(0x66+sign*128);CK(papacc_time_source_ed25519_check(k,32)==PAPACC_ED25519_VALID);
 }
}

static void order8_tests(void) {
 unsigned char p[32];
 /* Independent public bytes: libsodium1.0.21 ref10 small-order blacklist.
  * Only test DATA; no libsodium code or dependency incorporated. */
 CK(unhex(p,"26e8958fc2b227b045c3f489f2ef98f0d5dfac05d3c63339b13802886d53fc05")==32);
 CK(papacc_time_source_ed25519_check(p,32)==PAPACC_ED25519_INVALID);
 p[31]^=128;CK(papacc_time_source_ed25519_check(p,32)==PAPACC_ED25519_INVALID);
 CK(unhex(p,"c7176a703d4dd84fba3c0b760d10670f2a2053fa2c39ccc64ec7fd7792ac037a")==32);
 CK(papacc_time_source_ed25519_check(p,32)==PAPACC_ED25519_INVALID);
 p[31]^=128;CK(papacc_time_source_ed25519_check(p,32)==PAPACC_ED25519_INVALID);
 CK(unhex(p,"5252cc0a7f208133b620acbd4537eba2a4123bf0a8c2e4f980c3b31bb69765ea")==32);
 CK(papacc_time_source_ed25519_check(p,32)==PAPACC_ED25519_INVALID);
 p[31]^=128;CK(papacc_time_source_ed25519_check(p,32)==PAPACC_ED25519_INVALID);
 CK(unhex(p,"da99e28ba529cdde35a25fba9059e78ecaee239f99755b9b1aa4f65df00803e2")==32);
 CK(papacc_time_source_ed25519_check(p,32)==PAPACC_ED25519_INVALID);
 p[31]^=128;CK(papacc_time_source_ed25519_check(p,32)==PAPACC_ED25519_INVALID);
}
int main(void){key_tests();order8_tests();printf("checks=%u failures=%u DATA_ONLY\n",checks,failures);return failures?1:0;}

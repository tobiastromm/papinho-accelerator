/* Offline public-point vector derivation only. No keygen/signing/RNG. */
#include <stdio.h>
#include "strict_ed25519.c"
static void emit(const char *label, const unsigned char *k){unsigned i;printf("%s=",label);for(i=0;i<32;i++)printf("%02x",k[i]);puts("");}
static int build(const unsigned char *t, unsigned order, const char *label){
 unsigned char base[32],out[32],small[32]={0},zero[32]={0},id[32]={1};
 ge_p3 b,p,sum;ge_cached c;ge_p1p1 v;ge_p2 m;unsigned i;
 for(i=0;i<32;i++)base[i]=0x66;base[0]=0x58;
 if(ge_frombytes_vartime(&b,base)||ge_frombytes_vartime(&p,t))return 1;
 small[0]=(unsigned char)order;ge_double_scalarmult_vartime(&m,small,&p,zero);ge_tobytes(out,&m);if(memcmp(out,id,32))return 2;
 small[0]=(unsigned char)(order/2);ge_double_scalarmult_vartime(&m,small,&p,zero);ge_tobytes(out,&m);if(!memcmp(out,id,32))return 3;
 ge_p3_to_cached(&c,&p);ge_add(&v,&b,&c);ge_p1p1_to_p3(&sum,&v);ge_p3_tobytes(out,&sum);emit(label,out);
 if(papacc_time_source_ed25519_check(out,32)!=0)return 4;
 return 0;
}
int main(void){
 static const unsigned char t4[32]={0};
 static const unsigned char t8[32]={0x26,0xe8,0x95,0x8f,0xc2,0xb2,0x27,0xb0,0x45,0xc3,0xf4,0x89,0xf2,0xef,0x98,0xf0,0xd5,0xdf,0xac,0x05,0xd3,0xc6,0x33,0x39,0xb1,0x38,0x02,0x88,0x6d,0x53,0xfc,0x05};
 return build(t4,4,"mixed4L")||build(t8,8,"mixed8L");
}

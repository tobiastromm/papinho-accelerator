/* Opt-in offline adapter, same worker and root. No initialization/publication. */
#include "../../src/config/config_root8.h"
#include "server_config_root8_data_win32.h"
static PAPACC_U64 d3_u64(const PAPACC_U8 *p)
{PAPACC_U64 x=0;unsigned i;for(i=0;i<8U;i++)x|=(PAPACC_U64)p[i]<<(8U*i);return x;}
static unsigned d3_u32(const PAPACC_U8 *p)
{return (unsigned)p[0]|((unsigned)p[1]<<8)|((unsigned)p[2]<<16)|((unsigned)p[3]<<24);}
static int pending_d3_packet_valid(const PAPACC_CONFIG_PENDING *p)
{
 if(p->previous_bytes<PC_ROOT8_MIN||p->previous_bytes>PC_ROOT8_NO_ISSUER_MAX||
    p->next_bytes<PC_ROOT8_MIN||p->next_bytes>PC_ROOT8_NO_ISSUER_MAX)return 0;
 return !memcmp(p->previous,"PACROOT8",8)&&!memcmp(p->next,"PACROOT8",8)&&
  p->previous[8]==1&&!p->previous[9]&&p->next[8]==1&&!p->next[9]&&
  !(d3_u32(p->previous+16)&2U)&&!(d3_u32(p->next+16)&2U)&&
  !d3_u32(p->previous+52)&&!d3_u32(p->next+52)&&
  d3_u32(p->previous+12)==p->previous_bytes&&d3_u32(p->next+12)==p->next_bytes;
}

#if defined(PAPACC_ED11_WINDOWS_3MIB)
/* Frozen packet persistence guard, NOT component/authority validation. Only a
 * future reviewed canonical producer may authorize production entry. Existing
 * begin08 stays v1-only; no new public begin or CURRENT producer is installed.
 * The test entry below is compile-isolated, synthetic and cannot grant trust. */
static int pending_v3_image_guard(const unsigned char *b,DWORD n)
{
 unsigned N,B,K,D,q,off,s,u,v;unsigned char hash[32];
 if(n<PC_ROOT8_V3_MIN||n>PC_ROOT8_V3_MAX||memcmp(b,"PACROOT8",8)||
    b[8]!=3U||b[9]||b[10]!=0U||b[11]!=2U||d3_u32(b+12)!=n||d3_u32(b+16)!=3U)return 0;
 N=(unsigned)b[20]|((unsigned)b[21]<<8);K=(unsigned)b[252]|((unsigned)b[253]<<8);
 D=(unsigned)b[268]|((unsigned)b[269]<<8);
 if(!N||N>4U||!K||K>N||D>K||d3_u32(b+44)<672U)return 0;
 B=(d3_u32(b+44)-160U)/512U;
 if(!B||B>4U||d3_u32(b+44)!=160U+512U*B)return 0;
 off=512U+1536U*N;s=96U+768U*K;u=off+s;v=96U+384U*D;q=u+v+160U+512U*B+16384U;
 if(q+544U!=n||d3_u32(b+32)!=512U||d3_u32(b+36)!=1536U*N||
 d3_u32(b+244)!=off||d3_u32(b+248)!=s||d3_u32(b+260)!=u||d3_u32(b+264)!=v||
 d3_u32(b+40)!=u+v||d3_u32(b+48)!=u+v+160U+512U*B||d3_u32(b+52)!=16384U||
 d3_u32(b+56)!=q||d3_u32(b+60)!=512U||d3_u32(b+64)!=q+512U||d3_u32(b+68)!=32U)return 0;
 return store03_hash(b,n-32U,hash)&&!memcmp(hash,b+n-32U,32);
}
static int pending_v3_packet_valid(const PAPACC_CONFIG_PENDING *p)
{
 const unsigned char *a=p->previous,*b=p->next,*cp;unsigned aq,bq,as,bs,ap,bp,K,D;unsigned char hash[32];
 if(!pending_v3_image_guard(a,p->previous_bytes)||!pending_v3_image_guard(b,p->next_bytes))return 0;
 aq=d3_u32(a+56);bq=d3_u32(b+56);cp=a+aq;
 if(memcmp(cp,"PACR8CP3",8)||cp[8]!=3U||cp[9]||cp[10]!=0U||cp[11]!=2U||
 !store03_hash(cp,480U,hash)||memcmp(hash,cp+480U,32)||
 !store03_hash(a,aq,hash)||memcmp(hash,cp+88U,32)||
 memcmp(cp+16,a+72,32)||d3_u64(cp+48)!=d3_u64(a+104)||d3_u64(cp+56)!=d3_u64(a+112)||
 !d3_u64(cp+64)||!d3_u64(cp+72)||d3_u64(cp+80)!=d3_u64(a+120)||
 !store03_zero(b+bq,512U)||memcmp(a+72,b+72,32)||
 d3_u64(a+104)==(PAPACC_U64)-1||d3_u64(b+104)!=d3_u64(a+104)+1U||
 d3_u64(b+112)!=d3_u64(a+104)||d3_u64(b+136)!=d3_u64(a+104)||
 d3_u64(b+144)!=d3_u64(cp+64)||d3_u64(b+152)!=d3_u64(cp+72)||
 d3_u64(b+160)!=d3_u64(cp+80)||b[240]!=3U||b[241]||
 memcmp(b+176,cp+88,32)||memcmp(b+208,a+p->previous_bytes-32U,32))return 0;
 /* Bounded first native slice: preserve the ENTIRE carrier and unchanged
  * SNT/PDG rows. It cannot authorize a change of approval/history or issuer. */
 as=d3_u32(a+244);bs=d3_u32(b+244);ap=d3_u32(a+260);bp=d3_u32(b+260);
 K=(unsigned)a[252]|((unsigned)a[253]<<8);D=(unsigned)a[268]|((unsigned)a[269]<<8);
 if(K!=((unsigned)b[252]|((unsigned)b[253]<<8))||D!=((unsigned)b[268]|((unsigned)b[269]<<8))||
 memcmp(a+as+64U,b+bs+64U,768U*K)||memcmp(a+ap+64U,b+bp+64U,384U*D)||
 d3_u64(a+120)!=d3_u64(b+120)||memcmp(a+d3_u32(a+48),b+d3_u32(b+48),16384U))return 0;
 return 1;
}
#endif

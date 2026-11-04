/* Opt-in offline adapter, same worker and root. No initialization/publication. */
#include "../../src/config/config_root8.h"
#include "server_config_root8_data_win32.h"
#include "../../src/config/config_root8_time_source_key.h"
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
 * separately qualified canonical producer may authorize production entry. Existing
 * begin08 stays v1-only; no new public begin or CURRENT producer is installed.
 * The test entry below is compile-isolated, synthetic and cannot grant trust. */
static int pending_packet11_image_guard(PAPACC_STORE_HASH *hash_ctx,const unsigned char *b,DWORD n)
{
 unsigned N,B,K,D,H,F,version,q,off,s,u,v;unsigned char hash[32];
 if(n<PC_ROOT8_V3_MIN||n>PC_ROOT8_V4_MAX)return 0;
 version=b[8];
 if((version!=3U&&version!=4U)||n<(version==3U?PC_ROOT8_V3_MIN:PC_ROOT8_V4_MIN)||
 n>(version==3U?PC_ROOT8_V3_MAX:PC_ROOT8_V4_MAX)||memcmp(b,"PACROOT8",8)||b[9]||b[10]!=0U||b[11]!=2U||d3_u32(b+12)!=n||d3_u32(b+16)!=3U)return 0;
 N=(unsigned)b[20]|((unsigned)b[21]<<8);K=(unsigned)b[252]|((unsigned)b[253]<<8);
 D=(unsigned)b[268]|((unsigned)b[269]<<8);
 if(!N||N>4U||!K||K>N||D>K||d3_u32(b+44)<672U)return 0;
 B=(d3_u32(b+44)-160U)/512U;
 if(!B||B>4U||d3_u32(b+44)!=160U+512U*B)return 0;
 H=version==4U?((unsigned)b[284]|((unsigned)b[285]<<8)):0U;
 if(H>3U)return 0;F=version==4U?96U+384U*H:0U;
 off=512U+1536U*N;s=96U+768U*K;u=off+s;v=96U+384U*D;q=u+v+F+160U+512U*B+16384U;
 if(q+544U!=n||d3_u32(b+32)!=512U||d3_u32(b+36)!=1536U*N||
 d3_u32(b+244)!=off||d3_u32(b+248)!=s||d3_u32(b+260)!=u||d3_u32(b+264)!=v||
 d3_u32(b+40)!=u+v+F||d3_u32(b+48)!=u+v+F+160U+512U*B||d3_u32(b+52)!=16384U||
 d3_u32(b+56)!=q||d3_u32(b+60)!=512U||d3_u32(b+64)!=q+512U||d3_u32(b+68)!=32U)return 0;
 if(version==4U&&(b[274]!=1U||b[275]||d3_u32(b+276)!=u+v||d3_u32(b+280)!=F||
 b[286]!=128U||b[287]!=1U||b[288]!=1U||b[289]))return 0;
 return store03_hash(hash_ctx,b,n-32U,hash)&&!memcmp(hash,b+n-32U,32);
}
static int pending_packet11_valid(PAPACC_STORE_HASH *hash_ctx,const PAPACC_CONFIG_PENDING *p)
{
 const unsigned char *a=p->previous,*b=p->next,*cp;unsigned aq,bq,as,bs,ap,bp,K,D;unsigned char hash[32];
 if(!pending_packet11_image_guard(hash_ctx,a,p->previous_bytes)||!pending_packet11_image_guard(hash_ctx,b,p->next_bytes))return 0;
 aq=d3_u32(a+56);bq=d3_u32(b+56);cp=a+aq;
 if(memcmp(cp,a[8]==3U?"PACR8CP3":"PACR8CP4",8)||cp[8]!=a[8]||cp[9]||cp[10]!=0U||cp[11]!=2U||
 !store03_hash(hash_ctx,cp,480U,hash)||memcmp(hash,cp+480U,32)||
 !store03_hash(hash_ctx,a,aq,hash)||memcmp(hash,cp+88U,32)||
 memcmp(cp+16,a+72,32)||d3_u64(cp+48)!=d3_u64(a+104)||d3_u64(cp+56)!=d3_u64(a+112)||
 !d3_u64(cp+64)||!d3_u64(cp+72)||d3_u64(cp+80)!=d3_u64(a+120)||
 !store03_zero(b+bq,512U)||memcmp(a+72,b+72,32)||
 d3_u64(a+104)==(PAPACC_U64)-1||d3_u64(b+104)!=d3_u64(a+104)+1U||
 d3_u64(b+112)!=d3_u64(a+104)||d3_u64(b+136)!=d3_u64(a+104)||
 d3_u64(b+144)!=d3_u64(cp+64)||d3_u64(b+152)!=d3_u64(cp+72)||
 d3_u64(b+160)!=d3_u64(cp+80)||b[240]!=a[8]||b[241]||a[8]>b[8]||
 memcmp(b+176,cp+88,32)||memcmp(b+208,a+p->previous_bytes-32U,32))return 0;
 /* Bounded first native slice: preserve the ENTIRE carrier and unchanged
  * SNT/PDG rows. It cannot authorize a change of approval/history or issuer. */
 as=d3_u32(a+244);bs=d3_u32(b+244);ap=d3_u32(a+260);bp=d3_u32(b+260);
 K=(unsigned)a[252]|((unsigned)a[253]<<8);D=(unsigned)a[268]|((unsigned)a[269]<<8);
 if(K!=((unsigned)b[252]|((unsigned)b[253]<<8))||D!=((unsigned)b[268]|((unsigned)b[269]<<8))||
 memcmp(a+as+64U,b+bs+64U,768U*K)||memcmp(a+ap+64U,b+bp+64U,384U*D)||
 d3_u64(a+120)!=d3_u64(b+120)||memcmp(a+d3_u32(a+48),b+d3_u32(b+48),16384U))return 0;
 /* This frozen slice preserves existing temporal rows too. No new source
  * disposition is inferred from encoded bytes or a valid Ed25519 key. */
 K=a[8]==4U?((unsigned)a[284]|((unsigned)a[285]<<8)):0U;
 D=b[8]==4U?((unsigned)b[284]|((unsigned)b[285]<<8)):0U;
 if(K!=D||(K&&memcmp(a+d3_u32(a+276)+64U,b+d3_u32(b+276)+64U,384U*K)))return 0;
 return 1;
}
/* Packet14 is frozen documentary DATA only. No public positive producer.
 * Full component/trust qualification precedes freezing, as for packet11. */
static int q4_worker_hash(void *ctx,const PAPACC_U8 *p,PAPACC_SIZE n,PAPACC_U8 out[32])
{return n<=PC_ROOT8_V5_MAX&&store03_hash((PAPACC_STORE_HASH *)ctx,p,(unsigned)n,out);}
static PC_ROOT8_RESULT q4_frozen_component(void *ctx,unsigned kind,const PAPACC_U8 *p,PAPACC_SIZE n,const PAPACC_U8 *row,const PAPACC_U8 *header)
{
 if(kind==5U)return papacc_root8_body05_data(ctx,kind,p,n,row,header);
 if(kind==9U)return header[8]==5U?pc_root8_v5_key_data(ctx,kind,p,n,row,header):pc_root8_time_source_key_data(ctx,kind,p,n,row,header);
 /* Structural frozen guard only; NOT authentication/manifest resolution. */
 return kind>=5U&&kind<=10U?PC_ROOT8_OK:PC_ROOT8_INCOMPLETE;
}
static int pending_packet14_valid(PAPACC_STORE_HASH *ctx,const PAPACC_CONFIG_PENDING *p)
{
 if(p->previous_bytes>sizeof(p->previous)||p->next_bytes>sizeof(p->next))return 0;
 if(pc_root8_v5_base_link(p->next,p->next_bytes,p->previous,p->previous_bytes,q4_worker_hash,ctx,q4_frozen_component,ctx)!=PC_ROOT8_OK)return 0;
 return store03_zero(p->next+d3_u32(p->next+56),512U);
}
#endif

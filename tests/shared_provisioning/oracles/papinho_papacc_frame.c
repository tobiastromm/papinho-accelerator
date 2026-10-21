/* M0-M8 accepted PACCMP02/version2/suite1; outer framing subset of M2.
 * Never use success here as STRUCTURE/SIGNATURE/TRUST completion or KDF authority.
 */
#include "papinho_papacc_frame.h"
#include <string.h>
#include <limits.h>
#if ULONG_MAX < 0xffffffffUL
#error A 32-bit unsigned long is required.
#endif
static unsigned long pb_mp_u32(const unsigned char *p)
{
    return ((unsigned long)p[0]<<24)|((unsigned long)p[1]<<16)|
        ((unsigned long)p[2]<<8)|(unsigned long)p[3];
}
/* Equality comparisons avoid ordering pointers to unrelated C objects. */
static int pb_mp_alias(const unsigned char *a,size_t an,
    const unsigned char *b,size_t bn)
{
    size_t i;
    for(i=0;i<an;++i)if(a+i==b)return 1;
    for(i=0;i<bn;++i)if(b+i==a)return 1;
    return 0;
}
int papinho_papacc_frame_read(const unsigned char *data,size_t size,
    papinho_papacc_frame *out)
{
    unsigned long s,c,total;
    papinho_papacc_frame f;
    if(!data||!out||size<366U||size>131072UL)return 0;
    if(pb_mp_alias(data,size,(const unsigned char *)out,sizeof(*out)))return 0;
    if(memcmp(data,"PACCMP02",8)||data[8]!=0||data[9]!=2||
       data[10]!=0||data[11]!=1||pb_mp_u32(data+12)!=0||
       pb_mp_u32(data+16)!=(unsigned long)size||
       pb_mp_u32(data+24)!=600000UL)return 0;
    s=pb_mp_u32(data+88);c=pb_mp_u32(data+20);
    if(s==0||s>1024UL||c==0||c>65536UL)return 0;
    /* Both bounded before addition: no unchecked offset arithmetic. */
    total=364UL+s+c;
    if(total!=(unsigned long)size)return 0;
    f.spki_offset=92U;f.spki_length=(size_t)s;
    f.ciphertext_offset=(size_t)(92UL+s);f.ciphertext_length=(size_t)c;
    f.tag_offset=f.ciphertext_offset+f.ciphertext_length;
    f.signature_offset=f.tag_offset+16U;
    *out=f;
    return 1;
}

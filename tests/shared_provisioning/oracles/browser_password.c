#include "papinho_papacc_crypto.h"
#include <string.h>
static int pb_papacc_crypto_alias(const void *a,size_t an,const void *b,size_t bn)
{
    size_t x,y,limit=(size_t)-1;
    if(!a || !b || !an || !bn)return 0;
    x=(size_t)a;y=(size_t)b;
    if(x>limit-an || y>limit-bn)return 1;
    return x<y+bn && y<x+an;
}
int papinho_papacc_crypto_password(const unsigned char *input,size_t n,
    unsigned char out[32])
{
    unsigned char normalized[32];
    size_t i,count=0;
    int result=PAPINHO_PAPACC_CRYPTO_INVALID;
    if(!out || pb_papacc_crypto_alias(input,n,out,32))return result;
    papinho_c2_wipe(out,32);memset(normalized,0,sizeof(normalized));
    if(!input || n>128)goto done;
    for(i=0;i<n;++i) {
        unsigned char v=input[i];
        if(v==' ' || v=='-')continue;
        if(v>='a' && v<='z')v=(unsigned char)(v-'a'+'A');
        if(!((v>='A' && v<='Z') || (v>='2' && v<='7')) || count>=32)goto done;
        normalized[count++]=v;
    }
    if(count!=32)goto done;
    memcpy(out,normalized,32);result=PAPINHO_PAPACC_CRYPTO_ONLY;
done:
    papinho_c2_wipe(normalized,sizeof(normalized));return result;
}

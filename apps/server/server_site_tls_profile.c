#include "server_site_tls_profile.h"
#include <string.h>
#include <stdint.h>
static int nonzero(const PAPACC_U8 *p,PAPACC_SIZE n){PAPACC_U8 v=0;while(n--)v|=*p++;return v!=0;}
PAPACC_RESULT papacc_site_tls_catalog_validate(const PAPACC_SITE_TLS_CATALOG *c)
{
 PAPACC_SIZE i,j,n,start;
 if(!c)return PAPACC_RESULT_INVALID_ARGUMENT;
 if(!c->present){
  const PAPACC_U8 *b=(const PAPACC_U8 *)c;
  return nonzero(b,sizeof(*c))?PAPACC_RESULT_INVALID_STATE:PAPACC_RESULT_OK;
 }
 if(c->present!=1||c->version!=1||c->trust_system!=1||c->hostname_required!=1||
 c->sni_required!=1||c->tls13_only!=1||c->revoked>1||!c->revision||!c->profile_revision||
 !nonzero(c->identity,16)||!nonzero(c->profile_identity,16)||!nonzero(c->administrative_origin,16)||
 c->expires_utc_ms<=c->not_before_utc_ms||!c->max_bytes||!c->max_duration_ms||
 !c->max_concurrent||c->max_concurrent>2||!c->alpn_length||c->alpn_length>64)
 return PAPACC_RESULT_INVALID_STATE;
 for(i=0;i<c->alpn_length;){
  start=i;n=c->alpn[i++];if(!n||n>c->alpn_length-i)return PAPACC_RESULT_INVALID_STATE;
  for(j=0;j<start;){PAPACC_SIZE m=c->alpn[j++];
   if(m==n&&!memcmp(c->alpn+j,c->alpn+i,n))return PAPACC_RESULT_INVALID_STATE;j+=m;}
  i+=n;
 }
 for(i=c->alpn_length;i<64;++i)if(c->alpn[i])return PAPACC_RESULT_INVALID_STATE;
 return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_site_tls_catalog_select(const PAPACC_SITE_TLS_CATALOG *c,PAPACC_U32 ref,
 PAPACC_U64 revision,PAPACC_U64 utc_ms,PAPACC_SITE_TLS_CATALOG *out)
{
 if(!out)return PAPACC_RESULT_INVALID_ARGUMENT;
 if(c){uintptr_t a=(uintptr_t)c,b=(uintptr_t)out;
 if((a<=b?b-a:a-b)<sizeof(*c))return PAPACC_RESULT_INVALID_ARGUMENT;}
 memset(out,0,sizeof(*out));
 if(papacc_site_tls_catalog_validate(c)!=PAPACC_RESULT_OK||!c->present||c->revoked||
 c->reference!=ref||c->revision!=revision||utc_ms<c->not_before_utc_ms||utc_ms>=c->expires_utc_ms)
 return PAPACC_RESULT_INVALID_STATE;
 *out=*c;return PAPACC_RESULT_OK; /* metadata only: not Principal/grant/admission */
}

PAPACC_RESULT papacc_site_tls_alpn_allowed(const PAPACC_SITE_TLS_CATALOG *c,const PAPACC_U8 *v,PAPACC_SIZE n)
{
 PAPACC_SIZE i=0,j,len;int found;
 if(papacc_site_tls_catalog_validate(c)!=PAPACC_RESULT_OK||!c->present||!v||!n||n>256U)return PAPACC_RESULT_INVALID_STATE;
 while(i<n){len=v[i++];if(!len||len>n-i)return PAPACC_RESULT_INVALID_STATE;
  found=0;for(j=0;j<c->alpn_length;){PAPACC_SIZE allow=c->alpn[j++];
   if(allow==len&&!memcmp(c->alpn+j,v+i,len))found=1;j+=allow;}
  if(!found)return PAPACC_RESULT_NOT_SUPPORTED;i+=len;
 }
 return PAPACC_RESULT_OK;
}

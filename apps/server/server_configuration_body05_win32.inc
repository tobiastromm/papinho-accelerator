/* BODY0.5 preserves BODY0.4 Principal16; explicit new discriminator/custody. */
static int p05_body_encode(const PAPACC_CONFIG_VIEW *v,unsigned char *b)
{
 PAPACC_CONFIG_BODY04_VIEW old;const PAPACC_SITE_TLS_CATALOG *c=&v->config.site_tls;
 if(!c->present||papacc_site_tls_catalog_validate(c)!=PAPACC_RESULT_OK)return 0;
 old.base=*v;fix(&old.base);memset(&old.base.config.site_tls,0,sizeof(old.base.config.site_tls));
 if(papacc_configuration_body04_encode(&old,b,1024U)!=PAPACC_RESULT_OK)return 0;
 memcpy(b+512,"ADMP0501",8);memcpy(b+744,"SITE0101",8);
 memcpy(b+752,c->identity,16);memcpy(b+768,c->profile_identity,16);memcpy(b+784,c->administrative_origin,16);
 put64(b+800,c->revision);put64(b+808,c->profile_revision);
 put64(b+816,c->not_before_utc_ms);put64(b+824,c->expires_utc_ms);
 put64(b+832,c->max_bytes);put64(b+840,c->max_duration_ms);
 p03_put32(b+848,c->reference);p03_put32(b+852,c->max_concurrent);
 b[856]=(unsigned char)c->revoked;b[857]=1; b[858]=(unsigned char)c->alpn_length;
 memcpy(b+864,c->alpn,64);
 return store03_hash(b,992U,b+992U);
}
static int store05_decode(unsigned char *b,PAPACC_CONFIG_VIEW *v)
{
 unsigned char old[1024],hash[32];PAPACC_CONFIG_BODY04_VIEW typed;PAPACC_SITE_TLS_CATALOG c;
 if(memcmp(b+512,"ADMP0501",8)||memcmp(b+744,"SITE0101",8)||b[856]>1||b[857]!=1||
 b[858]>64||!store03_zero(b+859,5)||!store03_zero(b+928,64)||
 !store03_hash(b,992U,hash)||memcmp(hash,b+992,32))return 0;
 memset(&c,0,sizeof(c));c.present=1;c.version=1;c.trust_system=1;c.hostname_required=1;c.sni_required=1;c.tls13_only=1;
 memcpy(c.identity,b+752,16);memcpy(c.profile_identity,b+768,16);memcpy(c.administrative_origin,b+784,16);
 c.revision=get64(b+800);c.profile_revision=get64(b+808);
 c.not_before_utc_ms=get64(b+816);c.expires_utc_ms=get64(b+824);c.max_bytes=get64(b+832);c.max_duration_ms=get64(b+840);
 c.reference=p03_get32(b+848);c.max_concurrent=p03_get32(b+852);c.revoked=b[856];c.alpn_length=b[858];memcpy(c.alpn,b+864,64);
 if(papacc_site_tls_catalog_validate(&c)!=PAPACC_RESULT_OK)return 0;
 memcpy(old,b,1024);memcpy(old+512,"ADMP0401",8);memset(old+744,0,248);
 if(!store03_hash(old,992,old+992)||papacc_configuration_body04_decode(old,1024,&typed)!=PAPACC_RESULT_OK)return 0;
 *v=typed.base;v->config.site_tls=c;fix(v);return 1;
}

PAPACC_RESULT papacc_configuration_body05_encode(const PAPACC_CONFIG_VIEW *v,PAPACC_U8 *b,PAPACC_SIZE n)
{
 if(!v||!b||n!=1024U||body04_overlap(v,sizeof(*v),b,n))return PAPACC_RESULT_INVALID_ARGUMENT;
 return p05_body_encode(v,b)?PAPACC_RESULT_OK:PAPACC_RESULT_INVALID_STATE;
}
PAPACC_RESULT papacc_configuration_body05_decode(const PAPACC_U8 *b,PAPACC_SIZE n,PAPACC_CONFIG_VIEW *v)
{
 unsigned char bytes[1024];
 if(!v)return PAPACC_RESULT_INVALID_ARGUMENT;
 if(b&&body04_overlap(b,n,v,sizeof(*v)))return PAPACC_RESULT_INVALID_ARGUMENT;
 memset(v,0,sizeof(*v));if(!b||n!=1024U)return PAPACC_RESULT_INVALID_ARGUMENT;
 memcpy(bytes,b,1024);return store05_decode(bytes,v)?PAPACC_RESULT_OK:PAPACC_RESULT_INVALID_STATE;
}

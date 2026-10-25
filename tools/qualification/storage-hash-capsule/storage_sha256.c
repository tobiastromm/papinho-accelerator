#include "storage_sha256.h"
#include <string.h>
typedef char pointer_fits_size_t[(sizeof(size_t)>=sizeof(void *))?1:-1];
static int span(const void *p,size_t n)
{return !n || (p && n-1U<=(size_t)-1-(size_t)p);}
static int overlap(const void *p,size_t n,const void *q,size_t m)
{size_t a=(size_t)p,b=(size_t)q;return n&&m&&(a<=b?b-a<n:a-b<m);}
static void clear_hash(PACC_STORAGE_SHA256 *c)
{volatile unsigned char *p=(volatile unsigned char *)&c->hash;size_t n=sizeof(c->hash);while(n--)*p++=0;}
static int terminal(PACC_STORAGE_SHA256 *c,int r)
{clear_hash(c);c->state=r==PACC_HASH_STOPPED?PACC_HASH_CANCELLED:PACC_HASH_FAILED;c->busy=0;return r;}
static int enter(PACC_STORAGE_SHA256 *c)
{if(!span(c,sizeof(*c)))return PACC_HASH_INVALID;if(c->version!=1U||c->state!=PACC_HASH_ACTIVE)return PACC_HASH_STATE;
 if(c->busy)return PACC_HASH_BUSY;c->busy=1;return PACC_HASH_OK;}
int pacc_storage_sha_init(PACC_STORAGE_SHA256 *c)
{if(!span(c,sizeof(*c)))return PACC_HASH_INVALID;
 if(c->version||c->state||c->busy)return PACC_HASH_STATE;
 br_sha256_init(&c->hash);c->version=1U;c->state=PACC_HASH_ACTIVE;return PACC_HASH_OK;}
int pacc_storage_sha_update(PACC_STORAGE_SHA256 *c,const void *data,size_t n,PACC_STORAGE_STOP stop,void *sc)
{
 const unsigned char *p=data;size_t k;int r=enter(c);uint64_t maxbytes=(((uint64_t)1U)<<61)-1U;
 (void)sc;if(r)return r;
 if((stop&&!span((const void *)stop,sizeof(*stop)))||!span(data,n)||overlap(data,n,c,sizeof(*c)))return terminal(c,PACC_HASH_INVALID);
 if(c->hash.count>maxbytes||(uint64_t)n>maxbytes-c->hash.count)return terminal(c,PACC_HASH_LIMIT);
 if(stop&&*stop)return terminal(c,PACC_HASH_STOPPED);
 while(n){k=n;if(k>PACC_STORAGE_HASH_CHUNK)k=PACC_STORAGE_HASH_CHUNK;
  br_sha256_update(&c->hash,p,k);p+=k;n-=k;
  if(stop&&*stop)return terminal(c,PACC_HASH_STOPPED);
 }
 c->busy=0;return PACC_HASH_OK;
}
int pacc_storage_sha_final(PACC_STORAGE_SHA256 *c,unsigned char *out,size_t cap,PACC_STORAGE_STOP stop,void *sc)
{
 unsigned char staged[32];int r=enter(c);unsigned i;
 (void)sc;if(r)return r;
 if((stop&&!span((const void *)stop,sizeof(*stop)))||cap<32U||!span(out,32U)||overlap(out,32U,c,sizeof(*c)))return terminal(c,PACC_HASH_INVALID);
 if(stop&&*stop)return terminal(c,PACC_HASH_STOPPED);
 br_sha256_out(&c->hash,staged);
 if(stop&&*stop){volatile unsigned char *v=staged;for(i=0;i<32U;i++)v[i]=0;return terminal(c,PACC_HASH_STOPPED);}
 memcpy(out,staged,32U);clear_hash(c);c->state=PACC_HASH_DONE;c->busy=0;return PACC_HASH_OK;
}
int pacc_storage_hash_callback(void *ctx,const unsigned char *p,size_t n,unsigned char out[32])
{
 PACC_STORAGE_SHA256 c;PACC_STORAGE_HASH_CALL *call=ctx;int r;
 memset(&c,0,sizeof(c));r=pacc_storage_sha_init(&c);
 if(!r)r=pacc_storage_sha_update(&c,p,n,call?call->stop:NULL,call?call->context:NULL);
 if(!r)r=pacc_storage_sha_final(&c,out,32U,call?call->stop:NULL,call?call->context:NULL);
 return r==PACC_HASH_OK;
}

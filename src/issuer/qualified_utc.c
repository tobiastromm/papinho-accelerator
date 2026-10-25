#include "qualified_utc.h"
#include <string.h>
#define UTC_MAX (~(PAPACC_U64)0)
static int add(PAPACC_U64 a,PAPACC_U64 b,PAPACC_U64 *v)
{
 if(a>UTC_MAX-b)return 0;
 *v=a+b;return 1;
}
static int mul(PAPACC_U64 a,PAPACC_U64 b,PAPACC_U64 *v)
{
 if(b&&a>UTC_MAX/b)return 0;
 *v=a*b;return 1;
}
static int same(const PAPACC_UTC_CONTEXT *a,const PAPACC_UTC_CONTEXT *b)
{
 return a->owner==b->owner&&a->backend==b->backend&&a->source==b->source&&a->pin==b->pin&&
 !memcmp(a->root,b->root,32)&&!memcmp(a->candidate,b->candidate,32)&&
 a->process_lifetime==b->process_lifetime&&a->invocation==b->invocation&&
 a->revision==b->revision&&a->qualification==b->qualification&&
 a->continuity==b->continuity&&a->boundary==b->boundary;
}
static PAPACC_UTC_DATA_RESULT shape(const PAPACC_UTC_OBSERVATION *o)
{
 if(!o||o->version!=PAPACC_UTC_DATA_VERSION)return PAPACC_UTC_DATA_INVALID;
 if(o->present!=PAPACC_UTC_DATA_PRESENT||o->read_result!=PAPACC_RESULT_OK||
 o->quality_result!=PAPACC_RESULT_OK)return PAPACC_UTC_DATA_UNKNOWN;
 if(o->lower>o->upper||o->m0>o->m1||o->m1>=o->deadline||
 !o->rho_den||o->rho_num>=o->rho_den)return PAPACC_UTC_DATA_INVALID;
 return PAPACC_UTC_DATA_OK;
}
PAPACC_UTC_DATA_RESULT papacc_utc_data_propagate(const PAPACC_UTC_OBSERVATION *o,
 const PAPACC_UTC_CONTEXT *c,PAPACC_U64 m,PAPACC_U64 *lower,PAPACC_U64 *upper)
{
 PAPACC_UTC_DATA_RESULT r;PAPACC_U64 d,p,a,b,n,dl,du,l,u;
 if(!c||!lower||!upper||lower==upper)return PAPACC_UTC_DATA_INVALID;
 r=shape(o);if(r!=PAPACC_UTC_DATA_OK)return r;
 if(!same(&o->context,c)||m<o->m1)return PAPACC_UTC_DATA_DISCONTINUITY;
 if(m>=o->deadline)return PAPACC_UTC_DATA_EXPIRED;
 d=m-o->m1;
 /* A representable result with an unrepresentable intermediate is denied,
  * never saturated or silently approximated. All roundings are outward. */
 if(!mul(d,o->rho_den-o->rho_num,&p))return PAPACC_UTC_DATA_LIMIT;
 a=p/o->rho_den;
 if(!add(o->rho_den,o->rho_num,&n)||!mul(d,n,&p))return PAPACC_UTC_DATA_LIMIT;
 b=p/o->rho_den;
 if(p%o->rho_den&&!add(b,1,&b))return PAPACC_UTC_DATA_LIMIT;
 dl=a<=o->pal_error?0:a-o->pal_error;
 if(!add(b,o->pal_error,&du)||!add(o->lower,dl,&l)||!add(o->upper,du,&u))return PAPACC_UTC_DATA_LIMIT;
 *lower=l;*upper=u;return PAPACC_UTC_DATA_OK;
}
PAPACC_UTC_DATA_RESULT papacc_utc_data_observe(PAPACC_UTC_HISTORY *h,const PAPACC_UTC_OBSERVATION *o)
{
 PAPACC_UTC_DATA_RESULT r;PAPACC_U64 l,u;
 if(!h)return PAPACC_UTC_DATA_INVALID;
 if(h->stopped)return PAPACC_UTC_DATA_STOPPED;
 if(h->invalid)return PAPACC_UTC_DATA_INVALID;
 r=shape(o);
 if(r==PAPACC_UTC_DATA_OK&&h->has_last){
  if(o->native_utc<h->last.native_utc||o->m0<h->last_monotonic||
   o->rho_num!=h->last.rho_num||o->rho_den!=h->last.rho_den||
   o->pal_error!=h->last.pal_error||o->deadline!=h->last.deadline)
   r=PAPACC_UTC_DATA_DISCONTINUITY;
  else{
   r=papacc_utc_data_propagate(&h->last,&o->context,o->m1,&l,&u);
   if(r==PAPACC_UTC_DATA_OK&&(o->upper<l||o->lower>u))r=PAPACC_UTC_DATA_DISCONTINUITY;
  }
 }
 if(r!=PAPACC_UTC_DATA_OK){h->invalid=1;return r;}
 h->last=*o;h->last_monotonic=o->m1;h->has_last=1;return PAPACC_UTC_DATA_OK;
}
PAPACC_UTC_DATA_RESULT papacc_utc_data_query(PAPACC_UTC_HISTORY *h,const PAPACC_UTC_CONTEXT *c,
 PAPACC_U64 m,PAPACC_U64 *l,PAPACC_U64 *u)
{
 PAPACC_UTC_DATA_RESULT r;
 if(!h)return PAPACC_UTC_DATA_INVALID;
 if(h->stopped)return PAPACC_UTC_DATA_STOPPED;
 if(h->invalid)return PAPACC_UTC_DATA_INVALID;
 if(!h->has_last)return PAPACC_UTC_DATA_UNKNOWN;
 if(m<h->last_monotonic){h->invalid=1;return PAPACC_UTC_DATA_DISCONTINUITY;}
 r=papacc_utc_data_propagate(&h->last,c,m,l,u);
 if(r!=PAPACC_UTC_DATA_OK)h->invalid=1;
 else h->last_monotonic=m;
 return r;
}
void papacc_utc_data_invalidate(PAPACC_UTC_HISTORY *h)
{
 if(h)h->invalid=1;
}
void papacc_utc_data_stop(PAPACC_UTC_HISTORY *h)
{
 if(h){h->stopped=1;h->invalid=1;}
}
PAPACC_UTC_DATA_RESULT papacc_utc_data_within(PAPACC_U64 l,PAPACC_U64 u,PAPACC_U64 nb,PAPACC_U64 na)
{
 PAPACC_U64 a,b;
 if(l>u||nb>=na)return PAPACC_UTC_DATA_INVALID;
 if(!mul(nb,PAPACC_UTC_NS_PER_SECOND,&a)||!mul(na,PAPACC_UTC_NS_PER_SECOND,&b))return PAPACC_UTC_DATA_LIMIT;
 return a<=l&&u<b?PAPACC_UTC_DATA_OK:PAPACC_UTC_DATA_INVALID;
}
PAPACC_UTC_DATA_RESULT papacc_utc_data_filetime(PAPACC_U64 f,PAPACC_U64 *out)
{
 PAPACC_U64 epoch,v;
 epoch=(PAPACC_U64)116444736UL*(PAPACC_U64)1000000000UL;
 if(!out||f<epoch)return PAPACC_UTC_DATA_INVALID;
 if(!mul(f-epoch,100,&v))return PAPACC_UTC_DATA_LIMIT;
 *out=v;return PAPACC_UTC_DATA_OK;
}
PAPACC_RESULT papacc_issuer_qualified_utc_read(const struct PAPACC_ISSUER_BACKEND *backend,PAPACC_UTC_OBSERVATION *out)
{
 (void)backend;(void)out;
 return PAPACC_RESULT_NOT_SUPPORTED;
}

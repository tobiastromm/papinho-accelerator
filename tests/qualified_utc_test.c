#include "qualified_utc.h"
#include "issuer_core.h"
#include <stdio.h>
#include <string.h>
#define MAXIMUM (~(PAPACC_U64)0)
static unsigned checks,failures;
static void check(int value,unsigned line)
{
 ++checks;if(!value){++failures;printf("FAIL line %u\n",line);}
}
#define CHECK(x) check((x),(unsigned)__LINE__)
static PAPACC_UTC_OBSERVATION sample(void)
{
 PAPACC_UTC_OBSERVATION o;
 memset(&o,0,sizeof(o));o.version=PAPACC_UTC_DATA_VERSION;o.present=PAPACC_UTC_DATA_PRESENT;
 o.native_utc=1000;o.lower=990;o.upper=1010;o.m0=90;o.m1=100;o.deadline=10000;
 o.rho_num=1;o.rho_den=10;o.pal_error=2;
 return o;
}
static unsigned scalar_calls;
static PAPACC_ISSUER_RESULT scalar(void *p,PAPACC_U64 *v)
{
 (void)p;++scalar_calls;*v=1000;return PAPACC_ISSUER_OK;
}
int main(void)
{
 PAPACC_UTC_OBSERVATION o,n,saved;PAPACC_UTC_CONTEXT c;
 PAPACC_UTC_HISTORY h;PAPACC_ISSUER_BACKEND backend;
 PAPACC_U64 l,u,v,epoch;unsigned den,num,d,e,bit;
 o=sample();l=7;u=8;
 CHECK(papacc_utc_data_propagate(&o,&o.context,100,&l,&u)==PAPACC_UTC_DATA_OK&&l==990&&u==1012);
 CHECK(papacc_utc_data_propagate(&o,&o.context,101,&l,&u)==PAPACC_UTC_DATA_OK&&l==990&&u==1014);
 CHECK(papacc_utc_data_propagate(&o,&o.context,111,&l,&u)==PAPACC_UTC_DATA_OK&&l==997&&u==1025);
 /* Exhaustive small rational vectors: exact outward endpoints and errors,
  * including zero rate/duration/error and both fractional directions. */
 for(den=1;den<=9;++den)for(num=0;num<den;++num)for(d=0;d<32;++d)for(e=0;e<4;++e){
  PAPACC_U64 a,b;
  o=sample();o.rho_num=num;o.rho_den=den;o.pal_error=e;
  a=(d*(den-num))/den;b=(d*(den+num)+den-1)/den;
  CHECK(papacc_utc_data_propagate(&o,&o.context,o.m1+d,&l,&u)==PAPACC_UTC_DATA_OK);
  CHECK(l==o.lower+(a<=e?0:a-e)&&u==o.upper+b+e);
 }
 o=sample();
 for(bit=0;bit<13;++bit){n=o;n.present&=~(1U<<bit);l=7;u=8;
  CHECK(papacc_utc_data_propagate(&n,&n.context,100,&l,&u)==PAPACC_UTC_DATA_UNKNOWN&&l==7&&u==8);}
 n=o;n.read_result=PAPACC_RESULT_INTERNAL_ERROR;CHECK(papacc_utc_data_propagate(&n,&n.context,100,&l,&u)==PAPACC_UTC_DATA_UNKNOWN);
 n=o;n.quality_result=PAPACC_RESULT_NOT_SUPPORTED;CHECK(papacc_utc_data_propagate(&n,&n.context,100,&l,&u)==PAPACC_UTC_DATA_UNKNOWN);
 n=o;n.version=2;CHECK(papacc_utc_data_propagate(&n,&n.context,100,&l,&u)==PAPACC_UTC_DATA_INVALID);
 n=o;n.rho_den=0;CHECK(papacc_utc_data_propagate(&n,&n.context,100,&l,&u)==PAPACC_UTC_DATA_INVALID);
 n=o;n.rho_num=n.rho_den;CHECK(papacc_utc_data_propagate(&n,&n.context,100,&l,&u)==PAPACC_UTC_DATA_INVALID);
 n=o;n.lower=n.upper+1;CHECK(papacc_utc_data_propagate(&n,&n.context,100,&l,&u)==PAPACC_UTC_DATA_INVALID);
 n=o;n.m0=n.m1+1;CHECK(papacc_utc_data_propagate(&n,&n.context,100,&l,&u)==PAPACC_UTC_DATA_INVALID);
 CHECK(papacc_utc_data_propagate(&o,&o.context,99,&l,&u)==PAPACC_UTC_DATA_DISCONTINUITY);
 CHECK(papacc_utc_data_propagate(&o,&o.context,o.deadline,&l,&u)==PAPACC_UTC_DATA_EXPIRED);
 c=o.context;c.revision++;CHECK(papacc_utc_data_propagate(&o,&c,100,&l,&u)==PAPACC_UTC_DATA_DISCONTINUITY);
 c=o.context;c.source=&c;CHECK(papacc_utc_data_propagate(&o,&c,100,&l,&u)==PAPACC_UTC_DATA_DISCONTINUITY);
 c=o.context;c.pin=&c;CHECK(papacc_utc_data_propagate(&o,&c,100,&l,&u)==PAPACC_UTC_DATA_DISCONTINUITY);
 c=o.context;c.boundary++;CHECK(papacc_utc_data_propagate(&o,&c,100,&l,&u)==PAPACC_UTC_DATA_DISCONTINUITY);
 c=o.context;c.process_lifetime++;CHECK(papacc_utc_data_propagate(&o,&c,100,&l,&u)==PAPACC_UTC_DATA_DISCONTINUITY);
 n=o;n.rho_den=MAXIMUM;n.rho_num=1;CHECK(papacc_utc_data_propagate(&n,&n.context,100,&l,&u)==PAPACC_UTC_DATA_LIMIT);
 n=o;n.rho_den=MAXIMUM;n.rho_num=0;CHECK(papacc_utc_data_propagate(&n,&n.context,102,&l,&u)==PAPACC_UTC_DATA_LIMIT);
 n=o;n.pal_error=MAXIMUM;CHECK(papacc_utc_data_propagate(&n,&n.context,101,&l,&u)==PAPACC_UTC_DATA_LIMIT);
 n=o;n.upper=MAXIMUM;CHECK(papacc_utc_data_propagate(&n,&n.context,100,&l,&u)==PAPACC_UTC_DATA_LIMIT);
 n=o;n.lower=MAXIMUM-1;n.upper=MAXIMUM-1;n.pal_error=0;n.rho_num=0;n.rho_den=1;
 CHECK(papacc_utc_data_propagate(&n,&n.context,102,&l,&u)==PAPACC_UTC_DATA_LIMIT);

 n=o;n.m0=0;n.m1=0;n.deadline=MAXIMUM;n.rho_den=3;n.rho_num=2;
 CHECK(papacc_utc_data_propagate(&n,&n.context,MAXIMUM/4,&l,&u)==PAPACC_UTC_DATA_LIMIT);
 CHECK(papacc_utc_data_within(0,999999999,0,1)==PAPACC_UTC_DATA_OK);
 CHECK(papacc_utc_data_within(0,1000000000,0,1)==PAPACC_UTC_DATA_INVALID);
 CHECK(papacc_utc_data_within(999999999,1000000001,1,2)==PAPACC_UTC_DATA_INVALID);
 CHECK(papacc_utc_data_within(0,1,0,MAXIMUM)==PAPACC_UTC_DATA_LIMIT);
 epoch=(PAPACC_U64)116444736UL*(PAPACC_U64)1000000000UL;v=77;
 CHECK(papacc_utc_data_filetime(epoch-1,&v)==PAPACC_UTC_DATA_INVALID&&v==77);
 CHECK(papacc_utc_data_filetime(epoch,&v)==PAPACC_UTC_DATA_OK&&v==0);
 CHECK(papacc_utc_data_filetime(epoch+1,&v)==PAPACC_UTC_DATA_OK&&v==100);
 v=77;CHECK(papacc_utc_data_filetime(MAXIMUM,&v)==PAPACC_UTC_DATA_LIMIT&&v==77);
 memset(&h,0,sizeof(h));CHECK(papacc_utc_data_query(&h,&o.context,100,&l,&u)==PAPACC_UTC_DATA_UNKNOWN);
 CHECK(papacc_utc_data_observe(&h,&o)==PAPACC_UTC_DATA_OK);
 CHECK(papacc_utc_data_query(&h,&o.context,110,&l,&u)==PAPACC_UTC_DATA_OK);
 CHECK(papacc_utc_data_query(&h,&o.context,109,&l,&u)==PAPACC_UTC_DATA_DISCONTINUITY);
 CHECK(!memcmp(&h.last,&o,sizeof(o))&&h.invalid);
 CHECK(papacc_utc_data_observe(&h,&o)==PAPACC_UTC_DATA_INVALID);
 memset(&h,0,sizeof(h));CHECK(papacc_utc_data_observe(&h,&o)==PAPACC_UTC_DATA_OK);
 n=o;n.m0=110;n.m1=111;n.native_utc=1011;n.lower=997;n.upper=1025;
 CHECK(papacc_utc_data_observe(&h,&n)==PAPACC_UTC_DATA_OK);
 saved=h.last;n.native_utc--;n.m0=111;n.m1=112;
 CHECK(papacc_utc_data_observe(&h,&n)==PAPACC_UTC_DATA_DISCONTINUITY&&!memcmp(&h.last,&saved,sizeof(saved)));
 memset(&h,0,sizeof(h));CHECK(papacc_utc_data_observe(&h,&o)==PAPACC_UTC_DATA_OK);
 n=o;n.m0=110;n.m1=111;n.lower=2000;n.upper=2010;
 CHECK(papacc_utc_data_observe(&h,&n)==PAPACC_UTC_DATA_DISCONTINUITY);
 memset(&h,0,sizeof(h));CHECK(papacc_utc_data_observe(&h,&o)==PAPACC_UTC_DATA_OK);
 c=o.context;c.continuity++;
 CHECK(papacc_utc_data_query(&h,&c,100,&l,&u)==PAPACC_UTC_DATA_DISCONTINUITY);
 memset(&h,0,sizeof(h));CHECK(papacc_utc_data_observe(&h,&o)==PAPACC_UTC_DATA_OK);
 saved=h.last;papacc_utc_data_stop(&h);
 CHECK(papacc_utc_data_query(&h,&o.context,100,&l,&u)==PAPACC_UTC_DATA_STOPPED);
 CHECK(papacc_utc_data_observe(&h,&o)==PAPACC_UTC_DATA_STOPPED&&!memcmp(&h.last,&saved,sizeof(saved)));
 memset(&h,0,sizeof(h)); /* New process DATA, never restores authority. */
 CHECK(papacc_utc_data_query(&h,&o.context,100,&l,&u)==PAPACC_UTC_DATA_UNKNOWN);
 memset(&backend,0,sizeof(backend));backend.utc=scalar;saved=o;
 CHECK(papacc_issuer_qualified_utc_read(&backend,&o)==PAPACC_RESULT_NOT_SUPPORTED&&scalar_calls==0&&!memcmp(&saved,&o,sizeof(o)));
 CHECK(papacc_issuer_qualified_utc_read(NULL,&o)==PAPACC_RESULT_NOT_SUPPORTED&&!memcmp(&saved,&o,sizeof(o)));
 printf("CHECKS=%u FAILURES=%u CONTEXT=%u OBSERVATION=%u HISTORY=%u\n",checks,failures,(unsigned)sizeof(c),(unsigned)sizeof(o),(unsigned)sizeof(h));
 return failures?1:0;
}

#include "issuer_core.h"
#include <stdio.h>
#include <string.h>
static unsigned reads;
static PAPACC_ISSUER_RESULT read_port(void *p,const PAPACC_U8 id[16],PAPACC_ISSUER_VIEW *v,PAPACC_ISSUER_RECORD *r,int *found){(void)p;(void)id;(void)v;(void)r;(void)found;reads++;return PAPACC_ISSUER_UNSUPPORTED;}
static PAPACC_ISSUER_RESULT cas_port(void *p,const PAPACC_ISSUER_VIEW *v,const PAPACC_ISSUER_RECORD *a,const PAPACC_ISSUER_RECORD *b,PAPACC_U64 high,const PAPACC_U8 *d,PAPACC_SIZE n,int publish){(void)p;(void)v;(void)a;(void)b;(void)high;(void)d;(void)n;(void)publish;return PAPACC_ISSUER_UNSUPPORTED;}
static PAPACC_ISSUER_RESULT auth_port(void *p,const PAPACC_ISSUER_VIEW *v,const PAPACC_ISSUER_REQUEST *r,unsigned a){(void)p;(void)v;(void)r;(void)a;return PAPACC_ISSUER_DENIED;}
static PAPACC_ISSUER_RESULT clock_port(void *p,PAPACC_U64 *n){(void)p;(void)n;return PAPACC_ISSUER_UNSUPPORTED;}
int main(void){
 PAPACC_ISSUER_CORE c;PAPACC_ISSUER_ROOT_PORT r;PAPACC_ISSUER_BACKEND b;
 memset(&c,0,sizeof(c));memset(&r,0,sizeof(r));memset(&b,0,sizeof(b));
 r.ctx=&reads;r.read=read_port;r.cas=cas_port;r.test_only=1;b.authorize=auth_port;b.utc=clock_port;
 if(papacc_issuer_init(&c,&r,&b)!=PAPACC_ISSUER_DENIED||reads)return 1;
 r.test_only=0;b.test_only=1;if(papacc_issuer_init(&c,&r,&b)!=PAPACC_ISSUER_DENIED||reads)return 2;
 if(papacc_issuer_init(&c,NULL,&b)!=PAPACC_ISSUER_UNSUPPORTED)return 3;
 b.test_only=0;if(papacc_issuer_init(&c,&r,&b)!=PAPACC_ISSUER_UNSUPPORTED||reads!=1)return 4;
 puts("PASS production_gate_checks=4; test_adapters_rejected_before_root_access; absent_live_root=UNSUPPORTED");return 0;
}
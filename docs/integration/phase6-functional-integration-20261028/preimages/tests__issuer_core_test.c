#include "issuer_core.h"
#include <stdio.h>
#include <string.h>
/* Isolated SAME-root transaction model; no native storage/crypto/runtime. */
typedef struct MODEL {
 PAPACC_ISSUER_VIEW view;PAPACC_ISSUER_RECORD records[16];PAPACC_U8 der[16][32];unsigned count;
 unsigned commits,signs,acquires,releases,auths,clock_calls;
 int fail_cas,fail_sign,fail_release,deny_auth,bad_request,bad_cert,allow_publish,stop_on_sign,expire_on_acquire,reenter,reenter_result;
 PAPACC_U64 now;PAPACC_ISSUER_CORE *owner,*competitor;PAPACC_U8 competing_id[16];
} MODEL;
static MODEL m;
static PAPACC_ISSUER_BACKEND backend;static PAPACC_ISSUER_ROOT_PORT root;
static PAPACC_U8 output[128];static unsigned checks;
#define CHECK(x) do{checks++;if(!(x)){printf("FAIL line=%d checks=%u\n",__LINE__,checks);return 1;}}while(0)
static int slot(MODEL *x,const PAPACC_U8 id[16]){unsigned i;for(i=0;i<x->count;i++)if(!memcmp(id,x->records[i].request.id,16))return (int)i;return -1;}
static PAPACC_ISSUER_RESULT read_root(void *p,const PAPACC_U8 id[16],PAPACC_ISSUER_VIEW *v,PAPACC_ISSUER_RECORD *r,int *found){MODEL *x=p;int i=slot(x,id);*v=x->view;*found=i>=0;if(i>=0)*r=x->records[i];return PAPACC_ISSUER_OK;}
static PAPACC_ISSUER_RESULT copy_cert(void *p,const PAPACC_U8 id[16],const PAPACC_U8 digest[32],PAPACC_U8 *out,PAPACC_SIZE cap,PAPACC_SIZE *size){
 MODEL *x=p;int i=slot(x,id);if(i<0)return PAPACC_ISSUER_ABSENT;
 if(memcmp(digest,x->records[i].certificate_digest,32))return PAPACC_ISSUER_CONFLICT;
 *size=x->records[i].certificate_size;if(cap<*size)return PAPACC_ISSUER_LIMIT;memcpy(out,x->der[i],*size);return PAPACC_ISSUER_OK;
}
static PAPACC_ISSUER_RESULT cas_root(void *p,const PAPACC_ISSUER_VIEW *v,const PAPACC_ISSUER_RECORD *old,const PAPACC_ISSUER_RECORD *next,PAPACC_U64 high,const PAPACC_U8 *der,PAPACC_SIZE size,int publish){
 MODEL *x=p;int i;PAPACC_ISSUER_CORE *other;
 if(x->competitor){other=x->competitor;x->competitor=NULL;(void)papacc_issuer_reserve(other,x->competing_id);}
 if(v->revision!=x->view.revision||v->high_water!=x->view.high_water)return PAPACC_ISSUER_CONFLICT;
 i=slot(x,next->request.id);
 if((old&&(i<0||x->records[i].state!=old->state))||(!old&&i>=0))return PAPACC_ISSUER_CONFLICT;
 if(x->fail_cas==1)return PAPACC_ISSUER_UNCERTAIN;
 if(publish&&!x->allow_publish)return PAPACC_ISSUER_UNSUPPORTED;
 if(i<0){if(x->count==16)return PAPACC_ISSUER_LIMIT;i=(int)x->count++;}
 if(size>32)return PAPACC_ISSUER_LIMIT;
 if(size){if(!der)return PAPACC_ISSUER_INVALID;memcpy(x->der[i],der,size);}
 x->records[i]=*next;x->view.high_water=high;x->view.revision++;x->view.epoch++;x->commits++;
 if(publish)x->records[i].published_epoch=x->view.epoch;
 if(x->fail_cas==2)return PAPACC_ISSUER_UNCERTAIN;
 return PAPACC_ISSUER_OK;
}
static PAPACC_ISSUER_RESULT auth(void *p,const PAPACC_ISSUER_VIEW *v,const PAPACC_ISSUER_REQUEST *q,unsigned action){MODEL *x=p;(void)v;(void)q;(void)action;x->auths++;if(x->reenter){x->reenter=0;x->reenter_result=(int)papacc_issuer_reserve(x->owner,q->id);}return x->deny_auth?PAPACC_ISSUER_DENIED:PAPACC_ISSUER_OK;}
static PAPACC_ISSUER_RESULT utc(void *p,PAPACC_U64 *now){MODEL *x=p;x->clock_calls++;*now=x->now;return PAPACC_ISSUER_OK;}
static PAPACC_ISSUER_RESULT verify_req(void *p,const PAPACC_ISSUER_RECORD *r,const PAPACC_U8 *data,PAPACC_SIZE size){MODEL *x=p;(void)r;return !x->bad_request&&data&&size==1&&data[0]==7?PAPACC_ISSUER_OK:PAPACC_ISSUER_DENIED;}
static PAPACC_ISSUER_RESULT acquire(void *p,const PAPACC_ISSUER_VIEW *v,const PAPACC_ISSUER_RECORD *r,void **lease){MODEL *x=p;(void)v;(void)r;x->acquires++;if(x->expire_on_acquire)x->now=r->request.not_after;*lease=p;return PAPACC_ISSUER_OK;}
static PAPACC_ISSUER_RESULT sign_cert(void *p,void *lease,const PAPACC_ISSUER_RECORD *r,const PAPACC_U8 *csr,PAPACC_SIZE size,PAPACC_U8 *out,PAPACC_SIZE cap,PAPACC_SIZE *n){
 MODEL *x=p;int i=slot(x,r->request.id);(void)csr;(void)size;
 if(lease!=p||i<0||x->records[i].state!=PAPACC_ISSUER_SIGN_REQUESTED||r->counter>x->view.high_water)return PAPACC_ISSUER_INVALID;
 x->signs++;if(cap<3)return PAPACC_ISSUER_LIMIT;memcpy(out,"DER",3);*n=3;
 if(x->stop_on_sign)papacc_issuer_stop(x->owner);
 if(x->fail_sign)return PAPACC_ISSUER_UNCERTAIN;return PAPACC_ISSUER_OK;
}
static PAPACC_ISSUER_RESULT verify_cert(void *p,const PAPACC_ISSUER_RECORD *r,const PAPACC_U8 *der,PAPACC_SIZE n,PAPACC_U8 digest[32]){MODEL *x=p;(void)r;
 if(x->bad_cert||n!=3||memcmp(der,"DER",3))return PAPACC_ISSUER_DENIED;memset(digest,9,32);return PAPACC_ISSUER_OK;}
static PAPACC_ISSUER_RESULT release(void *p,void *lease){MODEL *x=p;(void)lease;x->releases++;return x->fail_release?PAPACC_ISSUER_RETAINED:PAPACC_ISSUER_OK;}
static void setup(void){
 memset(&m,0,sizeof(m));memset(&root,0,sizeof(root));memset(&backend,0,sizeof(backend));
 m.view.root[0]=1;m.view.authority[0]=2;m.view.generation[0]=3;m.view.prefix[0]=4;m.view.policy[0]=5;m.view.ca_spki_digest[0]=6;
 m.view.revision=1;m.view.epoch=1;m.view.high_water=1;m.view.ca_not_after=1000000;m.view.proofs=PAPACC_ISSUER_ROOT_PROOFS;m.now=100;
 root.ctx=&m;root.read=read_root;root.cas=cas_root;root.copy_certificate=copy_cert;root.test_only=1;
 backend.ctx=&m;backend.test_only=1;backend.authorize=auth;backend.utc=utc;backend.verify_request=verify_req;
 backend.acquire=acquire;backend.sign=sign_cert;backend.verify_certificate=verify_cert;backend.release=release;
}
static PAPACC_ISSUER_REQUEST request(unsigned id){PAPACC_ISSUER_REQUEST q;memset(&q,0,sizeof(q));q.id[0]=(PAPACC_U8)id;q.device[0]=8;q.csr_digest[0]=7;q.key_digest[0]=(PAPACC_U8)id;q.purpose=PAPACC_ISSUER_CLIENT;q.not_before=100;q.not_after=500;return q;}
int main(void){
 PAPACC_ISSUER_CORE c,d;PAPACC_ISSUER_REQUEST q,q2;PAPACC_ISSUER_RECORD r;PAPACC_U8 csr=7;unsigned before;PAPACC_U64 high;
 setup();memset(&c,0,sizeof(c));
#ifndef PAPACC_ISSUER_OFFLINE_TEST
 CHECK(papacc_issuer_init(&c,&root,&backend)==PAPACC_ISSUER_DENIED);
 CHECK(!c.initialized&&!m.signs&&!m.commits);
 CHECK(papacc_issuer_init(&c,NULL,&backend)==PAPACC_ISSUER_UNSUPPORTED);
 printf("PASS production_test_backend_rejection=%u\n",checks);return 0;
#else
 CHECK(papacc_issuer_init(&c,NULL,&backend)==PAPACC_ISSUER_UNSUPPORTED);
 CHECK(papacc_issuer_init(&c,&root,&backend)==PAPACC_ISSUER_OK);q=request(1);
 CHECK(papacc_issuer_prepare(&c,&q)==PAPACC_ISSUER_OK);before=m.commits;
 CHECK(papacc_issuer_prepare(&c,&q)==PAPACC_ISSUER_OK&&m.commits==before);
 q2=q;q2.key_digest[0]=99;CHECK(papacc_issuer_prepare(&c,&q2)==PAPACC_ISSUER_CONFLICT);
 CHECK(papacc_issuer_issue(&c,q.id,&csr,1,output,sizeof(output))==PAPACC_ISSUER_UNCERTAIN&&!m.signs);
 CHECK(papacc_issuer_reserve(&c,q.id)==PAPACC_ISSUER_OK&&m.view.high_water==2);
 CHECK(papacc_issuer_query(&c,q.id,&r)==PAPACC_ISSUER_OK&&r.serial[0]==4&&r.serial[15]==2&&r.counter==2);
 before=m.commits;CHECK(papacc_issuer_reserve(&c,q.id)==PAPACC_ISSUER_OK&&before==m.commits);
 CHECK(papacc_issuer_issue(&c,q.id,&csr,1,output,sizeof(output))==PAPACC_ISSUER_OK&&m.signs==1&&m.releases==1);
 CHECK(papacc_issuer_query(&c,q.id,&r)==PAPACC_ISSUER_OK&&r.state==PAPACC_ISSUER_ISSUED_UNPUBLISHED&&r.certificate_size==3);
 memset(output,0,sizeof(output));CHECK(papacc_issuer_issue(&c,q.id,&csr,1,output,sizeof(output))==PAPACC_ISSUER_OK&&m.signs==1&&!memcmp(output,"DER",3));
 CHECK(papacc_issuer_transition(&c,q.id,PAPACC_ISSUER_PUBLISHED)==PAPACC_ISSUER_UNSUPPORTED);
 CHECK(papacc_issuer_query(&c,q.id,&r)==PAPACC_ISSUER_OK&&r.state==PAPACC_ISSUER_ISSUED_UNPUBLISHED);
 m.allow_publish=1;CHECK(papacc_issuer_transition(&c,q.id,PAPACC_ISSUER_PUBLISHED)==PAPACC_ISSUER_OK);
 CHECK(papacc_issuer_query(&c,q.id,&r)==PAPACC_ISSUER_OK&&r.published_epoch!=0);
 CHECK(papacc_issuer_transition(&c,q.id,PAPACC_ISSUER_REVOKED)==PAPACC_ISSUER_OK);
 CHECK(papacc_issuer_transition(&c,q.id,PAPACC_ISSUER_PUBLISHED)==PAPACC_ISSUER_DENIED);
 q2=request(2);memcpy(q2.predecessor,q.id,16);
 CHECK(papacc_issuer_prepare(&c,&q2)==PAPACC_ISSUER_OK);
 CHECK(papacc_issuer_reserve(&c,q2.id)==PAPACC_ISSUER_OK&&m.view.high_water==3);
 CHECK(papacc_issuer_transition(&c,q2.id,PAPACC_ISSUER_ABANDONED)==PAPACC_ISSUER_OK&&m.view.high_water==3);
 CHECK(papacc_issuer_query(&c,q.id,&r)==PAPACC_ISSUER_OK&&r.state==PAPACC_ISSUER_REVOKED);
 papacc_issuer_stop(&c);before=m.auths;CHECK(papacc_issuer_query(&c,q.id,&r)==PAPACC_ISSUER_OK&&m.auths==before+1);
 CHECK(papacc_issuer_reserve(&c,q.id)==PAPACC_ISSUER_STOPPED);CHECK(papacc_issuer_finish(&c)==PAPACC_ISSUER_OK);
 CHECK(papacc_issuer_init(&c,&root,&backend)==PAPACC_ISSUER_INVALID);
 /* Two independently serialized owners, competing atomic root writers. */
 setup();memset(&c,0,sizeof(c));memset(&d,0,sizeof(d));CHECK(papacc_issuer_init(&c,&root,&backend)==0);CHECK(papacc_issuer_init(&d,&root,&backend)==0);
 q=request(1);q2=request(2);CHECK(papacc_issuer_prepare(&c,&q)==0);CHECK(papacc_issuer_prepare(&d,&q2)==0);
 m.competitor=&d;memcpy(m.competing_id,q2.id,16);CHECK(papacc_issuer_reserve(&c,q.id)==PAPACC_ISSUER_CONFLICT&&m.view.high_water==2&&!m.signs);
 CHECK(papacc_issuer_reserve(&c,q.id)==0&&m.view.high_water==3);CHECK(m.records[0].counter!=m.records[1].counter);
 /* Uncertain reservation burns committed counter; restart reads same request. */
 setup();memset(&c,0,sizeof(c));CHECK(papacc_issuer_init(&c,&root,&backend)==0);q=request(1);CHECK(papacc_issuer_prepare(&c,&q)==0);
 m.fail_cas=2;CHECK(papacc_issuer_reserve(&c,q.id)==PAPACC_ISSUER_UNCERTAIN&&m.view.high_water==2);
 CHECK(papacc_issuer_issue(&c,q.id,&csr,1,output,sizeof(output))==PAPACC_ISSUER_UNCERTAIN&&!m.signs);
 memset(&d,0,sizeof(d));m.fail_cas=0;CHECK(papacc_issuer_init(&d,&root,&backend)==0);CHECK(papacc_issuer_recover(&d,q.id)==0);
 CHECK(papacc_issuer_reserve(&d,q.id)==0&&m.view.high_water==2);
 /* Signature uncertainty retains output; new owner never signs same marker. */
 m.fail_sign=1;CHECK(papacc_issuer_issue(&d,q.id,&csr,1,output,sizeof(output))==PAPACC_ISSUER_UNCERTAIN&&m.signs==1);
 CHECK(d.retained_output==output);papacc_issuer_stop(&d);CHECK(papacc_issuer_finish(&d)==PAPACC_ISSUER_RETAINED);
 memset(&c,0,sizeof(c));CHECK(papacc_issuer_init(&c,&root,&backend)==0);CHECK(papacc_issuer_recover(&c,q.id)==PAPACC_ISSUER_UNCERTAIN);
 CHECK(papacc_issuer_query(&c,q.id,&r)==0&&r.state==PAPACC_ISSUER_RECONCILIATION_REQUIRED);
 CHECK(papacc_issuer_issue(&c,q.id,&csr,1,output,sizeof(output))==PAPACC_ISSUER_UNCERTAIN&&m.signs==1&&m.view.high_water==2);
 /* Opaque close failure retains lease and forbids repeat close/reinit. */
 setup();memset(&c,0,sizeof(c));CHECK(papacc_issuer_init(&c,&root,&backend)==0);q=request(1);CHECK(papacc_issuer_prepare(&c,&q)==0);CHECK(papacc_issuer_reserve(&c,q.id)==0);
 m.fail_release=1;CHECK(papacc_issuer_issue(&c,q.id,&csr,1,output,sizeof(output))==PAPACC_ISSUER_RETAINED&&c.lease!=NULL);
 before=m.releases;papacc_issuer_stop(&c);CHECK(papacc_issuer_finish(&c)==PAPACC_ISSUER_RETAINED&&m.releases==before);CHECK(papacc_issuer_init(&c,&root,&backend)==PAPACC_ISSUER_INVALID);
 /* Stop during signer cannot publish or discard signed result. */
 setup();memset(&c,0,sizeof(c));CHECK(papacc_issuer_init(&c,&root,&backend)==0);q=request(1);CHECK(papacc_issuer_prepare(&c,&q)==0);CHECK(papacc_issuer_reserve(&c,q.id)==0);
 m.owner=&c;m.stop_on_sign=1;CHECK(papacc_issuer_issue(&c,q.id,&csr,1,output,sizeof(output))==PAPACC_ISSUER_STOPPED&&c.retained_output==output);
 CHECK(m.records[0].state==PAPACC_ISSUER_SIGN_REQUESTED);CHECK(papacc_issuer_finish(&c)==PAPACC_ISSUER_RETAINED);
 /* Exhaustion, corrupt recovery, rollback and absent signer. */
 setup();memset(&c,0,sizeof(c));m.view.proofs=0;CHECK(papacc_issuer_init(&c,&root,&backend)==PAPACC_ISSUER_UNCERTAIN&&!m.signs);
 setup();memset(&c,0,sizeof(c));CHECK(papacc_issuer_init(&c,&root,&backend)==0);q=request(1);CHECK(papacc_issuer_prepare(&c,&q)==0);
 m.view.high_water=(PAPACC_U64)~(PAPACC_U64)0;high=m.view.high_water;CHECK(papacc_issuer_reserve(&c,q.id)==PAPACC_ISSUER_LIMIT&&m.view.high_water==high);
 m.view.high_water=1;CHECK(papacc_issuer_query(&c,q.id,&r)==PAPACC_ISSUER_UNCERTAIN);
 setup();memset(&c,0,sizeof(c));CHECK(papacc_issuer_init(&c,&root,&backend)==0);q=request(1);CHECK(papacc_issuer_prepare(&c,&q)==0);CHECK(papacc_issuer_reserve(&c,q.id)==0);
 backend.sign=NULL;CHECK(papacc_issuer_issue(&c,q.id,&csr,1,output,sizeof(output))==PAPACC_ISSUER_UNSUPPORTED&&!m.signs);
 /* Archived public DER lookup does not require a private-key signer. */
 setup();memset(&c,0,sizeof(c));CHECK(papacc_issuer_init(&c,&root,&backend)==0);q=request(1);CHECK(papacc_issuer_prepare(&c,&q)==0);CHECK(papacc_issuer_reserve(&c,q.id)==0);CHECK(papacc_issuer_issue(&c,q.id,&csr,1,output,sizeof(output))==0);
 backend.sign=NULL;backend.acquire=NULL;backend.release=NULL;memset(output,0,sizeof(output));
 CHECK(papacc_issuer_issue(&c,q.id,&csr,1,output,sizeof(output))==0&&!memcmp(output,"DER",3)&&m.signs==1&&m.acquires==1);
 /* Certificate lookup integrity and publish cannot manufacture CURRENT. */
 setup();memset(&c,0,sizeof(c));CHECK(papacc_issuer_init(&c,&root,&backend)==0);q=request(1);CHECK(papacc_issuer_prepare(&c,&q)==0);CHECK(papacc_issuer_reserve(&c,q.id)==0);CHECK(papacc_issuer_issue(&c,q.id,&csr,1,output,sizeof(output))==0);
 m.der[0][0]='X';CHECK(papacc_issuer_issue(&c,q.id,&csr,1,output,sizeof(output))==PAPACC_ISSUER_DENIED&&m.signs==1);m.der[0][0]='D';
 m.now=500;m.allow_publish=1;CHECK(papacc_issuer_transition(&c,q.id,PAPACC_ISSUER_PUBLISHED)==PAPACC_ISSUER_DENIED);CHECK(m.records[0].state==PAPACC_ISSUER_ISSUED_UNPUBLISHED);
 /* Policy revocation/auth failure, malformed records and temporal regression. */
 setup();memset(&c,0,sizeof(c));CHECK(papacc_issuer_init(&c,&root,&backend)==0);q=request(1);m.deny_auth=1;CHECK(papacc_issuer_prepare(&c,&q)==PAPACC_ISSUER_DENIED&&!m.commits);m.deny_auth=0;
 m.now=101;CHECK(papacc_issuer_prepare(&c,&q)==PAPACC_ISSUER_DENIED);m.now=100;
 CHECK(papacc_issuer_prepare(&c,&q)==0);CHECK(papacc_issuer_reserve(&c,q.id)==0);m.now=99;CHECK(papacc_issuer_issue(&c,q.id,&csr,1,output,sizeof(output))==PAPACC_ISSUER_DENIED&&!m.signs);m.now=100;
 m.bad_request=1;CHECK(papacc_issuer_issue(&c,q.id,&csr,1,output,sizeof(output))==PAPACC_ISSUER_DENIED&&!m.signs);m.bad_request=0;
 m.view.policy[0]++;CHECK(papacc_issuer_issue(&c,q.id,&csr,1,output,sizeof(output))==PAPACC_ISSUER_DENIED&&!m.signs);
 m.records[0].serial[15]++;CHECK(papacc_issuer_query(&c,q.id,&r)==PAPACC_ISSUER_UNCERTAIN);
 /* Expiration during acquire: marker durable, never call signer. */
 setup();memset(&c,0,sizeof(c));CHECK(papacc_issuer_init(&c,&root,&backend)==0);q=request(1);CHECK(papacc_issuer_prepare(&c,&q)==0);CHECK(papacc_issuer_reserve(&c,q.id)==0);m.expire_on_acquire=1;
 CHECK(papacc_issuer_issue(&c,q.id,&csr,1,output,sizeof(output))==PAPACC_ISSUER_DENIED&&!m.signs&&m.releases==1&&m.records[0].state==PAPACC_ISSUER_SIGN_REQUESTED);
 /* State reentrancy rejected; no implicit retry or second reservation. */
 setup();memset(&c,0,sizeof(c));CHECK(papacc_issuer_init(&c,&root,&backend)==0);q=request(1);m.owner=&c;m.reenter=1;CHECK(papacc_issuer_prepare(&c,&q)==0&&m.reenter_result==PAPACC_ISSUER_BUSY&&m.view.high_water==1);
 /* Full big endian counter and positive prefix without wrap. */
 m.view.high_water=((PAPACC_U64)0x01020304U<<32)|0x05060707U;CHECK(papacc_issuer_reserve(&c,q.id)==0);CHECK(papacc_issuer_query(&c,q.id,&r)==0);
 CHECK(r.serial[8]==1&&r.serial[9]==2&&r.serial[10]==3&&r.serial[11]==4&&r.serial[12]==5&&r.serial[13]==6&&r.serial[14]==7&&r.serial[15]==8);
 setup();memset(&c,0,sizeof(c));m.view.prefix[0]=128;CHECK(papacc_issuer_init(&c,&root,&backend)==PAPACC_ISSUER_UNCERTAIN);
 /* Durable SIGN_REQUESTED crash before any signer: recovery forbids retry. */
 setup();memset(&c,0,sizeof(c));CHECK(papacc_issuer_init(&c,&root,&backend)==0);q=request(1);CHECK(papacc_issuer_prepare(&c,&q)==0);CHECK(papacc_issuer_reserve(&c,q.id)==0);m.fail_cas=2;
 CHECK(papacc_issuer_issue(&c,q.id,&csr,1,output,sizeof(output))==PAPACC_ISSUER_UNCERTAIN&&!m.signs&&m.records[0].state==PAPACC_ISSUER_SIGN_REQUESTED);
 m.fail_cas=0;memset(&d,0,sizeof(d));CHECK(papacc_issuer_init(&d,&root,&backend)==0);CHECK(papacc_issuer_recover(&d,q.id)==PAPACC_ISSUER_UNCERTAIN&&!m.signs);
 /* Invalid output cannot be persisted; buffer remains owned for review. */
 setup();memset(&c,0,sizeof(c));CHECK(papacc_issuer_init(&c,&root,&backend)==0);q=request(1);CHECK(papacc_issuer_prepare(&c,&q)==0);CHECK(papacc_issuer_reserve(&c,q.id)==0);m.bad_cert=1;
 CHECK(papacc_issuer_issue(&c,q.id,&csr,1,output,sizeof(output))==PAPACC_ISSUER_DENIED&&c.retained_output==output&&m.records[0].state==PAPACC_ISSUER_SIGN_REQUESTED);
 /* Same authority/generation cannot silently change CA public-key identity. */
 setup();memset(&c,0,sizeof(c));CHECK(papacc_issuer_init(&c,&root,&backend)==0);q=request(1);CHECK(papacc_issuer_prepare(&c,&q)==0);m.view.ca_spki_digest[0]++;
 CHECK(papacc_issuer_reserve(&c,q.id)==PAPACC_ISSUER_UNCERTAIN&&!m.signs&&m.view.high_water==1);
 /* Accepted interval limits return an explicit proposal, never clamp issuance. */
 CHECK(papacc_issuer_interval(1000,100,2000,&high)==PAPACC_ISSUER_CONFIRMATION_REQUIRED&&high==1000);
 CHECK(papacc_issuer_interval((PAPACC_U64)~(PAPACC_U64)0,100,9000000,&high)==PAPACC_ISSUER_CONFIRMATION_REQUIRED&&high==100+(PAPACC_U64)90U*86400U);
 CHECK(papacc_issuer_interval((PAPACC_U64)~(PAPACC_U64)0,((PAPACC_U64)~(PAPACC_U64)0)-1,(PAPACC_U64)~(PAPACC_U64)0,&high)==0&&high==(PAPACC_U64)~(PAPACC_U64)0);
 setup();memset(&c,0,sizeof(c));CHECK(papacc_issuer_init(&c,&root,&backend)==0);q=request(1);q.not_after=m.view.ca_not_after+1;
 CHECK(papacc_issuer_prepare(&c,&q)==PAPACC_ISSUER_CONFIRMATION_REQUIRED&&!m.commits&&!m.signs);
 printf("PASS portable_issuer_checks=%u; model=OFFLINE_ONLY; core=%zu record=%zu root_view=%zu backend=%zu request=%zu root_port=%zu model_storage=%zu output=%zu; native=NOT_RUN\n",checks,sizeof(c),sizeof(r),sizeof(m.view),sizeof(backend),sizeof(q),sizeof(root),sizeof(m),sizeof(output));return 0;
#endif
}
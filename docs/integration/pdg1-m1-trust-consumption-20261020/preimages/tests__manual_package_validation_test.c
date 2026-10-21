/* PUBLIC SYNTHETIC CONTROL-FLOW ONLY. No crypto/trust/provenance authority.
 * This binary uses separately compiled OFFLINE_TEST core; never production. */
#include "manual_package_validation_internal.h"
#include <stdio.h>
#include <string.h>
static unsigned checks;
#define CHECK(x) do{checks++;if(!(x)){printf("FAIL line %u\n",(unsigned)__LINE__);return 1;}}while(0)
static unsigned char cfg[4096],iss[4096],zip[4096],raw[8192],expected[8192];
static unsigned char q[4096],staging[4096],output[4096],password[32];
static MP_AUTH_OPERATION op;static MP_AUTH_INPUT input;static size_t zipn,rawn;
typedef struct FIXTURE {
 unsigned calls[MP_AUTH_RELEASE+1],order[16],count;
 MP_AUTH_STAGE bad,stop_at;int bad_code,after,quiescent;
 /* Modeled independent source facts; not a productive authority adapter. */
 int present,purpose,known_status,revoked,valid,independent,pin,revision,identity,corrupt_archive;
 const MP_ENVELOPE *held_envelope;const MP_PAYLOAD *held_payload;
} FIXTURE;
static FIXTURE fixture;
static void b16(unsigned char *p,unsigned n){p[0]=(unsigned char)(n>>8);p[1]=(unsigned char)n;}
static void b32(unsigned char *p,unsigned long n){p[0]=(unsigned char)(n>>24);p[1]=(unsigned char)(n>>16);p[2]=(unsigned char)(n>>8);p[3]=(unsigned char)n;}
static size_t field(unsigned char *p,size_t o,unsigned t,const unsigned char *v,size_t n){b16(p+o,t);b32(p+o+2,(unsigned long)n);if(n)memcpy(p+o+6,v,n);return o+6+n;}
static int setup_bytes(void){
 unsigned t;size_t n,o=12;unsigned char x[256],head[92]={0},spki[1]={0},tag[16]={0},sig[256]={0};
 static const unsigned char fake[]="PUBLIC_FIXTURE";MP_PAYLOAD v;MP_ENVELOPE e;
 memset(&v,0,sizeof(v));memcpy(cfg,"PACCFG02",8);b16(cfg+8,2);b16(cfg+10,16);
 for(t=1;t<=16;t++){
  memset(x,0,sizeof(x));n=32;x[0]=1;
  if(t==1){n=13;memcpy(x,"accel.example",n);}else if(t==2){n=2;b16(x,7443);}
  else if(t==6||t==8||t>=14){n=8;memset(x,0,n);x[7]=(unsigned char)(t==16?2:1);}
  else if(t==9){n=8;memcpy(x,"papacc/1",n);}else if(t==10){n=2;b16(x,1);}
  else if(t==13){n=4;memcpy(x,"TEST",n);}o=field(cfg,o,t,x,n);
 }
 v.member[3].data=cfg;v.member[3].size=o;
 memcpy(iss,"PACISS02",8);b16(iss+8,2);b16(iss+10,5);o=12;
 for(t=1;t<=5;t++){memset(x,1,sizeof(x));n=t<=2?32:t==3?16:t==4?4:0;if(t==4)memcpy(x,"TEST",4);o=field(iss,o,t,x,n);}
 v.member[4].data=iss;v.member[4].size=o;
 for(t=0;t<3;t++){v.member[t].data=fake;v.member[t].size=sizeof(fake)-1;}
 if(mp_payload_prepare(&v,zip,sizeof(zip),&zipn,0)!=MP_DATA_ONLY)return 0;
 memcpy(head,"PACCMP02",8);b16(head+8,2);b16(head+10,1);b32(head+16,(unsigned long)(365+zipn));b32(head+20,(unsigned long)zipn);b32(head+24,600000);b32(head+88,1);
 e.spki.data=spki;e.spki.size=1;e.ciphertext.data=zip;e.ciphertext.size=zipn;
 e.tag.data=tag;e.tag.size=16;e.signature.data=sig;e.signature.size=256;
 return mp_envelope_prepare(head,&e,raw,sizeof(raw),&rawn,0)==MP_DATA_ONLY;
}
static void fake_wipe(void *u,unsigned char *p,size_t n){volatile unsigned char *v=p;(void)u;while(n--)*v++=0;}
static int quiet(void *u){return ((FIXTURE *)u)->quiescent;}
static int stop(void *u){FIXTURE *f=u;return f->stop_at&&op.stage==f->stop_at&&(!f->after||f->calls[f->stop_at]);}
static int step(void *u,MP_AUTH_STAGE s,MP_AUTH_OPERATION *o,const MP_ENVELOPE *e,const MP_PAYLOAD *v){
 FIXTURE *f=u;(void)e;(void)v;
 if(f->count>=16)return MP_AUTH_STEP_INVALID;f->order[f->count++]=(unsigned)s;f->calls[s]++;
 if(s==MP_AUTH_KDF&&(!f->calls[MP_AUTH_TRUST]||!f->present||f->revoked))return MP_AUTH_STEP_INVALID;
 if(s==MP_AUTH_TRUST||s==MP_AUTH_RECHECK_TRUST){
  if(!f->present||!f->independent||!f->pin||!f->revision)return MP_AUTH_STEP_SOURCE_ABSENT;
  if(!f->purpose)return MP_AUTH_STEP_WRONG_PURPOSE;if(!f->known_status)return MP_AUTH_STEP_UNKNOWN;
  if(f->revoked)return MP_AUTH_STEP_REVOKED;if(!f->valid)return MP_AUTH_STEP_EXPIRED;
  if(!f->identity||memcmp(raw,expected,rawn))return MP_AUTH_STEP_INVALID;
 }
 if(s==MP_AUTH_KDF)memset(o->key,0xcc,32);
 if(s==MP_AUTH_AEAD){memcpy(input.staging,zip,zipn);memset(input.quarantine,0xa8,zipn);o->staging_size=zipn;if(f->corrupt_archive)input.staging[30]='x';}
 if(s==f->bad){if(f->bad_code==MP_AUTH_STEP_UNCERTAIN){f->held_envelope=e;f->held_payload=v;}return f->bad_code;}
 return MP_AUTH_STEP_OK;
}
static const MP_AUTH_PORT port={step,fake_wipe,quiet};
static void reset(void){
 memset(&op,0,sizeof(op));memset(&input,0,sizeof(input));memset(&fixture,0,sizeof(fixture));
 fixture.present=fixture.purpose=fixture.known_status=fixture.valid=fixture.independent=fixture.pin=fixture.revision=fixture.identity=fixture.quiescent=1;
 memcpy(expected,raw,rawn);memset(password,'A',sizeof(password));memset(q,0x44,sizeof(q));memset(staging,0x44,sizeof(staging));memset(output,0x5a,sizeof(output));
 input.package.data=raw;input.package.size=rawn;input.password.data=password;input.password.size=sizeof(password);
 input.quarantine=q;input.quarantine_capacity=sizeof(q);input.staging=staging;input.staging_capacity=sizeof(staging);input.output=output;input.output_capacity=sizeof(output);
 input.other_reserved=sizeof(cfg)+sizeof(iss)+sizeof(zip)+(sizeof(raw)-rawn)+sizeof(expected)+sizeof(fixture);
 input.stopped=stop;input.stop_context=&fixture;
}
static int zero(const unsigned char *p,size_t n){size_t i;for(i=0;i<n;i++)if(p[i])return 0;return 1;}
static int unchanged(void){size_t i;for(i=0;i<sizeof(output);i++)if(output[i]!=0x5a)return 0;return 1;}
static MP_AUTH_RESULT run(void){return mp_auth_offline_test_run(&op,&input,&port,&fixture);}
int main(void){
 unsigned i;int code;size_t base_estimate,extra_credit;size_t limit=mp_package_validation_process_limit();
 static const MP_AUTH_STAGE phases[]={MP_AUTH_SIGNATURE,MP_AUTH_TRUST,MP_AUTH_KDF,MP_AUTH_AEAD,MP_AUTH_CONTENT,MP_AUTH_RECHECK_TRUST,MP_AUTH_RELEASE};
 CHECK(setup_bytes());reset();CHECK(run()==MP_AUTH_SYNTHETIC_ONLY);CHECK(op.output_size==zipn&&!memcmp(output,zip,zipn));
 CHECK(fixture.count==7);for(i=0;i<7;i++)CHECK(fixture.order[i]==(unsigned)phases[i]);
 CHECK(op.kdf_invocations==1&&op.aead_invocations==1&&op.content_invocations==1);CHECK(zero(q,sizeof(q))&&zero(staging,sizeof(staging))&&zero(op.key,32));
 /* Each independent evidence defect must prevent ANY KDF. */
 for(i=0;i<9;i++){
  reset();switch(i){case 0:fixture.present=0;break;case 1:fixture.independent=0;break;case 2:fixture.pin=0;break;case 3:fixture.revision=0;break;case 4:fixture.purpose=0;break;case 5:fixture.known_status=0;break;case 6:fixture.revoked=1;break;case 7:fixture.valid=0;break;default:fixture.identity=0;break;}
  CHECK(run()==MP_AUTH_DENIED);CHECK(op.first_failure_stage==MP_AUTH_TRUST);CHECK(op.kdf_invocations==0&&op.aead_invocations==0);CHECK(unchanged()&&zero(q,sizeof(q))&&zero(staging,sizeof(staging)));
 }
 reset();expected[rawn-1]^=1;CHECK(run()==MP_AUTH_DENIED&&op.kdf_invocations==0&&unchanged());
 /* Signature/password/tag/content/profile/key/final-status failures. Content
  * failure codes model the missing provider context; not native profile proof. */
 for(i=0;i<7;i++){
  reset();fixture.bad=phases[i];fixture.bad_code=i==5?MP_AUTH_STEP_REVOKED:MP_AUTH_STEP_INVALID;
  CHECK(run()==MP_AUTH_DENIED);CHECK(op.first_failure_stage==phases[i]);CHECK(unchanged());
  CHECK(zero(q,sizeof(q))&&zero(staging,sizeof(staging))&&zero(op.key,32));
  if(i<2)CHECK(op.kdf_invocations==0);
 }
 for(i=0;i<7;i++){
  reset();fixture.stop_at=phases[i];CHECK(run()==MP_AUTH_STOPPED&&unchanged());if(phases[i]==MP_AUTH_CONTENT)CHECK(op.first_failure_code==MP_STOPPED);CHECK(zero(q,sizeof(q))&&zero(staging,sizeof(staging))&&zero(op.key,32));
  reset();fixture.stop_at=phases[i];fixture.after=1;CHECK(run()==MP_AUTH_STOPPED&&unchanged());CHECK(zero(q,sizeof(q))&&zero(staging,sizeof(staging))&&zero(op.key,32));
 }
 /* An uncertain AEAD/close keeps key and workspace, no destructive retry. */
 reset();fixture.bad=MP_AUTH_AEAD;fixture.bad_code=MP_AUTH_STEP_UNCERTAIN;fixture.quiescent=0;
 CHECK(run()==MP_AUTH_RETAINED&&unchanged());CHECK(op.state==2&&op.key[0]==0xcc&&q[0]==0xa8);CHECK(fixture.held_envelope==&op.envelope&&fixture.held_envelope->ciphertext.data==raw+93);code=op.first_failure_code;
 CHECK(mp_package_validation_finish(&op)==MP_AUTH_RETAINED&&op.key[0]==0xcc);CHECK(run()==MP_AUTH_INVALID);CHECK(fixture.calls[MP_AUTH_AEAD]==1);
 fixture.quiescent=1;CHECK(mp_package_validation_finish(&op)==MP_AUTH_ABORTED);CHECK(zero(q,sizeof(q))&&zero(staging,sizeof(staging))&&zero(op.key,32));
 CHECK(unchanged()&&op.first_failure_code==code&&op.first_failure_stage==MP_AUTH_AEAD&&fixture.calls[MP_AUTH_AEAD]==1);
 reset();fixture.bad=MP_AUTH_RELEASE;fixture.bad_code=MP_AUTH_STEP_UNCERTAIN;fixture.quiescent=0;
 CHECK(run()==MP_AUTH_RETAINED&&unchanged());fixture.quiescent=1;CHECK(mp_package_validation_finish(&op)==MP_AUTH_ABORTED);CHECK(fixture.calls[MP_AUTH_RELEASE]==1&&unchanged());
 reset();fixture.bad=MP_AUTH_CONTENT;fixture.bad_code=MP_AUTH_STEP_UNCERTAIN;fixture.quiescent=0;
 CHECK(run()==MP_AUTH_RETAINED&&fixture.held_payload==&op.payload&&fixture.held_payload->member[2].data>=staging);
 CHECK(fixture.held_payload->member[2].data<staging+zipn&&unchanged());
 fixture.quiescent=1;CHECK(mp_package_validation_finish(&op)==MP_AUTH_ABORTED&&unchanged());
 reset();fixture.quiescent=0;CHECK(run()==MP_AUTH_RETAINED&&fixture.calls[MP_AUTH_RELEASE]==1&&unchanged());
 fixture.quiescent=1;CHECK(mp_package_validation_finish(&op)==MP_AUTH_ABORTED&&fixture.calls[MP_AUTH_RELEASE]==1&&unchanged());
 reset();fixture.corrupt_archive=1;CHECK(run()==MP_AUTH_DENIED&&op.first_failure_stage==MP_AUTH_CONTENT&&fixture.calls[MP_AUTH_CONTENT]==0&&unchanged());
 reset();fixture.stop_at=MP_AUTH_AEAD;fixture.after=1;fixture.quiescent=0;
 CHECK(run()==MP_AUTH_RETAINED&&op.first_failure_code==MP_AUTH_STOPPED&&unchanged()&&op.key[0]==0xcc);
 fixture.quiescent=1;CHECK(mp_package_validation_finish(&op)==MP_AUTH_ABORTED&&op.first_failure_code==MP_AUTH_STOPPED&&unchanged()&&zero(op.key,32));
 reset();input.provider_reserved=MP_AUTH_UNKNOWN_BYTES;CHECK(run()==MP_AUTH_LIMIT&&op.kdf_invocations==0&&fixture.count==0&&unchanged());
 reset();input.other_reserved=limit;CHECK(run()==MP_AUTH_LIMIT&&fixture.count==0&&unchanged());
 reset();input.staging=q;CHECK(run()==MP_AUTH_INVALID&&unchanged());
 reset();input.output=q;CHECK(run()==MP_AUTH_INVALID);
 reset();input.package.size--;CHECK(run()==MP_AUTH_INVALID&&fixture.count==0&&unchanged());
 reset();raw[9]=3;CHECK(run()==MP_AUTH_INVALID&&fixture.count==0&&unchanged());raw[9]=2;
 reset();CHECK(run()==MP_AUTH_SYNTHETIC_ONLY);CHECK(op.managed_estimate<=limit);base_estimate=op.managed_estimate;extra_credit=limit-base_estimate;
 reset();input.other_reserved+=extra_credit;CHECK(run()==MP_AUTH_SYNTHETIC_ONLY&&op.managed_estimate==limit);
 reset();input.other_reserved+=extra_credit+1;CHECK(run()==MP_AUTH_LIMIT&&fixture.count==0&&unchanged());
 printf("PASS %u SYNTHETIC_CONTROL_ONLY assertions limit=%zu op=%zu input=%zu; no productive trust/crypto success\n",checks,limit,sizeof(op),sizeof(input));return 0;
}
